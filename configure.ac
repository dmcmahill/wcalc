dnl $Id: configure.ac,v 1.5 2008/11/29 20:41:29 dan Exp $
dnl Process this file with autoconf to produce a configure script.
dnl
dnl
dnl Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
dnl 2007, 2008 Dan McMahill
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

AC_INIT(gtk-wcalc/wcalc.c)
AM_INIT_AUTOMAKE(wcalc, 1.0A)
AM_CONFIG_HEADER(config.h)
AM_MAINTAINER_MODE

AC_DEFINE(DEVEL_VERSION, 1, [Define if this is a development snapshot and we want to warn users about that])

dnl determine host type
AC_CANONICAL_HOST
AC_MSG_CHECKING(for windows)
case $host in
	*-*-cygwin* )
		WIN32=yes
		CFLAGS="$CFLAGS ${CYGWIN_CFLAGS}"
		CPPFLAGS="$CPPFLAGS ${CYGWIN_CPPFLAGS}"
		;;

	*-*-mingw* )
		WIN32=yes
		CFLAGS="$CFLAGS ${MINGW_CFLAGS:--mms-bitfields -mwindows}"
		CPPFLAGS="$CPPFLAGS ${MINGW_CPPFLAGS:--mms-bitfields -mwindows}"
		;;

	* )
		WIN32=no
esac
AC_MSG_RESULT($WIN32)
AC_SUBST(WIN32)
AM_CONDITIONAL(WIN32, test x$WIN32 = xyes)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AM_PROG_LIBTOOL

if test "x$WIN32" = "xyes" ; then
	AC_CHECK_TOOL([WINDRES], [windres], [no])
	if test "$WINDRES" = "no" ; then
		AC_MSG_ERROR([Could not locate windres.  This is a required tool for the windows build.])
	fi
fi

#
# used for processing the drawings to give png's
# and xpm's
#
AC_PATH_PROG(TGIF, tgif, notfound)
AM_CONDITIONAL(MISSING_TGIF, test x$TGIF = xnotfound)

#
# For building the documentation
#
AC_PATH_PROG(GROFF, groff, notfound)
AC_PATH_PROG(NEQN, neqn, notfound)
AC_PATH_PROG(TBL, tbl, notfound)
AM_CONDITIONAL(MISSING_NROFF, test x$GROFF = xnotfound -o x$NEQN = xnotfound -o x$TBl = xnotfound)

# w3m, lynx, and links all seem to support -dump to create an ascii file from
# html
AC_PATH_PROG(W3M, w3m links lynx, notfound)
AC_PATH_PROG(XSLTPROC, xsltproc, notfound)
AM_CONDITIONAL(MISSING_XSLT, test x$W3M = xnotfound -o x$XSLTPROC = xnotfound)

#
# Used by the web page and building the windows icons
#
AC_PATH_PROG(XPMTOPPM, xpmtoppm, notfound)
AC_PATH_PROG(PPMTOWINICON, ppmtowinicon, notfound)
AM_CONDITIONAL(MISSING_XPMTOPPM, test x$PPMTOWINICON = xnotfound -o x$XPMTOPPM = xnotfound)

# Used to build the windows icons
AC_PATH_PROG(CONVERT, convert, notfound)
AM_CONDITIONAL(MISSING_CONVERT, test x$CONVERT = xnotfound)

#
# Check for how to set the run time library path.  Allow users to disable
# since some systems have a no-rpath policy.
#
_use_rpath=yes
AC_MSG_CHECKING([if hardcoding of the runtime library path is desired (rpath)])
AC_ARG_ENABLE([rpath],
[  --disable-rpath         Disable hardcoding the runtime library path [default=enabled]],
[
if test "X$enable_rpath" = "Xno" ; then
	AC_MSG_RESULT([no])
	_use_rpath=no
else
	AC_MSG_RESULT([yes])
	_use_rpath=yes
fi
],
[
	AC_MSG_RESULT([yes])
	_use_rpath=yes
])

if test "X${_use_rpath}" = "Xyes" ; then
AC_LANG_PUSH(C)
wc_save_LIBS="$LIBS"
for fl in "-Wl,-rpath -Wl," "-Wl,--rpath -Wl," "-rpath " "--rpath " "-Wl,-R" "-Wl,-R -Wl," "-R" "-R " ; do
	LIBS="$wc_save_LIBS ${fl}path"
	AC_MSG_CHECKING([if $CC accepts ${fl}path for runtime libraries])
	AC_LINK_IFELSE([
#include <stdio.h>
int main(argc, argv)
int argc;
char **argv;
{
	printf("Hello, world\n");
	return 0;
}
],

	AC_MSG_RESULT([yes])
	rpath=$fl, 
	AC_MSG_RESULT([no]))

	test -n "$rpath" && break
done
RPATH_FLAG="$rpath"
LIBS="$wc_save_LIBS"
# restore the language settings
AC_LANG_POP(C)
else
	RPATH_FLAG=""
fi
AC_SUBST(RPATH_FLAG)

#
# ------------- libwcalc tests -------------------
#
# here we look to see if there is an installed version
# of libwcalc that we can use
PIXMAPS=pixmaps
LIB_WCALC=
INTLPO=
WCALC_CFLAGS=-I\${top_srcdir}/libwcalc
WCALC_LIBS=
wc_libwcalc_yesno=yes

