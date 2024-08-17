import re

while True:
    number = input("Number: ")
    try:
        number = int(number)
        if number > 0:
            break
    except ValueError:
        pass
number_str = str(number)
odd_number = re.findall(r'\d', number_str[-1::-2])
even_number = re.findall(r'\d', number_str[-2::-2])
sum = sum(map(int, odd_number))
for i in even_number:
    i = int(i)
    if i > 4:
        sum += 1
        i += 5
    sum += i * 2
if sum % 10 != 0:
    print('INVALID')
elif len(number_str) == 15 and (number_str[0:2] in {'34', '37'}):
    print('AMEX')
elif len(number_str) == 16 and (50 < int(number_str[0:2]) < 56):
    print('MASTERCARD')
elif (len(number_str) == 13 or len(number_str) == 16) and number_str[0] == '4':
    print('VISA')
else:
    print('INVALID')
