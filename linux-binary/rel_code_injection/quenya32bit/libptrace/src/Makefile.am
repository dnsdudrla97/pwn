lib_LTLIBRARIES     = libptrace.la

libptrace_ladir = ${includedir}/libptrace

libptrace_la_SOURCES = libptrace.h libptrace_config.h libptrace_error.h	\
                       libptrace.c
libptrace_la_HEADERS = libptrace.h libptrace_config.h libptrace_error.h

if TARGET_I386
libptrace_la_SOURCES += libptrace_x86.c libptrace_x86.h
libptrace_la_HEADERS += libptrace_x86.h
endif

if TARGET_X86_64
libptrace_la_SOURCES += libptrace_x86.c libptrace_x86.h
libptrace_la_HEADERS += libptrace_x86.h
endif

if TARGET_LINUX
libptrace_la_SOURCES += libptrace_linux.c libptrace_linux.h		\
                        libptrace_wrapper.c libptrace_wrapper.h
libptrace_la_HEADERS += libptrace_linux.h libptrace_wrapper.h
if TARGET_I386
libptrace_la_SOURCES += libptrace_linux_x86.c libptrace_linux_x86.h
libptrace_la_HEADERS += libptrace_linux_x86.h
endif
if TARGET_X86_64
libptrace_la_SOURCES += libptrace_linux_x86.c libptrace_linux_x86.h
libptrace_la_HEADERS += libptrace_linux_x86.h
endif
endif

if TARGET_WINDOWS
libptrace_la_SOURCES += libptrace_windows.c libptrace_windows.h
libptrace_la_HEADERS += libptrace_windows.h
if TARGET_I386
libptrace_la_SOURCES += libptrace_windows_i386.c libptrace_windows_i386.h
libptrace_la_HEADERS += libptrace_windows_i386.h
endif
endif
