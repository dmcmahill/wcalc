dnl $Id$
dnl Copyright 2001, Dan McMahill.  ALL RIGHTS RESERVED
dnl please refer to the file COPYING
dnl

dnl This is just like the AC_PROG_AWK that comes with autoconf
dnl except it gets the path as well.  Note that we go ahead and
dnl say that we provide AC_PROG_AWK since we did one better.
AC_DEFUN(AC_PATH_AWK,
  [AC_PATH_PROGS(AWK, mawk gawk nawk awk, )
  AC_PROVIDE([AC_PROG_AWK])dnl
])

dnl Now for various awk checks.
dnl

dnl AC_TRY_AWK(PROGRAM, [ACTION-IF-TRUE [, ACTION-IF-FALSE]])
AC_DEFUN(AC_TRY_AWK,
[AC_REQUIRE([AC_PROG_AWK])dnl
cat > conftest.awk <<EOF
[#]line __oline__ "configure"
[$1]
EOF
cat > conftest.txt <<EOF
foo bar
EOF
if ($AWK -f conftest.awk conftest.txt >/dev/null; exit) 2>&AC_FD_CC
then
dnl 
  AC_MSG_RESULT(yes)
  ifelse([$2], , :, [$2]) 
else
  AC_MSG_RESULT(no)
  echo "configure:__oline__: $AWK -f conftest.awk conftest.txt" >&AC_FD_CC
  echo "configure:__oline__: failed program was:" >&AC_FD_CC
  cat conftest.awk >&AC_FD_CC
  echo "configure:__oline__: failed input file was:" >&AC_FD_CC
  cat conftest.txt >&AC_FD_CC
ifelse([$3], , , [  rm -fr conftest*
  $3
])dnl
fi  
rm -fr conftest*])


dnl see if AWK has the 'gensub' function
dnl
AC_DEFUN(AC_AWK_GENSUB,
[AC_MSG_CHECKING([whether awk ($AWK) has gensub])
AC_TRY_AWK({gensub(/foo/,"bar","g");} ,[$2],[$3])
])dnl

dnl see if AWK has the 'strftime' function
dnl
AC_DEFUN(AC_AWK_STRFTIME,
[AC_MSG_CHECKING([whether awk ($AWK) has strftime])
AC_TRY_AWK({print strftime()} ,[$2],[$3])
])dnl

