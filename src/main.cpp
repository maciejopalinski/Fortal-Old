#include <argp.h>
#include "ErrorHandler/ErrorHandler.h"
#include "Token/Token.h"
#include "Lexer/Lexer.h"
#include "Utils/Utils.h"

const char *argp_program_version = "Fortal 1.0.0";
const char *argp_program_bug_address = "https://github.com/PoProstuMieciek/Fortal/issues";

const char *argp_program_doc = "Fortal -- compiler for C/C++ programming language";
const char *argp_args_doc = "FILE...";

argp_option options[] =
{
    { "verbose", 'v', 0, 0, "Produce verbose output" },
    { "quiet", 'q', 0, 0, "Produce reduced output" },
    { "debug", 'D', 0, 0, "Produce debug output (for development)" },

    { "output", 'o', "FILE", 0, "Place the output into FILE" },
    { 0 }
};

struct arguments
{
    const char *args[16] = { 0 };
    ReportingLevel reporting_level;
    const char *output_file;
};

error_t parse_opt(int key, char *value, argp_state *state)
{
    const size_t required_args = 0, max_args = 8;
    arguments *args = (arguments*) state->input;

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
            args->output_file = value;
        break;

        case ARGP_KEY_ARG:
            if (state->arg_num >= max_args) argp_usage(state); // Too many arguments

            args->args[state->arg_num] = value;
        break;

        case ARGP_KEY_END:
            if (state->arg_num < required_args) argp_usage(state); // Not enough arguments
        break;

        default:
            return ARGP_ERR_UNKNOWN;
        break;
    }
    return 0;
}

argp config = { options, parse_opt, argp_args_doc, argp_program_doc };

ErrorHandler error_handler = ErrorHandler();
Lexer lexer = Lexer();

int main(int argc, char **argv)
{
    arguments args;

    // Default values.
    args.reporting_level = E_DEFAULT;
    args.output_file = nullptr;

    argp_parse(&config, argc, argv, 0, 0, &args);

    error_handler.setReportingLevel(args.reporting_level);
    lexer.loadFromFile(args.args[0]);

    error_handler.log
    (
        E_EXTRA,
        "args[0] = '%s', reporting_level = %i, output = '%s'\n",
        args.args[0],
        args.reporting_level,
        args.output_file
    );

    while (true)
    {
        Token token = Token();

        lexer.getNextToken(&token);
        if (!token) break;

        token.debug();
    }

    error_handler.log(E_EXTRA, "Exiting...\n");
    return 0;
}