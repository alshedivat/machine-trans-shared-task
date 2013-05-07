#!/usr/bin/python
# -*- coding: utf-8 -*-

if __name__ == "__main__":
    import sys

    print '<?xml version="1.0" encoding="UTF-8"?>', \
          '\n<!DOCTYPE mteval SYSTEM "ftp://jaguar.ncsl.nist.gov/mt/resources/mteval-xml-v1.3.dtd">', \
          '\n<mteval>', \
          '\n<tstset setid="europarl_set" srclang="French" trglang="English" sysid="europarl_system">', \
          '\n<doc docid="test" genre="nw">'

    for i, line in enumerate(sys.stdin):
        print '<p><seg id="' + str(i+1) +'">', line, "</seg></p>"

    print '</doc>', '\n</tstset>', '\n</mteval>'
