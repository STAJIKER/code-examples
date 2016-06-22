#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
#
import sys
import re
from itertools import groupby
from operator import itemgetter

class Mapper:

    re_en = re.compile(ur"[a-z]+")
    re_ru = re.compile(ur"[а-яё]+")
    popular_en = ["the", "of", "and", "in", "a", "to", "was", "is", "s", "for", "on", "as", "by", "with", "he", "at", "that", "from", "his", "it"]
    popular_ru = ["в", "и", "на", "с", "категория", "по", "года", "из", "году", "а", "к", "не", "от", "для", "был", "что", "за", "его", "как", "до"]

    def __init__(self, lang):
        if lang == "en":
            self.re = Mapper.re_en
            self.popular = Mapper.popular_en
        else:
            self.re = Mapper.re_ru
            self.popular = Mapper.popular_ru
        self.results = {}

    def run(self):
        doc_count = 0
        data = self.readInput()
        for docid, content in data:
            text = content.lower()
            word_count = 0
            for match in self.re.finditer(text):
                word = match.group(0)
                if word not in self.popular:
                    self.addResult(word)
                    word_count += 1
            sys.stderr.write("reporter:counter:MyCounters,InputWords,%d\n" % word_count)
            doc_count += 1
            if doc_count % 1000 == 0:
                sys.stderr.write("reporter:status:Processed %d documents\n" % doc_count)
            self.emitResults(docid, word_count)
        sys.stderr.write("reporter:counter:MyCounters,NumberDocs,%d\n" % doc_count)

    def readInput(self):
        for line in sys.stdin:
            yield unicode(line, 'utf8').strip().split('\t', 1)

    def addResult(self, word):
        if word in self.results:
            self.results[word] += 1
        else:
            self.results[word] = 1

    def emitResults(self, docid, doc_words):
        for word, count in self.results.iteritems():
            print ('%s\t%s:%d:%d' % (word, docid, count, doc_words)).encode('utf-8')
        self.results = {}


class Reducer:

    def run(self):
        cur_word = '1'
        doc_count = 0
        data = self.readInput()
        for word, docid, count, doc_words in data:
            if cur_word != word:
                if cur_word != '1':
                    print ('%s+-1\t%d:0' % (cur_word, doc_count)).encode('utf-8')
                cur_word = word
                doc_count = 0
            doc_count += 1
            print ('%s+%s\t%s:%s' % (word, docid, count, doc_words)).encode('utf-8')
        print ('%s+-1\t%d:0' % (cur_word, doc_count)).encode('utf-8')

    def readInput(self):
        for line in sys.stdin:
            pair = unicode(line, 'utf8').strip().split('\t', 1)
            yield [pair[0]] + pair[1].split(':', 2)


if __name__ == "__main__":
    mr_func = sys.argv[1]
    if mr_func == "map":
        lang = sys.argv[2]
        mapper = Mapper(lang)
        mapper.run()
    elif mr_func == "reduce":
        reducer = Reducer()
        reducer.run()