// 2024/6/21 11:40:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HTTP/HTTP.h"
#include "Windows/MainWindow/MainWindow.h"


namespace HTTP
{
    static const wxString url = "https://www.recontr.com/post-data.php";
    static const wxString content_type = "application/x-www-form-urlencoded"; 
    static const wxString key = "api_key=PtmAT51b3j4F8";
}


void HTTP::SendPOST(uint id, float temp, float humidity, float pressure, float dew_point, float illuminate)
{
    if (temp == 0.0f && humidity == 0.0f && pressure == 0.0f && dew_point == 0.0f && illuminate == 0.0f)
    {
        return;
    }

    wxWebRequest request = wxWebSession::GetDefault().CreateRequest(&MainWindow::GetEventHandler(), url);

    wxDateTime time = wxDateTime::Now();

    wxString body = wxString::Format("api_key=PtmAT51b3j4F8&device=%u&model=\"bckm - md\"&location=Street&temperature=%f&humidity=%f&pressure=%f&DevPoint=%.2f&Illuminate=%.2f&meas_time=%d-%02d-%02d %02d:%02d:%02d",
        id, temp, humidity, pressure, dew_point, illuminate,
        time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    request.SetData(body, content_type);

    request.Start();
}


void HTTP::SendPOST(uint id, float illuminate)
{
    wxWebRequest request = wxWebSession::GetDefault().CreateRequest(&MainWindow::GetEventHandler(), url);

    wxDateTime time = wxDateTime::Now();

    wxString body = wxString::Format("api_key=PtmAT51b3j4F8&device=%u&location=Ya.Kolasa 73&Illuminate=%f&meas_time=%d-%02d-%02d %02d:%02d:%02d",
        id, illuminate,
        time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    request.SetData(body, content_type);

    request.Start();
}