AC_MSG_CHECKING([if libwcalc should be built])
AC_ARG_ENABLE([libwcalc],
[  --enable-libwcalc       Causes libwcalc to be built.  If
                           --disable-libwcalc is given, the
                           configure process will search
                           for a preinstalled libwcalc [default=yes]],
[
if test "X$enable_libwcalc" = "Xno" ; then
	LIB_WCALC=
	INTLPO=
	AC_MSG_RESULT([no])
	wc_libwcalc_yesno=no
else
	LIB_WCALC=libwcalc
	INTLPO=" intl po "
	AC_MSG_RESULT([yes])
	wc_libwcalc_yesno=yes
fi
],
[
LIB_WCALC=libwcalc
INTLPO=" intl po "
AC_MSG_RESULT([yes])
wc_libwcalc_yesno=yes
])


if test "X$wc_libwcalc_yesno" = "Xno" ; then
PKG_CHECK_MODULES(WCALC, wcalc = ${VERSION}, 
	AC_MSG_CHECKING([for libwcalc headers])
	WCALC_CFLAGS=`$PKG_CONFIG wcalc --cflags`
	AC_MSG_RESULT([$WCALC_CFLAGS])
	
	AC_MSG_CHECKING([for libwcalc library])
	WCALC_LIBS=`$PKG_CONFIG wcalc --libs`
	AC_MSG_RESULT([$WCALC_LIBS])

	PIXMAPS=
,
	AC_ERROR([Unable to locate an installed libwcalc-${VERSION}])
)

fi

# set to true if we're building our own copy of libwcalc
AM_CONDITIONAL(WC_LOCAL_LIBWCALC, test x$wc_libwcalc_yesno = xyes)

AC_SUBST(LIB_WCALC)
AC_SUBST(INTLPO)
AC_SUBST(PIXMAPS)
AC_SUBST(WCALC_CFLAGS)
AC_SUBST(WCALC_LIBS)

#
# ------------- scilab tests -------------------
#
SCI_WCALC=
wc_sci_wcalc_yesno=no

AC_ARG_WITH([scilab],
[  --with-scilab=[scidir]    Build the scilab frontend.  If scidir is
                          given then scidir should be the base of
                          your scilab installation.  For example,
                          /opt/scilab-2.6.  If scidir is not given
			  the scilab installation will be searched for.
			  [default = do not build sci-wcalc]],
[
if test "X$with_scilab" = "Xno" ; then
	SCI_WCALC=""
elif test "X$with_scilab" = "Xyes" ; then
	AC_PATH_PROG(SCILAB, scilab, no)
	if test "X$SCILAB" = "Xno" ; then
		AC_ERROR([No scilab installation could be found])
	fi

	AC_MSG_CHECKING([for the scilab base directory])
	[SCI=`$AWK  '/^[ \t]*SCI[ \t]*=/ {print;}' $SCILAB | sed -e 's;.*SCI.*=;;g' -e 's;\";;g'`]
	AC_MSG_RESULT([$SCI])

	if test "X$SCI" = "X" ; then
		AC_ERROR([Scilab base directory could not be found])
	fi

	SCI_WCALC=sci-wcalc
	wc_sci_wcalc_yesno=yes
else
	SCI=$with_scilab
	AC_PATH_PROG(SCILAB, scilab, no, [$SCI/bin:$PATH])
	if test "X$SCILAB" = "Xno" ; then
		AC_ERROR([No scilab installation could be found])
	fi

	SCI_WCALC=sci-wcalc
	wc_sci_wcalc_yesno=yes
fi
])


if test "X$SCI_WCALC" != "X"; then
	AC_MSG_CHECKING([for the scilab stack header])
	SCISTACK="$SCI/routines/stack-c.h"
	if test -f "$SCISTACK" ; then
		AC_MSG_RESULT([$SCISTACK])
	else
		AC_MSG_RESULT([no])
		AC_ERROR([The directory $SCI does not appear to be the root
of a complete (and working) scilab installation.])
	fi
fi

if test "X$SCI_WCALC" != "X"; then
	AC_MSG_CHECKING([for the scilab version])
	if test "X$SCI_VER" = "X" ; then
		if test -f $SCI/Version.incl ; then
			[SCI_VER=`$AWK  '/^[ \t]*SCIVERSION[ \t]*=/ {print;}' $SCI/Version.incl | \
				sed -e 's;.*SCIVERSION.*=;;g' -e 's;\";;g' -e 's;[Ss]cilab-;;g'`]
		else
			[SCI_VER=`$AWK  '/^[ \t]*VERSION[ \t]*=/ {print;}' $SCILAB | \
				sed -e 's;.*VERSION.*=;;g' -e 's;\";;g' -e 's;[Ss]cilab-;;g'`]
		fi
	fi
	AC_MSG_RESULT([$SCI_VER])
	if test "X$SCI_VER" = "X" ; then
		AC_ERROR([Could not extract the scilab version])
	fi
else
	SCI_VER=""
fi

AC_SUBST(SCI)
AC_SUBST(SCI_VER)
AC_SUBST(SCI_WCALC)

# ------------- done with scilab tests -------------------

#
# ------------- octave tests -------------------
#

OCT_WCALC=
wc_oct_wcalc_yesno=no

