/** @file tests.h
 *  @brief Function prototypes for tests and benchmarks in tests.c
 *
 *  @author Yll Kryeziu (ge94noh)
 */

#ifndef IMPLEMENTIERUNG_TESTS_H
#define IMPLEMENTIERUNG_TESTS_H

/**
 * @brief Simple function to test float invsqrt implementations with pre-defined
 * including some edge cases. Prints sample and calculations to console.
 */
void basicFunctionality_flt(void);

/**
 * @brief Simple function to test double invsqrt implementations with pre-defined
 * including some edge cases. Prints sample and calculations to console.
 */
void basicFunctionality_dbl(void);

/**
 * @brief Generates result_plot_flt.csv in ./benchmark_outputs for plotting
 * with gnuplot. The function calculates the invsqrt using the SIMD-implementation
 * for floats 0.1 to 10 in 0.1 increments. Relative error compared to native 1/sqrt()
 * is calculated and printed to .csv file.
 */
void plotRange_flt(void);

/**
 * @brief Iterates through every float and calculates the error for every implementation compared
 * exact result of 1/sqrt(). Maximum relative error for every implementation is printed to console.
 */
void benchmarkAccuracy_flt(void);

/**
 * @brief Iterates through many doubles and calculates the error for every implementation compared
 * exact result of 1/sqrt(). Maximum relative error for every implementation is printed to console.
 */
void benchmarkAccuracy_dbl(void);

/**
 * @brief Wrapper for time-benchmark for float implementations. Opens .csv file and handles fopen failure.
 * Runs the time-benchmark according to maxIncrements.
 * @param maxIncrement Determines the amount of times the benchmark is run
 * with increased sample size.
 */
void benchmarkTime_flt_wrapper(int maxIncrements);

/**
 * @brief Creates input array depending on sampleSize with floats of random sizes.
 * Repeats calculation of inverse square roots of every implementation according to
 * definition of TRIALS and measures average time. Writes sampleSize and measured times
 * to result_speed_flt.csv in ./benchmark_outputs for plotting.
 * @param sampleSize size of input and output array
 * @param file pointer to file previously openend in benchmark wrapper
 */
void benchmarkTime_flt(int sampleSize, FILE **file);

/**
 * @brief Wrapper for time-benchmark for float implementations. Opens .csv file and handles fopen failure.
 * Runs the time-benchmark according to maxIncrements.
 * @param maxIncrement Determines the amount of times the benchmark is run
 * with increased sample size.
 */
void benchmarkTime_dbl_wrapper(int maxIncrements);

/**
 * @brief Creates input array depending on sampleSize with doubles of random sizes.
 * Repeats calculation of inverse square roots of every implementation according to
 * definition of TRIALS and measures average time. Writes sampleSize and measured times
 * to result_speed_dbl.csv in ./benchmark_outputs for plotting.
 * @param sampleSize size of input and output array
 * @param file pointer to file previously openend in benchmark wrapper
 */
void benchmarkTime_dbl(int sampleSize, FILE **file);

/**
 * @brief Starts all test and benchmark executions
 */
void runTests(void);

#endif // IMPLEMENTIERUNG_TESTS_H
