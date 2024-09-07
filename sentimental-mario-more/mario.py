def get_int(prompt):
    try:
        return (int(input(prompt)))
    except ValueError:
        return 0


def create_pyramid(ht):
    for i in range(1, ht + 1):
        row = ((ht - i) * " ") + (i * "#")
        row = row + "  "
        row = row + (i * "#")
        print(row)


def main():
    h = 0
    while ((h < 1) or (h > 8)):
        h = get_int("Height (1-8): ")
    create_pyramid(h)


main()
