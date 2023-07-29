/** @file parser.c
 *  @brief Implementation of framework program
 *
 *  @author Ngoc Kim Ngan Nguyen (ge23fak)
 */
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>

#include "../include/parser.h"
#include "../include/magicnumber.h"
#include "../include/inverse_sqrt.h"

const char *usage_msg = // Usage desciption
    "Usage: ./main [options] file_name      Calculate Fast Inverse Square Root of floating point numbers read from the file file_name given by the user\n"
    "or:    ./main [options] x1 x2 ...      Calculate Fast Inverse Square Root of an arbitrary amount of floating point numbers x1, x2, ... given by the user in terminal\n"
    "or:    ./main -t                       Run tests and exit\n"
    "or:    ./main -h                       Show help message and exit\n"
    "or:    ./main --help                   Show help message and exit\n"
    "Example: ./main -V1 1.0 100.0 caculates Fast Inverse Square Root of 1.0 and 100.0 respectively using function version 1\n";
;

const char *help_msg = // Decription of all options
    "Positional arguments:\n"
    "  file_name                    The input file that contains an arbitrary amount of floating point numbers.\n"
    "                               File name is not allowed to start with a number.\n"
    "  floating point numbers, ...  Arbitrary amount of floating point numbers\n"
    "\n"
    "Optional arguments:\n"
    "  -V X     The Fast Inverse Squareroot function version, one of {0, 1, 2} (default: X = 0)\n"
    "  -B X     Measure runtime of performing X (optional argument) loop iterations (default: X = 1), X should be greater than 0, if X is not present -B has to be the last argument\n"
    "  -d       Interpret the input numbers as double\n"
    "  -t       Run tests and exit\n"
    "  -m       Calculate Magic Number and print out to the console and exit program, no matter the other arguments except -d for Double MagicNumber\n"
    "  -h       Show help message (this text) and exit\n"
    "  --help   Show help message (this text) and exit\n";
;

// Print out usage description to the console
void print_usage(void)
{
    fprintf(stderr, "%s", usage_msg);
}

// Print out description of all options and program's usage to the console
void print_help(void)
{
    print_usage();
    fprintf(stderr, "\n%s", help_msg);
}

// Print out a brief explanation of program's usage and terminate the program with EXIT_FAILURE
void exit_failure(void)
{
    print_usage();
    exit(EXIT_FAILURE);
}

typedef union
{
    void (*fn_flt)(size_t, float *, float *);   // Function type for floats
    void (*fn_dbl)(size_t, double *, double *); // Function type for doubles
} Func;

struct Version
{
    const char *name; // Version name
    Func fn;          // Corresponding function to version name
};

const struct Version versions[][2] = { // Look-up table for functions
    {
        {"0", {.fn_flt = fastInvSqrt_flt}},
        {"1", {.fn_flt = fastInvSqrt_flt_V1}},
        // Add more options for float here
    },
    {
        {"0", {.fn_dbl = fastInvSqrt_dbl}},
        {"1", {.fn_dbl = fastInvSqrt_dbl_V1}},
        // Add more options for double here
    }};

// Return the function corresponding to data type (float if db = 0, double if db = 1) and version name
Func get_version(int db, const char *version_name)
{
    for (size_t i = 0; i < (sizeof *versions) / (sizeof **versions); i++)
    {
        const struct Version *ver = &versions[db][i];
        // check if AVX available here
        if (!strcmp(ver->name, version_name))
        {
            return ver->fn;
        }
    }
    fprintf(stderr, "The given function version -V%s is invalid.\n", version_name); // error message
    print_usage();
    exit(EXIT_FAILURE);
}

// Print out an array with the length n and type float (db = 0) or double (db = 1) to the console
void print_out(int db, size_t n, void *out)
{
    if (!db)
    {
        float *out_flt = out;
        for (size_t i = 0; i < n; i++)
        {
            printf("%10.10f ", out_flt[i]);
        }
        printf("\n");
    }
    else
    {
        double *out_dbl = out;
        for (size_t i = 0; i < n; i++)
        {
            printf("%10.10f ", out_dbl[i]);
        }
        printf("\n");
    }
}

// Count the total number of lines in the file given by path
size_t sizeReadFile(const char *path)
{
    // Declare the necessary variables
    FILE *file;     // File pointer
    char line[350]; // Line buffer
    int count = 0;  // Counter for the number of lines

    // Open file
    if (!(file = fopen(path, "r")))
    { // r: read file
        perror("Error opening file");
        return count;
    }

    // Check if file type and mode is regular
    struct stat sb;
    if (fstat(fileno(file), &sb) == -1)
    {
        fprintf(stderr, "Error retrieving file stats");
        goto cleanup;
    }
    if (!S_ISREG(sb.st_mode) || sb.st_size <= 0)
    {
        fprintf(stderr, "Error processing file: Not a regular file or invalid size \n");
        goto cleanup;
    }

    // Count the number of lines
    while (fgets(line, sizeof(line), file))
    {
        count++;
    }

cleanup: // Close file
    fclose(file);
    return count;
}