AC_ARG_WITH([octave],
[  --with-octave=[octdir]    Build the octave frontend.  If octdir is
                          given then octdir should be the base of
                          your octave installation.  For example,
                          /opt/octave-2.6.  If octdir is not given
			  the octave installation will be searched for.
			  [default = do not build oct-wcalc]],
[
if test "X$with_octave" = "Xno" ; then
	OCT_WCALC=""
elif test "X$with_octave" = "Xyes" ; then
	AC_PATH_PROG([OCTAVE], [octave], no)
	if test "X$OCTAVE" = "Xno" ; then
		AC_ERROR([No octave installation could be found])
	fi

	AC_PATH_PROG([OCTAVE_CONFIG], [octave-config], no)
	if test "X$OCTAVE_CONFIG" = "Xno" ; then
		AC_ERROR([No octave installation could be found])
	fi

	AC_PATH_PROG([MKOCTFILE], [mkoctfile], no)
	if test "X$MKOCTFILE" = "Xno" ; then
		AC_ERROR([No octave installation could be found])
	fi

	AC_PATH_PROG([OCTMEX], [mex], no)
	if test "X$OCTMEX" = "Xno" ; then
		AC_ERROR([No octave mex could be found])
	fi

	OCT_WCALC=octave-wcalc
	wc_oct_wcalc_yesno=yes
else
	OCT=$with_octave
	AC_PATH_PROG([OCTAVE], [octave], no, [$OCT/bin:$PATH])
	if test "X$OCTAVE" = "Xno" ; then
		AC_ERROR([No octave installation could be found])
	fi

	AC_PATH_PROG([OCTAVE_CONFIG], [octave-config], no, [$OCT/bin:$PATH])
	if test "X$OCTAVE_CONFIG" = "Xno" ; then
		AC_ERROR([No octave installation could be found])
	fi

	AC_PATH_PROG([MKOCTFILE], [mkoctfile], no, [$OCT/bin:$PATH])
	if test "X$MKOCTFILE" = "Xno" ; then
		AC_ERROR([No octave installation could be found])
	fi

	AC_PATH_PROG([OCTMEX], [mex], no, [$OCT/bin:$PATH])
	if test "X$OCTMEX" = "Xno" ; then
		AC_ERROR([No octave mex could be found])
	fi

	OCT_WCALC=octave-wcalc
	wc_oct_wcalc_yesno=yes
fi
])


if test "X$OCT_WCALC" != "X"; then
	AC_MSG_CHECKING([if $OCTMEX is octave mex])
	AC_LANG(C)
	ac_ext=c
	ac_cpp='$OCTMEX $CPPFLAGS'
	ac_compile='$OCTMEX -c conftest.$ac_ext >&AS_MESSAGE_LOG_FD'
	ac_link='$OCTMEX conftest.$ac_ext $LIBS >&AS_MESSAGE_LOG_FD'
	ac_compiler_gnu=$ac_cv_c_compiler_gnu
	save_ac_exeext=$ac_exeext
	ac_exeext=.oct

	AC_LINK_IFELSE([
#include "mex.h"
void mexFunction(
                 int nlhs,
                 mxArray *plhs[[]],
                 int nrhs,
#if !defined(V4_COMPAT) && !defined(SCI_MEX)
                 const mxArray *prhs[[]]
#else
                 mxArray *prhs[[]]
#endif
                 )
{
	mexPrintf("Hello, world\n");
}
],
	AC_MSG_RESULT([yes])
,
	AC_ERROR([$OCTMEX is either broken or is not octave mex])
)

if test "X${_use_rpath}" = "Xyes" ; then
	wc_save_LIBS="$LIBS"
	rpath=""
	for fl in "-R" "-Wl,-rpath -Wl," "-Wl,--rpath -Wl," "-rpath " "--rpath " "-Wl,-R" "-Wl,-R -Wl," "-R" "-R " ; do
	    LIBS="$wc_save_LIBS ${fl}path"
	    AC_MSG_CHECKING([if $OCTMEX accepts ${fl}path for runtime libraries])
	    AC_LINK_IFELSE([
#include "mex.h"
void mexFunction(
                 int nlhs,
                 mxArray *plhs[[]],
                 int nrhs,
#if !defined(V4_COMPAT) && !defined(SCI_MEX)
                 const mxArray *prhs[[]]
#else
                 mxArray *prhs[[]]
#endif
                 )
{
	mexPrintf("Hello, world\n");
}
],

		AC_MSG_RESULT([yes])
		rpath=$fl, 
		AC_MSG_RESULT([no]))
		test -n "$rpath" && break
	done
	if test -n "$rpath"; then
	   OCTMEX_RPATH="$rpath"
	else
	   OCTMEX_RPATH=""
	fi
	LIBS="$wc_save_LIBS"
else
	OCTMEX_RPATH=""
fi

	# restore the language settings
	ac_exeext=$save_ac_exeext
	AC_LANG(C)

fi

if test "X$OCT_WCALC" != "X"; then
	AC_MSG_CHECKING([for the installed octave site oct directory])
	SITEOCT=`$OCTAVE_CONFIG --print LOCALVEROCTFILEDIR`
#	SITEOCT=`$OCTAVE_CONFIG --oct-site-dir`
	AC_MSG_RESULT([$SITEOCT])

	AC_MSG_CHECKING([for the installed octave site fcn directory])
	SITEOCTM=`$OCTAVE_CONFIG --print LOCALVERFCNFILEDIR`
