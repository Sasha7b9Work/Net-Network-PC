// 2024/08/07 10:28:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Windows/WindowLog.h"
#include "Settings/Settings.h"
#include "Windows/MainWindow/MainWindow.h"


enum
{
    ID_LINE
};



class FrameLog : public wxFrame
{
public:

    FrameLog();

    virtual ~FrameLog();

    static FrameLog *self;

    void AddLine(const wxString &);

private:

    static wxTextCtrl *text;
    static wxTextCtrl *line;

    void OnSize(wxSizeEvent &);
    void OnEventClose(wxCloseEvent &);
};


wxTextCtrl *FrameLog::text = nullptr;
wxTextCtrl *FrameLog::line = nullptr;
FrameLog *FrameLog::self = nullptr;


FrameLog::FrameLog() : wxFrame(nullptr, wxID_ANY, _("Ëîã"))
{
    Show(false);

    text = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, { 600, 300 }, wxTE_MULTILINE | wxTE_READONLY);

    line = new wxTextCtrl(this, ID_LINE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    line->SetFocus();

    wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));
    line->SetFont(font);
    text->SetFont(font);

    Bind(wxEVT_SIZE, &FrameLog::OnSize, this);
    Bind(wxEVT_CLOSE_WINDOW, &FrameLog::OnEventClose, this);

    wxTopLevelWindowMSW::Show();

    const WindowParameters window_log = SET::GUI::window_log.Get();

    wxRect rect = window_log.rect;

    SetPosition({ rect.x, rect.y });
    SetSize({ rect.width, rect.height });

    Show(window_log.shown);

    if (window_log.maximized)
    {
        Maximize(true);
    }
}


FrameLog::~FrameLog()
{
    SET::GUI::window_log.Set({ { GetPosition().x, GetPosition().y, GetSize().x, GetSize().y }, IsShown(), IsMaximized() });
}


void WindowLog::Delete()
{
    if (FrameLog::self)
    {
        delete FrameLog::self;

        FrameLog::self = nullptr;
    }
}


void WindowLog::Create()
{
    FrameLog::self = new FrameLog();
}


void FrameLog::AddLine(const wxString &str)
{
    text->WriteText(str + "\n");
}


void FrameLog::OnSize(wxSizeEvent &)
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


void FrameLog::OnEventClose(wxCloseEvent &)
{
    Show(false);

    MainWindow::SetTitleMenu(VIEW_LOG);
}


void WindowLog::SwitchVisibility()
{
    if (FrameLog::self)
    {
        FrameLog::self->Show(!FrameLog::self->IsShown());
    }
}


bool WindowLog::IsShown()
{
    return FrameLog::self ? FrameLog::self->IsShown() : false;
}


void WindowLog::AddLine(const wxString &line)
{
    if (FrameLog::self)
    {
        FrameLog::self->AddLine(line);
    }
}
