#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long initial_number;
    do
    {
        initial_number = get_long("Number: ");
    }
    while (initial_number < 0);
    int sum = 0;
    long number = initial_number;
    while (number > 0)
    {
        int digit = number % 10;
        sum = sum + digit;
        number = number / 10;
        digit = number % 10;
        if (digit > 4) // In this part, I figured since our highest number is 9, the maximum product
                       // they can yield is 18 when multiplied by two. Hence, if they're more than
                       // 4, we can subtract 5 from each of them. This ensures that they cannot
                       // exceed more than 8. We then add an extra 1 to the overall sum to
                       // compensate for the 5 that we've subtracted earlier."
        {
            digit = digit - 5;
            sum = sum + 1;
        }
        sum = sum + digit * 2;
        number = number / 10;
    }
    number = initial_number / 1000000000000;
    if ((sum % 10 == 0) & ((1000 > number) & (number > 99)) &
        ((number / 10 == 34) || (number / 10 == 37)))
    {
        printf("AMEX\n");
    }
    else if ((sum % 10 == 0) & ((10000 > number) & (number > 999)) &
             ((number / 100 == 51) || (number / 100 == 52) || (number / 100 == 53) ||
              (number / 100 == 54) || (number / 100 == 55)))
    {
        printf("MASTERCARD\n");
    }
    else if ((sum % 10 == 0) & ((((10 > number) & (number > 0)) & (number == 4)) ||
                                (((10000 > number) & (number > 999)) & (number / 1000 == 4))))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
