﻿// 2024/04/29 08:37:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/Server/Server.h"
#include "Utils/Timer.h"
#include "Settings/Settings.h"
#include "Display/Windows/MainWindow/MainWindow.h"


namespace ServerMeasures
{
    static bool is_connected = false;

    static wxSocketClient *socket = nullptr;

    // Послан запрос на соединение. Ждём
    static bool wait_connection = false;

    static uint time_last_send = 0;

    static void Init();
}


void ServerMeasures::Init()
{
    if (!socket)
    {
        socket = new wxSocketClient();

        socket->SetEventHandler(MainWindow::GetEventHandler(), SOCKET_ID);

        socket->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG);

        socket->Notify(true);
    }

    if (socket && !socket->IsConnected() && !wait_connection)
    {
        wait_connection = true;

        wxIPaddress *addr;
        wxIPV4address addr4;
        addr = &addr4;

        wxString host_name = SET::NETWORK::host_ip.Get();

        int port = SET::NETWORK::host_port.Get();

        LOG_WRITE("Connect to %s:%d", host_name.c_str().AsChar(), port);

        addr->Hostname(host_name);
        addr->Service((uint16)port);

        socket->Connect(*addr, false);
    }
}


void ServerMeasures::DeInit()
{
    if (socket)
    {
        socket->Close();

        while (socket->IsConnected())
        {

        }

        delete socket;
        socket = nullptr;
    }

    wait_connection = false;
    is_connected = false;
}


void ServerMeasures::Update()
{
    if (is_connected)
    {
        if (Timer::CurrentTime() - time_last_send > 5000)
        {
            Send(0xFFFFFFFF, Measure::Temperature, 25.0f);
        }

        return;
    }

    Init();
}


void ServerMeasures::Send(uint id, Measure::E type, float value)
{
    if (!is_connected)
    {
        return;
    }

    wxDateTime time = wxDateTime::Now();

    wxString message = wxString::Format("{\"id\":\"%08X\",\"%s\":\"%f\",\"time\":\"%d-%02d-%02d %02d:%02d:%02d\"}\r\n",
        id, Measure(type).GetTitle(), value,
        time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    wxScopedCharBuffer message_utf8 = message.ToUTF8();

    socket->Write(message_utf8, message_utf8.length());

    time_last_send = Timer::CurrentTime();
}



void ServerMeasures::CallbackOnSocketEvent(wxSocketEvent &event)
{
    switch (event.GetSocketEvent())
    {
    case wxSOCKET_INPUT:
        LOG_WRITE("wxSOCKET_INPUT");
        break;

    case wxSOCKET_LOST:
        LOG_WRITE("wxSOCKET_LOST");
        DeInit();
        break;

    case wxSOCKET_CONNECTION:
        LOG_WRITE("wxSOCKET_CONNECTION");
        wait_connection = false;
        is_connected = true;
        break;

    case wxSOCKET_OUTPUT:
        LOG_WRITE("wxSOCKET_OUTPUT");
        break;
    }
}
