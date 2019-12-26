#include<stdlib.h>

//match ifStmt(hasElse(cxxBoolLiteral(equals(true))))
int fun(){
   
   return 0;
}
int isTrue(){
    return 0;
}
int main(){
     int x=0;
     int y=0;
     int z=1;
     if(x){
         ++y;
     }

     if(x||fun()){
         x++;
     }else if(x==1){
         fun();
     }else if(y==1){
         y=0; 
     }else{
              
          if(z==1){

          }else if(y){

          }//--> herer is the issue
     }

     if(isTrue()){
         y++;
         fun();
         if(x){
             while (y==0)
             {
                 if(x==1){
                     break;
                 }
             }
             
         }else if(x){

         }else{
             x++;
         }
     }else if(x==1){
   
     }else if(x==9){

     }else if(x==7){

     }else{
         
     }

     if(y){
         x++;
     }else{
         fun();
     }
    return 0;
}