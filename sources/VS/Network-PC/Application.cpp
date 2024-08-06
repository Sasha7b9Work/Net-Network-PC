// 2022/04/29 13:56:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Application.h"
#include "Frame.h"
#include "Communicator/Server/Server.h"
#include "Controls/ConsoleSCPI.h"


wxFileConfig *g_file_config = nullptr;


wxIMPLEMENT_APP(Application);

enum
{
    TIMER_ID = 111
};


bool Application::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    g_file_config = new wxFileConfig("", "", wxGetCwd() + "/config.cfg");

    wxConfigBase::Set(g_file_config);

    // we use a PNG image in our HTML page
    wxImage::AddHandler(new wxPNGHandler);

    // create and show the main application window
    Frame *frame = new Frame(_("ГТЦ-3"));

    Init();

    Bind(wxEVT_TIMER, &Application::OnTimer, this, TIMER_ID);

    timer.SetOwner(this, TIMER_ID);

    timer.Start();

    frame->Show();

    return true;
}


void Application::OnTimer(wxTimerEvent &)
{
    Update();
}


int Application::OnExit()
{
    Log::DeInit();

    wxConfigBase::Get(false)->Flush();

    wxConfigBase::Set(nullptr);

    delete g_file_config;

    g_file_config = nullptr;

    ServerMeasures::DeInit();

    return wxApp::OnExit();
}
