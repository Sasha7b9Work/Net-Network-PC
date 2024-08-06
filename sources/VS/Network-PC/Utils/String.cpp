// 2022/03/30 08:25:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/String.h"
#include "Display/Font.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>


template      String<(int)DEFAULT_SIZE_STRING>::String(pchar, ...);
template      String<1024>::String(pchar, ...);
template int  String<(int)DEFAULT_SIZE_STRING>::Draw(int, int, Color);
template void String<(int)DEFAULT_SIZE_STRING>::Append(pchar);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawInCenterRect(int x, int y, int width, int height, Color);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawWithLimitation(int x, int y, Color color, int limitX, int limitY, int limitWidth,
    int limitHeight);
template void String<(int)DEFAULT_SIZE_STRING>::DrawRelativelyRight(int xRight, int y, Color);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawOnBackground(int x, int y, Color colorBackground);
template void String<(int)DEFAULT_SIZE_STRING>::DrawInRect(int x, int y, int width, int height, int dy);
template void String<(int)DEFAULT_SIZE_STRING>::SetFormat(pchar format, ...);
template void String<1024>::SetFormat(pchar format, ...);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground,
    Color colorFill);
template int  String<(int)DEFAULT_SIZE_STRING>::DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height,
    Color colorBackground, Color colorFill);
template void String<(int)DEFAULT_SIZE_STRING>::Append(char);
template void String<(int)DEFAULT_SIZE_STRING>::DrawInCenterRectOnBackground(int x, int y, int width, int height, Color colorText,
    int widthBorder, Color colorBackground);
template bool String<(int)DEFAULT_SIZE_STRING>::ToInt(int *);
template void String<(int)DEFAULT_SIZE_STRING>::AppendBytes(const void *, int);

template int  String<(int)1024>::Draw(int, int, Color);
template int  String<(int)1024>::DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBackground,
    Color colorFill);


template<int capa>
int String<capa>::Draw(int x, int, Color color)
{
    color.SetAsCurrent();

//    Display::memDC.SetFont(Font::Get());

//    Display::memDC.DrawText(buffer, x, y);

    return x;
}


template<int capacity>
void String<capacity>::SetFormat(pchar format, ...)
{
    std::va_list args;
    va_start(args, format);
    int num_symbols = std::vsprintf(buffer, format, args);
    va_end(args);

    if(num_symbols < 0 || num_symbols > capacity - 1)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, num_symbols);
    }
}


template<int capacity>
String<capacity>::String(pchar format, ...)
{
    std::va_list args;
    va_start(args, format);
    int num_symbols = std::vsprintf(buffer, format, args);
    va_end(args);

    if(num_symbols < 0 || num_symbols > capacity - 1)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, num_symbols);
    }
}


template<int capacity>
void String<capacity>::Append(pchar str)
{
    int need_size = Size() + (int)std::strlen(str) + 1;

    if (need_size > capacity)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, need_size);

        int pointer = Size();

        while (pointer < capacity)
        {
            Append(str[pointer++]);
        }

        buffer[capacity - 1] = '\0';

        LOG_WRITE(buffer);
    }
    else
    {
        std::strcat(buffer, str);
    }
}


template<int capacity>
void String<capacity>::AppendBytes(const void *bytes, int num_bytes)
{
    int need_size = Size() + num_bytes + 1;

    if (need_size > capacity)
    {
        LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, need_size);
        LOG_WRITE(c_str());
    }
    else
    {
        std::memcpy(buffer + Size(), bytes, (uint)num_bytes);
        buffer[need_size - 1] = '\0';
    }
}


template<int capacity>
void String<capacity>::Append(char symbol)
{
    if (!Filled())
    {
        int pos = (int)std::strlen(buffer);
        buffer[pos] = symbol;
        buffer[pos + 1] = '\0';
    }
    else
    {
        LOG_ERROR_TRACE("buffer is full");
    }
}


template<int capacity>
int String<capacity>::DrawInCenterRect(int eX, int , int , int , Color )
{
    return eX;
}


template<int capacity>
int String<capacity>::DrawWithLimitation(int x, int , Color , int , int , int , int )
{
    return x;
}


template<int capacity>
void String<capacity>::DrawRelativelyRight(int , int , Color )
{
}


template<int capacity>
int String<capacity>::DrawOnBackground(int x, int , Color )
{
    return x;
}


template<int capacity>
void String<capacity>::DrawInRect(int , int , int , int, int )
{
}


template<int capacity>
int String<capacity>::GetLenghtSubString(char *text)
{
    int result = 0;

    while (((*text) != ' ') && ((*text) != '\0'))
    {
        result += Font::GetLengthSymbol((uint8)*text);
        text++;
        result++;
    }

    return result;
}


template<int capacity>
int String<capacity>::DrawSubString(int x, int y, char *text)
{
    int numSymbols = 0;

    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = Char(*text).Draw(x, y);
        numSymbols++;
        text++;
        x++;
    }

    return numSymbols;
}


template<int capacity>
int String<capacity>::DrawSpaces(int x, int y, char *text, int *numSymbols)
{
    *numSymbols = 0;

    while (*text == ' ')
    {
        x = Char(*text).Draw(x, y);
        text++;
        (*numSymbols)++;
    }

    return x;
}


template<int capacity>
int String<capacity>::DrawStringInCenterRectAndBoundIt(int x, int , int , int , Color ,
    Color )
{
    return x;
}


template<int capacity>
int String<capacity>::DrawInBoundedRectWithTransfers(int x, int , int , Color , Color )
{
    return x;
}


template<int capacity>
void String<capacity>::DrawInCenterRectOnBackground(int , int , int , int , Color ,
    int , Color )
{
}


template<int capacity>
bool String<capacity>::ToInt(int *out)
{
    char *str = buffer;

    int sign = str[0] == '-' ? -1 : 1;

    if (str[0] < '0' || str[0] > '9')
    {
        ++str;
    }

    int length = (int)(std::strlen(str));

    if (length == 0)
    {
        return false;
    }

    *out = 0;

    int pow = 1;

    for (int i = length - 1; i >= 0; i--)
    {
        int val = str[i] & (~(0x30));
        if (val < 0 || val > 9)
        {
            return false;
        }
        *out += val * pow;
        pow *= 10;
    }

    if (sign == -1)
    {
        *out *= -1;
    }

    return true;
}
