
#include <stdio.h>
#include <stdlib.h>

int main()
{

    int x, y;
    scanf("%d", &y);
    scanf("%d", &x);
    switch (x)
    {

    case 1:

        switch (y)
        {

        case 1:
            printf("%d", 1);
            while (0)
            {
            case 6://it is non compliant 
                x++;
                break;
            }
            //break;
        case 2:
            if (y)
            {

            case 5://it is non compliant 
                x++;
            }
            //break;

        
            do
            {
            default://it is non compliant 
                break;
            } while (x--);
            x++;
            // break;
        }
        //break;

    case 4:
        printf("%d", 1);
        if (y)
        {

        case 5://it is non compliant 
            x++;
        }
        break;

    default:
        x++;
    }
    return 0;
}