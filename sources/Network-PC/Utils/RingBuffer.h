// 2023/10/16 10:59:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Communicator/Communicator.h"


template <class T, int capacity>
class RingBuffer
{
public:
    RingBuffer() : num_elements(0), index_in(0), index_out(0) { }

    void Append(T elem)
    {
        if (IsFull())
        {
            LOG_ERROR("Can not append element");
            return;
        }

        data[index_in] = elem;
        index_in++;

        if (index_in == capacity)
        {
            index_in = 0;
        }

        num_elements++;
    }

    void Append(T *buffer, int size)
    {
        for (int i = 0; i < size; i++)
        {
            Append(*buffer++);
        }
    }

    T Pop()
    {
        if (GetElementCount() == 0)
        {
            LOG_ERROR("Empty buffer");
            return T(0);
        }

        num_elements--;

        T result = data[index_out++];

        if (index_out == capacity)
        {
            index_out = 0;
        }

        return result;
    }

    T operator[](int i) const
    {
        if (i >= GetElementCount())
        {
            LOG_ERROR("Invalid index");
            return T(0);
        }

        int index = index_out + i;

        if (index >= capacity)
        {
            index -= capacity;
        }

        return data[index];
    }

    int GetElementCount() const
    {
        return num_elements;
    }

    bool IsFull() const
    {
        return (GetElementCount() == capacity);
    }

    void GetData(T *buffer, int size)
    {
        for (int i = 0; i < size; i++)
        {
            *buffer++ = Pop();
        }
    }

    void RemoveFirst(int num)
    {
        for (int i = 0; i < num; i++)
        {
            Pop();
        }
    }

private:
    T data[capacity];
    int num_elements;
    int index_in;
    int index_out;
};
