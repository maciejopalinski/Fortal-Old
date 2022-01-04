#include <argp.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

#include "ErrorHandler/ErrorHandler.h"
#include "Compiler/Compiler.h"
#include "Version.h"
#include "Package/Package.h"

const char *argp_program_version     = "Fortal " FORTAL_VERSION;
const char *argp_program_bug_address = "https://github.com/PoProstuMieciek/Fortal/issues";

const char *argp_program_doc = "Fortal -- compiler for Fortal programming language";
const char *argp_args_doc    = "FILE...";

argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output", 0 },
    {  "quiet", 'q', 0, 0, "Produce reduced output", 0 },
    {  "debug", 'D', 0, 0, "Produce debug output (for development)", 0 },
    { "output", 'o', "FILE", 0, "Place the output into FILE", 0 },
    0
};

struct arguments
{
    vector<string> filenames;
    ReportingLevel reporting_level;
    string output_file;
};

error_t parse_opt(int key, char *value, argp_state *state)
{
    arguments *args = (arguments *) state->input;

    switch (key)
    {
        case 'v':
            args->reporting_level = E_VERBOSE;
            break;

        case 'q':
            args->reporting_level = E_QUIET;
            break;

        case 'D':
            args->reporting_level = E_ALL;
            break;

        case 'o':
            args->output_file = string(value);
            break;

        case ARGP_KEY_ARG:
            args->filenames.push_back(string(value));
            break;

        case ARGP_KEY_END:
            if (state->arg_num < 1) argp_usage(state); // Not enough arguments
            break;

        default:
            return ARGP_ERR_UNKNOWN;
            break;
    }
    return 0;
}

argp config = { options, parse_opt, argp_args_doc, argp_program_doc };

int main(int argc, char **argv)
{
    arguments args;
    args.reporting_level = E_DEFAULT;
    argp_parse(&config, argc, argv, 0, 0, &args);

    ErrorHandler error_handler;
    error_handler.setReportingLevel(args.reporting_level);

    Compiler compiler(error_handler);

    for (auto filename : args.filenames)
    {
        compiler.addFile(filename);
    }

    compiler.compile();

    error_handler.log(E_EXTRA, "Exiting...");
    return 0;
}