AC_PREREQ([2.69])

dnl initialize autoconf
dnl when going to/from release set the nano(fourth number) right!
dnl releases only do wall, git and prerelease does Werror too
dnl 
AC_INIT(llmbs,2.0.1,liuzhihuic@qq.com)

dnl initialize wst-common
AG_WST_INIT

dnl check wst-common
AG_WST_CHECK_CHECKS

dnl check libevent
LIBEVENT_SUPPORT

dnl check jsonrpc
JSONRPC_SUPPORT

dnl check agora
AGORA_SUPPORT

dnl initialize automake (we require GNU make)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
# AM_INIT_AUTOMAKE(llmbs, 2.0.1)

dnl define PACKAGE_VERSION_* variables
# AS_VERSION
AC_PROG_CXX

dnl can autoconf find the source
AC_CONFIG_SRCDIR([wst/main.cc])

AC_CONFIG_MACRO_DIR([m4])

dnl Initialize prefix.
if test "$prefix" = "NONE"; then
   prefix="/opt/llmbs"
fi

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
)

AC_OUTPUT