// Read numbers from a the file given by path and return a pointer to an array storing these numbers
void *readFile(int db, size_t count, const char *path)
{
    // Declare the necessary variables
    void *res = NULL;
    FILE *file;     // File pointer
    char line[350]; // Line buffer

    // Allocate memory for returned array
    size_t size = 4 * db + 4; // size = 4 if db = 0 (float); size = 8 if db = 1 (double)
    res = malloc(count * size);
    if (!res)
    {
        perror("Error allocating memory for output array"); // Error message
        exit_failure();
    };

    // Open file again to read the values.
    // Here we do not need to check if opening file succeeds or not, as it was already done in the method sizeReadFile(const char* path).
    file = fopen(path, "r");

    if (!db)
    { // Numbers are considered as float
        float *numbers = res;
        count = 0;
        char *endptr;

        // Read each line of the file until end-of-file or an error occurs and convert the read string to a float
        while (fgets(line, sizeof(line), file))
        {
            endptr = NULL;
            float xi = strtof(line, &endptr);
            if (endptr == line || (*endptr != '\n' && *endptr != '\0'))
            {
                fprintf(stderr, "%s could not be converted to float\n", line);
                free(res);
                res = NULL;
                goto cleanup;
            }
            else if (errno == ERANGE)
            {
                fprintf(stderr, "%s over- or underflows float\n", line);
                free(res);
                res = NULL;
                goto cleanup;
            }
            else if (xi <= 0.0L)
            { // Input is not a positive number
                fprintf(stderr, "%s is not positive\n", line);
                free(res);
                res = NULL;
                goto cleanup;
            }
            numbers[count++] = xi;
        }
    }
    else
    { // Numbers are considered as double
        double *numbers = res;
        file = fopen(path, "r");
        count = 0;
        char *endptr;

        // Read each line of the file until end-of-file or an error occurs convert the read string to a double
        while (fgets(line, sizeof(line), file))
        {
            endptr = NULL;
            double xi = strtod(line, &endptr);
            if (endptr == line || (*endptr != '\n' && *endptr != '\0'))
            {
                fprintf(stderr, "%s could not be converted to double\n", line);
                free(res);
                res = NULL;
                goto cleanup;
            }
            else if (errno == ERANGE)
            {
                fprintf(stderr, "%s over- or underflows double\n", line);
                free(res);
                res = NULL;
                goto cleanup;
            }
            else if (xi <= 0.0L)
            { // Input is not a positive number
                fprintf(stderr, "%s is not positive\n", line);
                free(res);
                res = NULL;
                goto cleanup;
            }
            numbers[count++] = xi;
        }
    }

cleanup: // Close file
    fclose(file);

    return res;
}

// Read floating point numbers directly from terminal and return a pointer to an array storing these numbers
void *readTerminal(int db, int argc, char *argv[])
{
    // Note: optind does not need to be passed as a function argument because library getopt.h is already included.

    size_t n = argc - optind; // size of the input array
    size_t size = 4 * db + 4; // size = 4 if db = 0 (float); size = 8 if db = 1 (double)

    // Allocate memory for input array
    void *vals = malloc(n * size);
    if (!vals)
    {
        perror("Error allocating memory for output array"); // Error message
        exit_failure();
    };

    float *vals_flt = vals;
    double *vals_dbl = vals;

    if (!db)
    { // Numbers are considered as float
        while (optind < argc)
        {
            char *endptr;
            char *arg = argv[optind++];
            errno = 0; // Is a thread variable, therefore contain last previous so set zero before strtof call
            float xi = strtof(arg, &endptr);
            if (endptr == arg || *endptr != '\0')
            {
                fprintf(stderr, "%s could not be converted to float\n", arg);
                free(vals);
                exit_failure();
            }
            else if (errno == ERANGE)
            {
                fprintf(stderr, "%s over- or underflows float\n", arg);
                free(vals);
                exit_failure();
            }
            else if (xi <= 0.0L)
            { // Input is not a positive number
                fprintf(stderr, "%s is not positive\n", arg);
                free(vals);
                exit_failure();
            }
            *vals_flt++ = xi;
        }
    }
    else
    { // Numbers are considered as double
        while (optind < argc)
        {
            char *endptr;
            char *arg = argv[optind++];
            errno = 0; // Is a thread variable, therefore contain last previous so set zero before strtod call
            double xi = strtod(arg, &endptr);
            if (endptr == arg || *endptr != '\0')
            {
                fprintf(stderr, "%s could not be converted to double\n", arg);
                free(vals);
                exit_failure();
            }
            else if (errno == ERANGE)
            {
                fprintf(stderr, "%s over- or underflows double\n", arg);
                free(vals);
                exit_failure();
            }
            else if (xi <= 0.0L)
            { // Input is not a positive number
                fprintf(stderr, "%s is not positive\n", arg);
                free(vals);
                exit_failure();
            }
            *vals_dbl++ = xi;
        }
    }
    return vals;
}

// Set up method to measure runtime
static inline double curtime(void)
{
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

// Execute the function specified by version_name and type float/double with arguments n, vals in "loop" iterations
double execute(int db, const char *version_name, size_t n, void *vals, int loop)
{
    Func fun = get_version(db, version_name); // Get the function specified by version_name and type float (db = 0) / double (db = 1)
    double start, end;
    size_t size = 4 * db + 4; // size = 4 if db = 0 (float); size = 8 if db = 1 (double)

    // Allocate memory for output array
    void *out = malloc(n * size);
    if (!out)
    {
        perror("Error allocating memory for output array"); // Error message
        exit_failure();
    };

    // Print out input array
    print_out(db, n, vals);

    start = curtime();
    for (long i = 0; i < loop; i++)
    {
        fun.fn_flt(n, vals, out);
    }
    end = curtime();

    // Print out output array
    print_out(db, n, out);

    free(out); // Release memory space allocated to input array

    return end - start;
}