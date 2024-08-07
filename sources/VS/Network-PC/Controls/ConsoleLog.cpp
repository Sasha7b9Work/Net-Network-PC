// 2024/08/07 10:28:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls/ConsoleLog.h"


ConsoleLog *ConsoleLog::self = nullptr;


ConsoleLog::ConsoleLog(wxFrame *parent) : wxFrame(parent, wxID_ANY, _("Ћог"))
{
    wxTopLevelWindowMSW::Show();
}


void ConsoleLog::Create()
{
    self = new ConsoleLog(nullptr);
}
