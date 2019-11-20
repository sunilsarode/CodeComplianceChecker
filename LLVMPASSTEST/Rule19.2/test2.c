#include <stdio.h> 
  
struct  Union
{
    /* data */
    int i;
    int j;
    union test2
    {
        int k;
    } test2;
    
};
  
int main() 
{ 
    printf("Hi");
    return 0; 
}