import logging
import math
import os
import random

MAX_FAILURE_LIMIT = 5000
MAX_SWAP_COUNT_LIMIT = 5
START_SWAP_COUNT = 2

class SimpleSubstitutionDecoder:
    ngrams_directory = 'simple_substitution_decoder\\ngrams'
    upper_symbols_directory = 'simple_substitution_decoder\\upper_symbols'

    def __init__(self, text: str, language='ru-RU'):
        self.text = list(text)
        self.language = language
        self.read_ngrams()
        self.alphabet = ['А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С',
                         'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я', ' ']
        #  calculate log probabilities
        self.N = sum(self.ngrams.values())
        self.floor = math.log10(0.01 / self.N)
        for key in self.ngrams.keys():
            self.ngrams[key] = math.log10(self.ngrams[key] / self.N)
        self.score = 0

    def read_ngrams(self, encoding=None):
        ngrams_path = os.path.join(self.ngrams_directory, self.language, '3gr_space')
        self.ngrams = {}
        with open(ngrams_path, 'r', encoding=encoding) as file:
            raw_text = file.read()
            for line in raw_text.split('\n'):
                ngram = line.split(':')[0]
                count = line.split(':')[1]
                self.ngrams[ngram] = float(count)
        try:
            self.L = len(ngram)
        except:
            logging.error('File shouldn\'t be empty.')
            raise

    def decode(self):
        self.decoded_text = self.text.copy()
        self.score = self.get_score(self.decoded_text)
        failure_count = 0
        swap_count = 2
        while failure_count < MAX_FAILURE_LIMIT and swap_count <= MAX_SWAP_COUNT_LIMIT:
            current_text = swap_alphabet_symbols(self.decoded_text.copy(), self.alphabet, swap_count)
            current_score = self.get_score(current_text)
            if current_score > self.score:
                self.score = current_score
                self.decoded_text = current_text
                failure_count = 0
                swap_count = START_SWAP_COUNT
            else:
                failure_count += 1
            if failure_count == MAX_FAILURE_LIMIT:
                failure_count = 0
                swap_count += 1
                print(swap_count)
        self.decoded_text = ''.join(self.decoded_text)
        return self.decoded_text

    def get_score(self, text, ngram_len=3):
        score = 0.0
        for i in range(len(text) - ngram_len):
            if ''.join(text[i:i + ngram_len]) in self.ngrams:
                score += self.ngrams[''.join(text[i:i + ngram_len])]
            else:
                score += self.floor
        return score


def swap_alphabet_symbols(text, alphabet, count):
    symbols2swap = set()
    while len(symbols2swap) != count:
        symbols2swap.add(random.choice(alphabet))
    symbols2swap = list(symbols2swap)
    for idx, symbol in enumerate(text):
        if symbol in symbols2swap:
            text[idx] = symbols2swap[symbols2swap.index(symbol) - 1]
    return text
