#pragma once
#include <memory>
#include <vector>
#include <string>
using std::string;
using std::vector;
using std::unique_ptr;

template<class... Args>
string format(const char *format, Args... args)
{
    int size = snprintf(nullptr, 0, format, args...);

    unique_ptr<char[]> buf(new char[size + 1]);

    snprintf(buf.get(), size + 1, format, args...);

    return string(buf.get(), buf.get() + size);
}

vector<string> split_string(string s, string delimeter = " ");

string join_string(vector<string> v, string delimeter);