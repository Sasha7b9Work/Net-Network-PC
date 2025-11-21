// 2024/08/10 11:50:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SU
{
    void SplitToWords(const wxString &, std::vector<wxString> &);

    // Оставить number последних символов от in. Если строка меньше, то в начале добавить пробелы
    pchar LeaveTheLastOnes(pchar in, int number);
}
