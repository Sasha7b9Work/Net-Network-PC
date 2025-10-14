// 2024/6/21 11:40:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HTTP/HTTP.h"
#include "Display/Windows/MainWindow/MainWindow.h"
#include "Utils/Timer.h"
#include "Data/Sensors.h"


namespace HTTP
{
    struct UUIDS
    {
        uint Get(uint);
    private:
        std::map<uint, uint> uiids;     // Первое число - приходящий ID, второе - то, что мы отправляем
    };

    static UUIDS uiids;

    static const wxString url = "https://www.recontr.com/post-data.php";
    static const wxString content_type = "application/x-www-form-urlencoded"; 
    static const wxString key = "api_key=PtmAT51b3j4F8";
}


void HTTP::SendPOST(uint id, float temp, float humidity, float pressure, float dew_point, float illuminate)
{
    if (!SET::NETWORK::send_to_http.Get())
    {
        return;
    }

    if (temp == 0.0f && humidity == 0.0f && pressure == 0.0f && dew_point == 0.0f && illuminate == 0.0f)
    {
        return;
    }

    wxWebRequest request = wxWebSession::GetDefault().CreateRequest(&MainWindow::GetEventHandler(), url);

    wxDateTime time = wxDateTime::Now();

    wxString body = wxString::Format("api_key=PtmAT51b3j4F8&device=%u&model=bckm-mk3&location=Street&temperature=%.1f&humidity=%.1f&pressure=%.1f&DevPoint=%.1f&Illuminate=%.1f&meas_time=%d-%02d-%02d %02d:%02d:%02d",
        uiids.Get(id), temp, humidity, pressure, dew_point, illuminate,
        time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    request.SetData(body, content_type);

    request.Start();
}


uint HTTP::UUIDS::Get(uint id)
{
    if (id == 0xD5E0B863)
    {
        return 101;
    }

    return id;
}


void HTTP::Update()
{
    static TimeMeterMS meter;

    if ((int)meter.ElapsedTime() > SET::NETWORK::time_http.Get() * 60 * 1000)
    {
        meter.Reset();

        Sensor *sensor = Sensor::Pool::First();

        while (sensor)
        {
            sensor = Sensor::Pool::Next(sensor);
        }
    }
}
