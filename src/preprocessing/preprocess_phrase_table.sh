#!/bin/bash
gzip -cd ../../data/corpus/phrase-table.gz |\
./preprocess_phrase_table.py ../../data/corpus/full.vcb > ../../data/corpus/phrase_table
