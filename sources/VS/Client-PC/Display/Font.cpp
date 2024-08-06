// 2022/05/05 09:51:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Font.h"


namespace Font
{
    static wxFont font;
}


void Font::Set(wxFont _font)
{
    font = _font;
}


wxFont Font::Get()
{
    return font;
}


void Font::SetSize(int size)
{
    font.SetPointSize(size);
}
