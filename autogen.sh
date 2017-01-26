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

libtool_min_version=2.4.0
echo "Checking libtoolize version..."
libtoolize --version 2>&1 > /dev/null
rc=$?
if test $rc -ne 0 ; then
    echo "Could not determine the version of libtool on your machine"
    echo "libtool --version produced:"
    libtool --version
    exit 1
fi
lt_ver=`libtoolize --version | awk '{print $NF; exit}'`
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
echo "Running libtoolize..."
libtoolize --force --copy --automake || exit 1

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

# FIXME -- extract from configure.ac
autoconf_min_version=2.68

echo "Checking autoconf version..."
ac_ver=`autoconf --version | awk '{print $NF; exit}'`
ac_maj=`echo $ac_ver | sed 's;\..*;;g'`
ac_min=`echo $ac_ver | sed -e 's;^[0-9]*\.;;g'  -e 's;\..*$;;g'`
ac_teeny=`echo $ac_ver | sed -e 's;^[0-9]*\.[0-9]*\.;;g'`
echo "    $ac_ver"

case $ac_maj in
    0|1)
        echo "You must have autoconf >= ${autoconf_min_version} but you seem to have $ac_ver"
        exit 1
	;;

    2)
        if test $ac_min -lt 68 ; then
            echo "You must have autoconf >= ${autoconf_min_version} but you seem to have $ac_ver"
            exit 1
        fi
        ;;

    *)
        echo "You are running a newer autoconf than wcalc has been tested with."
	echo "It will probably work, but this is a warning that it may not."
	;;
esac

echo "Running autoconf..."
autoconf || exit 1
echo "... done with autoconf."
