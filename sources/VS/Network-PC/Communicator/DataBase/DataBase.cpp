// 2024/09/19 12:11:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/DataBase/DataBase.h"
#include <mysqlx/xapi.h>


bool DataBase::Connect(char *address)
{
    mysqlx_error_t *error;
    mysqlx_session_t *session = mysqlx_get_session_from_url(address, &error);

    return false;
}
