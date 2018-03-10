#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([FULL-PACKAGE-NAME], [VERSION], [BUG-REPORT-ADDRESS])
AC_CONFIG_SRCDIR([wst-ext/agora/v1.2.0/include/IAgoraRecordingEngine.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lRecordEngine':
AC_CHECK_LIB([RecordEngine], [main])
# FIXME: Replace `main' with a function in `-levent':
AC_CHECK_LIB([event], [main])
# FIXME: Replace `main' with a function in `-ljsoncpp':
AC_CHECK_LIB([jsoncpp], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])
# FIXME: Replace `main' with a function in `-lrecorder':
AC_CHECK_LIB([recorder], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h float.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h sys/vfs.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([dup2 getcwd localeconv localtime_r memset mkdir modf strchr strdup strtol strtoul strtoull])

AC_CONFIG_FILES([Makefile
                 wst-ext/agora/v1.3.0/samples/Makefile
                 wst-ext/agora/v2.1.0/samples/cpp/Makefile
                 wst/Makefile])
AC_CONFIG_SUBDIRS([wst-ext/libevent])
AC_OUTPUT
