#!/bin/sh
touch config.rpath
aclocal
autoconf
autoheader
libtoolize --automake --force --copy
automake -a -c

