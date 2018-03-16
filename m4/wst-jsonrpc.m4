dnl ##############################################################
dnl jsonrpc support
AC_DEFUN([WST_JSONRPC_CHECK],
[
    JSONRPC_INCS=`pwd`/ext/jsonrpc/include
    JSONRPC_LIBS=`pwd`/ext/jsonrpc/lib

    AC_SUBST(JSONRPC_INCS)
    AC_SUBST(JSONRPC_LIBS)
])