// 2024/08/26 13:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class WindowDiagram : public wxFrame
{
public:

    ~WindowDiagram();

    static WindowDiagram *self;

    static void Create(const wxSize &);

    static void UpdateArea();

    void SwitchVisibility();

private:

    WindowDiagram(const wxSize &);

    void OnEventSize(wxSizeEvent &);

    void OnEventClose(wxCloseEvent &);
};
