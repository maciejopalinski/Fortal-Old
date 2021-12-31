#include "ErrorHandler.h"
#include "../Utils/Colors.h"
#include "../Utils/Format.h"

ErrorHandler::ErrorHandler(ReportingLevel level)
{
    setReportingLevel(level);
}

void ErrorHandler::setReportingLevel(ReportingLevel level)
{
    this->reporting_level.setLevel(level);
}

const char *ErrorHandler::getPrefix(ReportingLevel flag)
{
    if (flag == E_FATAL) return COLOR_BOLD COLOR_RED "[FATAL]" COLOR_RESET;
    else if (flag == E_ERROR) return COLOR_BOLD COLOR_RED "[ERROR]" COLOR_RESET;
    else if (flag == E_WARN)  return COLOR_BOLD COLOR_MAGENTA "[WARN]" COLOR_RESET;
    else if (flag == E_INFO)  return COLOR_BOLD COLOR_CYAN "[INFO]" COLOR_RESET;
    else if (flag == E_EXTRA) return COLOR_GREEN "[EXTRA]" COLOR_RESET;
    else if (flag == E_DEBUG) return COLOR_LIGHT_BLACK "[DEBUG]" COLOR_RESET;
    else return "(null)";
}

FILE *ErrorHandler::getStream(ReportingLevel flag)
{
    if (flag == E_FATAL || flag == E_ERROR || flag == E_WARN)
    {
        return stderr;
    }
    else return stdout;
}

void ErrorHandler::terminate_compilation(int exit_code)
{
    log(E_FATAL, "Compilation terminated");
    exit(exit_code);
}

void ErrorHandler::throw_no_input_files()
{
    log(E_FATAL, "No input files");
    terminate_compilation();
}

void ErrorHandler::log_no_such_file_or_directory(const char *filename)
{
    log(E_FATAL, "%s: No such file or directory", filename);
}

void ErrorHandler::throw_invalid_token(Location location, const char c)
{
    logLocation(E_ERROR, location, "invalid token '%c'", c);
    terminate_compilation();
}

void ErrorHandler::throw_unexpected_token(Location location, string custom_message)
{
    logLocation(
        E_ERROR,
        location,
        custom_message.c_str()
    );

    terminate_compilation();
}

void ErrorHandler::throw_unexpected_token(Location location, string unexpected, string expected)
{
    return
        throw_unexpected_token(
            location,
            format(
                "unexpected %s, expected %s",
                unexpected.c_str(),
                expected.c_str()
            )
        );
}

void ErrorHandler::log_empty_character_literal(Location location)
{
    logLocation(
        E_WARN, location, "empty string literal is not allowed, assuming it is integer 0");
}