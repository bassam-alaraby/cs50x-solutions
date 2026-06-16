from cs50 import get_float


def main():
    # Prompt the user for the change
    while True:
        change = get_float("Change owed: $")
        if change > 0:
            change *= 100
            break

    # Calculate the minimum number of coins
    quarters = calc_coins(change, 25)
    change -= quarters * 25

    dimes = calc_coins(change, 10)
    change -= dimes * 10

    nickels = calc_coins(change, 5)
    change -= nickels * 5

    pennies = calc_coins(change, 1)
    change -= pennies * 1

    # Print it out
    sum = quarters + dimes + nickels + pennies
    print(sum)


def calc_coins(change, coin_value):
    coins = 0
    while change >= coin_value:
        coins += 1
        change -= coin_value
    return coins


main()
