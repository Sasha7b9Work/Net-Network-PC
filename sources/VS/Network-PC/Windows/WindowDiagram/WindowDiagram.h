// 2024/08/26 13:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class WindowDiagram : public wxFrame
{
public:

    static WindowDiagram *self;

    static void Create();

    static void Delete();

    void SwitchVisibility();

    void UpdateArea();

private:

    WindowDiagram();

    ~WindowDiagram();

    void OnEventSize(wxSizeEvent &);

    void OnEventClose(wxCloseEvent &);

    void OnEventCreateContextMenu(wxContextMenuEvent &);

    // Вызывается при выборе пункта контекстного меню
    void OnEventContextMenu(wxCommandEvent &);

    void ShowContextMenu(const wxPoint &);
};
