## -*- Makefile -*-
## $Id: Makefile.am,v 1.61 2012/03/19 10:57:24 dan Exp $

## Copyright (C) 2001, 2002, 2004, 2005, 2006, 2008, 2009, 2017 Dan McMahill
## All rights reserved.
##
## 
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
## 
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
## 
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
## 

# list of all the mex files
include $(srcdir)/mex.mk

scimandir=	${datadir}/${PACKAGE}-${VERSION}/sci-man
sciman_DATA=	${SCIHTML} ${SCIMANS} ${SCICATS} whatis whatis.htm
demodir=		${datadir}/${PACKAGE}-${VERSION}/sci-demos
demo_DATA=	${SCIDEMOS}

SCIDEMOS=	wcalc-demo.dem \
	air_coil_calc.dem \
	air_coil_syn.dem \
	coupled_stripline_calc.dem \
	bars_calc.dem \
	parallel_rc.dem \
	parallel_rl.dem \
	stripline_calc.dem

SCI_VER=		@SCI_VER@
scilibdir=	${libdir}/scilab-${SCI_VER}/${PACKAGE}-${VERSION}
scilib_LTLIBRARIES= libsci_wcalc.la

pkglibexecdir=  ${libexecdir}/scilab-${SCI_VER}/${PACKAGE}-${VERSION}
pkglibexec_SCRIPTS=	load_wcalc.sce

libsci_wcalc_la_SOURCES = \
	alert.c \
	libsci_wcalc.c \
	${MEX_SRCS}

if WC_LOCAL_LIBWCALC
libsci_wcalc_la_LIBADD = ../libwcalc/libwcalc.la
else
libsci_wcalc_la_LIBADD = 
endif

# Scilab seems to really want the .so extension
libsci_wcalc_la_LDFLAGS = -avoid-version -export-dynamic -module -shrext ".so"

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS} @WCALC_LIBS@

# note, we mangle the function name because with matlab, our function will have
# the same name as the filename, but in Scilab, we need to replace mexFunction with
# a more unique name
AM_CPPFLAGS = -I$(top_srcdir)/include @WCALC_CFLAGS@ -I@SCI@/routines -DmexFunction=mex_$* ${DMALLOC_CFLAGS}

SCIMANS=	${MEX_SRCS:.c=.man}
SCICATS=	${SCIMANS:.man=.cat}
SCIHTML=	${SCIMANS:.man=.htm}
SCIXML=		${SCIMANS:.man=.xml}

all-local:	${SCIMANS} load_wcalc.sce test_wcalc.sce wcalc_test.sci dot_scilab

MAINTAINERCLEANFILES=	${SCICATS} $SCIMANS} ${SCIHTML} whatis whatis.htm
CLEANFILES=		load_wcalc.sce test_wcalc.sce load_wcalc.sce.in ${TEST_OUT} \
			dot_scilab

EXTRA_DIST= mex.mk sciman.mk ${SCIMANS} README.newmodel demo.sce ${SCIDEMOS} \
	${RUN_TESTS} ${TEST_FILES} ${TEST_REF} \
	whatis whatis.htm ${SCICATS} ${SCIHTML} ${SCIXML} \
	wcalc_test.sci.in manpage.xsl htmlpage.xsl whatis.xsl man.dtd \
	dot_scilab.in see_also.xml wcalc.xml

BUILT_SOURCES= libsci_wcalc.c  whatis whatis.htm ${SCIMANS} ${SCICATS} ${SCIHTML}

RUN_TESTS=	run_tests.sh
check_SCRIPTS=	${RUN_TESTS}
TEST_FILES=	\
	air_coil_calc.tst \
	air_coil_syn.tst \
	bars_calc.tst \
	coax_calc.tst \
	coax_syn.tst \
	coplanar_calc.tst \
	coplanar_syn.tst \
	coupled_microstrip_calc.tst \
	coupled_microstrip_syn.tst \
	coupled_stripline_calc.tst \
	coupled_stripline_syn.tst \
	ic_microstrip_calc.tst \
	ic_microstrip_syn.tst \
	microstrip_calc.tst \
	microstrip_syn.tst \
	parallel_rc_p2s.tst \
	parallel_rc_s2p.tst \
	parallel_rl_p2s.tst \
	parallel_rl_s2p.tst \
	stripline_calc.tst \
	stripline_syn.tst

TEST_OUT=	${TEST_FILES:.tst=.dia}
TEST_REF=	${TEST_OUT:=.ref}

TESTS=	${RUN_TESTS}

wcalc_test.sci: $(srcdir)/wcalc_test.sci.in
	cp $< $@

#
# Generate the entry point function that scilab uses to access the
# mex-functions
#
libsci_wcalc.c: $(srcdir)/mex.mk $(srcdir)/Makefile.am
	@echo "****************************************************"
	@echo "Generating the scilab gateway function in $@"
	@echo "****************************************************"
	@echo "#include \"mex.h\"" > $@
	@for mex in ${MEX_SRCS} ; do \
		mexfn=`basename $$mex .c`; \
		echo "extern Gatefunc mex_$${mexfn};" >> $@ ; \
	done
	@echo "static GenericTable Tab[]={ " >> $@
	@for mex in ${MEX_SRCS} ; do \
		mexfn=`basename $$mex .c`; \
		echo "{mex_gateway, mex_$${mexfn},\"err msg\"}," >> $@ ; \
	done
	@echo "};" >> $@
	@echo "" >> $@
	@echo "int C2F(libsci_wcalc_gateway)()" >> $@
	@echo "{ Rhs = Max(0, Rhs);" >> $@
	@echo "(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);" >> $@
	@echo " return 0;" >> $@
	@echo "}" >> $@

