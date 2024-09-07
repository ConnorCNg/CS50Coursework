#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

void test(int *a, int *b);

#ifdef credit_test
int main(void)
{
    int x = 1;
    int y = 5;
    test(&x, &y);
    printf("%i\n", x);
    printf("%i\n", y);
}

void test(int *a, int *b)
{
    (*a) = (*a) + (*a);
    (*b) = (*b) + (*b);
}



int main(void)
{
    string s = "ab cd.";
    string i = "1";
    char x = i[0];
    char c = toupper((char)s[atoi(i)]);
    char z = 'Z';
    string test = "";
    int letter = 'A';
    float index = 1.7;

    printf("Grade %i\n", (int)round(index));
}

#endif

int main(void)
{
    while (bool x = false)
    {
        for (int i = 0; i < 5; i++)
        {
            x = true
            printf("%i\n", i)
            if (i == 3)
            {
                x =
            }
        }
    }
}
