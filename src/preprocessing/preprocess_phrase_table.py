#!/usr/bin/python
# -*- coding: utf-8 -*-

if __name__ == "__main__":
    import sys
    from contextlib import nested

    with open(sys.argv[1],"r") as full_vcb:
        vcb = dict([tuple(line.strip().split("\t")[1::-1]) for line in full_vcb])

        print "francias phrase ||| english phrase ||| P(fr_phrase | en_phrase)"
        for line in sys.stdin:
            fr_phrase, en_phrase, probabilities = line.split(" ||| ")[0:3]
            for fr_word in fr_phrase.split():
                print vcb[fr_word],
            print "|||",
            for en_word in en_phrase.split():
                print vcb[en_word],
            print "|||",
            print probabilities.split()[0]
