// 2024/08/26 13:39:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class WindowDiagram : public wxFrame
{
public:
    static WindowDiagram *self;

    static void Create(const wxSize &);

private:

    WindowDiagram(const wxSize &);

    void OnEventSize(wxSizeEvent &);
};
