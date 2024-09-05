// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/WindowTerminal.h"
#include "Communicator/ComPort/ComPort.h"
#include "Utils/String.h"
#include "Settings/Settings.h"
#include "Windows/MainWindow/MainWindow.h"

#undef CRC

#pragma warning(push, 0)
#include <wx/wx.h>
#include <wx/fontdlg.h>
#pragma warning(pop)


wxTextCtrl *WindowTerminal::text = nullptr;
wxTextCtrl *WindowTerminal::line = nullptr;
WindowTerminal *WindowTerminal::self = nullptr;


WindowTerminal::WindowTerminal() : wxFrame(nullptr, wxID_ANY, wxT("Терминал"))
{
    Show(false);

    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &WindowTerminal::OnSize, this);
    line->Bind(wxEVT_TEXT_ENTER, &WindowTerminal::OnTextEnter, this, ID_LINE);
    line->Bind(wxEVT_KEY_DOWN, &WindowTerminal::OnTextControlKeyDown, this, ID_LINE);
    Bind(wxEVT_CLOSE_WINDOW, &WindowTerminal::OnEventClose, this);

    wxTopLevelWindowMSW::Show();

    if (ComPort::Open())
    {
        AddLine("Обнаружено внешнее устройство");
        timerComPort.Bind(wxEVT_TIMER, &WindowTerminal::OnTimerComPort, this);
        timerComPort.Start(10);
    }
    else
    {
        AddLine("Внешнее устройство не обнаружено. Работает эмулятор");
    }

    wxRect rect = SET::GUI::window_scpi.Get().rect;

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    Show(SET::GUI::window_scpi.Get().shown);

    if (SET::GUI::window_scpi.Get().maximized)
    {
        Maximize(true);
    }
}

WindowTerminal::~WindowTerminal()
{
    SET::GUI::window_scpi.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, IsShown(), IsMaximized()});

    ComPort::Close();
}


void WindowTerminal::OnSize(wxSizeEvent &)
{
    wxPoint clientOrigin = GetClientAreaOrigin();

    wxSize clientSize = GetClientSize();

    int heightLine = line->GetSize().y;

    wxSize sizeText = clientSize;
    sizeText.y -= heightLine;

    text->SetSize(sizeText);

    line->SetPosition({ clientOrigin.x, clientSize.y - heightLine });
    line->SetSize({ text->GetSize().x, heightLine });
}


void WindowTerminal::OnTimerComPort(wxTimerEvent &)
{
    if (ComPort::IsOpened())
    {
        char buffer[4096];

        int n = ComPort::Receive(buffer, 4095);

        if (n)
        {
            buffer[n] = '\0';
            String<1024> message(">>> %s", buffer);
            AddText(message.c_str());
        }
    }
}


void WindowTerminal::Create()
{
    self = new WindowTerminal();
}


void WindowTerminal::OnTextEnter(wxCommandEvent &)
{
    history.Add(line->GetLineText(0));

    String<1024> txt("    %s", (pchar)line->GetLineText(0).mb_str());

    AddLine(txt.c_str());

    txt.SetFormat("%s\x0d", (pchar)line->GetLineText(0).mb_str());

    if (ComPort::IsOpened())
    {
        ComPort::Send(txt.c_str());
    }

    line->Clear();
}


void WindowTerminal::OnTextControlKeyDown(wxKeyEvent &event)
{
    if (event.GetKeyCode() == WXK_UP)
    {
        wxString txt = history.Prev();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else if (event.GetKeyCode() == WXK_DOWN)
    {
        wxString txt = history.Next();

        if (txt.size())
        {
            line->Clear();
            line->WriteText(txt);
        }
    }
    else
    {
        event.Skip();
    }
}


void WindowTerminal::AddLine(const wxString &str)
{
    AddText(str);
    AddText(wxT("\n"));
}

void WindowTerminal::AddText(const wxString &str)
{
    text->WriteText(str);
}


void WindowTerminal::SwitchVisibility()
{
    Show(!IsShown());
}


void WindowTerminal::OnEventClose(wxCloseEvent &)
{
    Show(false);

    MainWindow::SetTitleMenu(VIEW_TERMINAL);
}


void WindowTerminal::History::Add(const wxString &txt)
{
    if ((history.size() == 0) || 
        (history[history.size() - 1].compare(txt) != 0))
    {
        history.push_back(txt);
        position = history.size() - 1;
    }
}


wxString WindowTerminal::History::Next()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position++;
    if (position == history.size())
    {
        position = 0;
    }

    return result;
}


wxString WindowTerminal::History::Prev()
{
    if (history.size() == 0)
    {
        return "";
    }

    wxString result = history[position];

    position = (position == 0) ? (history.size() - 1) : (position - 1);

    return result;
}
