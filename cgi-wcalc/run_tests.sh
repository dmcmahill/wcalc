#!/bin/sh
#
# $Id: run_tests.sh,v 1.1 2004/09/03 03:18:51 dan Exp $
#
# Copyright (c) 2009 Dan McMahill
# All rights reserved.
#
# This code is derived from software written by Dan McMahill
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#        This product includes software developed by Dan McMahill
#  4. The name of the author may not be used to endorse or promote products
#     derived from this software without specific prior written permission.
# 
#  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
#  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
#  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
#  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
#  SUCH DAMAGE.
#

regen=no

while test -n "$1"
do
    case "$1"
    in

    -h|--help)
	echo "Sorry, help not available for this script yet"
	exit 0
	;;

    -r|--regen)
	# regenerate the 'golden' output files.  Use this with caution.
	# In particular, all differences should be noted and understood.
	regen=yes
	shift
	;;

    -*)
	echo "unknown option: $1"
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


# make sure we have the right paths when running this from inside the
# source tree and also from outside the source tree.
here=`pwd`
srcdir=${srcdir:-$here}
srcdir=`cd $srcdir && pwd`

rundir=${here}/run

TESTLIST=${srcdir}/tests.list

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
