// 2022/08/24 15:11:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// Память выделяется с дискретностью size_chunk
template<int size_chunk>
class DynamicBuffer
{
public:
    DynamicBuffer();

    void Append(uint8 *data, int size);

    int Size() const
    {
        if (!buffer)
        {
            return 0;
        }

        return last - first;
    }

    uint8 *Data() { return buffer + first; }

    // Перевыделить память. При этом данные из буфера сохраняются
    void Realloc(int size);

    // Убрать num_bytes байт из начала буфера
    void RemoveFirst(int num_bytes);

    // Уплотнить буфер (убрать из начала пустые байты)
    void Compact();

private:

    uint8 *buffer;      // Здесь хранятся данные. 0, если память не выделена
    int    capacity;    // Столько байт выделено.
    int    first;       // Первый действительный элемент
    int    last;        // Последний действительный элемент
};
