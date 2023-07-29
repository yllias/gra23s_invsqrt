/** @file inverse_sqrt.c
 *  @brief Implementation of fast inverse square root algorithm and alternatives
 *  @details For details of each function see inverse_sqrt.h
 *  @author Yll Kryeziu (ge94noh)
 */

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <immintrin.h>
#include "../include/inverse_sqrt.h"

void nativeSqrt_flt(size_t n, float vals[n], float out[n])
{
    for (size_t i = 0; i < n; i++)
    {
        out[i] = 1.f / sqrtf(vals[i]);
    }
}

void fastInvSqrt_flt_DoubleNewton(size_t n, float vals[n], float out[n])
{
    /* Use union instead of casting for type-punning
    to avoid undefined behaviour */
    union
    {
        float f;
        uint32_t i;
    } conv;

    const float threehalfs = 1.5f;

    // Read array float by float and apply algorithm operations
    for (size_t j = 0; j < n; ++j)
    {
        conv.f = vals[j];
        float xhalf = conv.f * 0.5f;

        conv.i = 0x5F375A86 - (conv.i >> 1); // Use magicnumber from Lomont and integer representation to get approximate result

        conv.f = conv.f * (threehalfs - (xhalf * conv.f * conv.f));
        conv.f = conv.f * (threehalfs - (xhalf * conv.f * conv.f)); // Use 2nd Newton iteration to further improve accuracy of result
        out[j] = conv.f;
    }
}

void fastInvSqrt_flt_V1(size_t n, float vals[n], float out[n])
{
    /* Use union instead of casting for type-punning
    to avoid undefined behaviour */
    union
    {
        float f;
        uint32_t i;
    } conv;

    const float threehalfs = 1.5f;

    // Read array float by float and apply algorithm operations
    for (size_t j = 0; j < n; ++j)
    {
        conv.f = vals[j];
        float xhalf = conv.f * 0.5f;

        conv.i = 0x5F375A86 - (conv.i >> 1); // Use magicnumber from Lomont and integer representation to get approximate result

        conv.f = conv.f * (threehalfs - (xhalf * conv.f * conv.f)); // Use single Newton iteration to improve accuracy of result
        out[j] = conv.f;
    }
}

void fastInvSqrt_flt(size_t n, float vals[n], float out[n])
{
    /* Use union instead of casting for type-punning
    to avoid undefined behaviour, same principle as with non-vectors */
    union
    {
        __m128 f; // 128 bit vector to store 4 32 bit single precision floating numbers
        __m128i i;
    } convSSE;

    // reduce instructions in loop with these constants
    const __m128 threehalfs = _mm_set1_ps(1.5f);
    const __m128i magicnumber = _mm_set1_epi32(0x5F375A86);
    const __m128 half = _mm_set1_ps(0.5f);
    size_t j;

    /* Read 128 bits from starting adress and load into vector,
    but only if number of remainting elements >= 4 */
    for (j = 0; j < (n & ~3ul); j += 4)
    {
        // apply algorithm operations, use SIMD-packed-instructions to edit 4 single precision floats at once
        convSSE.f = _mm_loadu_ps(&vals[j]);
        __m128 xhalf = _mm_mul_ps(convSSE.f, half);

        convSSE.i = _mm_sub_epi32(magicnumber, _mm_srli_epi32(convSSE.i, 1)); // Use magicnumber from Lomont and integer representation to get approximate result

        convSSE.f = _mm_mul_ps(convSSE.f, _mm_sub_ps(threehalfs, _mm_mul_ps(xhalf, _mm_mul_ps(convSSE.f, convSSE.f)))); // Use single Newton iteration to improve accuracy of result
        _mm_store_ps(&out[j], convSSE.f);
    }

    union
    {
        float x;
        uint32_t u;
    } conv;
    // Deal with the rest of the elements with scalar operations
    for (; j < n; j++)
    {
        conv.x = vals[j];
        float xhalf = conv.x * 0.5f;
        conv.u = 0x5F375A86 - (conv.u >> 1);
        conv.x = conv.x * (1.5f - (xhalf * conv.x * conv.x));
        out[j] = conv.x;
    }
}

