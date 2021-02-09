/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * Pair Assignment: Shaina Gabala and Pamela Li
 * sgabala2@jhu.edu
 * pli36@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include "apint.h"

ApInt *apint_create_from_u64(uint64_t val) {

  /* allocates memory for a new ApInt instance
  and sets its instance variables to default
  with the exception of data which is specified */
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->len = 1/* + val/(2^64)*/;
  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));
  new_apint->data[0] = val; 
  new_apint->isNegative = false;
  //if it overloads that means 1 will carry over to the next index
  return new_apint;
}

ApInt *apint_create_from_hex(const char *hex) {
  printf("\ncreating from hex!\nhere is your hex string: %s\n", hex);
  
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->len = 1;

  int hasMinus = 0;
  new_apint->isNegative = false;
  if((strlen(hex) > 16) && (hex[0] != '-')) {
    printf("this is a large value. strlen(hex) is: %d, adding length\n", strlen(hex));
    new_apint->len += strlen(hex)/16;
    printf("len is %d\n", new_apint->len);
  }
  
  else if (hex[0] == '-') {
    printf("this is a negative hex value\n");
    new_apint->len += (strlen(hex) - 1)/16;
    new_apint->isNegative = true;
    hasMinus = 1;
  }

  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));
  int r = new_apint->len; //-1 5
  int position = 0;
  uint64_t sum = 0;

  int i = strlen(hex)-1; //76

  while(r >= 0) {
    printf("this is r: %d\n", r);
    //i = 76 and r*16 = 80; r*16 - 16: 
    while((i > (r*16 - 16)) && (i >= hasMinus)) {
      sum += hexi_to_deci(hex[i]) * pow(16,position);
      i--;
      position++;
    }
    printf("the value in new_apint->d[r] is %lu\n", sum);
    new_apint->data[r] = sum;
      
    //reset
    position = 0;
    sum = 0;
    r--;
  } 
  
  /*for (int i = strlen(hex)-1; i >= 0 + hasMinus; i--) {
    printf("here is hex(i): %c\n", hex[i]);
 
    //TEST
    if(i/16 > 0) {
      printf("this is r: %d\n", r);
      while(i > (r*16 - 16)) {
	sum += hexi_to_deci(hex[i]) * 16(position);
	i--;
      }
      printf("the value in new_apint->d[r] is %lu\n", sum);
      new_apint->data[r] = sum;
      
      //reset
      sum = 0;
      position = 0;
      r--;
    } // might need to fix i

    if(i/16 == 0){
      sum += hexi_to_deci(hex[i]) * 16^(position); //+=
      position++;
    } 
    
  } */

  
  //new_apint->data[0] = sum; //?
      
  return new_apint;
}

void apint_destroy(ApInt *ap) {
  free(ap->data);
  free(ap);
}

int apint_is_zero(const ApInt *ap) {
  int isZero = 1; //default
  
  for(int i = 0; i < (int)ap->len; i++) { //0
    if( ap->data[i] != 0 ) {  
      isZero = 0; //detects index different from 0
    }
  }
  
  return isZero;
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
  int highest_bit = 0; //bit to return
  
  if( apint_is_zero(ap) == 1 ) {
    return -1;
  }

  for(int j = ap->len -1; j >=0; j--) { //iterating through array backwards
    uint64_t temp = ap->data[j]; //???
    for(int i = 0; i < 64; i++) {
      if((temp & 1) == 1) { //checks to see if the bit at right most end of the binary representation of ap is 1
	highest_bit = i; //stores position inside highest bit variable
      }
      temp >>=1; //shifts right to check next highest bit position
    }
  }

  if(ap->len > 1) {
    highest_bit += 54 * ap->len;
  }

  return highest_bit;
}

