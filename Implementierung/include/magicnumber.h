/** @file magicnumber.h
 *  @brief Function prototypes for the calculation of MagicNumber
 *
 *  @author Keihan Paksersht (ge23xit)
 */

#ifndef IMPLEMENTIERUNG_MAGICNUMBER_H
#define IMPLEMENTIERUNG_MAGICNUMBER_H

/**
 * @brief Print out the MagicNumber corresponding to the given data type float/double
 *
 * @details The method calculates and prints outs the MagicNumber of type float (db = 0) or double (db = 1)
 * as well as its relative error to the console.
 *
 * @param db db = 0 if considering float; db = 1 if considering double
 */
void print_magicnumber(int db);

#endif // IMPLEMENTIERUNG_MAGICNUMBER_H