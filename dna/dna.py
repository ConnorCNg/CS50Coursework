import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py databases.csv sequences.txt")
        sys.exit(1)

    # TODO: Read database file into a variable
    dbase = []
    with open(sys.argv[1], 'r') as file:
        reader = csv.reader(file)
        header = next(reader)
        for row in reader:
            rowdict = {}
            for i in range(0, len(row)):
                rowdict[header[i]] = row[i]
            dbase.append(rowdict)
    file.close()

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], 'r') as file2:
        reader2 = csv.reader(file2)
        sequence = next(reader2)
    file2.close()

    # TODO: Find longest match of each STR in DNA sequence
    consec = {}
    for i in range(1, len(header)):
        consec[header[i]] = longest_match(sequence[0], header[i])

    # TODO: Check database for matching profiles
    found = False
    for person in dbase:
        match = 0
        for i in range(1, len(header)):
            if int(person[header[i]]) == consec[header[i]]:
                match += 1
            else:
                break
        if match == len(header) - 1:
            print(person['name'])
            found = True
            break
    if found:
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

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
