#pragma once
#include <stdio.h>

#include "../Location/Location.h"
#include "ErrorLevel.h"

class ErrorHandler
{
    public:
        ErrorLevel reporting_level;

        ErrorHandler(ReportingLevel level = E_DEFAULT);

        void setReportingLevel(ReportingLevel level = E_DEFAULT);

        const char *getPrefix(ReportingLevel flag);

        FILE *getStream(ReportingLevel flag);

        template<class... Args>
        void log(ReportingLevel level, const char *f1, Args... args);

        template<class... Args>
        void logLocation(ReportingLevel level, Location location, const char *f1, Args... args);

        void terminate_compilation(int exit_code = 1);

        void throw_no_input_files();

        void log_no_such_file_or_directory(const char *filename);

        void throw_invalid_token(const Location &location, const char c);

        void throw_unexpected_eof(const Location &location);
        void throw_unexpected_token(const Location &location, string custom_message);
        void throw_unexpected_token(const Location &location, string unexpected, string expected);

        void log_empty_character_literal(const Location &location);
};

template<class... Args>
void ErrorHandler::log(ReportingLevel level, const char *f1, Args... args)
{
    if (reporting_level.testFlag(level))
    {
        auto stream = getStream(level);
        auto prefix = getPrefix(level);
        string f2   = format("%s %s\n", prefix, f1);

        fprintf(stream, f2.c_str(), args...);
    }
}

template<class... Args>
void ErrorHandler::logLocation(ReportingLevel level, Location location, const char *f1, Args... args)
{
    string location_str = location.getPrefix();
    string f2           = format("%s: %s", location_str.c_str(), f1);

    log(level, f2.c_str(), args...);
}