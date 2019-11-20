#include <stdio.h>

int main()
{
    int x = 012;//issue
    int y = 1030;//no issue
    int z = 0155;//issue
    double val=0.09;
    char str[] = "31\01267";// no issue
    printf("%d", x);
    return 0;
}