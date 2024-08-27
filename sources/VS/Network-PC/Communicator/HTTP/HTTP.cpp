// 2024/6/21 11:40:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/HTTP/HTTP.h"
#include "MainWindow.h"


namespace HTTP
{
    static const wxString url = "https://wltech.ru/wifi/post-data-mst.php";
    static const wxString content_type = "application/x-www-form-urlencoded";
    static const wxString key = "api_key=PtmAT51b3j4F8";
}


void HTTP::SendPOST(float *values, int count)
{
    wxWebRequest request = wxWebSession::GetDefault().CreateRequest(MainWindow::self, url);

    MainWindow::self->Bind(wxEVT_WEBREQUEST_STATE, &MainWindow::OnWebRequestState, MainWindow::self);

    wxDateTime time = wxDateTime::Now();

    wxString body{ "api_key=PtmAT51b3j4F8" };

    for (int i = 0; i < count; i++)
    {
        body += wxString::Format("&value%d=%10.2f", i + 1, values[i]);
    }

    body += wxString::Format("&meas_time=%d-%02d-%02d %02d:%02d:%02d",
        time.GetYear(), time.GetMonth() + 1, time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());

    request.SetData(body, content_type);

    request.Start();
}
