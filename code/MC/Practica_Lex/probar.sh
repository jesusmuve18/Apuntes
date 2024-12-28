#!/bin/bash

script_file=html-to-latex
text_file=fichero.html
output_file=salida.tex

flex++ -o ./$script_file.yy.cc ./$script_file.l
g++ -o ./$script_file ./$script_file.yy.cc
./$script_file ./$text_file > ./$output_file
# pdflatex ./$output_file