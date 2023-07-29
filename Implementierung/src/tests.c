/** @file tests.c
 *  @brief Implementation of tests and benchmarks
 *
 *  @author Yll Kryeziu (ge94noh)
 */

#define _POSIX_C_SOURCE 199309L
#define TRIALS 200
#define STEPS 500000
#define MAXINCREMENTS 20
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "../include/tests.h"
#include "../include/inverse_sqrt.h"

void basicFunctionality_flt()
{
    printf("Testing basic functionality of inverse sqrt for floats...\n");

    // Create array with samples and handle malloc failure
    float sample[] = {0.001f, 0.01f, FLT_MAX, FLT_MIN, 10.0f, 125.0f, 99.0f, 125.123567f, 0.1726835f, 199999.0f, 123675124.0f, 2.4f * 10e9};
    float *result = (float *)malloc(11 * sizeof(float));
    if (!result)
    {
        perror("Error allocating memory for result array");
        exit(EXIT_FAILURE);
    };

    printf("Sample:\n");
    for (size_t i = 0; i < 11; i++)
    {
        printf("%6.10f ", sample[i]);
    }
    printf("\n");

    for (size_t i = 0; i < 11; i++)
    {
        result[i] = 1.0f / sqrtf(sample[i]);
    }

    printf("Exact results:\n");
    for (size_t i = 0; i < 11; i++)
    {
        printf("%6.10f ", result[i]);
    }
    printf("\n");

    fastInvSqrt_flt_V1(11, sample, result);
    printf("Scalar results:\n");
    for (size_t i = 0; i < 11; i++)
    {
        printf("%6.10f ", result[i]);
    }
    printf("\n");

    fastInvSqrt_flt(11, sample, result);
    printf("SIMD results:\n");
    for (size_t i = 0; i < 11; i++)
    {
        printf("%6.10f ", result[i]);
    }
    printf("\n\n");

    free(result);
}
void basicFunctionality_dbl()
{
    printf("Testing basic functionality of inverse sqrt for doubles...\n");

    // Create array with samples and handle malloc failure
    double sample[] = {0.001, 25, DBL_MAX, DBL_MIN, 10.0, 125.0, 99.0, 125.123567, 0.1726835, 199999.0, 123675124.0, 2.4 * 10e99, 4.1256543e-5, 9.1236658, 453627189};
    double *result = (double *)malloc(15 * sizeof(double));
    if (!result)
    {
        perror("Error allocating memory for result array");
        exit(EXIT_FAILURE);
    };

    printf("Sample:\n");
    for (size_t i = 0; i < 15; i++)
    {
        printf("%6.10f ", sample[i]);
    }
    printf("\n");

    for (size_t i = 0; i < 15; i++)
    {
        result[i] = 1.0 / sqrt(sample[i]);
    }

    printf("Exact results:\n");
    for (size_t i = 0; i < 15; i++)
    {
        printf("%6.10f ", result[i]);
    }
    printf("\n");

    fastInvSqrt_dbl_V1(15, sample, result);
    printf("Scalar results:\n");
    for (size_t i = 0; i < 15; i++)
    {
        printf("%6.10f ", result[i]);
    }
    printf("\n");

    fastInvSqrt_dbl(15, sample, result);
    printf("SIMD results:\n");
    for (size_t i = 0; i < 15; i++)
    {
        printf("%6.10f ", result[i]);
    }
    printf("\n\n");

    free(result);
}
void plotRange()
{
    printf("Generating data for plotting curve and error...\n");
    printf("Results will be stored in ./benchmark_outputs/result_plot_flt.csv\n\n");

    /* Create arrays of size one to work around array input of function
    and handle failure of malloc and fopen by cleaning all previous allocations */
    float *sample = (float *)malloc(sizeof(float));
    if (!sample)
    {
        perror("Error allocating memory for sample array");
        exit(EXIT_FAILURE);
    };

    float *result = (float *)malloc(sizeof(float));
    if (!result)
    {
        perror("Error allocating memory for result array");
        free(sample);
        exit(EXIT_FAILURE);
    };

    FILE *file;
    if (!(file = fopen("./benchmark_outputs/results_plot_flt.csv", "w+")))
    {
        perror("Error opening file");
        free(sample);
        free(result);
        exit(EXIT_FAILURE);
    }

    fprintf(file, "val, actual, res, absError, relError\n"); // print header of .csv file

    float f = 0.0f;
    for (int i = 0; i < 1000; i++)
    {
        f = (f * 100 + 1) / 100; // deals with floating point error, effectively adding 0.01 to f 1000 times
        sample[0] = f;
        fastInvSqrt_flt(1, sample, result);
        float reference = 1.0f / sqrtf(f);
        double error = reference - result[0];
        double relativeError = 100 * fabs(error) / reference; // calculate the relative error compared to native 1/sqrt()
        // print results to .csv for later plotting
        fprintf(file, "%f, %f, %f, %f, %f\n", sample[0], reference, result[0], error, relativeError);
    }

    fclose(file);
    free(sample);
    free(result);
}
void benchmarkAccuracy_flt()
{
    printf("Running benchmark for accuracy of inverse sqrt for floats...\n");

    /* Create arrays of size 4 to use for SIMD-optimized function
    and handle failure of malloc */
    double maxError = 0.0;
    float *sample = (float *)malloc(4 * sizeof(float));
    if (!sample)
    {
        perror("Error allocating memory for sample array");
        exit(EXIT_FAILURE);
    };
    float *result = (float *)malloc(4 * sizeof(float));
    if (!result)
    {
        perror("Error allocating memory for result array");
        free(sample);
        exit(EXIT_FAILURE);
    };

    // union for type-punning within defined behaviour
    union
    {
        float f;
        uint32_t x;
    } conv;
    maxError = 0.0;
    // Iterate through every possible float by incrementing the integer representation
    for (conv.x = 0x00800000; conv.x < 0x7f800000; conv.x++)
    {
        sample[0] = conv.f;
        fastInvSqrt_flt_V1(1, sample, result);
        float reference = 1.0f / sqrtf(conv.f);
        // Calculate relative error of implementation compared to 1/sqrt() and update maximum error
        double relativeError = 100 * fabs(reference - result[0]) / reference;
        maxError = relativeError > maxError ? relativeError : maxError;
    }
    printf("Scalar Fast Inverse Square Root maximum relative error: \t%10.10f %%\n", maxError);

    maxError = 0.0;
    // Same as routine as fastInvSqrt_flt_V1
    for (conv.x = 0x00800000; conv.x < 0x7f800000; conv.x++)
    {
        sample[0] = conv.f;
        fastInvSqrt_flt_DoubleNewton(1, sample, result);
        float reference = 1.0f / sqrtf(conv.f);
        double relativeError = 100 * fabs(reference - result[0]) / reference;
        maxError = relativeError > maxError ? relativeError : maxError;
    }
    printf("2x Newton Fast Inverse Square Root maximum relative error: \t%10.10f %%\n", maxError);

    maxError = 0.0;
    // Same routine as above but this time stop incrementing so that the inner for-loop doesnt go above biggest float
    for (conv.x = 0x00800000; conv.x + 4 < 0x7f800000; conv.x++)
    {
        // Inner loop fills array with next 4 values to make use of SIMD
        for (size_t i = 0; i < 4; i++)
        {
            sample[i] = conv.f;
            conv.x++;
        }
        fastInvSqrt_flt(4, sample, result);
        float reference;
        for (size_t i = 0; i < 4; i++)
        {
            reference = 1.0f / sqrtf(sample[i]);
            double relativeError = 100 * fabs(reference - result[i]) / reference;
            maxError = relativeError > maxError ? relativeError : maxError;
        }
    }
    printf("SIMD Fast Inverse Square Root maximum relative error: \t\t%10.10f %%\n", maxError);
    printf("\n");

    free(result);
    free(sample);
}
void benchmarkAccuracy_dbl()
{
    printf("Running benchmark for accuracy of inverse sqrt for doubles...\n");

    /* Create arrays of size 2 to use for SIMD-optimized function
        and handle failure of malloc */
    double maxError = 0.0;
    double *sample = (double *)malloc(2 * sizeof(double));
    if (!sample)
    {
        perror("Error allocating memory for sample array");
        exit(EXIT_FAILURE);
    };
    double *result = (double *)malloc(2 * sizeof(double));
    if (!result)
    {
        perror("Error allocating memory for result array");
        free(sample);
        exit(EXIT_FAILURE);
    };

    // union for type-punning within defined behaviour
    union
    {
        double d;
        uint64_t x;
    } conv;
    maxError = 0.0;
    /* Iterate through doubles by incrementing the integer representation,
    NOTE that integer representation is incremented by 1llu<<30 to reduce the amount of doubles to test */
    for (conv.x = 0x10000000000000; conv.x < 0x7ff0000000000000; conv.x += 1llu << 30)
    {
        sample[0] = conv.d;
        fastInvSqrt_dbl_V1(1, sample, result);
        double reference = 1.0 / sqrt(conv.d);
        // Calculate relative error of implementation compared to 1/sqrt() and update maximum error
        double relativeError = 100 * fabs(reference - result[0]) / reference;
        maxError = relativeError > maxError ? relativeError : maxError;
    }
    printf("Scalar Fast Inverse Square Root maximum relative error: \t%10.10f %%\n", maxError);

    maxError = 0.0;
    // Same as routine as fastInvSqrt_dbl_V1
    for (conv.x = 0x10000000000000; conv.x < 0x7ff0000000000000; conv.x += 1llu << 30)
    {
        sample[0] = conv.d;
        fastInvSqrt_dbl_DoubleNewton(1, sample, result);
        double reference = 1.0 / sqrt(conv.d);
        double relativeError = 100 * fabs(reference - result[0]) / reference;
        maxError = relativeError > maxError ? relativeError : maxError;
    }
    printf("2x Newton Fast Inverse Square Root maximum relative error: \t%10.10f %%\n", maxError);

    maxError = 0.0;
    // Same routine as above but this time stop incrementing so that the inner for-loop doesnt go above biggest double
    for (conv.x = 0x10000000000000; conv.x + 2 < 0x7ff0000000000000; conv.x += 1llu << 30)
    {
        // Inner loop fills array with next 2 values to make use of SIMD
        for (size_t i = 0; i < 2; i++)
        {
            sample[i] = conv.d;
            conv.x++;
        }
        fastInvSqrt_dbl(2, sample, result);
        double reference;
        for (size_t i = 0; i < 2; i++)
        {
            reference = 1.0 / sqrt(sample[i]);
            double relativeError = 100 * fabs(reference - result[i]) / reference;
            maxError = relativeError > maxError ? relativeError : maxError;
        }
    }
    printf("SIMD Fast Inverse Square Root maximum relative error: \t\t%10.10f %%\n", maxError);

    printf("\n");

    free(result);
    free(sample);
}
void benchmarkTime_flt_wrapper(int maxIncrements)
{
    FILE *file;
    if (!(file = fopen("./benchmark_outputs/results_speed_flt.csv", "w+")))
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Running runtime benchmark for float algorithms...\n");
        printf("Results will be stored in ./benchmark_outputs/results_speed_flt.csv\n\n");
        fprintf(file, "sampleSize, timeNative, timeFISQ, time2Newton, timeSSE\n"); // print header for .csv file

        /* Runs the time benchmark for certain array sizes. maxIncrements determines
        how often the benchmark is run and how often the array size is incread.
        STEPS determines the increase after each iteration. */
        for (int i = 1; i < maxIncrements + 1; i++)
        {
            benchmarkTime_flt(i * STEPS, &file);
        }
    }
    fclose(file);
}
void benchmarkTime_flt(int sampleSize, FILE **file)
{
    // Variables to store runtime
    double timeNative = 0.0;
    double timeV1 = 0.0;
    double timeSSE = 0.0;
    double time2Newton = 0.0;
    struct timespec start;
    struct timespec stop;

    srand(time(0));

    // Create array with samples and output array and handle malloc failures
    float *sample = (float *)malloc(sampleSize * sizeof(float));
    if (!sample)
    {
        perror("Error allocating memory for sample array");
        fclose(*file);
        exit(EXIT_FAILURE);
    };
    float *result = (float *)malloc(sampleSize * sizeof(float));
    if (!result)
    {
        perror("Error allocating memory for result array");
        free(sample);
        fclose(*file);
        exit(EXIT_FAILURE);
    };

    /* Generate random sample and use for every implementation
    Essentially: Generate from smallest to biggest positive floats
    rand()/RAND_MAX generates floats between 0.0 and 1.0.
    Multiply with randomly generated exponents from 10^-10 to 10^10,
    although size of number has no effect on speed */
    for (int i = 0; i < sampleSize; i++)
    {
        sample[i] = ((float)rand() / RAND_MAX) * pow(10, (rand() % 20 - 10));
    }

    /* Mark time before for-loop, then run function TRIALS times.
    Stop time and divide total time by TRIALS to get avg time
    Repeat for every implementation. */
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        nativeSqrt_flt(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    timeNative += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    timeNative /= TRIALS;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        fastInvSqrt_flt_V1(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    timeV1 += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    timeV1 /= TRIALS;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        fastInvSqrt_flt_DoubleNewton(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    time2Newton += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    time2Newton /= TRIALS;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        fastInvSqrt_flt(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    timeSSE += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    timeSSE /= TRIALS;

    // Print sample size and times to .csv
    fprintf(*file, "%i, %10.10f, %10.10f, %10.10f, %10.10f\n", sampleSize, timeNative, timeV1, time2Newton, timeSSE);

    free(sample);
    free(result);
}
void benchmarkTime_dbl_wrapper(int maxIncrements)
{
    FILE *file;
    if (!(file = fopen("./benchmark_outputs/results_speed_dbl.csv", "w+")))
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Running runtime benchmark for float algorithms...\n");
        printf("Results will be stored in ./benchmark_outputs/results_speed_dbl.csv\n\n");
        fprintf(file, "sampleSize, timeNative, timeFISQ, time2Newton, timeSSE\n"); // print header for .csv files

        /* Runs the time benchmark for certain array sizes. maxIncrements determines
        how often the benchmark is run and how often the array size is incread.
        STEPS determines the increase after each iteration. */
        for (int i = 1; i < maxIncrements + 1; i++)
        {
            benchmarkTime_dbl(i * STEPS, &file);
        }
    }
    fclose(file);
}
void benchmarkTime_dbl(int sampleSize, FILE **file)
{
    // Variables to store runtime
    double timeNative = 0.0;
    double timeV1 = 0.0;
    double timeSSE = 0.0;
    double time2Newton = 0.0;
    struct timespec start;
    struct timespec stop;

    srand(time(0));

    // Create array with samples and output array and handle malloc failures
    double *sample = (double *)malloc(sampleSize * sizeof(double));
    if (!sample)
    {
        perror("Error allocating memory for sample array");
        fclose(*file);
        exit(EXIT_FAILURE);
    };
    double *result = (double *)malloc(sampleSize * sizeof(double));
    if (!result)
    {
        perror("Error allocating memory for result array");
        free(sample);
        fclose(*file);
        exit(EXIT_FAILURE);
    };

    /*  Generate random sample and use for every implementation
    Essentially: Generate from smallest to biggest positive double
    rand()/RAND_MAX generates doubles between 0.0 and 1.0.
    Multiply with randomly generated exponents from 10^-10 to 10^10,
    although size of number has no effect on speed */
    for (int i = 0; i < sampleSize; i++)
    {
        sample[i] = ((double)rand() / RAND_MAX) * pow(10, (rand() % 20 - 10));
    }

    /* Mark time before for-loop, then run function TRIALS times.
    Stop time and divide total time by TRIALS to get avg time
    Repeat for every implementation. */
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        nativeSqrt_dbl(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    timeNative += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    timeNative /= TRIALS;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        fastInvSqrt_dbl_V1(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    timeV1 += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    timeV1 /= TRIALS;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        fastInvSqrt_dbl_DoubleNewton(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    time2Newton += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    time2Newton /= TRIALS;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int i = 0; i < TRIALS; i++)
    {
        fastInvSqrt_dbl(sampleSize, sample, result);
    }
    clock_gettime(CLOCK_MONOTONIC, &stop);
    timeSSE += (stop.tv_sec - start.tv_sec + 1e-9 * (stop.tv_nsec - start.tv_nsec));
    timeSSE /= TRIALS;
    // Print sample size and times to .csv
    fprintf(*file, "%i, %10.10f, %10.10f, %10.10f, %10.10f\n", sampleSize, timeNative, timeV1, time2Newton, timeSSE);

    free(sample);
    free(result);
}
void runTests(void)
{
    // kick off all tests and benchmarks
    basicFunctionality_flt();
    basicFunctionality_dbl();
    plotRange();

    benchmarkAccuracy_flt();
    benchmarkAccuracy_dbl();

    benchmarkTime_flt_wrapper(MAXINCREMENTS);
    benchmarkTime_dbl_wrapper(MAXINCREMENTS);
}