#!/usr/bin/python
# -*- coding: utf-8 -*-

en = open('../../data/corpus/en.vcb', "r")
fr = open('../../data/corpus/fr.vcb', "r")

full = open('../../data/corpus/full.vcb', "w")

words = set()

for line in en:
  words.add(line.strip().split()[1])

for line in fr:
  words.add(line.strip().split()[1])

index = 0
for word in words:
  index += 1
  print str(index) + "\t" + word
