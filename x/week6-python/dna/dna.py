import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: dna.py databases/[..].csv sequences/[..].txt")
        sys.exit(1)

    # Read database file into a variable
    database = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            database.append(row)

        STRs = []
        for col in reader.fieldnames:
            if col != "name":
                STRs.append(col)

    # Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        sequence = file.read()

    # Find longest match of each STR in DNA sequence
    counts = {}
    for STR in STRs:
        longest_run = longest_match(sequence, STR)
        counts[STR] = longest_run

    # Check database for matching profiles
    for person in database:
        found = True
        for STR in counts:
            if counts[STR] != int(person[STR]):
                found = False
                break
        if found:
            print(person["name"])
            break
    else:
        print("No match")

    sys.exit(0)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in sequence, return longest run found
    return longest_run


main()
