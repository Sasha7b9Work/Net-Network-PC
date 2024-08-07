// 2024/08/07 10:28:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/ConsoleLog.h"


wxTextCtrl *ConsoleLog::text = nullptr;
wxTextCtrl *ConsoleLog::line = nullptr;
ConsoleLog *ConsoleLog::self = nullptr;


enum
{
    ID_LINE
};



ConsoleLog::ConsoleLog(wxFrame *parent) : wxFrame(parent, wxID_ANY, _("Ëîã"))
{
    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &ConsoleLog::OnSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &ConsoleLog::OnClose, this);

    wxTopLevelWindowMSW::Show();
}


void ConsoleLog::Create()
{
    self = new ConsoleLog(nullptr);
}


void ConsoleLog::AddLine(const wxString &str)
{
    text->WriteText(str + "\n");
}


void ConsoleLog::OnSize(wxSizeEvent &)
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


void ConsoleLog::OnClose(wxCloseEvent &)
{
    Show(false);
}
