dnl ######################################################
dnl agora support
AC_DEFUN([WST_AGORA_CHECK],
[
    AGORA_INCS=`pwd`/wst-ext/agora/v2.1.0/include
    AGORA_LIBS=`pwd`/wst-ext/agora/v2.1.0/libs

    AC_SUBST(AGORA_INCS)
    AC_SUBST(AGORA_LIBS)
])