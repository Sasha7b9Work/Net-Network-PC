// 2022/08/24 15:11:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/DynamicBuffer.h"
#include <cstring>


template      DynamicBuffer<SIZE_MESSAGE>::DynamicBuffer();
template void DynamicBuffer<SIZE_MESSAGE>::Append(uint8 *, int);
template void DynamicBuffer<SIZE_MESSAGE>::RemoveFirst(int);


template      DynamicBuffer<128>::DynamicBuffer();
template void DynamicBuffer<128>::Append(uint8 *, int);
template void DynamicBuffer<128>::RemoveFirst(int);


template<int size_chunk>
DynamicBuffer<size_chunk>::DynamicBuffer() : buffer(nullptr), capacity(0), first(0), last(0)
{
    Realloc(size_chunk);
}


template<int size_chunk>
void DynamicBuffer<size_chunk>::Append(uint8 *data, int size)
{
    while (last + size > capacity)
    {
        Realloc(capacity + size_chunk);
    }

    std::memcpy(buffer + last, data, size);
    last += size;
}


template<int size_chunk>
void DynamicBuffer<size_chunk>::Realloc(int size)
{
    uint8 *new_buffer = new uint8[size];

    if (buffer)
    {
        if (Size() != 0)
        {
            int old_size = Size();

            std::memcpy(new_buffer, Data(), Size());

            first = 0;
            last = old_size;
        }
        else
        {
            first = 0;
            last = 0;
        }

        delete [] buffer;

    }

    buffer = new_buffer;

    capacity = size;
}


template<int size_chunk>
void DynamicBuffer<size_chunk>::RemoveFirst(int num_bytes)
{
    if (Size() == 0)
    {
        return;
    }

    if (num_bytes > Size())
    {
        num_bytes = Size();
    }

    first += num_bytes;

    Compact();
}


template<int size_chunk>
void DynamicBuffer<size_chunk>::Compact()
{
    if (!buffer || first == 0)
    {
        return;
    }

    std::memcpy(buffer, buffer + first, last - first);

    int num_bytes = first;

    last -= num_bytes;
    first -= num_bytes;
}
