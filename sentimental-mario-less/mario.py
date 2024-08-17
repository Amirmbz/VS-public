while True:
    name = input("Height: ")
    if name.isdigit():
        name = int(name)
        if name > 0 and name < 9:
            break
for i in range(name):
    print(f" " * (name - i - 1), end="")
    print(f"#" * (i + 1))
