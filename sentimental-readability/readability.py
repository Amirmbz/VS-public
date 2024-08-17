text = input('Text: ')
letters = [char for char in text if char.isalpha()]
words = text.split()
sentences = text.count('.') + text.count('?') + text.count('!')
lett = (len(letters) / len(words)) * 100
sent = (sentences / len(words)) * 100
grade = round(0.0588 * lett - 0.296 * sent - 15.8)
if 0 < grade < 16:
    print(f'Grade {grade}')
elif grade > 15:
    print('Grade 16+')
else:
    print('Before Grade 1')
