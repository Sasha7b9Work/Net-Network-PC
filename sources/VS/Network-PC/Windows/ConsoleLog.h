// 2024/08/07 10:28:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ConsoleLog : public wxFrame
{
public:

    static ConsoleLog *self;

    static void Create();

    void AddLine(const wxString &);

private:

    ConsoleLog();

    static wxTextCtrl *text;
    static wxTextCtrl *line;

    void OnSize(wxSizeEvent &);
    void OnClose(wxCloseEvent &);
};
