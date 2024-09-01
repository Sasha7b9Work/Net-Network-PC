// 2024/08/07 10:28:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class FrameLog : public wxFrame
{
public:

    virtual ~FrameLog();

    static FrameLog *self;

    static void Create();

    void AddLine(const wxString &);

    void SwitchVisibility();

private:

    FrameLog();

    static wxTextCtrl *text;
    static wxTextCtrl *line;

    void OnSize(wxSizeEvent &);
    void OnEventClose(wxCloseEvent &);
};
