#include<iostream>

using namespace std;
void fun(){
    
}
int main(){

     int x=0;
     if(x==1){
         cout<<x<<endl;
     }else if(x==2){
          x++;
     }else if (x==5){
         --x;
     }else{
         cout<<x<<endl;
         
     }

     if(x==9){
          fun();
     }else{
         fun();
     }
     
    return 0;
}
