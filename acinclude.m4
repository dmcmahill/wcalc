dnl $Id: acinclude.m4,v 1.4 2002/06/12 11:29:58 dan Exp $
dnl
dnl Copyright (c) 1999, 2000, 2001, 2002 Dan McMahill
dnl All rights reserved.
dnl
dnl This code is derived from software written by Dan McMahill
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl 1. Redistributions of source code must retain the above copyright
dnl    notice, this list of conditions and the following disclaimer.
dnl 2. Redistributions in binary form must reproduce the above copyright
dnl    notice, this list of conditions and the following disclaimer in the
dnl    documentation and/or other materials provided with the distribution.
dnl 3. All advertising materials mentioning features or use of this software
dnl    must display the following acknowledgement:
dnl        This product includes software developed by Dan McMahill
dnl 4. The name of the author may not be used to endorse or promote products
dnl    derived from this software without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
dnl IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
dnl OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
dnl IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
dnl INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
dnl BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
dnl LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
dnl AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
dnl OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
dnl OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
dnl SUCH DAMAGE.
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


# see if AWK has the 'gensub' function
# AC_AWK_GENSUB(ACTION-IF-TRUE [, ACTION-IF-FALSE])
#
AC_DEFUN(AC_AWK_GENSUB,
[AC_MSG_CHECKING([whether awk ($AWK) has gensub])
AC_TRY_AWK([{gensub(/foo/,"bar","g");}] ,[$1] ,[$2])
])dnl

# see if AWK has the 'strftime' function
# AC_AWK_STRFTIME(ACTION-IF-TRUE [, ACTION-IF-FALSE])
#
AC_DEFUN(AC_AWK_STRFTIME,
[AC_MSG_CHECKING([whether awk ($AWK) has strftime])
AC_TRY_AWK([{print strftime()}] ,[$1] ,[$2])
])dnl


dnl PKG_CHECK_MODULES(GSTUFF, gtk+-2.0 >= 1.3 glib = 1.3.4, action-if, action-not)
dnl defines GSTUFF_LIBS, GSTUFF_CFLAGS, see pkg-config man page
dnl also defines GSTUFF_PKG_ERRORS on error
AC_DEFUN(PKG_CHECK_MODULES, [
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


