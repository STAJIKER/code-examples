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

    def __init__(self, lang):
        if lang == "en":
            self.re = Mapper.re_en
        else:
            self.re = Mapper.re_ru
        self.results = {}

    def run(self):
        doc_count = 0
        data = self.readInput()
        for docid, content in data:
            text = content.lower()
            word_count = 0
            for match in self.re.finditer(text):
                word = match.group(0)
                self.addResult(word)
                word_count += 1
            sys.stderr.write("reporter:counter:MyCounters,InputWords,%d\n" % word_count)
            doc_count += 1
            if doc_count % 1000 == 0:
                self.emitResults()
                sys.stderr.write("reporter:status:Processed %d documents\n" % doc_count)        
        self.emitResults()

    def readInput(self):
        for line in sys.stdin:
            yield unicode(line, 'utf8').strip().split('\t', 1)

    def addResult(self, word):
        if word in self.results:
            self.results[word] += 1
        else:
            self.results[word] = 1

    def emitResults(self):
        for word, count in self.results.iteritems():
            print ('%s\t%d' % (word, count)).encode('utf-8')
        self.results = {}


class Reducer:

    def run(self):
        data = self.readInput()
        for word, group in groupby(data, itemgetter(0)):
            total_count = sum(int(count) for word, count in group)
            print ('%s\t%d' % (word, total_count)).encode('utf-8')

    def readInput(self):
        for line in sys.stdin:
            yield unicode(line, 'utf8').strip().split('\t', 1)


if __name__ == "__main__":
    mr_func = sys.argv[1]
    if mr_func == "map":
        lang = sys.argv[2]
        mapper = Mapper(lang)
        mapper.run()
    elif mr_func == "reduce":
        reducer = Reducer()
        reducer.run()