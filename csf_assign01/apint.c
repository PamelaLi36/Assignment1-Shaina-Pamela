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
  ApInt * new_apint = (ApInt*)malloc(sizeof(ApInt));
  int startidx = 0;
  new_apint->len = 1;
  new_apint->isNegative = false;

  //if there are any leading zeroes
  if(hex[startidx] == 0) {
    while(hex[startidx] == 0) { startidx++; }
  }

  //if there is overflow 
  if((strlen(hex) >= 16) && (hex[0] != '-')) {
    new_apint->len += (strlen(hex) - 1)/16;
  }
  
  else if (hex[0] == '-') {
    new_apint->len += (strlen(hex) - 1)/16;
    new_apint->isNegative = true;
    startidx = 1;
  }

  new_apint->data = (uint64_t*)malloc(new_apint->len*sizeof(uint64_t));
  int r = new_apint->len - 1;
  int position = 0;
  uint64_t sum = 0;
  int i = strlen(hex)-1;
 
  while(r >= 0) {
    while(position < 16 && (i >= startidx)) {
      sum += (uint64_t)(hexi_to_deci(hex[i])) * (uint64_t)(pow(16,position));
      i--;
      position++;
    }

    new_apint->data[r] = sum;
    
    //reset
    position = 0;
    sum = 0;
    r--;
  }
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
  //new_apint->data[0] = ap->data[0];
  new_apint->len = ap->len;
  for (int i = 0; i <  new_apint->len;i++){
    new_apint->data[i] = ap->data[i];
  }
  if( ( apint_is_zero(ap) ) || ( ap->isNegative ) ) { //checks if ap is negative and sets new ApInt instance to opposite sign
    new_apint->isNegative = false;
  } else {
    new_apint->isNegative = true;
  }
  
  return new_apint;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
  ApInt * new_apint = NULL;//(ApInt*)malloc(sizeof(ApInt));
  ApInt * new_a = apint_negate(a);
  ApInt * new_b = apint_negate(b);
  printf("Entered apint_add\n");
  if ((a->isNegative && b->isNegative)||(!(a->isNegative) && !(b->isNegative))) { //-a-b  ;  a+b
    new_apint = unsigned_add(a, b);
     printf("Got passed unisigned add in apint_add\n");
    new_apint->isNegative = a->isNegative;
  }
  else {   // -a + b or a - b -> a - b  or b-a
    new_apint = unsigned_sub(new_a,  new_b);
    if ((apint_compare(new_a,new_b) == 1)) { // if the magnitude of a is bigger than b  
      new_apint->isNegative = a->isNegative;
    }else{
      new_apint->isNegative = b->isNegative;
    }
    if (apint_is_zero(new_apint)){
      new_apint->isNegative = true;
    }
  }
  apint_destroy(new_a);
  apint_destroy(new_b);
  //new_apint->len = (sizeof(new_apint->data))/(sizeof(new_apint->data[0]));
  printf("This is sizeof new_apint->data: %lu\n", sizeof(new_apint->data));
   printf("This is sizeof uint64_t: %lu\n", sizeof(uint64_t));
  
  printf("This is len of new_apint: %lu\n", new_apint->len);
  
  //printf("This is new_apint[0]: %lu\n", new_apint->data[0]);
  //if ( new_apint->len > 1){
  //   printf("This is new_apint[1]: %lu\n", new_apint->data[1]);
   //}
   printf("This is new_apint: \n");
     for (int j = 0; j < new_apint->len; j++){
       printf("%lu ", new_apint->data[j]);
     }
   return new_apint;
}

	
ApInt *apint_sub(const ApInt *a, const ApInt *b) {
  ApInt * new_apint = NULL;//(ApInt*)malloc(sizeof(ApInt));
  ApInt * new_a = apint_negate(a);
  new_a->isNegative = false;
  ApInt * new_b = apint_negate(b);
  new_b->isNegative = false;
  printf("\nThis is a: ");
  for ( int i = 0 ; i < a->len; i++){
    printf("%lu ", a->data[i]);
  }
  printf("\n");
   printf("\nThis is b: ");
    for ( int i = 0 ; i < b->len; i++){
    printf("%lu ", b->data[i]);
  }
  printf("\n");
  printf("\nThis is new_a: ");
  for ( int i = 0 ; i < new_a->len; i++){
    printf("%lu ", new_a->data[i]);
  }
  printf("\n");
   printf("\nThis is new_b: ");
    for ( int i = 0 ; i < new_b->len; i++){
    printf("%lu ", new_b->data[i]);
  }
  printf("\n");

  if((a->isNegative && b->isNegative)||(!(a->isNegative) && !(b->isNegative))) { //-a + b  , a - b ,  b- a, -b - (-a)
    new_apint = unsigned_sub(new_a, new_b);
    if ( apint_compare(new_a,new_b) == 1 ) {//comparing magnitudes
      //-a - (-b) = -a +b, a - b
      printf("Sign of %lu is %d and %lu is %d\n", a->data[0], a->isNegative, b->data[0], b->isNegative);
       printf("the answer depends on sign of a \n");
      new_apint->isNegative = a->isNegative;
    }
    else if (apint_compare( new_b, new_a ) == 1 ){ //b has a larger  magnitude than a -a - (-b) = -a +b ;  a- b)
      printf("Problem area in sub\n"); 
      new_apint->isNegative = !(b->isNegative);
       printf("This is the val for isNegative: %d\n",  new_apint->isNegative);
    }else { // if answer is 0
      printf("the answer is 0 so it's positive\n");
      new_apint->isNegative = false;
    }
    
  }
  else {
     new_apint =  unsigned_add(new_a, new_b); // a - (-b)   -a -b 
    new_apint->isNegative = a->isNegative;
  }
  // new_apint->len = sizeof(new_apint->data)/sizeof(uint64_t);
  //printf("After subtracting, this is the length of new_apint: %lu\n",  new_apint->len ); 
  apint_destroy(new_a);
  apint_destroy(new_b);
  printf("Returned from ap sub with data = %lu\n", new_apint->data[0]);
  return  new_apint;
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
	  return 1;
	  }
	else if (left->data[i] < right->data[i]) {
	  return -1;
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
ApInt * unsigned_add(ApInt *a, ApInt *b) {
  ApInt* temp = (ApInt*)malloc( 1 *sizeof(ApInt));
  ApInt* other = NULL;
  uint64_t overflow = 0;

   printf("Entered unsigned_add\n");
  if ( a->len < b->len ) {
    temp->data =add_padding(a->data,a->len, b->len);
    temp->len = b->len;
    other = b; 
  }
  else { //this includes if they have teh same length 
    temp->data = add_padding(b->data,b->len, a->len);
    temp->len = a->len;
    other = a;
  }
  
  printf("This is temp->len and other->len: %lu and %lu\n", temp->len, other->len);
  printf("before create new_data in unsigned add\n");
  ApInt* new_data = (ApInt*)malloc(1 *sizeof(ApInt));
  new_data->data = (uint64_t*)malloc((temp->len) *sizeof(uint64_t));
  new_data->len = temp->len;

    printf("before for loop in unsigned add\n");
  for ( int i = temp->len-1; i >= 0; i--){
    printf("executing for loop in unsigned add\n");
    new_data->data[i] = other->data[i] + temp->data[i] + overflow;
    overflow = 0;
    if((new_data->data[i] < temp->data[i]) ||  (new_data->data[i] < other->data[i]) ) {
      overflow++;
    }	
  }
  
  if (overflow == 1) {
    //uint64_t* temp2 = new_data->data;
    //new_data->data =
    add_padding(new_data->data, other->len, (other->len + 1));
    new_data->len = (other->len + 1);
    new_data->data[0] = 1ul;
    printf("In overflow: new_data[0]= %lu,  new_data[1]= %lu\n", new_data->data[0],  new_data->data[1]);
    //apint_destroy(temp2); 
  }
  apint_destroy(temp);
  //apint_destroy(temp2);
  // printf("Return from unsigned_add with: %lu + %lu = %lu\n", a->data[0], b->data[0],new_data[0]);    
  printf("Outside of overflow: new_data[0]= %lu,  new_data[1]= %lu\n", new_data->data[0],  new_data->data[1]);
  return new_data;
}


ApInt * unsigned_sub(ApInt *a, ApInt *b) { //just need the magnitude
  ApInt* temp = (ApInt*)malloc( 1 *sizeof(ApInt)); //has the smaller number
  ApInt* other = NULL; //other is the bigger number
  uint64_t borrow = 0; 
  //boolean isGreater; //temp is greater 
  printf("Now in unsigned_sub\n");
  if ( a->len < b->len ) {
    temp->data = add_padding(a->data,a->len, b->len);
    temp->len = b->len;
    other = b;
  }
  else {
    temp->data = add_padding(b->data,b->len, a->len);
    temp->len = a->len;
    other = a;
  }
  printf("This is temp->len and other->len: %lu and %lu\n", temp->len, other->len);
  ApInt* new_data = (ApInt*)malloc(1 *sizeof(ApInt));
   new_data->data = (uint64_t*)malloc((temp->len) *sizeof(uint64_t));
  new_data->len = (temp->len);
  for( int i = temp->len -1; i >= 0; i--){
    printf("Current vals: temp[i] = %lu and other[i] = %lu\n",temp->data[i],temp->data[i] );
    if ((borrow > 1) && other->data[i] != 0){ //checking if you need to borrow
      printf("Need to borrow from next index\n");
      other->data[i] -= borrow;
      borrow = 0; 
    }
    if ((other->data[i] < temp->data[i])&& (other->len > 1)){
      printf("Need to borrow bc other is smaller than data\n");
      borrow += 1u;
    }
    if ( temp->data[i] > other->data[i]) { // getting the magnitude 
      new_data->data[i] = temp->data[i] - other->data[i];
      printf("Now subtracting %lu - %lu = %lu\n", temp->data[i],other->data[i],  new_data->data[i]);
    }else {
      new_data->data[i] = other->data[i] - temp->data[i];
      printf("Now subtracting %lu - %lu = %lu\n",other->data[i],temp->data[i],  new_data[i]);
    } 
    
    if ((borrow > 0) && (other->data[i] == 0)){ //when its 0 
      new_data->data[i] = ((2ul << 63) - 1) - new_data->data[i];
      borrow--;
      printf("After borrow: %lu\n", new_data->data[i]);
    }
    else if ( borrow > 0) { 
      new_data->data[i] = ((2ul << 63) - 1) - new_data->data[i];
      new_data->data[i] = new_data->data[i] -1;
    }
  } //end of for loop 
  if ((new_data->data[0] == 0) && (new_data->len >1)) {
    uint64_t* temp2 = new_data->data;
    new_data = rem_padding(new_data, temp->len);
    apint_destroy(temp2); 
  }
  apint_destroy(temp);
  //apint_destroy(temp2);   
  //printf("Now in unsigned_sub: %lu - %lu = %lu\n", a->data[0], b->data[0],new_data[0]);
  printf("Returning from unsigned_sub: \n");
  
  /*for ( int j = 0; j <(sizeof(new_data)/sizeof(uint64_t))-1; j++){
    printf("%lu ", new_data->data[j]);
    }*/
  return new_data;    
}

uint64_t* add_padding(uint64_t* a, uint32_t curr_len,uint32_t new_len){
  uint64_t* temp = (uint64_t*)malloc((new_len) *sizeof(uint64_t));
  int r = new_len - 1; //data storage for the new data
   printf("Entered add_padding\n");
  
  for ( int i = curr_len -1; i >= 0 ; i--){
    temp[r] = a[i];
    printf("a[%d] = %lu , temp[%d] = %lu\n",i, a[i],r, temp[r]);
    r--;
  }
  for ( ; r >= 0; r--){
    printf("Padding: Adds 0\n");
    temp[r] = 0ul;
    printf("temp[%d] = %lu\n", r,  temp[r]);
  }
   printf("This is after padding: \n");
   for (int j = 0; j < new_len-1; j++){
     printf("%lu ", temp[j]);
   }
   //free(a->data);
   //a->data = temp;
   //a->len = new_len;
   
   return temp;
}

uint64_t* rem_padding(uint64_t* a, uint32_t curr_len){
  int num2remove = 0; //how many indices to remove
  int idx = 0;
   printf("Entering remove padding: \n");
  while (a[idx] == 0) {
    num2remove++;
    idx++;
  }
  uint64_t* temp = (uint64_t*)malloc((curr_len - num2remove) *sizeof(uint64_t));
  int r = num2remove; // 000 000 100 va = 2
   printf("Entered rem_padding\n");
  for ( int i = 0; i < (curr_len - num2remove); i++){
    temp[i] = a[r];
    printf("loop in rem iterated\n");
    r++;
  }
   printf("This is after remove  padding: \n");
   for (int j = 0; j < (curr_len - num2remove); j++){
     printf("%lu ", temp[j]);
   }
   printf("This was before remove padding: %lu\n", curr_len);
    printf("This was after remove padding: %lu\n", curr_len-num2remove);
   //free(a->data);
   //a->data = temp;
   //a->len = (a->len - num2remove);
   return temp;
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
