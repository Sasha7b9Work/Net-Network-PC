// 2024/6/21 11:40:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HTTP/HTTP.h"
#include "Frame.h"


namespace HTTP
{
    static const wxString url = "https://wltech.ru/wifi/post-data-mst.php";
    static const wxString content_type = "application/x-www-form-urlencoded";
    static const wxString key = "api_key=PtmAT51b3j4F8";
}


void HTTP::SendPOST(TypeMeasure::E type, float value)
{
    wxWebRequest request = wxWebSession::GetDefault().CreateRequest(Frame::self, url);

    Frame::self->Bind(wxEVT_WEBREQUEST_STATE, &Frame::OnWetRequestState, Frame::self);

    wxDateTime time = wxDateTime::Now();

    wxString body = wxString::Format("api_key=PtmAT51b3j4F8&value%d=%10.2f&meas_time = %d-%02d-%02d %02d:%02d:%02d", (int)type, value,
        time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    request.SetData(body, content_type);

    request.Start();
}
