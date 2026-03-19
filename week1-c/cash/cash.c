#include <cs50.h>
#include <stdio.h>

int calculate_coins(int cents, int coin_value);

int main(void)
{
    // prompt the user for change owed
    int change;
    do
    {
        change = get_int("Change owed (in cents)? ");
    }
    while (change < 0);

    // Calculate how many quarters you should give customer
    int quarters = calculate_coins(change, 25);
    // Subtract the value of those quarters from cents
    change -= quarters * 25;

    int dimes = calculate_coins(change, 10);
    change -= dimes * 10;

    int nickels = calculate_coins(change, 5);
    change -= nickels * 5;

    int pennies = calculate_coins(change, 1);
    change -= pennies * 1;

    int sum = (quarters + dimes + nickels + pennies);

    printf("The minimum coins needed is %i\n", sum);
}

int calculate_coins(int cents, int coin_value)
{
    int coins = 0;
    while (cents >= coin_value)
    {
        coins++;
        cents -= coin_value;
    }
    return coins;
}
