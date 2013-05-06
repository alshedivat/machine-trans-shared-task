#!/bin/bash
pv ../../data/corpus/shared_task_test.lowercased.fr | perl -p -i -e 's/[&]/&amp;/g' | perl -p -i -e 's/"/&quot;/g' | perl -p -i -e 's/\[/&#91;/g' | perl -p -i -e 's/\]/&#93;/g' | perl -p -i -e "s/'/&apos;/g" > ../../data/corpus/input.txt
