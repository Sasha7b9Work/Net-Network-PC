// 2024/09/02 14:05:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Windows/WindowEmulator.h"



class FrameEmulator : public wxFrame
{
public:
    FrameEmulator() : wxFrame(nullptr, wxID_ANY, _("Ёмул€тор"))
    {
        Show(true);
    }
private:
};


namespace WindowEmulator
{
    static FrameEmulator *frame = nullptr;
}



void WindowEmulator::Create()
{
    frame = new FrameEmulator();
}


void WindowEmulator::Delete()
{
    SAVE_DELETE(frame);
}
