import math
import os
import random
import re

RUSSIAN_ALPHABET = (r'[^А-ЯЁ]')


def read_ngrams(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        row_text = file.read()
        return {
            ngram.split()[0]: math.log10(float(ngram.split()[1]))
            for ngram in row_text.split('\n') if ngram.split()
        }


def delete_non_alphabet_characters(text):
    pattern = re.compile(RUSSIAN_ALPHABET)
    text = pattern.sub('', text)
    return text


def read_text_to_decode(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        row_text = file.read()
        text = delete_non_alphabet_characters(row_text)
        return text


def get_score(text, ngrams, ngram_len=4):
    score = 0.0
    for i in range(len(text) - ngram_len):
        if text[i:i + ngram_len] in ngrams:
            score += ngrams[text[i:i + ngram_len]]
    return score


def swap_two_symbols(text, symbol_1, symbol_2):
    result = ''
    for symbol in text:
        if symbol == symbol_1:
            result += symbol_2
        elif symbol == symbol_2:
            result += symbol_1
        else:
            result += symbol
    return result


def decode(text, ngrams, iteration_number=100000):
    cyrillic_upper_symbols = ['А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р',
                              'С',
                              'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я']
    score = get_score(text, ngrams)
    failure = 0
    for i in range(iteration_number):
        symbol_1 = random.choice(cyrillic_upper_symbols)
        symbol_2 = random.choice(cyrillic_upper_symbols)
        current_text = swap_two_symbols(text, symbol_1, symbol_2)
        current_score = get_score(current_text, ngrams)
        if current_score > score:
            score = current_score
            text = current_text
            failure = 0
        else:
            failure += 1
        if failure > 1000:
            break
    return text


TO_DECODE_DIR = 'simple_substitution_decoder/texts'
DECODED_DIR = 'simple_substitution_decoder/decoded_texts'
TEXT_TO_DECODE = 'text_4'

ngrams = read_ngrams('simple_substitution_decoder/ngrams/russian_quadgrams.txt')
text = read_text_to_decode(os.path.join(TO_DECODE_DIR, TEXT_TO_DECODE))
result_text = ''
max_score = 0
for i in range(5):
    result = decode(text, ngrams)
    score = get_score(result, ngrams)
    print(score)
    if score > max_score:
        max_score = score
        result_text = result

print(result_text, max_score)
with open(os.path.join(DECODED_DIR, TEXT_TO_DECODE), 'w', encoding='utf-8') as file:
    file.write(result_text)
