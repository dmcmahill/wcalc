#!/bin/sh
#
# $Id$
#

SCILAB=${SCILAB:-scilab}
srcdir=${srcdir:-`pwd`}

HOME=$srcdir

${SCILAB} -nw -f ${srcdir}/wcalc_test.sce