#	SITEOCTM=`$OCTAVE_CONFIG --m-site-dir`
	AC_MSG_RESULT([$SITEOCTM])

	AC_MSG_CHECKING([for the installed octave libexec directory])
	OCTLIBEXECDIR=`$OCTAVE_CONFIG --print LIBEXECDIR`
	AC_MSG_RESULT([$OCTLIBEXECDIR])

	AC_MSG_CHECKING([for the installed octave share directory])
	OCTDATADIR=`$OCTAVE_CONFIG --print DATADIR`
	AC_MSG_RESULT([$OCTDATADIR])

	MYSITEOCT=`echo ${SITEOCT} | sed "s;^${OCTLIBEXECDIR}/;;g"`
	MYSITEOCTM=`echo ${SITEOCTM} | sed "s;^${OCTDATADIR}/;;g"`
	AC_MSG_CHECKING([for where we should install octave .oct files])
	AC_MSG_RESULT([$MYSITEOCT])
	AC_MSG_CHECKING([for where we should install octave .m files])
	AC_MSG_RESULT([$MYSITEOCTM])

	# if we're building our own libwcalc, then 
	# we don't have an installed place to link to yet
	AC_MSG_CHECKING([for the octave mex library arguments])
	if test "X$WCALC_LIBS" = "X" ; then
		if test "X$OCTMEX_RPATH" = "X" ; then
			WCALC_OCTLIBS="-L../libwcalc/.libs -lwcalc -lm"
		else
			WCALC_OCTLIBS="-L../libwcalc/.libs ${OCTMEX_RPATH}\${libdir} -lwcalc -lm"
		fi
	else
		for fl in $WCALC_LIBS ; do
			case $fl in
			-l* )
				WCALC_OCTLIBS="$WCALC_OCTLIBS $fl"
				;;
			-L*)
				WCALC_OCTLIBS="$WCALC_OCTLIBS $fl"
				if test "X$OCTMEX_RPATH" != "X" ; then
					tmp=`echo $fl | sed 's/^-L//'`
					WCALC_OCTLIBS="$WCALC_OCTLIBS ${OCTMEX_RPATH}/${tmp}"
				fi
				;;
			*)
				AC_MSG_WARN([Skipping option $fl])
				;;
			esac
		done
	fi
	AC_MSG_RESULT([$WCALC_OCTLIBS])
else
	OCT_VER=""
fi

AC_SUBST(WCALC_OCTLIBS)
AC_SUBST(OCT_WCALC)
AC_SUBST(OCTAVE)
AC_SUBST(OCTAVE_CONFIG)
AC_SUBST(MKOCTFILE)
AC_SUBST(OCTMEX)
AC_SUBST(OCTMEX_RPATH)
AC_SUBST(SITEOCT)
AC_SUBST(SITEOCTM)
AC_SUBST(MYSITEOCT)
AC_SUBST(MYSITEOCTM)

# ------------- done with octave tests -------------------

# ------------- enable/disable web pages  -------------------

wc_htdocs_yesno=no

AC_MSG_CHECKING([if the web site should be generated])
AC_ARG_ENABLE([htdocs],
[  --enable-htdocs         Causes the web page to be built [default=no]],
[
if test "X$enable_htdocs" = "Xno" ; then
	HTDOCS=""
	AC_MSG_RESULT([no])
	wc_htdocs_yesno=no
else
	HTDOCS="htdocs"
	AC_MSG_RESULT([yes])
	wc_htdocs_yesno=yes
fi
],
[
HTDOCS=""
AC_MSG_RESULT([no])
wc_htdocs_yesno=no
])
AC_SUBST(HTDOCS)

# ------------- done with --enable-htdocs -------------------

# ------------- enable/disable sourceforge content  ---------

wc_sf_wcalc_yesno=no
AC_MSG_CHECKING([if the sourceforge specific web content should be added])
AC_ARG_ENABLE([sourceforge],
[  --enable-sourceforge    Causes the sourceforge specific web content to be added [default=no]],
[
if test "X$enable_sourceforge" = "Xno" ; then
	SOURCEFORGE_NO=""
	SOURCEFORGE_YES="#"
	AC_MSG_RESULT([no])
	wc_sf_wcalc_yesno=no
else
	SOURCEFORGE_NO="#" 
	SOURCEFORGE_YES="" 
	AC_MSG_RESULT([yes])
	wc_sf_wcalc_yesno=yes
fi
],
[
SOURCEFORGE_NO=""
SOURCEFORGE_YES="#"
AC_MSG_RESULT([no])
wc_sf_wcalc_yesno=no
])
AC_SUBST(SOURCEFORGE_NO)
AC_SUBST(SOURCEFORGE_YES)
# ------------- done with --enable-sourceforge -------------------

# ------------- enable/disable stdio frontend  -------------------

wc_stdio_wcalc_yesno=no

AC_MSG_CHECKING([if the stdio frontend should be compiled])
AC_ARG_ENABLE([stdio],
[  --enable-stdio          Causes the stdio frontend to be built [default=yes]],
[
if test "X$enable_stdio" = "Xno" ; then
	STDIO_WCALC=""
	AC_MSG_RESULT([no])
	wc_stdio_wcalc_yesno=no
else
	STDIO_WCALC="stdio-wcalc"
	AC_MSG_RESULT([yes])
	wc_stdio_wcalc_yesno=yes
fi
],
[
STDIO_WCALC="stdio-wcalc"
AC_MSG_RESULT([yes])
wc_stdio_wcalc_yesno=yes
])
AC_SUBST(STDIO_WCALC)

# ------------- done with --enable-stdio -------------------

# ------------- enable/disable CGI frontend  -------------------
wc_cgi_wcalc_yesno=no

AC_MSG_CHECKING([if the CGI frontend should be compiled])
AC_ARG_ENABLE([cgi],
[  --enable-cgi            Causes the CGI frontend to be built [default=yes]],
[
if test "X$enable_cgi" = "Xno" ; then
	CGI_WCALC=""
	AC_MSG_RESULT([no])
	wc_cgi_wcalc_yesno=no
else
	CGI_WCALC="cgi-wcalc"
	AC_MSG_RESULT([yes])
	wc_cgi_wcalc_yesno=yes
fi
],
[
CGI_WCALC="cgi-wcalc"
AC_MSG_RESULT([yes])
wc_cgi_wcalc_yesno=yes
])
AC_SUBST(CGI_WCALC)

