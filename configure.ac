AC_PREREQ([2.69])

dnl initialize autoconf
dnl when going to/from release set the nano(fourth number) right!
dnl releases only do wall, git and prerelease does Werror too
dnl 
AC_INIT([llmbs],[2.0.1],[liuzh@llvision.com])

dnl initialize wst-common
AG_WST_INIT

dnl initialize automake (we require GNU make)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

dnl Initialize prefix.
if test "x$prefix" = "xNONE"; then
   prefix="/opt/llmbs"
#    echo "prefix=$prefix"
fi

dnl check libevent
WST_LIBEVENT_CHECK

dnl check jsonrpc
WST_JSONRPC_CHECK

dnl check agora
WST_AGORA_CHECK

dnl check common
WST_CHECKS

dnl define PACKAGE_VERSION_* variables
# AS_VERSION
AC_PROG_CXX

dnl can autoconf find the source
AC_CONFIG_SRCDIR([wst/main.cc])

AC_CONFIG_HEADERS([config.h])


dnl llmbs versioning, this is mostly informational
WST_VERSION_MAJOR=$PACKAGE_VERSION_MAJOR
WST_VERSION_MINOR=$PACKAGE_VERSION_MINOR
WST_VERSION_MICRO=$PACKAGE_VERSION_MICRO
WST_VERSION_NANO=$PACKAGE_VERION_NANO
AC_SUBST(WST_VERSION_MAJOR)
AC_SUBST(WST_VERSION_MINOR)
AC_SUBST(WST_VERSION_MICRO)
AC_SUBST(WST_VERSION_NANO)

dnl Add parameters for aclocal (keep in sync with Makefile.am)
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4 -I wst-common/m4")

AC_CONFIG_FILES(
    Makefile
    wst/Makefile
    conf/Makefile
    tools/Makefile
)

AC_OUTPUT

echo  "

Configuration
    Version                     : ${VERSION}
    Source code location        : ${srcdir}
    Prefix                      : ${prefix}
    Compiler                    : ${CXX}

    Bin path                    : ${WST_BIN_DIR}
    Conf path                   : ${WST_CONF_DIR}
    Logs path                   : ${WST_LOGS_DIR}
    Tools path                  : ${WST_TOOLS_DIR}
"