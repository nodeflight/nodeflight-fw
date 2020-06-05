#pragma once

/**
 * Initialize random number generator
 *
 * Note that the backend needs to be initialized by the platform itself
 */
void random_init(
    void);

/**
 * Get a random number
 *
 * This method is thread safe, however, exeuction time may vary.
 *
 * For fast random numbers, consider using a pseudorandom library, for which this can be used as seed.
 *
 * @return 32 bit true random number
 */
uint32_t random_get(
    void);
