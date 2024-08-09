// 2024/08/08 21:06:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Config.h"


namespace Config
{
    static wxConfigBase *config = nullptr;
    static wxFileConfig *file_config = nullptr;

    static void SetFile(const wxString &);
}


void Config::Init()
{
    file_config = new wxFileConfig("", "", "config.cfg");

    SetFile("");
}


void Config::DeInit()
{

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