CGI_STATIC=""
wc_cgi_static_yesno=no

AC_MSG_CHECKING([if the CGI frontend should be statically linked])
AC_ARG_ENABLE([cgistatic],
[  --enable-cgistatic      Causes the CGI frontend to be statically linked [default=no]],
[
if test "X$enable_cgistatic" = "Xno" ; then
	CGI_STATIC=""
	AC_MSG_RESULT([no])
	wc_cgi_static_yesno=no
else
	CGI_STATIC="-all-static"
	AC_MSG_RESULT([yes])
	wc_cgi_static_yesno=yes
fi
],
[
CGI_STATIC=""
AC_MSG_RESULT([no])
wc_cgi_static_yesno=no
])
AC_SUBST(CGI_STATIC)

# ------------- done with --enable-cgi -------------------


# ------------- enable/disable GTK+ frontend  -------------------

AC_MSG_CHECKING([if gtk-wcalc should be built])

wc_gtk1_wcalc_yesno=no

# keep track of if we're building for gtk1 because that is the program default
# or because the user explicitly asked for gtk1
wc_gtk1_askedfor=no

AC_ARG_ENABLE([gtk1],
[  --enable-gtk1           Causes the GTK+ frontend to be built with gtk1 [default=no]],
[
if test "X$enable_gtk1" = "Xno" ; then
	GTK1=no
	wc_gtk1_wcalc_yesno=no
else
	GTK1=yes
	wc_gtk1_wcalc_yesno=yes
fi
],
[
GTK1=no
wc_gtk1_wcalc_yesno=no
])

wc_gtk2_wcalc_yesno=yes

AC_ARG_ENABLE([gtk2],
[  --enable-gtk2           Causes the GTK+ frontend to be built with gtk2 [default=yes]],
[
if test "X$enable_gtk2" = "Xno" ; then
	GTK2=no
	wc_gtk2_wcalc_yesno=no
	wc_gtk2_askedfor=yes
else
	GTK2=yes
	wc_gtk2_wcalc_yesno=yes
fi
],
[
GTK2=yes
wc_gtk2_wcalc_yesno=yes
])

#
# If the user has asked for gtk1 with --enable-gtk1 and not explicitly asked
# for gtk2 with --enable-gtk2 (but got gtk2 enabled simply via the defaults)
# then disable gtk2.  This way you can do --enable-gtk1 without needing 
# --disable-gtk2
#
if test "X$GTK1" = "Xyes" -a "X$GTK2" = "Xyes" -a "X$wc_gtk2_askedfor" = "Xno"; then
	GTK2=no
	wc_gtk2_wcalc_yesno=no
fi

if test "X$GTK1" = "Xyes" -a "X$GTK2" = "Xyes" ; then
   AC_ERROR([You may not specify gtk1 and gtk2 at the same time])

fi
if test "X$GTK1" = "Xyes" -o "X$GTK2" = "Xyes" ; then
   GTK_WCALC=gtk-wcalc
   if test "X$GTK1" = "Xyes" ; then
      AC_MSG_RESULT([yes, gtk1])
   else
      AC_MSG_RESULT([yes, gtk2])
   fi
else
   GTK_WCALC=""
   AC_MSG_RESULT([no])
fi
AC_SUBST(GTK_WCALC)

# ------------- done with --enable-gtk -------------------

# ------------- pathnames  -------------------

# where to install the CGI programs

AC_MSG_CHECKING([where to install the CGI programs])
AC_ARG_WITH([cgibin],
[  --with-cgibin=cgibin    Install the CGI programs in the specified
                          directory.  [default = libexecdir/cgi-bin]],
[
if test "X$with_cgibin" = "Xno" ; then
	AC_ERROR([--without-cgibin is not allowed])
else
	CGIBINDIR=$with_cgibin
fi
],
[
CGIBINDIR='${libexecdir}/cgi-bin'
])
AC_MSG_RESULT([$CGIBINDIR])
AC_SUBST(CGIBINDIR)

# path to the CGI programs on the webserver

AC_MSG_CHECKING([URI path to  the CGI programs])
AC_ARG_WITH([cgipath],
[  --with-cgipath=cgipath  Path used by the web server to access
                          the CGI programs. [default = /cgi-bin/]],
[
if test "X$with_cgipath" = "Xno" ; then
	AC_ERROR([--without-cgipath is not allowed])
else
	CGIPATH=$with_cgipath
fi
],
[
CGIPATH='/cgi-bin/'
])
AC_MSG_RESULT([$CGIPATH])
AC_SUBST(CGIPATH)

# where to install the HTML pages and .PNG files:

AC_MSG_CHECKING([where to install the html pages])
AC_ARG_WITH([htmldir],
[  --with-htmldir=htmldir  Install the HTML files and PNG images needed
                          for the web page and the the CGI interface to
                          this directory.  The specified directory must
                          be accessible to the web server.
                          [default = pkgdatadir]],
[
if test "X$with_htmldir" = "Xno" ; then
	AC_ERROR([--without-htmldir is not allowed])
else
	HTMLDIR=$with_htmldir
fi
],
[
HTMLDIR='${pkgdatadir}'
])
AC_MSG_RESULT([$HTMLDIR])
AC_SUBST(HTMLDIR)

