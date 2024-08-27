// 2024/08/07 10:28:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/WindowLog.h"
#include "Settings/Settings.h"


wxTextCtrl *WindowLog::text = nullptr;
wxTextCtrl *WindowLog::line = nullptr;
WindowLog *WindowLog::self = nullptr;


enum
{
    ID_LINE
};



WindowLog::WindowLog() : wxFrame(nullptr, wxID_ANY, _("Ëîã"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &WindowLog::OnSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &WindowLog::OnClose, this);

    wxTopLevelWindowMSW::Show();

    wxRect rect = SET::GUI::window_log.Get().rect;

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    Show(SET::GUI::window_log.Get().shown);

    if (SET::GUI::window_log.Get().maximized)
    {
        Maximize(true);
    }
}


WindowLog::~WindowLog()
{
    SET::GUI::window_log.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, IsShown(), IsMaximized() });
}


void WindowLog::Create()
{
    self = new WindowLog();
}


void WindowLog::AddLine(const wxString &str)
{
    text->WriteText(str + "\n");
}


void WindowLog::OnSize(wxSizeEvent &)
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


void WindowLog::OnClose(wxCloseEvent &)
{
    Show(false);
}


void WindowLog::SwitchVisibility()
{
    Show(!IsShown());
}
