
#include <stdio.h>
#include <stdlib.h>

int main()
{

    int x, y;
    switch (x)
    {
    default://compliant
        x++;
        break;

    case 1:

        switch (y)
        {

        case 1:
            printf("%d", 1);
            while (0)
            {
                break;
            }
        //break;
        default://non compliant ,
            x++;
            break;
        case 2:;
            // break;
        }
        //break;

    case 4:
        printf("%d", 1);
        if (y == 1)
        {
            break;
        }
        //break;

    case 5:
        x++;
        //break;
    }
    return 0;
}