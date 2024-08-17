name = input("cash: ")
name = int(float(name) * 100)
print(int(name / 25) +
      int((name % 25) / 10) +
      int(((name % 25) % 10) / 5) +
      int((((name % 25) % 10) % 5) / 1))
