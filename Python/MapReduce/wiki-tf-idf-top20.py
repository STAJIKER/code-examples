#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
#
import sys
import re
from itertools import groupby
from operator import itemgetter

class Mapper:

    def run(self):
        for line in sys.stdin:
            print line.strip()


class Reducer:

    popular_en = [u"the", u"of", u"and", u"in", u"a", u"to", u"was", u"is", u"s", u"for", u"on", u"as", u"by", u"with", u"he", u"at", u"that", u"from", u"his", u"it"]
    popular_ru = [u"в", u"и", u"на", u"с", u"категория", u"по", u"года", u"из", u"году", u"а", u"к", u"не", u"от", u"для", u"был", u"что", u"за", u"его", u"как", u"до"]

    def __init__(self, top_size, lang):
        self.top_size = top_size
        self.lang = lang
        if lang == "en":
            self.popular = Reducer.popular_en
        else:
            self.popular = Reducer.popular_ru

    def run(self):
        data = self.readInput()
        answer = '1'
        cur_word = '1'
        emit_count = 0
        for word, docid, tf_idf in data:
            if word not in self.popular:
                if cur_word != word:
                    if answer != '1':
                        print answer.encode('utf-8')
                    answer = word
                    cur_word = word
                    emit_count = 0
                if emit_count < self.top_size:
                    answer += '\t' + docid + ':' + tf_idf
                    emit_count += 1
        print answer.encode('utf-8')

    def readInput(self):
        for line in sys.stdin:
            yield unicode(line, 'utf-8').strip().split('\t')[0].split('+', 2)


if __name__ == "__main__":
    mr_func = sys.argv[1]
    if mr_func == "map":
        mapper = Mapper()
        mapper.run()
    elif mr_func == "reduce":
        top_size = int(sys.argv[2])
        lang = sys.argv[3]
        reducer = Reducer(top_size, lang)
        reducer.run()