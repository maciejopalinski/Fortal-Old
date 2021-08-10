#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
using namespace std;

#include "ErrorLevel.h"
#include "../Token/Token.h"
#include "../Utils/Location.h"
#include "../Utils/Colors.h"

class ErrorHandler
{
    public:
        ErrorHandler(ReportingLevel level = E_DEFAULT);

        ErrorLevel reporting_level = ErrorLevel();
        ErrorLevel setReportingLevel(ReportingLevel level = E_DEFAULT);

        const char *getPrefix(ReportingLevel flag);
        FILE *getStream(ReportingLevel flag);

        void log(ReportingLevel level, const char *format, va_list args);
        void log(ReportingLevel level, const char *format, ...);

        void logLocation(ReportingLevel level, Location *location, const char *format, va_list args);
        void logLocation(ReportingLevel level, Location *location, const char *format, ...);

        void terminate_compilation(int exit_code = 1);

        void no_input_files();
        void no_such_file_or_directory(const char *filename);
        void file_read_error(const char *filename);

        void invalid_token(Location *location, const char c);
        void unexpected_token(Location *location, const char *expected, const char *unexpected);

        void assume_multichar_is_string(Token *token);
};

extern ErrorHandler error_handler;