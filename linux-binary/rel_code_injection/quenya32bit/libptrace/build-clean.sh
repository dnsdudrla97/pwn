#!/bin/bash

rm -rf autom4te.cache/
rm -f aclocal.m4 autoscan.log config.guess config.sub configure
rm -f depcomp install-sh ltmain.sh Makefile.in missing stamp-h1
rm -f config.h config.h.in config.h.in~ mingw32-config.cache
rm -f src/Makefile.in tests/linux/Makefile.in
rm -f tests/windows/psapiutil/Makefile.in tests/windows/psapi/Makefile.in
rm -f tests/windows/Makefile.in config.log config.status Makefile
