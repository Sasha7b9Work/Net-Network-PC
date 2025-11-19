// 2024/6/21 11:40:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HTTP/HTTP.h"
#include "Display/Windows/MainWindow/MainWindow.h"
#include "Utils/Timer.h"
#include "Data/Sensors.h"


/*

    https://www.recontr.com/charts33.php

*/


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

    static float GetLastMeasure(Sensor *, Measure::E);

    struct StructMeasure
    {
        void Set(uint id, float v)
        {
            values[id] = v;
        }

        float Get(uint id) const
        {
            auto it = values.find(id);

            if (it != values.end())
            {
                return it->second;
            }

            return 0.0f;
        }

    private:

        std::map<uint, float> values;
    };

    static StructMeasure meas_temp;
    static StructMeasure meas_humidity;
    static StructMeasure meas_pressure;
    static StructMeasure meas_dew_point;
    static StructMeasure meas_illuminate;

    struct StructTimer
    {
        TimeMeterMS &GetMeter(uint id)
        {
            if (meters.find(id) == meters.end())
            {
                meters[id] = TimeMeterMS();
            }

            return meters[id];
        }

    private:

        std::map<uint, TimeMeterMS> meters;
    };

    static StructTimer timers;
}


void HTTP::SendPOST(uint id, float temp, float humidity, float pressure, float dew_point, float illuminate)
{
    if (!SET::NETWORK::send_to_http.Get())
    {
        return;
    }

    if (temp != 0.0f)
    {
        meas_temp.Set(id, temp);
    }
    else if (humidity != 0.0f)
    {
        meas_humidity.Set(id, humidity);
    }
    else if (pressure != 0.0f)
    {
        meas_pressure.Set(id, pressure);
    }
    else if (dew_point != 0.0f)
    {
        meas_dew_point.Set(id, dew_point);
    }
    else if (illuminate != 0.0f)
    {
        meas_illuminate.Set(id, illuminate);
    }

    TimeMeterMS &meter = timers.GetMeter(id);

    if (meter.ElapsedTime() >= 10000)
    {
        meter.Reset();

        wxWebRequest request = wxWebSession::GetDefault().CreateRequest(&MainWindow::GetEventHandler(), url);

        wxDateTime time = wxDateTime::Now();

        wxString body = wxString::Format("api_key=PtmAT51b3j4F8&device=%u&model=bckm-mk3&location=Улица\x20Якуба\x20Коласа&temperature=%.1f&humidity=%.1f&pressure=%.1f&DevPoint=%.1f&Illuminate=%.1f&meas_time=%d-%02d-%02d %02d:%02d:%02d",
            uiids.Get(id), meas_temp.Get(id), meas_humidity.Get(id), meas_pressure.Get(id), meas_dew_point.Get(id), meas_illuminate.Get(id),
            time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

        request.SetData(body, content_type);

        request.Start();
    }
}


uint HTTP::UUIDS::Get(uint id)
{
    if (id == 0xD5E0B863)           // С дисплеем
    {
        return 101;
    }
    else if (id == 0xa606b321)      // Без дисплея
    {
        return 102;
    }
    else if (id == 0x766b5dc9)      // Спящее устройство
    {
        return 103;
    }

    return id;
}


void HTTP::_Update()
{
    static TimeMeterMS meter;

    if ((int)meter.ElapsedTime() > SET::NETWORK::time_http.Get() * 10000)
    {
        meter.Reset();

        Sensor *sensor = Sensor::Pool::First();

        while (sensor)
        {
            float temp = GetLastMeasure(sensor, Measure::Temperature);
            float humidity = GetLastMeasure(sensor, Measure::Humidity);
            float pressure = GetLastMeasure(sensor, Measure::Pressure);
            float dew_point = GetLastMeasure(sensor, Measure::DewPoint);
            float illuminate = GetLastMeasure(sensor, Measure::Illuminate);

            SendPOST(sensor->Id(), temp, humidity, pressure, dew_point, illuminate);

            float k = 1.05f;
            SendPOST(101, temp * k, humidity * k, pressure * k, dew_point * k, illuminate * k);

            k = 0.95f;
            SendPOST(103, temp * k, humidity * k, pressure * k, dew_point * k, illuminate * k);

            k = 1.1f;
            SendPOST(104, temp * k, humidity * k, pressure * k, dew_point * k, illuminate * k);

            k = 0.9f;
            SendPOST(105, temp * k, humidity * k, pressure * k, dew_point * k, illuminate * k);

            sensor = Sensor::Pool::Next(sensor);
        }
    }
}


float HTTP::GetLastMeasure(Sensor *sens, Measure::E meas)
{
    const DataArray &data = sens->GetMeasures(meas);

    const DataPoint &point = data.Last();

    return point.value;
}