# path to the HTML pages and .PNG files for the webserver:


AC_MSG_CHECKING([for the URI path to the web pages])
AC_ARG_WITH([htmlpath],
[  --with-htmlpath=htmlpath    Path used by the web server to access the
                          HTML files and PNG images.
                          [default = /wcalc/]],
[
if test "X$with_htmlpath" = "Xno" ; then
	AC_ERROR([--without-htmlpath is not allowed])
else
	HTMLPATH=$with_htmlpath
fi
],
[
HTMLPATH='/wcalc/'
])
AC_MSG_RESULT([$HTMLPATH])
AC_SUBST(HTMLPATH)


# ------------- done with path tests -------------------

# ------------- enable/disable Matlab/MEX frontend  -------------------

MATLAB_DIR=""
wc_mex_wcalc_yesno=no

# because automake insists that .m files are for objective-C
OBJC="echo"
AC_SUBST(OBJC)

AC_ARG_WITH([matlab],
[  --with-matlab=matlabdir Base of matlab installation.
                          [default=do not build matlab interface]],
[
if test "X$with_matlab" = "Xno" ; then
	MEX_WCALC=""
	wc_mex_wcalc_yesno=no
elif test "X$with_matlab" = "Xyes" ; then
	AC_PATH_PROG(MEX, mex, no)
	if test "X$MEX" = "Xno" ; then
		AC_ERROR([No matlab mex could be found])
	fi

	MEX_WCALC=mex-wcalc
	wc_mex_wcalc_yesno=yes
else
	MATLAB_DIR=$with_matlab

	AC_PATH_PROG(MEX, mex, no, [$MATLAB_DIR/bin:$PATH])
	if test "X$MEX" = "Xno" ; then
		AC_ERROR([No matlab mex could be found])
	fi

	MEX_WCALC=mex-wcalc
	wc_mex_wcalc_yesno=yes
fi
])

AC_SUBST(MATLAB_DIR)
AC_SUBST(MEX_WCALC)

# ------------- done with --with-matlab -------------------

# mex -h (from matlab 6 release 12.1) says:
#   sol2, SunOS 5.x - .mexsol
#	hpux            - .mexhpux
#	hp700           - .mexhp7
#	ibm_rs          - .mexrs6
#	sgi             - .mexsg
#	alpha           - .mexaxp
#	glnx86          - .mexglx
#	Windows         - .dll

MEXSFX="none"
if test "X$MEX_WCALC" != "X" ; then

AC_MSG_CHECKING([for matlab mex suffix])
case $host in
	rs6000-*-aix* )
		MEXSFX="mexrs6"
		;;

	*-*-cygwin* )
		MEXSFX="dll"
		;;

	hppa*-*-hpux* )
		MEXSFX="mexhpux"
		;;

	*-*-irix* )
		MEXSFX="mexsg"
		;;

	i?86-*-linux* )
		MEXSFX="mexglx"
		;;

	*-*-mingw* )
		MEXSFX="dll"
		;;

	alpha-*-netbsd* )
		# use the osf/alpha version
		MEXSFX="mexaxp"
		;;

	i386-*-netbsd* )
		# use the linux x86 version
		MEXSFX="mexglx"
		;;

	mips*-*-netbsd* )
		# use the irix/mips version
		MEXSFX="mexsg"
		;;

	rs6000-*-netbsd* )
		# use the aix/rs6000 version
		MEXSFX="mexrs6"
		;;

	sparc-*-netbsd* )
		# use the solaris/sparc version
		MEXSFX="mexsol"
		;;

	alpha-*-osf* )
		MEXSFX="mexaxp"
		;;

	hppa*-*-osf )
		MEXSFX="mexhp7"
		;;

	*-*-solaris* )
		MEXSFX="mexsol"
		;;

	* )
		MEXSFX="unknown"
		;;

esac
AC_MSG_RESULT([$MEXSFX])
fi
AC_SUBST(MEXSFX)

WCALC_MEXLIBS=""
if test "X$MEX_WCALC" != "X"; then
	AC_MSG_CHECKING([if $MEX is matlab mex])
	AC_LANG(C)
	ac_ext=c
	ac_cpp='$MEX $CPPFLAGS'
	ac_compile='$MEX -c conftest.$ac_ext >&AS_MESSAGE_LOG_FD'
	ac_link='$MEX conftest.$ac_ext $LIBS >&AS_MESSAGE_LOG_FD'
	ac_compiler_gnu=$ac_cv_c_compiler_gnu
	save_ac_exeext=$ac_exeext
	ac_exeext=.${MEXSFX}

	AC_LINK_IFELSE([
#include "mex.h"
void mexFunction(
                 int nlhs,
                 mxArray *plhs[[]],
                 int nrhs,
#if !defined(V4_COMPAT) && !defined(SCI_MEX)
                 const mxArray *prhs[[]]
#else
                 mxArray *prhs[[]]
#endif
                 )
{
	mexPrintf("Hello, world\n");
}
],
	AC_MSG_RESULT([yes])
,
	AC_ERROR([$MEX is either broken or is not matlab mex])
)

