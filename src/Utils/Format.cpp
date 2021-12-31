#include "Format.h"

vector<string> split_string(string s, string delimeter)
{
    vector<string> output;

    size_t pos = 0;
    string token;
    while ((pos = s.find(delimeter)) != string::npos)
    {
        token = s.substr(0, pos);
        output.push_back(token);
        s.erase(0, pos + delimeter.length());
    }
    if (!s.empty()) output.push_back(s);

    return output;
}

string join_string(vector<string> v, string delimeter)
{
    string output;

    if (!v.empty())
    {
        size_t i;
        for (i = 0; i < v.size() - 1; i++)
        {
            string elem = v[i];
            output += elem;
            output += delimeter;
        }
        output += v[i];
    }

    return output;
}