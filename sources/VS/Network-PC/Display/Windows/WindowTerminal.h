﻿// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#pragma warning(push, 0)
#include <wx/frame.h>
#include <wx/timer.h>
#pragma warning(pop)


class WindowTerminal : public wxFrame
{
public:

    static void Create();
    static WindowTerminal *self;

    virtual ~WindowTerminal();

    void SwitchVisibility();

    void AddLine(const wxString &line);
    void AddText(const wxString &text);

private:

    enum
    {
        ID_LINE
    };

    WindowTerminal();

    void OnSize(wxSizeEvent &);
    void OnTextEnter(wxCommandEvent &);
    void OnTextControlKeyDown(wxKeyEvent &);
    void OnEventClose(wxCloseEvent &);

    wxTimer timerComPort;
    void OnTimerComPort(wxTimerEvent &);

    class History
    {
    public:
        void Add(const wxString &text);
        wxString Next();
        wxString Prev();

    private:
        std::vector<wxString> history;
        uint position = 1;
    };

    History history;

    static wxTextCtrl *text;
    static wxTextCtrl *line;
};
