#include <stdio.h> 
typedef int int32_t;

void f ( void )
{
int32_t j = 0;
L1:
    ++j;
    if ( 10 == j )
    {
    goto L2;
    }
    goto L2;

L2:
    ++j;

    goto L1;
}
  
// function to check even or not 
void checkEvenOrNot(int num) 
{ 
 
 even:
     printf("%d",9) ;  
    if (num % 2 == 0) 
        // jump to even 
        goto even;  
    else
        // jump to odd 
        goto odd;  
  
even: 
    printf("%d is even", num); 
    // return if even 
    return;  
odd: 
    printf("%d is odd", num); 
} 

void fun(int *restrict a, int *volatile b){

}  

int main() { 
    int num = 26; 
    int  *restrict ptr;
    int *volatile ptr1;
    int Goto=0;
   // checkEvenOrNot(num); 
    f();
    return 0; 
} 