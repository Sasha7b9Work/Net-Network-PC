// 2024/08/07 10:28:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class WindowLog : public wxFrame
{
public:

    virtual ~WindowLog();

    static WindowLog *self;

    static void Create();

    void AddLine(const wxString &);

    void SwitchVisibility();

private:

    WindowLog();

    static wxTextCtrl *text;
    static wxTextCtrl *line;

    void OnSize(wxSizeEvent &);
    void OnClose(wxCloseEvent &);
};
