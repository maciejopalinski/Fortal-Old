#pragma once
#include <memory>
#include <string>
using std::string;
using std::unique_ptr;

template<class... Args>
string format(const char *format, Args... args)
{
    int size = snprintf(nullptr, 0, format, args...);

    unique_ptr<char[]> buf(new char[size + 1]);

    snprintf(buf.get(), size + 1, format, args...);

    return string(buf.get(), buf.get() + size);
}