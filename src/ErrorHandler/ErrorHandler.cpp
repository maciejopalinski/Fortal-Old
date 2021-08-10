#include "ErrorHandler.h"

ErrorHandler::ErrorHandler(ReportingLevel level)
{
    this->reporting_level.setLevel(level);
}

ErrorLevel ErrorHandler::setReportingLevel(ReportingLevel level)
{
    this->reporting_level.setLevel(level);
    return this->reporting_level;
}

const char *ErrorHandler::getPrefix(ReportingLevel flag)
{
         if (flag == E_FATAL) return COLOR_BOLD COLOR_RED     "[FATAL] " COLOR_RESET;
    else if (flag == E_ERROR) return COLOR_BOLD COLOR_RED     "[ERROR] " COLOR_RESET;
    else if (flag == E_WARN)  return COLOR_BOLD COLOR_MAGENTA "[WARN] "  COLOR_RESET;
    else if (flag == E_INFO)  return COLOR_BOLD COLOR_CYAN    "[INFO] "  COLOR_RESET;
    else if (flag == E_EXTRA) return COLOR_GREEN              "[EXTRA] " COLOR_RESET;
    else if (flag == E_DEBUG) return COLOR_LIGHT_BLACK        "[DEBUG] " COLOR_RESET;
    else return "";
}

FILE *ErrorHandler::getStream(ReportingLevel flag)
{
    if (flag == E_FATAL || flag == E_ERROR || flag == E_WARN)
    {
        return stderr;
    }
    else return stdout;
}

void ErrorHandler::log(ReportingLevel level, const char *format, va_list args)
{
    if (reporting_level.testFlag(level))
    {
        size_t format_size = strlen(format);

        auto prefix = getPrefix(level);
        size_t prefix_size = strlen(prefix);

        char *buffer = new char[format_size + prefix_size + 1];
        strcpy(buffer, prefix);
        strcat(buffer, format);

        auto stream = getStream(level);

        vfprintf(stream, buffer, args);
        delete[] buffer;
    }
}

void ErrorHandler::log(ReportingLevel level, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    log(level, format, args);
    va_end(args);
}

void ErrorHandler::logLocation(ReportingLevel level, Location *location, const char *format, va_list args)
{
    char *location_buffer = new char[location->filename_size + 64];
    int location_size = location->write(location_buffer);
    if (location_size < 0)
    {
        delete[] location_buffer;
        return;
    };

    size_t format_size = strlen(format);
    char *buffer = new char[format_size + location_size + 8];
    sprintf(buffer, "%s: %s", location_buffer, format);

    log(level, buffer, args);

    delete[] location_buffer;
    delete[] buffer;
}

void ErrorHandler::logLocation(ReportingLevel level, Location *location, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logLocation(level, location, format, args);
    va_end(args);
}

void ErrorHandler::terminate_compilation(int exit_code)
{
    log(E_FATAL, "Compilation terminated\n");
    exit(exit_code);
}

void ErrorHandler::no_input_files()
{
    log(E_FATAL, "No input files\n");
    terminate_compilation();
}

void ErrorHandler::no_such_file_or_directory(const char *filename)
{
    log(E_FATAL, "%s: No such file or directory\n", filename);
}

void ErrorHandler::file_read_error(const char *filename)
{
    log(E_FATAL, "%s: File read error\n", filename);
    terminate_compilation();
}

void ErrorHandler::invalid_token(Location *location, const char c)
{
    logLocation
    (
        E_ERROR,
        location,
        "invalid token '%c'\n",
        c
    );
    terminate_compilation();
}

void ErrorHandler::unexpected_token(Location *location, const char *expected, const char *unexpected)
{
    logLocation
    (
        E_ERROR,
        location,
        "unexpected '%s', expected '%s'\n",
        unexpected, expected
    );
}

void ErrorHandler::assume_multichar_is_string(Token *token)
{
    if (token->type == TOKEN_LITERAL)
    {
        logLocation
        (
            E_WARN,
            token->location,
            "assuming multi-character character literal meant to be a string literal\n"
        );
    }
}