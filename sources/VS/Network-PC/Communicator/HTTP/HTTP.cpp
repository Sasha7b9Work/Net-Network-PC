// 2024/6/21 11:40:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HTTP/HTTP.h"
#include "Windows/MainWindow/MainWindow.h"


namespace HTTP
{
    static const wxString url = "https://wltech.ru/wifi/post-data-mst2.php";
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

    wxString body = wxString::Format("api_key=PtmAT51b3j4F8&device=%u&location=Street&value1=%f&value2=%f&value3=%f&DevPoint=%.2f&Illuminate=%.2f&meas_time=%d-%02d-%02d %02d:%02d:%02d",
        id, temp, humidity, pressure, dew_point, illuminate,
        time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    request.SetData(body, content_type);

    request.Start();
}


/*
void HTTP::SendPOST(float *values, int count)
{
    wxWebRequest request = wxWebSession::GetDefault().CreateRequest(&MainWindow::GetEventHandler(), url);

    wxDateTime time = wxDateTime::Now();

    wxString body{ "api_key=PtmAT51b3j4F8" };

    for (int i = 0; i < count; i++)
    {
        body += wxString::Format("&value%d=%10.2f", i + 1, values[i]);
    }

    body = wxString::Format("api_key=PtmAT51b3j4F8&device=1&location=Street&value1=24.75&value2=54.27&value3=994.01&DevPoint=%.2f&Illuminate=%.2f&meas_time=%d-%02d-%02d %02d:%02d:%02d",
        values[Measure::DewPoint], values[Measure::Illuminate],
        time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    request.SetData(body, content_type);

    request.Start();
}
*/
