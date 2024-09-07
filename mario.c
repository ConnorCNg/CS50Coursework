#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (int count = 1; count <= height; count++)
    {
        int nspace;
        int nhash;
        // Left-side pyramid
        for (nspace = 0; nspace < (height - count); nspace++)
        {
            printf(" ");
        }
        for (nhash = 0; nhash < (count); nhash++)
        {
            printf("#");
        }
        // 2-space gap
        printf("  ");
        // Right-side pyramid
        for (nhash = 0; nhash < (count); nhash++)
        {
            printf("#");
        }
        // for (nspace = 0; nspace < (height - count); nspace++)
        //{
        //    printf(" ");
        // }

        printf("\n");
    }
}
