#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>

void sums(long evenodd, long digit);
void next(long *nextNum, long *nextDig);
long oddSum = 0;
long evenSum = 0;

int main(void)
{
    long number = get_long("Number: "); // credit card number to be evaled
    long curDig = number % 10;          // current digit used in Luhn's algorithm
    long digitnum;                      // current digit position #
    long lastNum = number;              // used in forloop exit criteria
    bool visa = false;
    bool amex = false;
    bool mastercard = false;

    // Quit after reaching last digit or finding number is longer than max possible length 16
    for (digitnum = 1; ((curDig != lastNum) && (digitnum < 17)); digitnum++)
    {
        lastNum = number;

        // Equation for function next() and getting the digit in the next position
        next(&number, &curDig);

        // Testing values each step
        // printf("%ld\n", number);
        // printf("%ld\n", lastNum);
        // printf("%ld\n", curDig);
        // printf("%ld\n", digitnum);

        // Summing up the even and odd numbers
        sums(digitnum, curDig);

        // Determine if it's Visa, Amex, or Mastercard based on the # of digits and the leading 2 digits
        if (digitnum > 10)
        {
            // Visa
            //visa = (((digitnum == 11) || (digitnum == 14)) && (number > 39) && (number < 50));
            //amex = ((digitnum == 13) && ((number == 34) || (number == 37)));
            //if (mastercard = ((digitnum == 14) & (number > 50) && (number < 56))) // idk how to do a conditional and set at the same time
            if (((digitnum == 11) || (digitnum == 14)) && (number > 39) && (number < 50)) // If whole number starts with a "4"
            {
                visa = true;
            }
            // Amex
            else if ((digitnum == 13) && ((number == 34) || (number == 37)))
            {
                amex = true;
            }
            // Mastercard
            else if ((digitnum == 14) & (number > 50) && (number < 56))
            {
                mastercard = true;
            }
            //#endif
        }
    }
    // Evluate checksum and print type of valid card or invalid
    if ((oddSum + evenSum) % 10 == 0)
    {
        if (visa)
        {
            printf("VISA\n");
        }
        else if (amex)
        {
            printf("AMEX\n");
        }
        else if (mastercard)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
    // Testing digit sums and number length count
    // printf("length %ld\n", digitnum);
    // printf("odd sum %ld\n", oddSum);
    // printf("even sum %ld\n", evenSum);
}

// Summing the evenly and oddly positioned digits per Luhn's Algorithm
// oddSum and evenSum will be added together at the end to get the checksum.
// input: sums(digitnum, curDig), Called by main()
void sums(long evenodd, long digit)
{
    // Sum up oddly positioned numbers
    if ((evenodd % 2) == 1)
    {
        oddSum += digit;
    }
    // Sum up the sum of the digits of 2 times the evenly positioned digits
    else if ((evenodd % 2) == 0)
    {
        int evenMod = ((2 * digit) % 10);
        if (evenMod == (digit * 2))
        {
            evenSum += (digit * 2);
        }
        else
        {
            // Multiplying a digit in the 1's place by 2 will only result in a number with 2 digits max
            evenSum += (evenMod + (((digit * 2) - evenMod) / 10));
        }
    }
}

// Gets the digit in the one's place, removes that number from the one's place, shifts the number down by a factor of 10
// Input: next(&number, &curDig), Called by main()
void next(long *nextNum, long *nextDig)
{
    // Get's the digit in the ones place
    (*nextDig) = (*nextNum) % 10;
    // Remove digit in the one's place so mod 10 can grab the digit in the next position up
    (*nextNum) = (((*nextNum) - (*nextDig)) / 10);
}
