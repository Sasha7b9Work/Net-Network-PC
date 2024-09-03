// 2024/08/26 13:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class FrameDiagram : public wxFrame
{
public:

    ~FrameDiagram();

    static FrameDiagram *self;

    static void Create();

    void SwitchVisibility();

    void UpdateArea();

private:

    FrameDiagram();

    void OnEventSize(wxSizeEvent &);

    void OnEventClose(wxCloseEvent &);

    void OnEventCreateContextMenu(wxContextMenuEvent &);

    // Вызывается при выборе пункта контекстного меню
    void OnEventContextMenu(wxCommandEvent &);

    void ShowContextMenu(const wxPoint &);
};