char *apint_format_as_hex(const ApInt *ap) {
  char store[2 + 16*ap->len]; //16 hexidecimal + '\0' + '-' = 18
  char* storep = store; 
  char *return_string = (char*)calloc(2 + 16*ap->len, sizeof(char));
  int idx = 0;

  if( apint_is_zero(ap) == 1 ) { //special case if ap's numerical value is 0
    *return_string = '0';
    *(return_string + 1) = '\0';
    printf("returning 0\n");
    return return_string;
  }
  
  for(int j = ap->len - 1; j >= 0; j--) {
    uint64_t temp = ap->data[j];
    int appendZeros = 0;
    
    if(temp == 0 && j != 0) {
      appendZeros = 16;
    }
    
    while ((temp != 0) || appendZeros > 0){
      *(storep + idx) = deci_to_hexi(temp%16);
      idx++;
      temp = temp/16;
      appendZeros--;
      printf("gg\n");
    }
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
  printf("returning following string: %s\n", return_string);
  return return_string;
}

ApInt *apint_negate(const ApInt *ap) {
  //create new ApInt instance to return with same numerical value as ap
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->data = (uint64_t*)malloc( ap->len * sizeof(uint64_t));
  new_apint->data[0] = ap->data[0];
  new_apint->len = ap->len;
  
  if( ( apint_is_zero(ap) ) || ( ap->isNegative ) ) { //checks if ap is negative and sets new ApInt instance to opposite sign
    new_apint->isNegative = false;
  } else {
    new_apint->isNegative = true;
  }

  return new_apint;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {

  //creates new Apint instance to return
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  ApInt * small;
  ApInt * large;
  uint64_t overflow = 0;

  if(apint_compare(a,b) == 1) {
    small = b; 
    large = a;
  }
  else if((apint_compare(a,b) == -1) || (apint_compare(a,b) == 0)) {
    small = a;
    large = b;
    printf("here is small data(a) = %lu, and big(b): %lu\n", small->data[0], large->data[0]);
  }

  new_apint->len = large->len;
  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));

  int r = small->len - 1; //index for smaller obj

  if((a->isNegative && b->isNegative)||(!(a->isNegative) && !(b->isNegative))) { //cheks if both ApInts are negative or if both are positive
    for( int i = large->len - 1; i >= 0; i--) {
      if( r >= 0 ) {
	printf("here is small data(a) = %lu, and big(b): %lu\n", small->data[r], large->data[i]);
	new_apint->data[i] = addVal(large->data[i] + overflow, small->data[r]); // add unsigned values
	r--;
      }
      else {
	new_apint->data[i] = large->data[i] + overflow;
      }

      overflow = 0;
      if( (new_apint->data[i] < small->data[i]) ||  (new_apint->data[i] < large->data[i]) ) {
	overflow++;
      }
    }

    if(overflow == 1) {
      new_apint->data = (uint64_t*)realloc(new_apint->data, large->len +1);
      add_reorganize(new_apint);
      new_apint->len = large->len +1;
      new_apint->data[0] = new_apint->data[0] + overflow;
    }
    if (a->isNegative){
    new_apint->isNegative = true; // set appropriate sign for addition of two negative values
    }
    else {
      new_apint->isNegative = false;
    }
  }
  /* else if(!(a->isNegative) && !(b->isNegative)) { //checks if both ApInts are positive
    printf("entering both positive loop\n");
    for( int i = large->len - 1; i >= 0; i--) {
      printf("len is: %lu, and i is: %d\n", large->len, i);
      if(r >= 0) {
	printf("large data is: %lu, and small data is: %lu\n", large->data[i], small->data[r]);
	new_apint->data[i] = addVal(large->data[i] + overflow, small->data[r]);
	printf("overflow: %d\n", overflow);
	printf("when r = 0, new_apint->data[i] after adding is: %lu\n", new_apint->data[i]);
	r--;
      }
      else {
	new_apint->data[i] = small->data[i] + overflow;
      }

      overflow = 0;
      if ((new_apint->data[i] < small->data[i]) || (new_apint->data[i] < large->data[i])) {
	overflow++;
      }
    }

    if(overflow == 1) {
      printf("there is overflow\n");
      new_apint->data = (uint64_t*)realloc(new_apint->data, large->len + 1);
      new_apint->len = large->len + 1;
      add_reorganize(new_apint);
      
      printf("\nreorganized array\n");
      for(int i = 0; i < large->len + 1; i++) {
	printf("%lu\n", new_apint->data[i]);
      }
      
      new_apint->data[0] = new_apint->data[0] + overflow;
    }
    new_apint->isNegative = false; // set appropriate sign for addition of two positive values
}*/
  else { // when either a or b is negative
    ApInt * temp = new_apint;

    ApInt * a_copy = (ApInt*)malloc(sizeof(ApInt));
    a_copy->len = a->len;
    a_copy->isNegative = a->isNegative;
    a_copy->data = (uint64_t*)malloc(a->len*sizeof(uint64_t));
    for(int i = 0; i < (int)a->len; i++) {
      a_copy->data[i] = a->data[i];
    }

    ApInt * b_copy = (ApInt*)malloc(sizeof(ApInt));
    b_copy->len = b->len;
    b_copy->isNegative = b->isNegative;
    b_copy->data = (uint64_t*)malloc(b->len*sizeof(uint64_t));
    for(int i = 0; i < (int)b->len; i++) {
      b_copy->data[i] = b->data[i];
    }
    
    if(a->isNegative) {// -a+b, b-a
      a_copy->isNegative = false;
      new_apint = apint_sub(b_copy, a_copy);
    } else if(b->isNegative) { //a-b  
      b_copy->isNegative = false;
      new_apint = apint_sub(a_copy, b_copy);
    }
    printf("This is a_copy->isNegative after change: %d\n", a_copy->isNegative);
     printf("This is b_copy->isNegative after change: %d\n", b_copy->isNegative);
     // new_apint = apint_sub(b_copy, a_copy);
    printf("This is new_apint->isNegative:%d \n", new_apint->isNegative);
    apint_destroy(temp);
    apint_destroy(a_copy);
    apint_destroy(b_copy);
    
  }

  printf("here is sum new_apint at 0: %lu\n", new_apint->data[0]);
  return new_apint;
}


