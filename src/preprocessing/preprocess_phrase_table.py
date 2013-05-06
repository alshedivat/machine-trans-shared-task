#!/usr/bin/python
# -*- coding: utf-8 -*-

if __name__ == "__main__":
    import sys
    from contextlib import nested

    with nested(open(sys.argv[1],"r"), open(sys.argv[2],"r")) as (en, fr):
        en_vcb = dict([tuple(line.split("\t")[1::-1]) for line in en])
        fr_vcb = dict([tuple(line.split("\t")[1::-1]) for line in fr])

        print "francias phrase ||| english phrase ||| P(fr_phrase | en_phrase)"
        for line in sys.stdin:
            fr_phrase, en_phrase, probabilities = line.split(" ||| ")[0:3]
            for fr_word in fr_phrase.split():
                print fr_vcb[fr_word],
            print "|||",
            for en_word in en_phrase.split():
                print en_vcb[en_word],
            print "|||",
            print probabilities.split()[0]

