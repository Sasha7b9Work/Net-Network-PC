// 2022/04/29 13:56:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/Communicator.h"
#include "Communicator/HC12/HC12.h"
#include "Communicator/LAN/ServerTCP.h"
#include "Settings/Settings.h"


void Communicator::Init()
{
    HC12::Init(SET::COM::port_hc12.Get());
    ServerTCP::Init(777);
}


void Communicator::Update()
{
    HC12::Update();
    ServerTCP::Update();
}
