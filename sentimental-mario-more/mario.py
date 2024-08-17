while True:
    name = input("Height: ")
    if name.isdigit():
        name = int(name)
        if name > 0 and name < 9:
            break
for i in range(name):
    print(" " * (name - i - 1), end="")
    print("#" * (i + 1), end="  ")
    print("#" * (i + 1))
