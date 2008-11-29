dnl $Id: acinclude.m4,v 1.7 2004/11/04 03:51:34 dan Exp $
dnl
dnl Copyright (C) 1999, 2000, 2001, 2002, 2004 Dan McMahill
dnl All rights reserved.
dnl
dnl 
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; version 2 of the License.
dnl 
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl 
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
dnl 

dnl This is just like the AC_PROG_AWK that comes with autoconf
dnl except it gets the path as well.  Note that we go ahead and
dnl say that we provide AC_PROG_AWK since we did one better.
AC_DEFUN([AC_PATH_AWK],
  [AC_PATH_PROGS(AWK, mawk gawk nawk awk, )
  AC_PROVIDE([AC_PROG_AWK])dnl
])

dnl Now for various awk checks.
dnl

dnl AC_TRY_AWK(PROGRAM, [ACTION-IF-TRUE [, ACTION-IF-FALSE]])
AC_DEFUN([AC_TRY_AWK],
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


# see if AWK has the 'gensub' function
# AC_AWK_GENSUB(ACTION-IF-TRUE [, ACTION-IF-FALSE])
#
AC_DEFUN([AC_AWK_GENSUB],
[AC_MSG_CHECKING([whether awk ($AWK) has gensub])
AC_TRY_AWK([{gensub(/foo/,"bar","g");}] ,[$1] ,[$2])
])dnl

# see if AWK has the 'strftime' function
# AC_AWK_STRFTIME(ACTION-IF-TRUE [, ACTION-IF-FALSE])
#
AC_DEFUN([AC_AWK_STRFTIME],
[AC_MSG_CHECKING([whether awk ($AWK) has strftime])
AC_TRY_AWK([{print strftime()}] ,[$1] ,[$2])
])dnl


#------------------------------------------------
#
# This is from pkg-config
#

dnl PKG_CHECK_MODULES(GSTUFF, gtk+-2.0 >= 1.3 glib = 1.3.4, action-if, action-not)
dnl defines GSTUFF_LIBS, GSTUFF_CFLAGS, see pkg-config man page
dnl also defines GSTUFF_PKG_ERRORS on error
AC_DEFUN([PKG_CHECK_MODULES], [
  succeeded=no

  if test -z "$PKG_CONFIG"; then
    AC_PATH_PROG(PKG_CONFIG, pkg-config, no)
  fi

  if test "$PKG_CONFIG" = "no" ; then
     echo "*** The pkg-config script could not be found. Make sure it is"
     echo "*** in your path, or set the PKG_CONFIG environment variable"
     echo "*** to the full path to pkg-config."
     echo "*** Or see http://www.freedesktop.org/software/pkgconfig to get pkg-config."
  else
     PKG_CONFIG_MIN_VERSION=0.9.0
     if $PKG_CONFIG --atleast-pkgconfig-version $PKG_CONFIG_MIN_VERSION; then
        AC_MSG_CHECKING(for $2)

        if $PKG_CONFIG --exists "$2" ; then
            AC_MSG_RESULT(yes)
            succeeded=yes

            AC_MSG_CHECKING($1_CFLAGS)
            $1_CFLAGS=`$PKG_CONFIG --cflags "$2"`
            AC_MSG_RESULT($$1_CFLAGS)

            AC_MSG_CHECKING($1_LIBS)
            $1_LIBS=`$PKG_CONFIG --libs "$2"`
            AC_MSG_RESULT($$1_LIBS)
        else
            $1_CFLAGS=""
            $1_LIBS=""
            ## If we have a custom action on failure, don't print errors, but 
            ## do set a variable so people can do so.
            $1_PKG_ERRORS=`$PKG_CONFIG --errors-to-stdout --print-errors "$2"`
            ifelse([$4], ,echo $$1_PKG_ERRORS,)
        fi

        AC_SUBST($1_CFLAGS)
        AC_SUBST($1_LIBS)
     else
        echo "*** Your version of pkg-config is too old. You need version $PKG_CONFIG_MIN_VERSION or newer."
        echo "*** See http://www.freedesktop.org/software/pkgconfig"
     fi
  fi

  if test $succeeded = yes; then
     ifelse([$3], , :, [$3])
  else
     ifelse([$4], , AC_MSG_ERROR([Library requirements ($2) not met; consider adjusting the PKG_CONFIG_PATH environment variable if your libraries are in a nonstandard prefix so pkg-config can find them.]), [$4])
  fi
])


