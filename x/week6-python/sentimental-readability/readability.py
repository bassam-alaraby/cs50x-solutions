from cs50 import get_string


def main():
    # Prompt the user for text
    txt = get_string("Text: ")

    # Count the number of letters, words, sentences in the text
    letters = count_letters(txt)
    words = count_words(txt)
    sentences = count_sentences(txt)

    # Compute Coleman-Liau index
    l = (letters / words) * 100
    s = (sentences / words) * 100

    index = 0.0588 * l - 0.296 * s - 15.8
    grade = round(index)

    # Print the grade level
    if grade > 16:
        print("Grade 16+")
    elif grade < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {grade}",)


def count_letters(txt):
    letters = 0
    for c in txt:
        if c.isalpha():
            letters += 1
    return letters


def count_words(txt):
    spaces = 0
    for c in txt:
        if c == " ":
            spaces += 1

    words = spaces + 1
    return words


def count_sentences(txt):
    sentences = 0
    for c in txt:
        if c in [".", "!", "?"]:
            sentences += 1
    return sentences


main()
