#include<iostream>

int fun(){
   return 1;
}
int main(){
    int x=0;
    int z=1;
    int i=0;
    if(x==1||x++){
        std::cout<<"Testing side effects"<<std::endl;
    }else if(z&&x==i++){

    }
    if(x==0&&fun()){
        
    }
    int y=x+1&&fun();
    return 0;
}