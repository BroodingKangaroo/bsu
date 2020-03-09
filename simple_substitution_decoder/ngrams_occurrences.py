import json
import logging
import re
from collections import defaultdict
from typing import (DefaultDict, Pattern, Dict)

from tqdm import tqdm

logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s :: %(levelname)s :: %(message)s",
    datefmt='%m-%d %H:%M:%S'
)
logger = logging.getLogger()
logger.setLevel(logging.INFO)


def normalize_text(text: str) -> str:
    """
    Bring all letters to uppercase and remove all unsupported symbols.
    :param text: text to normalize
    :return: normalized text
    """
    logger.info('Normalizing text.')
    text = text.upper()
    russian_alphabet_pattern: Pattern[str] = re.compile(r'[^А-Я ]')
    text = re.sub(russian_alphabet_pattern, '', text)
    return text


def count_ngrams_number(text: str, ngram_len: int) -> DefaultDict[str, int]:
    """
    Iterate throw symbols in text and count the number of times that each of ngrams occurred in the text.
    :param text: text to get ngrams numbers
    :param ngram_len: characters number in ngram
    :return: ngrams are keys and number of their occurrences in text are values
    """
    result: DefaultDict[str, int] = defaultdict(int)
    logger.info('Getting ngrams from text.')
    for idx in tqdm(range(len(text) - ngram_len + 1)):
        ngram = text[idx:idx + ngram_len]
        if ngram_check(ngram):
            result[ngram] += 1
    return result


def ngram_check(ngram: str) -> bool:
    """
    Check if ngram is possible for Russian language.
    """
    return not re.search(re.compile(r'([ЫЬЪ]{2,})|( [^АИЯУЕОКВ] )|( {2,})|(Ъ )'), ngram)


def count_ngrams_occurrences(input_file_path: str, output_file_path: str, length: int) -> None:
    """
    Count how often ngram occur in text. Save result of calculation to output file.
    :param input_file_path: path to input file in txt format
    :param output_file_path: path to output file in txt format
    :param length: length of ngram, i.e. for trigrams length=3
    """
    with open(input_file_path, 'r') as input_file:
        text = input_file.read()
    normalized_text: str = normalize_text(text)
    ngrams: Dict[str, int] = count_ngrams_number(normalized_text, length)
    logger.info('Saving results.')
    with open(output_file_path, 'w', encoding='utf-8') as output_file:
        json.dump(ngrams, output_file, ensure_ascii=False)
    logger.info('Done.')


if __name__ == '__main__':
    count_ngrams_occurrences('texts/result.txt', 'ngrams/ru-RU/new_space_trigrams.json', 3)
