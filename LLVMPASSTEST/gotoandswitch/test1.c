

void f1 ( int a ){


    int x;
    if ( a <= 0 ){
        goto L2;  /* Non-compliant */
    }
    goto L1;
    if ( a == 0 ){
        goto L1;
    }
    goto L2;   /* Non-compliant */

    L1:
      x++;
    L3:
      x++;
    L4:    
        if ( a > 0 )
        {
            L2: 
            ;
        }
}

int main(){
    
    return 0;
}