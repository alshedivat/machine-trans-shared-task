#!/bin/bash
./mteval.pl -s ../../data/corpus/input.xml -r ../../data/corpus/input_google_reference.xml -t ../../data/corpus/output.xml -d
./mteval.pl -s ../../data/corpus/input.xml -r ../../data/corpus/input_yandex_reference.xml -t ../../data/corpus/output.xml -d
./mteval.pl -s ../../data/corpus/input.xml -r ../../data/corpus/input_bing_reference.xml -t ../../data/corpus/output.xml -d
