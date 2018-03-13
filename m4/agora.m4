dnl ######################################################
dnl agora support
AC_DEFUN([AGORA_SUPPORT],
[
    AGORA_INCS=`pwd`/wst-ext/agora/v2.1.0/include
    AGORA_LIBS=`pwd`/wst-ext/agora/v2.1.0/libs

    AC_SUBST(AGORA_INCS)
    AC_SUBST(AGORA_LIBS)
])