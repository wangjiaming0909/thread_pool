#!/bin/bash

# makeup the configure.ac file 
echo AC_PREREQ\([2.69]\) >> configure.ac
echo AC_INIT\(a.out, 1.0\) >> configure.ac
echo AM_INIT_AUTOMAKE\(subdir-objects\) >> configure.ac
echo AC_CONFIG_SRCDIR\([main.cpp]\) >> configure.ac
echo AC_CONFIG_HEADERS\([config.h]\) >> configure.ac
echo CXXFLAGS="-pthread -O0 -g -W  -Werror -Wextra -std=c++11" >> configure.ac
echo AC_PROG_CXX >> configure.ac
echo AC_PROG_CC >> configure.ac
echo AC_PROG_CPP >> configure.ac
echo AC_OUTPUT\(Makefile\) >> configure.ac &&


aclocal && echo aclocal success ...&&
autoheader && echo autoheader success .... &&
autoconf && echo autoconf success ... &&
echo bin_PROGRAMS=a.out >> Makefile.am
echo a_out_SOURCES=main.cpp >> Makefile.am && 
echo add Makefile.am success ... && 
touch NEWS LICESE ChangeLog AUTHORS README &&
automake --add-missing && echo automake --add-missing success...
#./configure && echo configure success 