void nativeSqrt_dbl(size_t n, double vals[n], double out[n])
{
    for (size_t i = 0; i < n; i++)
    {
        out[i] = 1.0 / sqrt(vals[i]);
    }
}

void fastInvSqrt_dbl_DoubleNewton(size_t n, double vals[n], double out[n])
{
    /* Use union instead of casting for type-punning
    to avoid undefined behaviour */
    union
    {
        double d;
        uint64_t i;
    } conv;

    const double threehalfs = 1.5;

    // Read array double by double and apply algorithm operations
    for (size_t j = 0; j < n; ++j)
    {
        conv.d = vals[j];
        double xhalf = conv.d * 0.5;

        conv.i = 0x5FE6EB50C7B537A9 - (conv.i >> 1); // Use magicnumber from Robertson and integer representation to get approximate result

        conv.d = conv.d * (threehalfs - (xhalf * conv.d * conv.d));
        conv.d = conv.d * (threehalfs - (xhalf * conv.d * conv.d)); // Use 2nd Newton iteration to further improve accuracy of result

        out[j] = conv.d;
    }
}

void fastInvSqrt_dbl_V1(size_t n, double vals[n], double out[n])
{
    /* Use union instead of casting for type-punning
    to avoid undefined behaviour */
    union
    {
        double d;
        uint64_t i; // 64 bits are needed to represent double as integer
    } conv;

    const double threehalfs = 1.5;

    // Read array double by double and apply algorithm operations
    for (size_t j = 0; j < n; ++j)
    {
        conv.d = vals[j];
        double xhalf = conv.d * 0.5;

        conv.i = 0x5FE6EB50C7B537A9 - (conv.i >> 1); // Use magicnumber from Robertson and integer representation to get approximate result

        conv.d = conv.d * (threehalfs - (xhalf * conv.d * conv.d)); // Use single Newton iteration to improve accuracy of result
        out[j] = conv.d;
    }
}

void fastInvSqrt_dbl(size_t n, double vals[n], double out[n])
{
    /* Use union instead of casting for type-punning
    to avoid undefined behaviour, same principle as with non-vectors */
    union
    {
        __m128d d; // 128 bit d-Vector to store 2 64 bit double precision floating numbers
        __m128i i;
    } convSSE;

    // reduce instructions in loop with these constants
    const __m128d threehalfs = _mm_set1_pd(1.5);
    const __m128i magicnumber = _mm_set1_epi64x(0x5FE6EB50C7B537A9);
    const __m128d half = _mm_set1_pd(0.5);
    size_t j;

    /* Read 128 bits from starting adress and load into vector,
    but only if number of remainting elements >= 2 */
    for (j = 0; j < (n & ~1ul); j += 2)
    {
        /* apply algorithm operations, use SIMD-packed-instructions to edit 2 double precision floatnumbers at once
        Note that this uses double precision instructions, as doubles are being processed */
        convSSE.d = _mm_loadu_pd(&vals[j]);
        __m128d xhalf = _mm_mul_pd(convSSE.d, half);

        convSSE.i = _mm_sub_epi64(magicnumber, _mm_srli_epi64(convSSE.i, 1)); // Use magicnumber from Robertson and integer representation to get approximate result

        convSSE.d = _mm_mul_pd(convSSE.d, _mm_sub_pd(threehalfs, _mm_mul_pd(xhalf, _mm_mul_pd(convSSE.d, convSSE.d)))); // Use single Newton iteration to improve accuracy of result
        _mm_store_pd(&out[j], convSSE.d);
    }

    union
    {
        double x;
        uint64_t u;
    } conv;
    // Deal with the rest of the elements with scalar operations
    for (; j < n; j++)
    {
        conv.x = vals[j];
        double xhalf = conv.x * 0.5;
        conv.u = 0x5FE6EB50C7B537A9 - (conv.u >> 1);
        conv.x = conv.x * (1.5 - (xhalf * conv.x * conv.x));
        out[j] = conv.x;
    }
}