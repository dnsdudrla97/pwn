# libptrace, a process tracing and manipulation library.
#
# Dedicated to Merle Planten.
#
# Copyright (C) 2006-2008 Ronald Huizer
#
# This library is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

AC_PREREQ(2.60)
AC_INIT([libptrace], [0.1], [scrippie@xs4all.nl])

# Variable definitions
COMPILE_HDR=./src/libptrace_config.h

# Host settings
#
# build:	system we're building on
# host:		system we're building for,
# target:	system the program we're building will build for
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

case "$host_os" in
	linux*)
		TARGET_OS=linux
		AC_CONFIG_SRCDIR([src/libptrace_linux.c])
		;;
	mingw32*)
		TARGET_OS=windows
		AC_CONFIG_SRCDIR([src/libptrace_windows.c])
		;;
	*)
		AC_MSG_ERROR([Target host not supported!])
		;;
esac

case "$host_cpu" in
	i[[3456]]86)
		TARGET_CPU=i386
		;;
	x86_64)
		TARGET_CPU=x86_64
		;;
	*)
		AC_MSG_WARN([unsupported CPU type])
		;;
esac

# Initialize the automake subsystem, and pass it two conditionals based
# on the host architecture.
AM_INIT_AUTOMAKE
AM_CONDITIONAL(TARGET_LINUX, test "$TARGET_OS" = linux)
AM_CONDITIONAL(TARGET_WINDOWS, test "$TARGET_OS" = windows)
AM_CONDITIONAL(TARGET_I386, test "$TARGET_CPU" = i386)
AM_CONDITIONAL(TARGET_X86_64, test "$TARGET_CPU" = x86_64)

AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_CHECK_TOOL([DLLTOOL], [dlltool], [false])

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([limits.h stddef.h stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

dnl If this is a gnu compiler, pass -Wall
if test "$ac_cv_c_compiler_gnu" = "yes"; then
        CFLAGS="$CFLAGS -Wall"
fi

# Checks for library functions.
AC_CHECK_FUNCS([memset strdup strerror])

# Check if we can compile MMX/SSE instructions
dnl Check for MMX assembly

AC_ARG_ENABLE(mmx,
	AC_HELP_STRING([--enable-mmx], [enable MMX support (default=auto)]),,
	enable_mmx=yes)

AC_ARG_ENABLE(sse,
	AC_HELP_STRING([--enable-sse], [enable SSE support (default=auto)]),,
	enable_sse=$enable_mmx)

if test "$enable_mmx" = yes; then
	AC_MSG_CHECKING(whether we can compile MMX code)
    
	AC_COMPILE_IFELSE([asm ("movq 0, %mm0");],
		AC_DEFINE(USE_MMX, 1,
		          [Define to 1 if MMX assembly is available.])
		AC_MSG_RESULT(yes)
		MMX_FLAGS="-mmmx -DENABLE_MMX"
	
		if test "$enable_sse" = yes; then
			AC_MSG_CHECKING(whether we can compile SSE code)
	      
			AC_COMPILE_IFELSE([asm ("movntps %xmm0, 0");],
				AC_DEFINE(USE_SSE, 1,
				          [Define to 1 if SSE assembly is available.])
				SSE_FLAGS="-msse -mfpmath=sse -DENABLE_SSE"
				AC_MSG_RESULT(yes),
				enable_sse=no
				AC_MSG_RESULT(no)
				AC_MSG_WARN(
					[The assembler does not support the SSE command set.]
				)
			)
	      
		fi,
		enable_mmx=no
		AC_MSG_RESULT(no)
		AC_MSG_WARN(
			[The assembler does not support the MMX command set.]
		)
	)
fi

AM_CONDITIONAL(USE_MMX, test "$enable_mmx" = yes)
AM_CONDITIONAL(USE_SSE, test "$enable_sse" = yes)

# Test wether ptrace() supports extended floating point register access
#
if test "$TARGET_OS" = "linux"; then
	AC_CHECK_HEADERS([sys/user.h])
	AC_CHECK_HEADERS([sys/ptrace.h])

	AC_CHECK_TYPES([struct user_regs_struct],,
		AC_MSG_ERROR(
			[struct user_regs_struct is needed to continue!]
		),
		[[#include <sys/types.h>
		  #include <sys/user.h>]]
	)

	AC_CHECK_TYPES([struct user_fpregs_struct],,,
		[#include <sys/user.h>]
	)

	AC_CHECK_TYPES(
		[struct user_fpxregs_struct],
		HAVE_USER_FPXREGS_STRUCT=yes,,
		[#include <sys/user.h>]
	)

	AC_MSG_CHECKING(whether PTRACE_GETFPXREGS is defined)

	AC_TRY_COMPILE([#include <sys/ptrace.h>],
		       [ptrace(PTRACE_GETFPXREGS, 0, 0, 0);],
		AC_DEFINE(HAVE_PTRACE_GETFPXREGS, 1,
		          [Define to 1 is PTRACE_GETFPXREGS is available.])
		HAVE_PTRACE_GETFPXREGS=yes
		AC_MSG_RESULT(yes),
		HAVE_PTRACE_GETFPXREGS=no
		AC_MSG_RESULT(no)
	)

	AC_MSG_CHECKING(whether PTRACE_SETFPXREGS is defined)

	AC_TRY_COMPILE([#include <sys/ptrace.h>],
		       [ptrace(PTRACE_SETFPXREGS, 0, 0, 0);],
		AC_DEFINE(HAVE_PTRACE_SETFPXREGS, 1,
		          [Define to 1 is PTRACE_SETFPXREGS is available.])
		HAVE_PTRACE_SETFPXREGS=yes
		AC_MSG_RESULT(yes),
		HAVE_PTRACE_SETFPXREGS=no
		AC_MSG_RESULT(no)
	)
fi

# Generate a compilation specific header file for installation on linux.
#
# This also defines the ptrace() __ptrace_request LIBPTRACE_GETFPUSTATE
# and LIBPTRACE_SETFPUSTATE according to what is appropriate.
if test "$TARGET_OS" = "linux"; then
	echo "#ifndef __LIBPTRACE_CONFIG_H" > $COMPILE_HDR
	echo "#define __LIBPTRACE_CONFIG_H" >> $COMPILE_HDR
	echo >> $COMPILE_HDR
	echo "#include <stdint.h>" >> $COMPILE_HDR
	echo "#include <sys/user.h>" >> $COMPILE_HDR

	if test "$enable_sse" = "yes" &&				\
	   test "$HAVE_PTRACE_GETFPXREGS" = "yes" &&			\
	   test "$HAVE_PTRACE_SETFPXREGS" = "yes" &&			\
	   test "$HAVE_USER_FPXREGS_STRUCT" = "yes";
	then
		echo >> $COMPILE_HDR
		echo "struct ptrace_fpu_state {" >> $COMPILE_HDR
		echo -e "[\tuint8_t __buf[sizeof(struct user_fpxregs_struct)];]" >> $COMPILE_HDR
		echo "};" >> $COMPILE_HDR

		AC_DEFINE(LIBPTRACE_GETFPUSTATE, PTRACE_GETFPXREGS,
		          [Define GETFPUSTATE for extended floating point.])
		AC_DEFINE(LIBPTRACE_SETFPUSTATE, PTRACE_SETFPXREGS,
		          [Define SETFPUSTATE for extended floating point.])
	else
		echo >> $COMPILE_HDR
		echo "struct ptrace_fpu_state {" >> $COMPILE_HDR
		echo -e "[\tuint8_t __buf[sizeof(struct user_fpregs_struct)];]" >> $COMPILE_HDR
		echo "};" >> $COMPILE_HDR

		AC_DEFINE(LIBPTRACE_GETFPUSTATE, PTRACE_GETFPREGS,
		          [Define GETFPUSTATE for extended floating point.])
		AC_DEFINE(LIBPTRACE_SETFPUSTATE, PTRACE_SETFPREGS,
		          [Define SETFPUSTATE for extended floating point.])
	fi

	echo >> $COMPILE_HDR
	echo -e "#endif\t/* __LIBPTRACE_CONFIG_H */" >> $COMPILE_HDR
fi

# Specify the files to generate
#
AC_CONFIG_FILES([Makefile src/Makefile])
AC_CONFIG_FILES([tests/linux/Makefile])
AC_CONFIG_FILES([tests/windows/Makefile])
AC_CONFIG_FILES([tests/windows/psapi/Makefile])
AC_CONFIG_FILES([tests/windows/psapiutil/Makefile])

AC_OUTPUT