ApInt *apint_sub(const ApInt *a, const ApInt *b) {
   //creates new Apint instance to return
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  new_apint->len = 1;
  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));
  new_apint->isNegative = false;
  ApInt * small;
  ApInt * large;
  uint64_t borrow = 0;

  printf("\nxoming in, this is a: %lu, and b: %lu\n", a->data[0], b->data[0]);
  if(apint_compare(a,b) == 1) {
    small = b; 
    large = a;
  }
  else if((apint_compare(a,b) == -1) || (apint_compare(a,b) == 0)) {
    small = a;
    large = b;
    printf("this is a: %lu, and b: %lu\n", a->data[0], b->data[0]);
    printf("here is small data(a) = %lu, and big(b): %lu\n", small->data[0], large->data[0]);
  }

  new_apint->len = large->len;
  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));

  int r = small->len - 1; //index for smaller obj

  if((a->isNegative && b->isNegative)||(!(a->isNegative) && !(b->isNegative))) { //cheks if both ApInts are negative or if both are positive
    for( int i = large->len - 1; i >= 0; i--) {
      if ( borrow == 1){//decrement the current index if in the previous index, it was borrowed from
	large->data[i]-= i;
	  borrow = 0;
      }
      if(large->data[i] < small->data[r]){
	borrow++;
      }
      
      if( r >= 0 ) {
	printf("here is small data(a) = %lu, and big(b): %lu\n", small->data[r], large->data[i]);
	printf("This is borrow * pow(2,63): %lu; and borrow = %lu\n",(borrow * pow(2,63)),borrow); 
	new_apint->data[i] = subVal(large->data[i], small->data[r]); // add unsigned values
	printf("This is new_apt before borrow:%lu\n",	new_apint->data[i] );
	//new_apint->data[i] = subVal((borrow * (uint64_t)pow(2,63)), new_apint->data[i]);//find a way to incorporate borrowing
		printf("This is new_apt after borrow:%lu\n",new_apint->data[i] );
	r--;
      }
      else {
	new_apint->data[i] = large->data[i] - borrow;
      }
    }
    if ( large->data[0] == 0){
      sub_reorganize(new_apint);
      new_apint->data = (uint64_t* ) realloc(new_apint->data, large->len-1);
      new_apint->len = large->len-1;
    }
    if((large->isNegative) || (apint_compare(b, a) == 1)){
	new_apint->isNegative = true;
      }
      else{
	new_apint->isNegative = false;
      }
    /*printf("This is new_apint returned: ");
      for(int i = new_apint->len-1; i >= 0; i--){
	printf("%ul", new_apint->data[i]);
      }
      printf("\n");*/
	}
  /* if(!(a->isNegative) && !(b->isNegative)) {  //cheks if both ApInts are positive
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
    }*/
  else {  //cheks if either a or b is  negative
    //if one value is negative, then we know result will be an addition anyway
    ApInt * temp = new_apint;
    ApInt * a_copy = (ApInt*)malloc(sizeof(ApInt));
    a_copy->len = a->len;
    a_copy->isNegative = a->isNegative;
    a_copy->data = (uint64_t*)malloc(a->len*sizeof(uint64_t));
    for(int i = 0; i < (int)a->len; i++) {
      a_copy->data[i] = a->data[i];
    }
    ApInt * b_copy = (ApInt*)malloc(sizeof(ApInt));
    b_copy->len = b->len;
    b_copy->isNegative = b->isNegative;
    b_copy->data = (uint64_t*)malloc(b->len*sizeof(uint64_t));
    for(int i = 0; i < (int)b->len; i++) {
      b_copy->data[i] = b->data[i];
    }
    
    if(b->isNegative) {
      b_copy->isNegative = false;
    } else { 
      b_copy->isNegative = true;
    }
    
    new_apint = apint_add(a_copy, b_copy);
    apint_destroy(temp);
    apint_destroy(a_copy);
    apint_destroy(b_copy);
    
  }
  printf("This is new_apint returned: ");
  for(int i = new_apint->len-1; i >= 0; i--){
    printf("%lu", new_apint->data[i]);
  }
  printf("\n");
