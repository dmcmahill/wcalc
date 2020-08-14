#!/bin/sh
#

# Copyright (c) 2003, 2005, 2007, 2008, 2009, 2017 Dan McMahill
# All rights reserved.
#
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
# 


# a leftover cache from a different version will cause no end of headaches
rm -fr autom4te.cache

############################################################################
#
# autopoint (gettext)
#

echo "Checking autopoint version..."
ver=`autopoint --version | awk '{print $NF; exit}'`
ap_maj=`echo $ver | sed 's;\..*;;g'`
ap_min=`echo $ver | sed -e 's;^[0-9]*\.;;g'  -e 's;\..*$;;g'`
ap_teeny=`echo $ver | sed -e 's;^[0-9]*\.[0-9]*\.;;g'`
echo "    $ver"

case $ap_maj in
    0)
        if test $ap_min -lt 14 ; then
            echo "You must have gettext >= 0.14.0 but you seem to have $ver"
            exit 1
        fi
        ;;
esac
echo "Running autopoint..."
autopoint --force || exit 1

############################################################################
#
# libtoolize (libtool)
#

case `uname` in
    Darwin*)
        LIBTOOLIZE=${LIBTOOLIZE:-glibtoolize}
        ;;
    *)
        LIBTOOLIZE=${LIBTOOLIZE:-libtoolize}
        ;;
esac

libtool_min_version=2.4.0
echo "Checking libtoolize version..."
${LIBTOOLIZE} --version 2>&1 > /dev/null
rc=$?
if test $rc -ne 0 ; then
    echo "Could not determine the version of libtool on your machine"
    echo "${LIBTOOLIZE} --version produced:"
    libtool --version
    exit 1
fi
lt_ver=`${LIBTOOLIZE} --version | awk '{print $NF; exit}'`
lt_maj=`echo $lt_ver | sed 's;\..*;;g'`
lt_min=`echo $lt_ver | sed -e 's;^[0-9]*\.;;g'  -e 's;\..*$;;g'`
lt_teeny=`echo $lt_ver | sed -e 's;^[0-9]*\.[0-9]*\.;;g'`
echo "    $lt_ver"

case $lt_maj in
    0|1)
        echo "You must have libtool >= ${libtool_min_version} but you seem to have $lt_ver"
        exit 1
	;;

    2)
        if test $lt_min -lt 4 ; then
            echo "You must have libtool >= ${libtool_min_version} but you seem to have $lt_ver"
            exit 1
        fi
        ;;

    *)
        echo "You are running a newer libtool than wcalc has been tested with."
	echo "It will probably work, but this is a warning that it may not."
	;;
esac
echo "Running ${LIBTOOLIZE}..."
${LIBTOOLIZE} --force --copy --automake || exit 1

############################################################################
#
# aclocal

echo "Checking aclocal version..."
acl_ver=`aclocal --version | awk '{print $NF; exit}'`
echo "    $acl_ver"

echo "Running aclocal..."
aclocal -I m4 $ACLOCAL_FLAGS || exit 1
echo "... done with aclocal."

############################################################################
#
# autoheader

echo "Checking autoheader version..."
ah_ver=`autoheader --version | awk '{print $NF; exit}'`
echo "    $ah_ver"

echo "Running autoheader..."
autoheader || exit 1
echo "... done with autoheader."

############################################################################
#
# automake

echo "Checking automake version..."
am_ver=`automake --version | awk '{print $NF; exit}'`
echo "    $am_ver"

echo "Running automake..."
automake --foreign --force --copy --add-missing || exit 1
echo "... done with automake."

############################################################################
#
# autoconf

# look for the AC_PREREQ([2.68]) line in configure.ac
autoconf_min_version=`awk '/^[ \t]*AC_PREREQ\(/ {v=$0; gsub(/^[^0-9]*/, "", v); gsub(/[^0-9]*$/, "", v); print v;}' configure.ac`
autoconf_rest=${autoconf_min_version}

autoconf_major=`echo $autoconf_rest | sed -e 's;[.].*;;g'`
autoconf_rest=`echo $autoconf_rest | sed -e 's;^[0-9]*[.]*;;g'`
echo "$autoconf_rest"

autoconf_minor=`echo $autoconf_rest | sed -e 's;[.].*;;g'`
autoconf_rest=`echo $autoconf_rest | sed -e 's;^[0-9]*[.]*;;g'`
echo "$autoconf_rest"

autoconf_teeny=${autoconf_rest}
if test -z "${autoconf_teeny}" ; then
    autoconf_teeny=0
fi
echo "Extracted autoconf_min_version=${autoconf_min_version} from configure.ac (${autoconf_major} ${autoconf_minor} ${autoconf_teeny})"

echo "Checking autoconf version..."
ac_ver=`autoconf --version | awk '{print $NF; exit}'`
ac_maj=`echo $ac_ver | sed 's;\..*;;g'`
ac_min=`echo $ac_ver | sed -e 's;^[0-9]*\.;;g'  -e 's;\..*$;;g'`
ac_teeny=`echo $ac_ver | sed -e 's;^[0-9]*\.[0-9]*\.;;g'`
echo "    $ac_ver"

ac_too_old=no
ac_too_new=no
if test $ac_maj -lt $autoconf_major ; then
    ac_too_old=yes
elif  test $ac_maj -eq $autoconf_major -a $ac_min -lt $autoconf_minor ; then
    ac_too_old=yes
elif test $ac_maj -gt $autoconf_major ; then
    ac_too_new=yes
else
    ac_too_old=no
fi

if test ${ac_too_old} = yes ; then
    echo "You must have autoconf >= ${autoconf_min_version} but you seem to have $ac_ver"
    exit 1
fi

if test ${ac_too_new} = yes ; then
    echo "You are running a newer autoconf than wcalc has been tested with."
    echo "It will probably work, but this is a warning that it may not."
fi

echo "Running autoconf..."
autoconf || exit 1
echo "... done with autoconf."
