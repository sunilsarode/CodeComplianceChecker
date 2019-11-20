//clang -Xclang -ast-dump -fsyntax-only test.cc
int main()
{
    int x = 1, y = 2, z = 0;

    // Outer Switch
    switch (x)
    {

    case 1://non compliant

        switch (y)
        {

        case 2:
            while (1)
            {
                break;
            }
            do
            {
                /* code */
                break;
            } while (x == 0);

            if (y == 1)
            {
            }
            else
            {
            }

            if (y == 2)
            {
            }
            else if (x == 1)
            {
            }
            else
            {
                /* code */
            }

            for (int i = 0; i < 0; i++)
            {
                break;
            }

            break;

        case 3://non compliant
            if(y){
                 x++;
                 break;
            }
        default:
            break;
        }

    case 3://non compliant
        switch (y)
        {
        case 2://non compliant
            //default:
            switch (z == 1)
                break;
            while (y++)
                ;
        }

    case 4:
        break;

    // If x == 5
    case 5:
        break;

    default:
        x--;
        break;
    }
    return 0;
}