// 2022/04/29 13:56:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Table/Table.h"
#include "Settings/Settings.h"


class DiagramPool;


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    void OnAbout(wxCommandEvent &event);

    static Frame *self;

    void OnWetRequestState(wxWebRequestEvent &);

private:

    ModeView::E mode_view = ModeView::Full;

    wxToolBar *toolBar = nullptr;

    wxBoxSizer *sizer = nullptr;

    void OnSize(wxSizeEvent &);

    void OnCloseWindow(wxCloseEvent &);

    void OnMenuTool(wxCommandEvent &);

    void OnMenuSettings(wxCommandEvent &);

    void SetModeView(ModeView::E);

    void OnSocketEvent(wxSocketEvent &);
};
