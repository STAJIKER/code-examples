#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
#
import sys
import re
from itertools import groupby
from operator import itemgetter
from math import log

class Mapper:

    def run(self):
        for line in sys.stdin:
            print line.strip()


class Reducer:

    def __init__(self, lang):
        if lang == "en":
            self.number_docs = 4268154
        else:
            self.number_docs = 1236325

    def run(self):
        data = self.readInput()
        doc_count = 1
        for word, docid, count, doc_words in data:
            if docid == "-1":
                doc_count = float(count)
            else:
                tf_idf = float(count) / float(doc_words) * log(self.number_docs / doc_count)
                print ('%s+%s+%.9f\t' % (word, docid, tf_idf)).encode('utf-8')

    def readInput(self):
        for line in sys.stdin:
            key, value = unicode(line, 'utf8').strip().split('\t', 1)
            yield key.split('+', 1) + value.split(':', 1)


if __name__ == "__main__":
    mr_func = sys.argv[1]
    if mr_func == "map":
        mapper = Mapper()
        mapper.run()
    elif mr_func == "reduce":
        lang = sys.argv[2]
        reducer = Reducer(lang)
        reducer.run()