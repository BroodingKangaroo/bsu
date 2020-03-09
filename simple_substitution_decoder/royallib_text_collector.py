import io
import re
import zipfile
from typing import Dict
from typing import List

import requests
import tqdm
from bs4 import BeautifulSoup

# TODO добавить тесты: скачать страницу к себе и проверить отдельно функции

USER_REQUEST_HEADER = {
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:45.0) Gecko/20100101 Firefox/45.0'
}

ROYALLIB_URL = 'https://royallib.com/'
ALPHABET_BOOK_URL_TRIGGER = 'books'
BOOK_URL_TRIGGER = 'book/'


def get_html_text(url: str, headers: Dict[str, str]) -> str:
    """
    Get text of html document by url
    :param url: url to page to get text of html
    :param headers: headers to simulate user requests
    :return: text of html document
    """
    page = requests.get(url, headers=headers)
    return page.text


def get_txt_zip_link(page_url: str) -> str:
    """
    Get link to zip archive with txt file of book using url to book page at Royallib.
    At the book page usually is only one url to zip archive with txt.
    :param page_url: url to book page
    :return: url to zip archive with book in txt format
    """
    royallib_txt_zip_download_prefix = '//royallib.com/get/txt/'
    html_text = get_html_text(page_url, headers=USER_REQUEST_HEADER)
    soup = BeautifulSoup(html_text, 'lxml')
    a_list = soup.find_all('a')
    for a in a_list:
        href = a.get('href')
        if href and href.startswith(royallib_txt_zip_download_prefix):
            return f'https:{href}'


def get_page_urls_by_trigger(page_url: str, url_trigger: str) -> List[str]:
    """
    Get url from html page only if it contains url_trigger.
    :param page_url: url to page to collect urls
    :param url_trigger: text which url should contain
    :return: appropriate urls
    """
    html_text = get_html_text(page_url, USER_REQUEST_HEADER)
    soup = BeautifulSoup(html_text, 'lxml')
    a_tags = soup.find_all('a', href=re.compile(url_trigger))
    hrefs = [f"https:{a_tag.get('href')}" for a_tag in a_tags]
    return hrefs


def get_txt_zip_urls(urls_number: int) -> List[str]:
    """
    Collect urls of zip with book in txt format from Royallib.com
    :param urls_number: number of urls to collect
    :return: urls
    """
    # get pages with books by alphabet
    hrefs = get_page_urls_by_trigger(ROYALLIB_URL, ALPHABET_BOOK_URL_TRIGGER)

    # get urls to zip archives with books in txt format
    for href in hrefs:
        # get url of page of each book
        current_hrefs = get_page_urls_by_trigger(href, BOOK_URL_TRIGGER)

        # get url to zip with txt file of each book
        refs = []
        for ref in tqdm.tqdm(current_hrefs):
            refs.append(get_txt_zip_link(ref))
            if len(refs) == urls_number:
                break
        return refs


def remove_royallib_marks(text: str) -> str:
    """
    Remove royallib marks, that Royallib insert in the beginning and ending of book
    """
    start = text.find('Приятного чтения!')
    end = text.rfind('Спасибо, что скачали книгу в бесплатной электронной библиотеке')
    text = text[start + len('Приятного чтения!'):end]
    return text


def collect_royallib_texts_to_file(output_file_path: str, text_number: int) -> None:
    """
    Collect books in txt format from Royallib.com and merge all of them in txt file.
    :param output_file_path: path to output file in txt format
    :param text_number: number of books which will be stored in output_file
    """
    refs = get_txt_zip_urls(text_number)
    for ref in refs:
        r = requests.get(ref)
        with open(output_file_path, 'a') as output_file:
            with r, zipfile.ZipFile(io.BytesIO(r.content)) as archive:
                for file in archive.namelist():
                    if file.endswith('.txt'):
                        text = archive.read(file).decode("windows-1251")
                        text = remove_royallib_marks(text)
                        output_file.write(text)


if __name__ == '__main__':
    collect_royallib_texts_to_file('result.txt', 5)
