"""Decode string which is encoded by simple substitution cipher"""
import json
import logging
import math
import secrets
from typing import (Dict, List)

import tqdm

logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s :: %(levelname)s :: %(message)s",
    datefmt='%m-%d %H:%M:%S'
)
logger = logging.getLogger()
logger.setLevel(logging.INFO)


class SimpleSubstitutionDecoder:
    """Decode simple substitution cipher"""

    def __init__(self, text: str, ngrams: Dict[str, int], alphabet: List[str], start_swap_count=2,
                 failure_count_limit=1000, swap_count_limit=3):
        self._text = list(text)
        self._alphabet = alphabet
        self._n = sum(ngrams.values())
        self._len = len(list(ngrams.keys())[0])
        self._floor = math.log10(0.01 / self._n)
        self._ngrams = self._calculate_log_probabilities(ngrams)
        self._start_swap_count = start_swap_count
        self._swap_count_limit = swap_count_limit
        self._failure_count_limit = failure_count_limit

    def _calculate_log_probabilities(self, ngrams: Dict[str, float]) -> Dict[str, float]:
        """
        Calculate log probability for each ngram. The probability of a specific ngram is calculated
        by dividing the count of that ngram by the total number of ngram in our training corpus.
        The log probability is simply the logarithm of this number.
        """
        for key in ngrams.keys():
            ngrams[key] = math.log10(ngrams[key] / self._n)
        return ngrams

    def decode(self, repeat_count):
        """
        Make several repetitions of the decoding process and select the best result.
        """
        max_score_text = ''
        max_score = -math.inf
        for _ in tqdm.tqdm(range(repeat_count)):
            score, text = self._decode_inner()
            if score > max_score:
                logger.info(score)
                max_score = score
                max_score_text = ''.join(text)
                logger.info(max_score_text)
        return max_score_text

    def _decode_inner(self):
        """
        Main decode function. Implement hill-climbing algorithm.
        Swap swap_count symbols in the text.
        If after that transposition text became more "closer" to Russian then we apply this transposition.
        Otherwise we roll back this transposition and try other one. Repeat the described procedure some times.
        If max_fitness_score does not change already self._failure_count_limit times increase swap_count by 1.
        If swap_count became grater than self._swap_count_limit end the procedure.
        :return: text witch is more "closer" to Russian from all text variants.
        """
        max_fitness_score_text = self._text.copy()
        max_fitness_score = self._get_fitness_score(max_fitness_score_text)
        failure_count = 0
        swap_count = self._start_swap_count
        while failure_count <= self._failure_count_limit and swap_count <= self._swap_count_limit:
            current_fitness_score_text = self._swap_alphabet_symbols(max_fitness_score_text.copy(), swap_count)
            current_fitness_score = self._get_fitness_score(current_fitness_score_text)
            if current_fitness_score > max_fitness_score:
                max_fitness_score = current_fitness_score
                max_fitness_score_text = current_fitness_score_text
                failure_count = 0
                swap_count = self._start_swap_count
            else:
                failure_count = failure_count + 1 if failure_count != self._failure_count_limit else 0
                swap_count = swap_count + 1 if failure_count == self._failure_count_limit else swap_count
        return max_fitness_score, max_fitness_score_text

    def _get_fitness_score(self, text):
        """
        Calculate log probability score of the text. This log probability is used as the 'fitness' of a
        piece of text, a higher number means it is more likely to be decoded text,
        while a lower number means it is less likely to be decoded text.
        """
        score = 0.0
        for i in range(len(text) - self._len):
            if ''.join(text[i:i + self._len]) in self._ngrams:
                score += self._ngrams[''.join(text[i:i + self._len])]
            else:
                score += self._floor
        return score

    def _swap_alphabet_symbols(self, text, number):
        """
        Swap number of random chosen from self.alphabet symbols in text cyclically. Let number=3
        symbols2swap={'А','Б','В'} then all occurrences of 'A' will be replaced by 'Б', 'Б' will be replaced by 'В'
        and 'В' will be replaced by 'А'.
        :param text: text in which needs to swap symbols
        :param number: number of symbols to swap
        :return: text with symbols that are already swapped
        """
        symbols2swap = set()
        while len(symbols2swap) != number:
            symbols2swap.add(secrets.choice(self._alphabet))
        symbols2swap = list(symbols2swap)
        for idx, symbol in enumerate(text):
            if symbol in symbols2swap:
                text[idx] = symbols2swap[symbols2swap.index(symbol) - 1]
        return text


if __name__ == '__main__':
    russian_alphabet = ['А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С',
                        'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я', ' ']
    with open('ru_trigrams.json', 'r') as ngrams_file:
        ngrams_ = json.load(ngrams_file)
    with open('texts/new_text_1.txt', 'r', encoding='utf-8') as text_file:
        text_ = text_file.read()
        decoder = SimpleSubstitutionDecoder(text_, ngrams_, russian_alphabet)
    decoded_text = decoder.decode(500)
    with open('result.txt', 'w', encoding='utf-8') as output_file:
        output_file.write(decoded_text)
