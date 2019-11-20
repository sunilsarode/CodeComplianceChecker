#include <stdlib.h>
#include <stdint.h>

typedef long long int ll;

/* Non-compliant - reuse */
void func(void)
{
    {
        typedef unsigned char u8_t;
    }
    {
        typedef unsigned char u8_t;
    }
}
typedef float float32_t;

/* Non-compliant - reuse */
typedef float mass;
void func1(void)
{
    float32_t mass = 0.0f;
}
typedef long long int ll;

/* Compliant - exception */
typedef struct list
{
    struct list *next;
    uint16_t element;
} list;

/* Non-compliant - tag "chain" not
* associated with typedef
*/
typedef struct
{
    struct chain
    {
        struct chain *list;
        uint16_t
            element;
    } s1;
    uint16_t length;
} chain;

int main()
{

    return 0;
}