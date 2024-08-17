while True:
    name = input("Change: ")
    try:
        name = int(float(name) * 100)
        if name > 0:
            break
    except ValueError:
        pass
print(int(name / 25) +
      int((name % 25) / 10) +
      int(((name % 25) % 10) / 5) +
      int((((name % 25) % 10) % 5) / 1))
