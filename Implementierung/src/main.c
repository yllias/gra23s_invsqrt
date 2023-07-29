/** @file main.c
 *  @brief Implementation of main program
 *
 *  @author Ngoc Kim Ngan Nguyen (ge23fak)
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h> // use this library for getopt_long instead of <unistd.h> which is used for getopt
#include <errno.h>

#include "../include/inverse_sqrt.h"
#include "../include/magicnumber.h"
#include "../include/parser.h"
#include "../include/tests.h"

int main(int argc, char *argv[])
{
    if (argc == 1)
    { // There are no optional and positional arguments.
        exit_failure();
    }

    // Define and initialise standard values
    char *version_name = "0"; // Basic version
    int db = 0;               // db = 1 if option -d is set, otherwise 0
    int b = 0;                // b = 1 if option -B is set, otherwise 0
    int m = 0;                // m = 1 if option -m is set, otherwise 0
    long loop = 1;            // Number of loop iterations to measure runtime if option -B is set
    void *vals;               // Input array
    size_t n;                 // Size of the input array

    struct option long_options[] = {
        // Define long option --help
        {"help", no_argument, 0, 'h'},
    };

    int c;
    while ((c = getopt_long(argc, argv, "V:B::dmht", long_options, NULL)) != -1)
    {
        switch (c)
        {
        case 'V': // Choose an Inverse Squareroot function version
            version_name = optarg;
            break;
        case 'B': // Measure runtime of performing optarg loop iterations
            b = 1;
            if (optarg == NULL && optind < argc && argv[optind][0] != '-')
            {
                optarg = argv[optind++];
            }
            if (optarg != NULL)
            {
                char *endptr;
                loop = strtol(optarg, &endptr, 10);
                if (endptr == optarg || *endptr != '\0')
                {
                    fprintf(stderr, "%s could not be converted to long\n", optarg);
                    exit_failure();
                }
                else if (errno == ERANGE)
                {
                    fprintf(stderr, "%s over- or underflows long\n", optarg);
                    exit_failure();
                }
                else if (loop <= 0) {
                    fprintf(stderr, "Number of iterations %ld is not greater than 0\n", loop);
                    exit_failure();
                }
            }
            break;
        case 'd': // Interpret input values as double
            db = 1;
            break;
        case 'm': // Calculate and print magic number
            m = 1;
            break;
        case 'h': // Show help message and exit
            print_help();
            return EXIT_SUCCESS;
        case 't':
            runTests();
            return EXIT_SUCCESS;
        case '?': // Unknown options, show usage message and exit
            print_usage();
            return EXIT_FAILURE;
        default:
            abort();
        }
    }

    // If option -m is set, print out the calculated magic number corresponding to type float/double and terminate the program.
    // Options other than -m and -d are ignored.
    if (m)
    {
        print_magicnumber(db);
        return EXIT_SUCCESS;
    }

    // Positional argument is either an arbitrary amount of floating point numbers or a file name, in which floating point numbers are given.
    // Positional argument muss be given in terminal.
    if (optind == argc)
    { // There are no positional arguments.
        fprintf(stderr, "Missing arguments\n");
        exit_failure();
    }

    if (optind == argc - 1)
    {
        // There is only one positional argument, which could either be a single number or a filename.
        // The filename may not start with a number. If this is the case, we interpret the argument as a number and jump to the command line "terminal".
        // Otherwise, we consider the argument as a filename.
        char *endptr;
        strtol(argv[optind], &endptr, 10);
        if (endptr != argv[optind])
            goto terminal; // Check whether the filename starts with a number

        n = sizeReadFile(argv[optind]); // Count the total number of lines in the given file and allocate this to the size of input array n.
        if (!n)
            exit_failure();

        vals = readFile(db, n, argv[optind]); // Allocate floating point numbers read from the given file to the input array
        if (!vals)
            exit_failure();

        goto execute;
    }

// Positional arguments are interpreted as floating point numbers here.
terminal:
    n = argc - optind;                   // Allocate the amount of positional arguments to the size of input array n.
    vals = readTerminal(db, argc, argv); // Allocate floating point numbers read directly from terminal to the input array

// Calculate the inverse square root based on selected options and measure runtime
execute:
    double time2 = execute(db, version_name, n, vals, loop);

    if (b)
    { // Print out the measured runtime if option -B is set
        printf("Runtime in %ld loops: %f\n", loop, time2);
    }

    free(vals); // Release memory space allocated to input array

    return EXIT_SUCCESS;
}