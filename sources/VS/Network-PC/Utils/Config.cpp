// 2024/08/08 21:06:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Config.h"
#include "Settings/Settings.h"


namespace Config
{
    wxConfigBase *config = nullptr;
    static wxFileConfig *file_config = nullptr;

    static void SetFile(const wxString &);
}


void Config::Init()
{
    file_config = new wxFileConfig("", "", wxGetCwd() + "/config.cfg");

    SetFile("");
}


void Config::DeInit()
{
    wxConfigBase::Get(false)->Flush();
    wxConfigBase::Set(nullptr);

    delete file_config;

    file_config = nullptr;
}


void Config::SetFile(const wxString &file_path)
{
    static wxString current_file = "unimaginable name";

    if (current_file == file_path)
    {
        return;
    }

    current_file = file_path;

    static wxFileConfig *conf = nullptr;

    if (conf)
    {
        delete conf;
        conf = nullptr;
    }

    if (file_path.IsEmpty())
    {
        wxConfigBase::Set(file_config);
        config = wxConfigBase::Get(false);
    }
    else
    {
        conf = new wxFileConfig("", "", file_path);

        wxConfigBase::Set(conf);
        config = wxConfigBase::Get(false);
    }
}


void Config::Write(pchar key, const WindowParameters &params)
{
    Write(wxString(key) + "_rect", params.rect);
    Write(wxString(key) + "_shown", params.shown);
}


WindowParameters Config::Read(pchar key, const WindowParameters &def)
{
    wxRect rect = Read(wxString(key) + "_rect", def.rect);
    bool shown = Read(wxString(key) + "_shown", def.shown);

    return { rect, shown };
}
