/** @headerfile inverse_sqrt.h
 *  @brief Function prototypes for the implementation of the Fast Inverse Square Root
 *  algorithms and alternatives
 *
 *  @author Yll Kryeziu (ge94noh)
 */

#ifndef IMPLEMENTIERUNG_INVERSE_SQRT_H
#define IMPLEMENTIERUNG_INVERSE_SQRT_H

/**
 * @brief Calculate the reciprocal square root of input array of n floats
 * using 1/sqrtf(x) and write results into output array.
 *
 * @details The method takes a pointer to an array of floats values with the length n
 * as parameter and calculates the reciprocal of the square root of each value using
 * the native Square Root function from math.h: 1/sqrtf(x).
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of float values in the arrays
 * @param vals Pointer to the input array with float values
 * @param out Pointer to the float output array, where the results are written
 */
void nativeSqrt_flt(size_t n, float vals[n], float out[n]);

/**
 * @brief Calculate the reciprocal square root of input array of n floats
 * using the Fast Inverse Square Root algorithm with 2 Newton-Raphson iterations
 * and write results into output array.
 *
 * @details The method takes a pointer to an array of float values with the length n
 * as parameter and calculates the reciprocal of the square root of each value by iterating
 * through the array and using the Fast Inverse Square Root algorithm. Note that this algorithm
 * uses the MagicNumber 0x5F375A86 for floats as determined by Lomont/Robertson and an additional
 * Newton-Raphson iteration to further improve accuracy of the results.
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of float values in the arrays
 * @param vals Pointer to the input array with float values
 * @param out Pointer to the float output array, where the results are written
 */
void fastInvSqrt_flt_DoubleNewton(size_t n, float vals[n], float out[n]);

/**
 * @brief Calculate the reciprocal square root of input array of n floats
 * using the Scalar implementation of the Fast Inverse Square Root algorithm
 * and write results into output array.
 *
 * @details The method takes a pointer to an array of float values with the length n
 * as parameter and calculates the reciprocal of the square root of each value by iterating
 * through the array and using the Scalar Fast Inverse Square Root algorithm. Note that this
 * algorithm uses the MagicNumber 0x5F375A86 for floats as determined by Lomont/Robertson.
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of float values in the arrays
 * @param vals Pointer to the input array with float values
 * @param out Pointer to the float output array, where the results are written
 */
void fastInvSqrt_flt_V1(size_t n, float vals[n], float out[n]);

/**
 * @brief Calculate the reciprocal square root of input array of n floats
 * using the SIMD implementation of the Fast Inverse Square Root algorithm
 * and write results into output array.
 *
 * @details The method takes a pointer to an array of float values with the length n
 * as parameter and calculates the reciprocal of the square root of each value by iterating
 * through the array and using the SIMD Fast Inverse Square Root algorithm. Note that this
 * algorithm uses the MagicNumber 0x5F375A86 for floats as determined by Lomont/Robertson.
 * In contrast to the Scalar implementation, this method is optimized using SIMD-instructions
 * to achieve higher speeds.
 * Using loop unrolling, 4 floats are read and processed at once instead of just 1.
 * If the number of floats is smaller then 4, the rest of the floats is processed using
 * scalar instructions.
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of float values in the arrays
 * @param vals Pointer to the input array with float values
 * @param out Pointer to the float output array, where the results are written
 */
void fastInvSqrt_flt(size_t n, float vals[n], float out[n]);

/**
 * @brief Calculate the reciprocal square root of input array of n doubles
 * using 1/sqrt(x) and write results into output array.
 *
 * @details The method takes a pointer to an array of doubles values with the length n
 * as parameter and calculates the reciprocal of the square root of each value using
 * the native Square Root function from math.h: 1/sqrt(x).
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of double values to be used as input
 * @param vals Pointer to the input array with double values
 * @param out Pointer to the double output array, where the results are written
 */
void nativeSqrt_dbl(size_t n, double vals[n], double out[n]);

/**
 * @brief Calculate the reciprocal square root of input array of n doubles
 * using the Fast Inverse Square Root algorithm with 2 Newton-Raphson iterations
 * and write results into output array.
 *
 * @details The method takes a pointer to an array of doubles values with the length n
 * as parameter and calculates the reciprocal of the square root of each value by iterating
 * through the array and using the Fast Inverse Square Root algorithm. Note that this algorithm
 * uses the MagicNumber 0x5FE6EB50C7B537A9 for doubles as determined by Lomont/Robertson
 * and an additional Newton-Raphson iteration to further improve accuracy of the results.
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of double values in the arrays
 * @param vals Pointer to the input array with double values
 * @param out Pointer to the double output array, where the results are written
 */
void fastInvSqrt_dbl_DoubleNewton(size_t n, double vals[n], double out[n]);

/**
 * @brief Calculate the reciprocal square root of input array of n doubles
 * using the Scalar implementation of the Fast Inverse Square Root algorithm
 * and write results into output array.
 *
 * @details The method takes a pointer to an array of doubles values with the length n
 * as parameter and calculates the reciprocal of the square root of each value by iterating
 * through the array and using the Scalar Fast Inverse Square Root algorithm. Note that this
 * algorithm uses the MagicNumber 0x5FE6EB50C7B537A9 for floats as determined by Lomont/Robertson.
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of doubles values in the arrays
 * @param vals Pointer to the input array with doubles values
 * @param out Pointer to the doubles output array, where the results are written
 */
void fastInvSqrt_dbl_V1(size_t n, double vals[n], double out[n]);

/**
 * @brief Calculate the reciprocal square root of input array of n doubles
 * using the SIMD implementation of the Fast Inverse Square Root algorithm
 * and write results into output array.
 *
 * @details The method takes a pointer to an array of float values with the length n
 * as parameter and calculates the reciprocal of the square root of each value by iterating
 * through the array and using the SIMD Fast Inverse Square Root algorithm. Note that this
 * algorithm uses the MagicNumber 0x5FE6EB50C7B537A9 for doubles as determined by Lomont/Robertson.
 * In contrast to the Scalar implementation, this method is optimized using SIMD-instructions
 * to achieve higher speeds.
 * Using loop unrolling, 2 doubles are read and processed at once instead of just 1.
 * If the number of doubles is smaller then 2, the rest of the doubles is processed using
 * scalar instructions.
 * The resulting values are written into the output array, which
 * is also the last argument of the method.
 *
 * @param n Number of double values in the arrays
 * @param vals Pointer to the input array with double values
 * @param out Pointer to the double output array, where the results are written
 */
void fastInvSqrt_dbl(size_t n, double vals[n], double out[n]);
#endif // IMPLEMENTIERUNG_INVERSE_SQRT_H
