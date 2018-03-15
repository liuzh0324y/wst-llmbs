AC_DEFUN([WST_CONF_CHECK],
[
    dnl choose a location to install conf files in
    if test "x$PACKAGE_TARNAME" = "x"
    then
        AC_MSG_ERROR([Internal error - PACKAGE_TARNAME not set])
    fi
    confdir="\$(WST_CONF_DIR)/conf"

    dnl enable/disable conf documentation building
    AC_ARG_ENABLE(conf,
    AC_HELP_STRING([--enable-conf],
                   [use conf to install[default=yes]]),,
                   enable_conf=yes)
                
    have_conf=yes

    if test "x$have_conf" = "xyes";
    then
        AC_MSG_NOTICE(Installing conf in $confdir)
        AC_SUBST(confdir)
    fi
])