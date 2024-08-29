// 2022/09/05 08:53:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"
#include "Settings/Settings.h"
#include "Windows/WindowDiagram/Canvas.h"


/*
*   ������ � ��������
*/


class Diagram : public wxPanel
{
public:

    Diagram(wxWindow *parant, Measure::E);

    void SetSizeArea(int, int);

private:

    Canvas *canvas = nullptr;

public:

    class Pool : public wxPanel
    {
        friend class WindowDiagram;
    public:

        static Pool *Create(wxWindow *);

        static Pool *self;

        static void Rebuild();

        static void SetSizeArea(int width, int height);

        static void OnEventSize();

    private:

        static void UpdateArea();

        Pool(wxWindow *parent);

        static Diagram *diagrams[Measure::Count];
    };
};
