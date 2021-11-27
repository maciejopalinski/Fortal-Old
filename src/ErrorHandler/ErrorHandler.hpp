#pragma once
#include <stdio.h>
using namespace std;

#include "ErrorLevel.hpp"
#include "../Utils/Colors.h"
#include "../Utils/Format.hpp"
#include "../Location/Location.hpp"
#include "../Token/TokenBase.hpp"

class ErrorHandler
{
    public:
        ErrorHandler(ReportingLevel level = E_DEFAULT)
        {
            setReportingLevel(level);
        }

        ErrorLevel reporting_level = ErrorLevel();

        void setReportingLevel(ReportingLevel level = E_DEFAULT)
        {
            this->reporting_level.setLevel(level);
        }

        const char *getPrefix(ReportingLevel flag)
        {
                 if (flag == E_FATAL) return COLOR_BOLD COLOR_RED     "[FATAL]" COLOR_RESET;
            else if (flag == E_ERROR) return COLOR_BOLD COLOR_RED     "[ERROR]" COLOR_RESET;
            else if (flag == E_WARN)  return COLOR_BOLD COLOR_MAGENTA "[WARN]"  COLOR_RESET;
            else if (flag == E_INFO)  return COLOR_BOLD COLOR_CYAN    "[INFO]"  COLOR_RESET;
            else if (flag == E_EXTRA) return COLOR_GREEN              "[EXTRA]" COLOR_RESET;
            else if (flag == E_DEBUG) return COLOR_LIGHT_BLACK        "[DEBUG]" COLOR_RESET;
            else return "";
        }

        FILE *getStream(ReportingLevel flag)
        {
            if (flag == E_FATAL || flag == E_ERROR || flag == E_WARN)
            {
                return stderr;
            }
            else return stdout;
        }

        template<class ... Args>
        void log(ReportingLevel level, const char *f1, Args ... args)
        {
            if (reporting_level.testFlag(level))
            {
                auto stream = getStream(level);
                auto prefix = getPrefix(level);
                string f2 = format("%s %s\n", prefix, f1);

                fprintf(stream, f2.c_str(), args...);
            }
        }

        template<class ... Args>
        void logLocation(ReportingLevel level, Location location, const char *f1, Args ... args)
        {
            string location_str = location.getPrefix();
            string f2 = format("%s: %s", location_str.c_str(), f1);

            log(level, f2.c_str(), args...);
        }

        void terminate_compilation(int exit_code = 1)
        {
            log(E_FATAL, "Compilation terminated");
            exit(exit_code);
        }

        void throw_no_input_files()
        {
            log(E_FATAL, "No input files");
            terminate_compilation();
        }

        void log_no_such_file_or_directory(string filename)
        {
            log(E_FATAL, "%s: No such file or directory", filename.c_str());
        }

        void throw_invalid_token(Location location, const char c)
        {
            logLocation
            (
                E_ERROR,
                location,
                "invalid token '%c'",
                c
            );
            terminate_compilation();
        }

        void log_unexpected_token(Location location, string expected, string unexpected)
        {
            logLocation
            (
                E_ERROR,
                location,
                "unexpected '%s', expected '%s'",
                unexpected.c_str(), expected.c_str()
            );
        }
};