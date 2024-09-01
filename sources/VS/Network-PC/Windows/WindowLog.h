// 2024/08/07 10:28:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace WindowLog
{
    void Create();

    void Destroy();

    void AddLine(const wxString &);

    void SwitchVisibility();

    bool IsShown();
}
