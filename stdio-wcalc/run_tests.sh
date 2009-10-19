#!/bin/sh
#
# $Id: run_tests.sh,v 1.1 2004/09/03 03:18:51 dan Exp $
#
# Copyright (c) 2003, 2004, 2009 Dan McMahill
# All rights reserved.
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


regen=no
list_only=no

# make sure we have the right paths when running this from inside the
# source tree and also from outside the source tree.
here=`pwd`
srcdir=${srcdir:-$here}
srcdir=`cd $srcdir && pwd`

rundir=${here}/run

TESTLIST=${srcdir}/tests.list


usage() {

$0 - Interact with the stdio-wcalc test suite

$0 -h|--help
$0 -l|--list-tests
$0 [-r|--regen] [testname1 [testname2 [...] ] ]

Command line options:

  -h|--help        Displays this help message and exits

  -l|--list-tests  Lists all available tests

  -r|--regen       Instead of running the selected tests and checking for a correct
		   result, the results are treated as "golden" and replace the reference
		   files.  This option is for developer use and should be used with
		   caution and the resulting reference file should be checked very
		   carefully.

If no test name is given then $0 will run all of the tests which are 
configured in ${TESTLIST}.  To run a single test or a subset of the complete tests,
just list the test names on the $0 command line.

} 

while test -n "$1"
do
    case "$1"
    in

    -h|--help)
	usage
	exit 0
	;;

    -l|--list-tests)
	# just list the available tests
	list_only=yes
	shift
	;;

    -r|--regen)
	# regenerate the 'golden' output files.  Use this with caution.
	# In particular, all differences should be noted and understood.
	regen=yes
	shift
	;;

    -*)
	echo "unknown option: $1"
	usage
	exit 1
	;;

    *)
	break
	;;

    esac
done

if [ "X$regen" = "Xyes" ]; then
    sufx="ref"
else
    sufx="log"
fi


if [ ! -f $TESTLIST ]; then
    echo "ERROR: ($0)  Test list $TESTLIST does not exist"
    exit 1
fi

# fail/pass/total counts
fail=0
pass=0
skip=0
tot=0

if test -z "$1" ; then
    all_tests=`awk 'BEGIN{FS="|"} /^#/{next} {print $1}' $TESTLIST | sed 's; ;;g'`
else
    all_tests=$*
fi

if test "x$list_only" = "xyes" ; then
	echo "All tests:"
	for t in $all_tests ; do
		echo "  ${t}"
	done
	exit 0
fi

echo "Starting tests in $here."
echo "Source directory is $srcdir"

for t in $all_tests ; do

    dirs=`grep "^[ \t]*${t}[ \t]*|" $TESTLIST | awk 'BEGIN{FS="|"} {print $2}'`
    files=`grep "^[ \t]*${t}[ \t]*|" $TESTLIST | awk 'BEGIN{FS="|"} {print $3}'`
    args=`grep "^[ \t]*${t}[ \t]*|" $TESTLIST | awk 'BEGIN{FS="|"} {print $4}'`

    tot=`expr $tot + 1`

    echo "Test:  $t"
    ${here}/stdio-wcalc -v ${srcdir}/${t}.dat 2>/dev/null > ${here}/${t}.dia

    if [ -f ${srcdir}/${t}.ref ]; then
	if diff ${srcdir}/${t}.ref ${here}/${t}.dia >/dev/null ; then
	    if [ "X$regen" != "Xyes" ]; then
		echo "PASS"
	    else
		echo "Not regenerated (no change)"
	    fi
	    pass=`expr $pass + 1`
	else
	    if [ "X$regen" != "Xyes" ]; then
		echo "FAILED:  See diff ${srcdir}/${t}.ref ${here}/${t}.dia"
	    else	       
		mv ${here}/${t}.dia ${srcdir}/${t}.ref 
		echo "Regenerated (changed)"
	    fi
	    fail=`expr $fail + 1`
	fi
    else
	if [ "X$regen" != "Xyes" ]; then
	    echo "No reference file.  Skipping"
	else	       
	    mv ${here}/${t}.dia ${srcdir}/${t}.ref 
	    echo "Regenerated (created)"
	fi
	skip=`expr $skip + 1`
    fi


done

if [ "X$regen" != "Xyes" ]; then

    echo "Passed $pass, failed $fail, skipped $skip out of $tot tests."

    rc=0

    if [ $pass -ne $tot ]; then
	rc=`expr $tot - $pass`
    fi

    exit $rc
else

    echo "Created $skip, Regenerated $fail, Saved $pass out of $tot tests"

    exit 0
fi