return new_apint;
}

int apint_compare(const ApInt *left, const ApInt *right) {

  printf("here is left data: %lu, and right d: %lu\n", left->data[0], right->data[0]);
  
  //checks if both left and right instances are positive
  if( !(left->isNegative) && !(right->isNegative) ) {
    //checks for larger value using length first
    if(left->len > right->len) {
      return 1;
    }
    else if(left->len < right->len) {
      return -1;
    }
    else { //if same length, iterate through data array to figure out which is bigger
      for(int i = 0; i < (int)left->len; i++) {
	if(left->data[i] > right->data[i]) {
	  return 1;
	}
	else if (left->data[i] < right->data[i]) {
	  return -1;
	}
      }
      return 0;
    }
  }
  //checks if both left and right instances are negative
  else if (left->isNegative && right->isNegative) {
    //in this case the superior unsigned value will result in the smaller ApInt instance
    printf("they are both negative\n");
    //checks for larger value using length first
    if(left->len > right->len) {
      return -1;
    }
    else if(left->len < right->len) {
	return 1;
    }
    else {
      //if same length, iterate through data array to figure out which is bigger
      for(int i = 0; i < (int)left->len; i++) {
	if(left->data[i] > right->data[i]) {
	  return -1;
	  }
	else if (left->data[i] < right->data[i]) {
	  return 1;
	}
      }
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

void add_reorganize(ApInt* a) {
  for(int i = a->len - 1; i >= 1; i--) {
    a->data[i] = a->data[i-1]; //CHECK
  }
  a->data[0] = 0;
}

uint64_t subVal(uint64_t a, uint64_t b) {
  return a - b;
}

void sub_reorganize(ApInt* a) {
  for(int i = 0; i < (int)a->len - 1; i++) {
    a->data[i] = a->data[i+1];
  }
}

char deci_to_hexi( uint64_t temp ) {
  switch(temp) {
  case 0 : return '0';
    break;
  case 1 : return'1';
    break;
  case 2 : return '2';
    break;
  case 3 : return '3';
    break;
  case 4 : return '4';
    break;
  case 5 : return '5';
    break;
  case 6 : return '6';
    break;
  case 7 : return '7';
    break;
  case 8 : return '8';
    break;
  case 9 : return '9';
      break;
  case 10 : return 'a';
    break;
  case 11 : return 'b';
    break;
  case 12 : return 'c';
    break;
  case 13 : return 'd';
    break;
  case 14 : return 'e';
    break;
  case 15 : return 'f';
    break;
    }
}

uint64_t hexi_to_deci(const char hex) {
  switch(hex) {
  case '0' : return 0;
    break;
  case '1' : return 1;
    break;
  case '2' : return 2;
    break;
  case '3' : return 3;
    break;
  case '4' : return 4;
    break;
  case '5' : return 5;
    break;
  case '6' : return 6;
    break;
  case '7' : return 7;
    break;
  case '8' : return 8;
    break;
  case '9' : return 9;
      break;
  case 'a' : return 10;
    break;
  case 'b' : return 11;
    break;
  case 'c' : return 12;
    break;
  case 'd' : return 13;
    break;
  case 'e' : return 14;
    break;
  case 'f' : return 15;
    break;
  }
}
