#!/bin/bash

#autoscan && echo autoscan success ..&&
#mv configure.scan configure.ac && echo rename success ...&&
aclocal && echo aclocal success ...&&
autoheader && echo autoheader success .... &&
autoconf && echo autoconf success ... &&
echo bin_PROGRAMS=a.out\
 a_out_SOURCES=main.cpp >> Makefile.am && echo add Makefile.am success ... && 
touch NEWS LICESE ChangeLog AUTHORS README &&
automake --add-missing && echo automake --add-missing success &&
./configure && echo configure success 



