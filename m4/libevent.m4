dnl ##########################################################
dnl libevent support
AC_DEFUN([LIBEVENT_SUPPORT],
[
    LIBEVENT_INCS=`pwd`/ext/libevent/include
    LIBEVENT_LIBS=`pwd`/ext/libevent/lib

    AC_SUBST(LIBEVENT_INCS)
    AC_SUBST(LIBEVENT_LIBS)
])