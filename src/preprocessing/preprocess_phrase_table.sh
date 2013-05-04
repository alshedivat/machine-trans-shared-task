#!/bin/bash
gzip -cd ../../data/shad-shared-task/training/model/phrase-table.gz | ./preprocess_phrase_table.py ../../data/corpus/en.vcb ../../data/corpus/fr.vcb | gzip -c > ../../data/corpus/phrase_table.gz
