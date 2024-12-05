#!/bin/bash

flex++ ./html-to-latex.l
g++ -o html-to-latex lex.yy.cc
./html-to-latex ./fichero.txt