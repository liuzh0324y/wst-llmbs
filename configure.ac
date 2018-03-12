AC_PREREQ([2.69])

dnl initialize autoconf
dnl when going to/from release set the nano(fourth number) right!
dnl releases only do wall, git and prerelease does Werror too
dnl 
AC_INIT(llmbs,2.0.1)
AG_WST_INIT

dnl initialize automake (we require GNU make)
AM_INIT_AUTOMAKE([-Wno-portability 1.14 no-dist-gzip dist-xz tar-ustar subdir-objects])

dnl can autoconf find the source
AC_CONFIG_SRCDIR(wst/main.cc)

AC_CONFIG_MACRO_DIR([m4])
