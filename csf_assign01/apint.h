/*
 * Arbitrary-precision integer data type
 * Function headers
 * CSF Assignment 1 
 *
 * Pair Assignment: Shaina Gabala and Pamela Li
 * sgabala2@jhu.edu
 * pli36@jhu.edu
 */

#ifndef APINT_H
#define APINT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

  /*
   * Representation: the data field is a little-endian bitstring ---
   * data[0] is bits 0..63, data[1] is bits 64..127, etc.
   */
  typedef struct {
    uint32_t len; /*number of data values */
    bool isNegative; /* true if negative, false if positive */
    uint64_t *data; /* data values */
  } ApInt;

  
  //Constructors and destructors
  /*
   * Creates an ApInt instance with a uint64_t value.
   *
   * Parameters:
   *   val -- the uint64_t value to put into the data of the ApInt instance
   *
   * Returns:
   *   a pointer to an ApInt with specified val value
   */
  ApInt *apint_create_from_u64(uint64_t val);

  /*
   * Creates an ApInt instance whose value is passed through
   * as an arbitrary sequence of hexadecimal digits
   *
   * Parameters:
   *   hex -- a string representing the arbitrary sequence 
   *          of hexadecimal digits to used as the value
   *
   * Returns:
   *   a pointer to an ApInt instance with specified hex value
   */
  ApInt *apint_create_from_hex(const char *hex);

  /*
   * Deallocates the memory used by an ApInt instance.
   *
   * Parameters:
   *   ap -- the ApInt instance to deallocate 
   */
  void apint_destroy(ApInt *ap);


  
  //Operations
  /*
   * Checks to see if the ApInt instance whose address is passed is 
   * numerically equal to zero, and returns an integer value
   * based on the answer.
   *
   * Parameters:
   *   ap -- the constant ApInt instance address to check
   *
   * Returns:
   *   1 if the ApInt is numerically equal to 0
   *   0 if the ApInt has a non-zero value
   */
  int apint_is_zero(const ApInt *ap);
  
   /*
   * Checks to see if the ApInt instance whose address is passed is 
   * negative (numerically less than 0), and returns an integer value
   * based on the answer.
   *
   * Parameters:
   *   ap -- the constant ApInt instance address to check
   *
   * Returns:
   *   1 if the ApInt is negative
   *   0 if otherwise
   */
  int apint_is_negative(const ApInt *ap);

   /*
   * Gets some bits of the binary representation of the Apint 
   * instance passed into the method.
   *
   * Parameters:
   *   ap -- the constant ApInt instance address to get bits from
   *   n -- indicates which bits to return. If n is 0, bits 0 -> 63 
   *        are returnes, if n is 1, bits 64 -> 127, etc.
   *
   * Returns:
   *   a uint64_t value containing 64 bits of the binary 
   *   representation of ap.
   */
  uint64_t apint_get_bits(const ApInt *ap, unsigned n);

   /*
   * Finds the position of the most significant bit set to 1 in the
   * representation of the ApInt instance whose address is passed 
   * into the method.
   *
   * Parameters:
   *   ap -- the constant ApInt instance to check
   *
   * Returns:
   *   -1 if the ApInt instance pointed to represents the value 0
   *   an integer representing the position of the highest bit set.
   */
  int apint_highest_bit_set(const ApInt *ap);

   /*
   * Converts a ApInt data value into hexadecimal digit representation.
   *
   * Parameters:
   *   ap -- the constant ApInt instance ton convert
   *
   * Returns:
   *   a pointer to a dynamically-allocated C character string
   *   containing the hexadecimal representation of ap.
   */
  char *apint_format_as_hex(const ApInt *ap);

   /*
   * Negates the value of the Apint instance whose address is passed
   * into the method.
   *
   * Parameters:
   *   ap -- the constant ApInt instance to check
   *
   * Returns:
   *   a new ApInt instance with the opposite sign from ap,
   *   or as a special case, if ap's value is numerically  equal to 0,
   *   it returns an identical 0 value ApInt instance
   */
  ApInt *apint_negate(const ApInt *ap);

   /*
   * Computes the sum (a + b) of two ApInt instances.
   *
   * Parameters:
   *   a -- the first constant ApInt instance to participate in the sum
   *   b -- the second constant ApInt instance to participate in the sum
   *
   * Returns:
   *   a pointer to an ApInt instance representing the sum of a and b.
   */
  ApInt *apint_add(const ApInt *a, const ApInt *b);

  /*
   * Computes the difference (a - b) of two ApInt instances.
   *
   * Parameters:
   *   a -- the first constant ApInt instance to participate in the diff
   *   b -- the second constant ApInt instance to participate in the diff
   *
   * Returns:
   *   a pointer to an ApInt instance representing the difference 
   *   of a and b.
   */
  ApInt *apint_sub(const ApInt *a, const ApInt *b);

  /*
   * Computes the values of two ApInt instances.
   *
   * Parameters:
   *   left -- the constant ApInt instance on the left of 
   *           the comparison equation
   *   right -- the constant ApInt instance on the right of
   *            the comparison equation 
   *
   * Returns:
   *   a negative value if left < right
   *   a positive value if left > right
   *   0 if the values are equal
   */
  int apint_compare(const ApInt *left, const ApInt *right);



  
  //Helper functions
  /*
   * Computes the sum of two unsigned integers (uint64_t) instances.
   *
   * Parameters:
   *   a -- the first uint64_t instance to participate in the sum
   *   b -- the second uint64_t instance to participate in the sum
   *
   * Returns:
   *   a uint64_t instance representing the sum of a and b.
   */
  uint64_t addVal(uint64_t a, uint64_t b);

  /*
   * Computes the difference of two unsigned integers (uint64_t) instances.
   *
   * Parameters:
   *   a -- the first uint64_t instance to participate in the diff
   *   b -- the second uint64_t instance to participate in the diff
   *
   * Returns:
   *   an uint64_t instance representing the difference  of a and b.
   */
  uint64_t subVal(uint64_t a, uint64_t b);
  
#ifdef __cplusplus
}
#endif

#endif /* APINT_H */