if test "X${_use_rpath}" = "Xyes" ; then
	wc_save_LIBS="$LIBS"
	rpath=""
	for fl in "-Wl,-rpath -Wl," "-Wl,--rpath -Wl," "-rpath " "--rpath " "-Wl,-R" "-Wl,-R -Wl," "-R" "-R " ; do
	    LIBS="$wc_save_LIBS ${fl}path"
	    AC_MSG_CHECKING([if $MEX accepts ${fl}path for runtime libraries])
	    AC_LINK_IFELSE([
#include "mex.h"
void mexFunction(
                 int nlhs,
                 mxArray *plhs[[]],
                 int nrhs,
#if !defined(V4_COMPAT) && !defined(SCI_MEX)
                 const mxArray *prhs[[]]
#else
                 mxArray *prhs[[]]
#endif
                 )
{
	mexPrintf("Hello, world\n");
}
],

		AC_MSG_RESULT([yes])
		rpath=$fl, 
		AC_MSG_RESULT([no]))
		test -n "$rpath" && break
	done
	if test -n "$rpath"; then
	   MEX_RPATH="$rpath"
	else
	   MEX_RPATH=""
	fi
	LIBS="$wc_save_LIBS"
else
	MEX_RPATH=""
fi
	# restore the language settings
	ac_exeext=$save_ac_exeext
	AC_LANG(C)

	# if we're building our own libwcalc, then 
	# we don't have an installed place to link to yet
	AC_MSG_CHECKING([for the matlab mex library arguments])
	if test "X$WCALC_LIBS" = "X" ; then
		if test "X$MEX_RPATH" = "X" ; then
			WCALC_MEXLIBS="-L../libwcalc/.libs -lwcalc -lm"
		else
			WCALC_MEXLIBS="-L../libwcalc/.libs ${MEX_RPATH}\${libdir} -lwcalc -lm"
		fi
	else
		for fl in $WCALC_LIBS ; do
			case $fl in
			-l* )
				WCALC_MEXLIBS="$WCALC_MEXLIBS $fl"
				;;
			-L*)
				WCALC_MEXLIBS="$WCALC_MEXLIBS $fl"
				if test "X$MEX_RPATH" != "X" ; then
					tmp=`echo $fl | sed 's/^-L//'`
					WCALC_MEXLIBS="$WCALC_MEXLIBS ${MEX_RPATH}/${tmp}"
				fi
				;;
			*)
				AC_MSG_WARN([Skipping option $fl])
				;;
			esac
		done
	fi
	AC_MSG_RESULT([$WCALC_MEXLIBS])
fi
AC_SUBST(WCALC_MEXLIBS)

# ------------- end of matlab mex -------------------

AC_EXEEXT
AC_SUBST(EXEEXT)

dnl see if make sets MAKE
AC_PROG_MAKE_SET

dnl check for AWK capabilities
AC_AWK_GENSUB(
[
AWK_GENSUB_YES=""
AWK_GENSUB_NO="@#"
] ,[
AWK_GENSUB_YES="@#"
AWK_GENSUB_NO=""
])
AC_SUBST(AWK_GENSUB_YES)
AC_SUBST(AWK_GENSUB_NO)

dnl check for headers
AC_HEADER_STDC
AC_CHECK_HEADERS(locale.h string.h)

dnl check for libc functions
AC_CHECK_FUNCS(vsnprintf)

dnl check for math functions
AC_CHECK_LIB(m,sin)
AC_CHECK_FUNCS(rint)


dnl gettext
AM_GNU_GETTEXT([external])

# ------------- GTK -------------------
dnl GTK checks
GTK_CFLAGS=""
GTK_LIBS=""

if test "X$GTK2" = "Xyes" ; then
   PKG_CHECK_MODULES(GTK, gtk+-2.0 >= 2.2.0, 
			  GTK22="yes"
			  GTK_VER=`$PKG_CONFIG --version gtk+-2.0`,
			  GTK22="no")

   if test "X$GTK22" = "Xno" ; then
      AC_ERROR([Did not find gtk-2.2.0 or higher])
   fi
fi

if test "X$GTK1" = "Xyes" ; then
	AC_PATH_PROG(GTK_CONFIG,gtk-config,none)
	if test "$WIN32" = "yes"; then
		dnl XXX this is a hack.  It needs to be detected...
		GTK_VER=${GTK_VER:-1.3}
		GLIB_VER=${GLIB_VER:-1.3}
		GTK=${GTK:-c:/cygwin/usr/local/src/gtk+}
		GLIB=${GLIB:-c:/cygwin/usr/local/src/glib}
		AC_MSG_NOTICE([

-------------------------------------------------------------
It sems that you have requested gtk1 but you are on
a windows (cygwin/mingw) system.  You should really
consider using gtk2 instead.  If, however, you insist
on gtk1, you should probably set the following variables:

GTK_VER   currently ${GTK_VER}
GLIB_VER  currently ${GLIB_VER}
GTK       currently ${GTK}
GLIB      currently ${GLIB}

Since these 4 settings are not checked with configure, 
you will have to make sure they are correct for your system.
-------------------------------------------------------------

])
		GLIB_CFLAGS="-I${GLIB} -I${GLIB}/gmodule"
		GTK_CFLAGS="$GLIB_CFLAGS -I${GTK}/gdk -I${GTK}"
		GLIB_LIBS="-L${GLIB} -lglib-${GLIB_VER} \
		   -L${GLIB}/gmodule -lgmodule-${GLIB_VER} \
		   -L${GLIB}/gthread -lgthread-${GLIB_VER} \
		   -L${GLIB}/gobject -lgobject-${GLIB_VER}"
		GTK_LIBS="$GLIB_LIBS \
		  -L${GTK}/gtk -lgtk-${GTK_VER} \
		  -L${GTK}/gdk -lgdk-${GTK_VER}"
	elif test "x$GTK_CONFIG" = "xnone"; then
		AC_MSG_ERROR([You must have gtk installed on your system])
	else
		AC_MSG_CHECKING([if gtk1 is version 1.2 or later])
		GTK_VER=`$GTK_CONFIG --version`

		case $GTK_VER in
		0.* )
			AC_MSG_ERROR([you need at least version 1.2 of gtk1])
			;;
		1.1* )
			AC_MSG_ERROR([you need at least version 1.2 of gtk1])
			;;
		2.* )
			AC_MSG_ERROR([you need version 1.2.* of gtk1])
			;;
		* )
			AC_MSG_RESULT([$GTK_VER])
			AC_MSG_CHECKING([for gtk1 CFLAGS])
			GTK_CFLAGS=`$GTK_CONFIG --cflags`
			AC_MSG_RESULT([$GTK_CFLAGS])
			AC_MSG_CHECKING([for gtk1 libs])
			GTK_LIBS=`$GTK_CONFIG --libs`
			AC_MSG_RESULT([$GTK_LIBS])
			;;
		esac	
	fi
