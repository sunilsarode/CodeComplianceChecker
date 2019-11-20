//#include <stdio.h>

 int v1[20];
 int v2[20];

int total(int n, int a[static 20])//static keyword non compliant 
{
    int i;
    a[0]=1;
    int sum = 0U;
    /* Undefined behaviour if a has fewer than 20 elements */
    for (i = 0U; i < n; ++i)
    {
        sum = sum + a[i];
    }

    return sum;
}
/*
void g(void)
{
    int x;
    x = total(10U, v1);
    x = total(20U, v2);
    printf("%d\n",x);
    

}
int main(){
    g();

    return 0;
}*/
