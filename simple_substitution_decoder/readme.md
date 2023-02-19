# Simple substitution decoder
The program is intended to decode texts encoded by the [simple substitution cipher](https://en.wikipedia.org/wiki/Substitution_cipher). 
It uses [frequency analysis](https://en.wikipedia.org/wiki/Frequency_analysis) to accomplish the goal. The examples of the encoded and decoded texts in Russian can be found in the examples folder.

It can be run in three steps:

1) Run royallib_text_collector.py to query public online library [Royallib.com](https://royallib.com/) with free books available. It downloads 10000 books from the site, removes headings and compresses them in one file.
2) Run ngrams_occurrences.py to create a json file in which each ngram encountered associated with the number of times it has been found.
3) Run decoder.py to decode the text encoded with the simple substitution cipher.

Before going through all of the steps look for a moment in the source code of each file (in the `__name__ == '__main__'` section) and make necessary changes in order to make the pathes of files correspond with the ones in your system.
