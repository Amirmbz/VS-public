import csv
import sys


def main():

    if len(sys.argv) != 3:
        print("ERROR")
        return

    database = []
    with open(sys.argv[1], mode='r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            person = {}
            for key, value in row.items():
                person[key] = value
            database.append(person)

    with open(sys.argv[2], 'r') as file:
        sequencese = file.read()

    ans = {}
    keys = list(database[0].keys())

    for key in keys:
        ans[key] = longest_match(sequencese, key)

    for row in database:
        if all(int(ans[key]) == int(row[key]) for key in keys[1:]):
            print(row['name'])
            return

    print("No match")
    return


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

        # Check for a subsequence match in a "substring" (a subset of
        # characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of
        # consecutive matches
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

    # After checking for runs at each character in seqeuence, return longest
    # run found
    return longest_run


main()
