import io
import re
import zipfile

import requests
import tqdm
from bs4 import BeautifulSoup

# TODO добавить тесты: скачать страницу к себе и проверить отдельно функции

HEADER = {
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:45.0) Gecko/20100101 Firefox/45.0'
}
ROYALLIB_TXT_DOWNLOAD_PREFIX = '//royallib.com/get/txt/'


def get_html_text(url, headers):
    page = requests.get(url, headers=headers)
    return page.text


def get_txt_file_link(url):
    html_text = get_html_text(url, headers=HEADER)
    soup = BeautifulSoup(html_text, 'lxml')
    a_list = soup.find_all('a')
    for a in a_list:
        href = a.get('href')
        if href and href.startswith(ROYALLIB_TXT_DOWNLOAD_PREFIX):
            yield href


def get_book_list(html_text):
    soup = BeautifulSoup(html_text, 'lxml')
    a_list = soup.find_all('a', href=re.compile('books'))
    hrefs = [a.get('href') for a in a_list]
    for href in hrefs:
        html_text = get_html_text(f'https:{href}', headers=HEADER)
        soup = BeautifulSoup(html_text, 'lxml')
        a_list = soup.find_all('a', href=re.compile('book/'))
        current_hrefs = [a.get('href')[2:] for a in a_list]
        for ref in tqdm.tqdm(current_hrefs):
            for txt in get_txt_file_link(f'https://{ref}'):
                yield txt


with open('test.html', 'r') as html_file:
    page_text = html_file.read()

refs = []
for ref in get_book_list(page_text):
    refs.append(ref)
    if len(refs) > 5:
        break

for ref in refs:
    r = requests.get(f'https:{ref}')
    with open('test.txt', 'w') as result_file:
        with r, zipfile.ZipFile(io.BytesIO(r.content)) as archive:
            for file in archive.namelist():
                if file.endswith('.txt'):
                    file_text = archive.read(file).decode("windows-1251")
                    #  remove livelib watermark
                    start = file_text.find('Приятного чтения!')
                    end = file_text.rfind('Спасибо, что скачали книгу в бесплатной электронной библиотеке')
                    file_text = file_text[start + 18:end]
                    result_file.write(file_text)
