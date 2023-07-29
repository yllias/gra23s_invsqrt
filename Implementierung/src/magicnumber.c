/** @file magicnumber.h
 *  @brief Implementation of the calculation of MagicNumber
 *
 *  @author Ngoc Kim Ngan Nguyen (ge23fak)
 */

#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <math.h>

#include "../include/magicnumber.h"

// Calculate MagicNumber for Floats save its relative error in parameter error
uint32_t magicnumber_flt(double *error)
{
    union
    {
        float f;
        uint32_t x;
    } conv;
    uint32_t minC = 0x5F300000;   // Lower bound
    uint32_t maxC = 0x5F400000;   // Upper bound
    uint32_t delta = 0x10000;     // Increment of MagicNumber for each iteration step
    double maxError;              // Maximum error of each float with a specific MagicNumber
    double minMaxError = DBL_MAX; // Smallest maximum error which the tested values of MagicNumber can give
    uint32_t minMaxC = 0;             // MagicNumber of the smallest maximum error minMaxError
    while (delta > 0)
    {
        // Test over circa 16*2*5 = 160 values of MagicNumber
        for (uint32_t c = minC; c < maxC; c += delta)
        {
            maxError = 0.0;
            // Test over 2^24 values of float numbers
            for (uint32_t m = 0; m < (1 << 24); m++)
            {
                conv.x = 0x3F000000 + m; // x in [0.5, 2) with mantissa m
                float xhalf = conv.f * 0.5L;
                double reference = sqrt(conv.f); // True square root
                conv.x = c - (conv.x >> 1);
                float y = conv.f;
                y = y * (1.5F - (xhalf * y * y)); // Newton's iteration
                double relativeError = fabs(reference * y - 1.0) * 100.0;
                maxError = relativeError > maxError ? relativeError : maxError;
            }
            if (maxError < minMaxError)
            {
                minMaxError = maxError;
                minMaxC = c;
            }
        }
        // Update lower and upper bound. Update delta.
        minC = minMaxC - delta;
        maxC = minMaxC + delta;
        delta = delta >> 4;
    }
    *error = minMaxError;
    return minMaxC;
}

// Calculate MagicNumber for Doubles and save its relative error in parameter error
uint64_t magicnumber_dbl(double *error)
{
    // Initialise lower and upper bound based on the calculated MagicNumber for Floats.
    double sigma = 127 - magicnumber_flt(error) / (1.5 * pow(2, 23));
    double init_dbl = 1.5 * pow(2, 52) * (1023 - sigma);
    union
    {
        double d;
        uint64_t x;
    } conv;
    uint64_t minC = ((uint64_t)init_dbl) - (1llu << 32); // Lower bound
    uint64_t maxC = minC + (1llu << 32);                 // Upper bound
    uint64_t delta = 1llu << 28;                         // 2^28, Increment of MagicNumber for each iteration step
    const uint64_t mantissa = 1llu << 52;                // 2^52
    double maxError;                                     // Maximum error of each float with a specific MagicNumber
    double minMaxError = DBL_MAX;                        // Smallest maximum error which the tested values of MagicNumber can give
    uint64_t minMaxC = 0;                                    // MagicNumber of the smallest maximum error minMaxError
    while (delta > 0)
    {
        // Test over circa 16*2*8 = 256 values of MagicNumber
        for (uint64_t c = minC; c < maxC; c += delta)
        {
            maxError = 0.0;
            // Test over 2^25 values of float numbers
            for (uint64_t m = 0; m < mantissa * 2; m += 1llu << 28)
            {                                  // m += 2^28
                conv.x = 0x3FE * mantissa + m; // x in [0.5, 2) with mantissa m
                double xhalf = conv.d * 0.5;
                double reference = sqrt(conv.d); // True square root
                conv.x = c - (conv.x >> 1);
                double y = conv.d;
                y = y * (1.5 - (xhalf * y * y)); // Newton's iteration
                double relativeError = fabs(reference * y - 1.0) * 100.0;
                maxError = relativeError > maxError ? relativeError : maxError;
            }
            if (maxError < minMaxError)
            {
                minMaxError = maxError;
                minMaxC = c;
            }
        }
        // Update lower and upper bound. Update delta.
        minC = minMaxC - delta;
        maxC = minMaxC + delta;
        delta = delta >> 4;
    }
    *error = minMaxError;
    return minMaxC;
}

// Print out the MagicNumber corresponding to the given type float/double
void print_magicnumber(int db)
{
    double error;
    if (!db)
    {
        printf("MagicNumber for Floats: 0x%x\n", magicnumber_flt(&error));
    }
    else
    {
        printf("MagicNumber for Doubles: 0x%lx\n", magicnumber_dbl(&error));
    }
    printf("With Maximum Error: %.10f\n", error);
}
