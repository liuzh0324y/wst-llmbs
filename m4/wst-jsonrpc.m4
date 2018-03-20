dnl ##############################################################
dnl jsonrpc support
AC_DEFUN([WST_JSONRPC_CHECK],
[
    JSONRPC_INCS=`pwd`/wst-ext/jsoncpp/include
    JSONRPC_LIBS=`pwd`/wst-ext/jsoncpp/lib

    AC_SUBST(JSONRPC_INCS)
    AC_SUBST(JSONRPC_LIBS)
])