#
# Generate a scilab script that loads the wcalc interface.  This
# generated script would generally be executed in the users
# .scilab file to load wcalc at startup.
#
# We make 2 versions, one has the final installation path hard
# coded in it ("load_wcalc.sce"), the other has the build directory
# path hard coded in it and is used for testing before installation
# ("test_wcalc.sce")
#
load_wcalc.sce.in: $(srcdir)/mex.mk  $(srcdir)/Makefile.am
	@echo "****************************************************"
	@echo "Generating the scilab loader function in $@"
	@echo "****************************************************"
	@echo "// Execute this script from within Scilab to load" > $@
	@echo "// the wcalc interface.  Do this with:" >> $@
	@echo "//    --> exec(\"${pkglibexecdir}/load_wcalc.sce\");" >> $@
	@echo "// The exec line can be added to the .scilab file in" >> $@
	@echo "// your home directory." >> $@
	@echo "" >> $@
	@echo "scilab_functions=[..." >> $@
	@for mex in ${MEX_SRCS} ; do \
		mexfn=`basename $$mex .c`; \
		echo "\"$${mexfn}\";" >> $@ ; \
	done
	@echo "]; " >> $@
	@echo "" >> $@
	@echo "addinter(\"@libpath@\"+\"libsci_wcalc.so\",..." >> $@
	@echo "    \"libsci_wcalc_gateway\",..." >> $@
	@echo "     scilab_functions);" >> $@
	@echo "" >> $@
	@echo "%helps=[%helps; \"@manpath@\", \"Wcalc\"];" >> $@
	@echo "" >> $@
	@echo "// make_help_index was not present prior to scilab 3.0" >> $@
	@echo "if( exists('make_help_index') ), make_help_index(); end" >> $@
	@echo "" >> $@
	@echo "// demolist isn't defined in versions of scilab before 2.6" >> $@
	@echo "if (exists('demolist') == 0), demolist=[]; end" >> $@
	@echo "demolist=[demolist ; 'Wcalc Demos' ..." >> $@
	@echo "'@demopath@/wcalc-demo.dem'];" >> $@

load_wcalc.sce:	load_wcalc.sce.in
	sed     -e 's;@libpath@;${scilibdir}/;g' \
		-e 's;@demopath@;${demodir};g' \
		-e 's;@manpath@;${scimandir};g' $< > $@
	@echo "****************************************************"
	@echo "* To use the scilab interface after installation   *"
	@echo "* execute the file \"load_wcalc.sce\" from within    *"
	@echo "* scilab.  This is done by:                        *"
	@echo "       --> exec(\"${pkglibexecdir}/load_wcalc.sce\");"
	@echo "* you may wish to add that command to your .scilab *"
	@echo "* file to have it loaded automatically on startup. *"
	@echo "****************************************************"

test_wcalc.sce:	load_wcalc.sce.in
	sed     -e "s;@libpath@;`pwd`/.libs/;g" \
		-e "s;@demopath@;`pwd`/;g" \
		-e "s;@manpath@;`pwd`;g" \
		-e "s;${pkglibexecdir}/load_wcalc.sce;`pwd`/test_wcalc.sce;g" $< > $@
	@echo "****************************************************"
	@echo "* To test the scilab interface before installation *"
	@echo "* execute the file \"test_wcalc.sce\" from within    *"
	@echo "* scilab.  This is done by:                        *"
	@echo "       --> exec(\"`pwd`/test_wcalc.sce\");"
	@echo "****************************************************"


whatis: whatis.htm
	@echo "****************************************************"
	@echo "Generating the whatis table"
	@echo "****************************************************"
	${W3M} -dump whatis.htm > $@

whatis.htm: ${SCIMANS}  $(srcdir)/Makefile.am $(srcdir)/whatis.xsl
	@echo "****************************************************"
	@echo "Generating the whatis.htm table"
	@echo "****************************************************"
	echo "<html>" > $@
	echo "<head>" >> $@
	echo '<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">' >> $@
	echo "<title>${PACKAGE}-${VERSION} Functions</title>" >> $@
	echo "</head>" >> $@
	echo '<body bgcolor="FFFFFF">' >> $@
	echo '<h1>${PACKAGE}-${VERSION} Functions</h1>' >> $@
	echo "<dl>" >> $@
	${XSLTPROC} $(srcdir)/whatis.xsl ${SCIXML}  >> $@
	@echo "</dl></body></html>" >> $@

dot_scilab: $(srcdir)/dot_scilab.in
	sed \
		-e 's;@scidir@;${pkglibexecdir};g' \
		-e 's;@pkgname@;${PACKAGE}-${VERSION};g' \
		$(srcdir)/dot_scilab.in > dot_scilab

SUFFIXES=

# used by the XSLT templates
TARGETPROGRAM=	scilab
HTMLSUFFIX=		htm

include $(srcdir)/sciman.mk
