

void f1 ( int a ){


    int x;
    if ( a <= 0 ){
        L2:
        goto L1;  /* Non-compliant */
    }else{
        L1:
        x++;
        goto L2;
    }
    goto L1;
    if ( a == 0 ){
        goto L1;
    }
   
}

int main(){
    
    return 0;
}