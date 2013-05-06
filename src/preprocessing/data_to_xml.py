#!/usr/bin/python
# -*- coding: utf-8 -*-

if __name__ == "__main__":
    import sys

    for i, line in enumerate(sys.stdin):
        print '<p><seg id="' + str(i+1) +'">', line, "</seg></p>"

