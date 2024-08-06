// 2023/11/10 13:56:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/ComPort/ComPort.h"


bool ComPort::Open()
{
    return false;
}


bool ComPort::IsOpened()
{
    return false;
}


void ComPort::Close()
{

}


int ComPort::Receive(void * /*buffer*/, int /*size*/)
{
    return 0;
}


void ComPort::Send(pchar)
{

}
