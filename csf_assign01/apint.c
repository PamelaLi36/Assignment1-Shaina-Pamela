/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * Pair Assignment: Shaina Gabala and Pamela Li
 * sgabala2@jhu.edu
 * pli36@jhu.edu
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "apint.h"

ApInt *apint_create_from_u64(uint64_t val) {

  /* allocates memory for a new ApInt instance
  and sets its instance variables to default
  with the exception of data which is specified */
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->len = 1;
  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));
  new_apint->data[0] = val;
  new_apint->isNegative = false;

  return new_apint;
}

ApInt *apint_create_from_hex(const char *hex) {
  /* TODO: implement */
  assert(0);
  return NULL;
}

void apint_destroy(ApInt *ap) {
  free(ap->data);
  free(ap);
}

int apint_is_zero(const ApInt *ap) {
  if( ap->data[0] == 0 ) { //checks numerical value of ap
    return 1; //returns appropriate int based on checked condition
  }
  else {
    return 0; //returns appropriate int based on checked condition
  }
}

int apint_is_negative(const ApInt *ap) {
  if( ap->isNegative ) { //checks if ap is a negative value
    return 1; //returns appropriate int based on checked condition
  }
  else {
    return 0; //returns appropriate int based on checked condition
  }
}

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
  return ap->data[n]; //gets bits of ap binary representation  at specified n position
}

int apint_highest_bit_set(const ApInt *ap) {

  uint64_t temp = ap->data[0]; //temporary unsigned int storing ap's numerical value
  int highest_bit = 0; //bit to return
  
  if(temp == 0) {
    return -1;
  }

  for(int i = 0; i < 64; i++) { //iterating through binary rep of ap
    if((temp & 1) == 1) { //checks to see if the bit at right most end of the binary representation of ap is 1
      highest_bit = i; //stores position inside highest bit variable
    }
    temp >>=1; //shifts right to check next highest bit position
  }

  return highest_bit;
}

char *apint_format_as_hex(const ApInt *ap) {
  uint64_t temp = ap->data[0];
  char store[18]; //16 hexidecimal + '\0' + '-' = 18
  char* storep = store; 
  char *return_string = (char*)calloc(18, sizeof(char));
  int idx = 0;

  if( temp == 0 ) { //special case if ap's numerical value is 0
    *return_string = '0';
    *(return_string + 1) = '\0';
    return return_string;
  }
  
  while (temp != 0) { //iterates until last remainder possible of temp/16
    //conversion from base 10 to hexadecimal
    switch(temp % 16) {
    case 0 : *(storep + idx) = '0';
      break;
    case 1 : *(storep + idx) = '1';
      break;
    case 2 : *(storep + idx) = '2';
      break;
    case 3 : *(storep + idx) = '3';
      break;
    case 4 : *(storep + idx) = '4';
      break;
    case 5 : *(storep + idx) = '5';
      break;
    case 6 : *(storep + idx) = '6';
      break;
    case 7 : *(storep + idx) = '7';
      break;
    case 8 : *(storep + idx) = '8';
      break;
    case 9 : *(storep + idx) = '9';
      break;
    case 10 : *(storep + idx) = 'a';
      break;
    case 11 : *(storep + idx) = 'b';
      break;
    case 12 : *(storep + idx) = 'c';
      break;
    case 13 : *(storep + idx) = 'd';
      break;
    case 14 : *(storep + idx) = 'e';
      break;
    case 15 : *(storep + idx) = 'f';
      break;
    }
    idx++;
    temp = temp/16; //updates temp to figure out next char in hexadecimal representation
    
  }

  int i = 0; //new index for reversed string return_string
  if ( ap->isNegative) {
    *(return_string + i) = '-';
    i++;
  }

  for(int r= 0; r <= idx; r++) { //r is an index that will iterate store from reverse order
    *(return_string + i) = store[idx - r - 1];
    i++; // i and r are separate to account for the mismatched index when the data value is negative
  }
 
  *(return_string + i) = '\0';
  return return_string;
}

