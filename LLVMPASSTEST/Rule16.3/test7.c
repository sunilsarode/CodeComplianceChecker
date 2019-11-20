
#include <stdio.h>
#include <stdlib.h>
#include </home/sunil/LLVMPASSTEST/Rule16.3/head.c>
#include "myhead.h"
int main(){

int x,y;
switch(x){

  case 1:

     switch(y){
       
        case 1:
            printf("%d",1);
            while(0){
                break;
            }
            //break;
        case 2:

            //break;



        default:
            x++;
           // break;
     }
     //break;

  case 4:
      printf("%d",1);
      if(y==1){
          break;
      }
     //break;

  case 5:
      x++;
     //break; 
  default:
     x++;
     break;
}
return 0;
}