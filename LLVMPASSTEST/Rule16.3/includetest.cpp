#include <iostream> 
#include </home/sunil/LLVMPASSTEST/case_test/simple.cpp>
#include "funhead.h"
#include<bits/stdc++.h>
using namespace std; 
  
int main() 
{ 
    int x = 1, y = 2; 
  
    // Outer Switch 
    switch (x) { 
  
    // If x == 1 
    case 1: 
  
        // Nested Switch 
  
        switch (y) { 
  
        // If y == 2 
        case 2: 
            cout << "Choice is 2"; 
            while(1){
                break;
            }
            break; 
  
        // If y == 3 
        case 3: 
            cout << "Choice is 3"; 
            //break; 

        default:
           break;
        } 
    //break; 

    case 3:
         switch(y){
             case 2:
             default:
                break;
         }
  
    // If x == 4 
    case 4: 
        cout << "Choice is 4"; 
        break; 
  
    // If x == 5 
    case 5: 
        cout << "Choice is 5"; 
        //break; 
  
    default: 
        cout << "Choice is other than 1, 2 3, 4, or 5"; 
        //break; 
    } 
    return 0; 
} 