ApInt *apint_negate(const ApInt *ap) {
  //create new ApInt instance to return with same numerical value as ap
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->data = (uint64_t*)malloc( ap->len * sizeof(uint64_t));
  new_apint->data[0] = ap->data[0];
  new_apint->len = ap->len;
  
  if( ap->isNegative ) { //checks if ap is negative and sets new ApInt instance to opposite sign
    new_apint->isNegative = false;
  } else {
    new_apint->isNegative = true;
  }

  return new_apint;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {

  //creates new Apint instance to return
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->len = 1;
  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));

  if(a->isNegative && b->isNegative) { //cheks if both ApInts are negative
    new_apint->data[0] = addVal(a->data[0], b->data[0]); // add unsigned values
    new_apint->isNegative = true; // set appropriate sign for addition of two negative values
  }
  else if(!(a->isNegative) && !(b->isNegative)) { //checks if both ApInts are positive
    new_apint->data[0] = addVal(a->data[0], b->data[0]); //adds unsigned values
    new_apint->isNegative = false; // set appropriate sign for addition of two positive values
  }
  else { // when either a or b is negative

    //when a is negative
    //check which unsigned value is larger / or if they are equal
    // subract values and set appropriate sign accordingly
    if((a->isNegative) && (a->data[0] > b->data[0])) {
      new_apint->data[0] = subVal(a->data[0], b->data[0]);
      new_apint->isNegative = true;
    } else if((a->isNegative) && (b->data[0] >= a->data[0])) { 
      new_apint->data[0] = subVal(b->data[0], a->data[0]);
      new_apint->isNegative = false;
    }
    //when b is negative
    //check which unsigned value is larger / or if they are equal
    // subract values and set appropriate sign accordingly
    else if((b->isNegative) && (b->data[0] > a->data[0])) {
      new_apint->data[0] = subVal(b->data[0], a->data[0]);
      new_apint->isNegative = true;
    } else if((b->isNegative) && (a->data[0] >= b->data[0])) {
      new_apint->data[0] = subVal(a->data[0], b->data[0]);
      new_apint->isNegative = false;
    }
  }

  return new_apint;
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
   //creates new Apint instance to return
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->len = 1;
  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));
  new_apint->isNegative = false;

  if(!(a->isNegative) && !(b->isNegative)) {  //cheks if both ApInts are positive
    //check with unsigned value is larger / equal to
    //depending on that subtract values in correct order
    //set appropriate sign 
    if(a->data[0] >= b->data[0] ) {
      new_apint->data[0] = subVal(a->data[0], b->data[0]);
      new_apint->isNegative = false;
    } else {
      new_apint->data[0] = subVal(b->data[0], a->data[0]);
      new_apint->isNegative = true;
    }
  }
  else if(a->isNegative && b->isNegative) {  //cheks if both ApInts are negative
    //check with unsigned value is larger / equal to
    //depending on that subtract values in correct order
    //set appropriate sign
    if(a->data[0] > b->data[0] ) {
      new_apint->data[0] = subVal(a->data[0], b->data[0]);
      new_apint->isNegative = true;
    } else {
      new_apint->data[0] = subVal(b->data[0], a->data[0]);
      new_apint->isNegative = false;
    }
  }
  else {  //cheks if either a or b is  negative
    //if one value is negative, then we know result will be an addition anyway
    new_apint->data[0] = addVal(a->data[0], b->data[0]);

    //in a - b if a is negative then the answer will be negative
    // if b is negative, then the answer will be positive
    if(a->isNegative) {
      new_apint->isNegative = true;
    }
    else {
      new_apint->isNegative = false;
    }
  }

  return new_apint;
}

int apint_compare(const ApInt *left, const ApInt *right) {
  //checks if both left and right instances are positive
  if( !(left->isNegative) && !(right->isNegative) ) {
    //checks for larger value or equality and returns appropriate int
    if(left->data[0] > right->data[0]) {
      return 1;
    }
    else if(left->data[0] < right->data[0]) {
      return -1;
    }
    else {
      return 0;
    }
  }
  //checks if both left and right instances are negative
  else if (left->isNegative && right->isNegative) {
    //in this case the superior unsigned value will result in the smaller ApInt instance
    //checks value and returns appropriate int
    if(left->data[0] > right->data[0]) {
      return -1;
    }
    else if(left->data[0] < right->data[0]) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else { //if either left or right is negative,
    //the negative value will always be the smaller
    if(left->isNegative) {
      return -1;
    }
    else {
      return 1;
    }
  }
}


//helper functions
uint64_t addVal(uint64_t a, uint64_t b) {
  return a + b;
}


uint64_t subVal(uint64_t a, uint64_t b) {
  return a - b;
}
