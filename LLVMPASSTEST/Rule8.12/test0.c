#include <stdio.h>
#include <stdlib.h>

enum colour
{
    red = 3,
    blue,
    green,//green will get value as 5
    yellow = 5,

};
enum day
{
    sunday = 1,
    monday,//monday have value as 2
    tuesday = 2,
    wednesday,
    thursday = 10,
    friday,
    saturday
};

enum state
{
    working,
    failed
};
enum result
{
    failed,
    passed
};
int main()
{
    enum colour col;
    col = blue;
    col = green;
    printf("%d", col);
    col = yellow;
    printf("%d", col);
    return 0;
}