fi

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)
	

# ------------- dmalloc -------------------
dnl dmalloc checks
wc_dmalloc_yesno=no
AC_MSG_CHECKING([if dmalloc debugging should be enabled])
AC_ARG_ENABLE([dmalloc],
[  --enable-dmalloc        Compile and link with dmalloc for malloc debugging [default=no]],
[
if test "X$enable_dmalloc" != "Xno" ; then
	AC_MSG_RESULT([yes])
	AC_CHECK_HEADERS(dmalloc.h)
	DMALLOC_CFLAGS="-I/usr/pkg/include -DDMALLOC"
	DMALLOC_LIBS="-L/usr/pkg/lib -R/usr/pkg/lib -ldmalloc"
	wc_dmalloc_yesno=yes
else
	AC_MSG_RESULT([no])
	DMALLOC_CFLAGS=""
	DMALLOC_LIBS=""
	wc_dmalloc_yesno=no
fi
],
[
DMALLOC_CFLAGS=""
DMALLOC_LIBS=""
AC_MSG_RESULT([no])
wc_dmalloc_yesno=no
])
AC_SUBST(DMALLOC_CFLAGS)
AC_SUBST(DMALLOC_LIBS)

# ------------- ElectricFence -------------------
dnl ElectricFence checks
wc_efence_yesno=no

AC_MSG_CHECKING([if ElectricFence debugging should be enabled])
AC_ARG_ENABLE([efence],
[  --enable-efence         Link with ElectricFence for malloc debugging [default=no]],
[
if test "X$enable_efence" != "Xno" ; then
	AC_MSG_RESULT([yes])
	EFENCE_LIBS="-L/usr/pkg/lib -R/usr/pkg/lib -lefence"
	wc_efence_yesno=yes
else
	AC_MSG_RESULT([no])
	EFENCE_LIBS=""
	wc_efence_yesno=no
fi
],
[
EFENCE_LIBS=""
AC_MSG_RESULT([no])
wc_efence_yesno=no
])
AC_SUBST(EFENCE_LIBS)

LIBWALC_LDFLAGS=""
if test "x$GCC" = "xyes"; then
	dnl Only use -Wall if we have gcc
	if test -z "`echo "$CFLAGS" | grep "\-Wall" 2> /dev/null`" ; then
		CFLAGS="$CFLAGS -Wall"
	fi
	
	dnl gcc on sparc with shared libs version of libwcalc
	dnl segfaults on ic_microstrip.c.  
	case $host in
   	sparc-*-* )
		AC_MSG_NOTICE([Avoiding gcc on sparc with shared lib bug])
		#LIBWALC_LDFLAGS="-all-static"
		;;
	esac

fi
AC_SUBST(LIBWALC_LDFLAGS)

AC_OUTPUT(
	Makefile 
	doc/Makefile
	include/Makefile
	m4/Makefile
	po/Makefile.in
	libwcalc/Makefile
	libwcalc/wcalc-config
	libwcalc/wcalc.pc
	cgi-wcalc/Makefile
	gtk-wcalc/Makefile
	htdocs/Makefile
	htdocs/mex-man/Makefile
	htdocs/octave-man/Makefile
	htdocs/sci-man/Makefile
	htdocs/stdio-man/Makefile
	mex-wcalc/Makefile
	mex-wcalc/mexopts.sh
	octave-wcalc/Makefile
	sci-wcalc/Makefile
	stdio-wcalc/Makefile
	pixmaps/Makefile
	utils/Makefile
	win32/Makefile
)


# print a summary of paths and such since there are a bunch
AC_MSG_NOTICE([
--------------------------------------------
   cgi-wcalc options

html directory = $HTMLDIR
html path      = $HTMLPATH
cgi directory  = $CGIBINDIR
cgi path       = $CGIPATH
--------------------------------------------
   The following will be built

   htdocs      $wc_htdocs_yesno
   libwcalc    $wc_libwcalc_yesno
   cgi-wcalc   $wc_cgi_wcalc_yesno
   static cgi? $wc_cgi_static_yesno
   gtk1-wcalc  $wc_gtk1_wcalc_yesno
   gtk2-wcalc  $wc_gtk2_wcalc_yesno
   mex-wcalc   $wc_mex_wcalc_yesno
   oct-wcalc   $wc_oct_wcalc_yesno
   sci-wcalc   $wc_sci_wcalc_yesno
   stdio-wcalc $wc_stdio_wcalc_yesno
   sourceforge $wc_sf_wcalc_yesno

   dmalloc debugging         $wc_dmalloc_yesno
   ElectricFence debugging   $wc_efence_yesno

--------------------------------------------
]
)

