// 2022/09/05 08:53:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Data/Sensors.h"
#include "Display/Diagram/Canvas.h"
#include "Settings/Settings.h"


/*
*   Виджет с графиком
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

        void Rebuild();

        void SetSizeArea(int width, int height);

        void OnEventSize();

    private:

        void UpdateArea();

        Pool(wxWindow *parent);

        Diagram *pool[Measure::Count];
    };
};
