#include <stdio.h> 
  
// Inline function in C 
static inline int foo() 
{ 
    return 2; 
} 
 inline int fun() 
{ 
    return 0; 
} 
  
  
// Driver code 
int main() 
{ 
  
    int ret; 
  
    // inline function call 
    ret = foo(); 
    ret = fun();
  
    printf("Output is: %d\n", ret); 
    return 0; 
} 