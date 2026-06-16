from cs50 import get_int


def main():
    # Prompt the user for the height
    while True:
        height = get_int("Height: ")
        if 1 <= height <= 8:
            break

    # Print the pyramid
    for i in range(1, height + 1):
        print_row(i, height)


def print_row(bricks, height):
    spaces = height - bricks

    # Print the right pyramid and the gap
    print(spaces * " ", end="")

    print(bricks * "#", end="  ")

    # Print the left pyramid
    print(bricks * "#")


main()
