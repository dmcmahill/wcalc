## -*- Makefile -*-
## $Id: Makefile.am,v 1.35 2004/11/09 12:55:44 dan Exp $

## Copyright (c) 2001, 2002, 2004 Dan McMahill
## All rights reserved.
##
## This code is derived from software written by Dan McMahill
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. All advertising materials mentioning features or use of this software
##    must display the following acknowledgement:
##        This product includes software developed by Dan McMahill
##  4. The name of the author may not be used to endorse or promote products
##     derived from this software without specific prior written permission.
## 
##  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
##  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
##  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
##  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
##  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
##  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
##  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
##  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
##  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
##  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
##  SUCH DAMAGE.
##

# list of all the mex files
include $(srcdir)/mex.mk

scimandir=	${datadir}/${PACKAGE}-${VERSION}/man
sciman_DATA=	${SCIHTML} ${SCIMANS} ${SCICATS} whatis whatis.htm
demodir=		${datadir}/${PACKAGE}-${VERSION}/demos
demo_DATA=	${SCIDEMOS}

SCIDEMOS=	wcalc-demo.dem

SCI_VER=		@SCI_VER@
scilibdir=	${libdir}/scilab-${SCI_VER}/${PACKAGE}-${VERSION}
scilib_LTLIBRARIES= libsci_wcalc.la

pkglibexecdir=  ${libexecdir}/scilab-${SCI_VER}/${PACKAGE}-${VERSION}
pkglibexec_SCRIPTS=	load_wcalc.sce

libsci_wcalc_la_SOURCES = \
	alert.c \
	libsci_wcalc.c \
	${MEX_SRCS}

libsci_wcalc_la_LIBADD = @WCALC_LDADD@
libsci_wcalc_la_LDFLAGS = -avoid-version -export-dynamic -module

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS} @WCALC_LIBS@

# note, we mangle the function name because with matlab, our function will have
# the same name as the filename, but in Scilab, we need to replace mexFunction with
# a more unique name
INCLUDES = -I$(top_srcdir)/include @WCALC_CFLAGS@ -I@SCI@/routines -DmexFunction=mex_$* ${DMALLOC_CFLAGS}

SCIMANS=	${MEX_SRCS:.c=.man}
SCICATS=	${SCIMANS:.man=.cat}
SCIHTML=	${SCIMANS:.man=.htm}

all-local:	${SCIMANS} load_wcalc.sce test_wcalc.sce wcalc_test.sci

MAINTAINERCLEANFILES=	${SCICATS} ${SCIHTML} whatis whatis.htm
CLEANFILES=		load_wcalc.sce test_wcalc.sce load_wcalc.sce.in ${TEST_OUT}

EXTRA_DIST= mex.mk sciman.mk ${SCIMANS} README.newmodel demo.sce ${SCIDEMOS} \
	${RUN_TESTS} ${TEST_FILES} ${TEST_REF} \
	whatis whatis.htm ${SCICATS} ${SCIHTML} \
	wcalc_test.sci.in

BUILT_SOURCES= libsci_wcalc.c  whatis whatis.htm ${SCICATS} ${SCIHTML}

RUN_TESTS=	run_tests.sh
check_SCRIPTS=	${RUN_TESTS}
TEST_FILES=	\
	air_coil_calc.tst \
	air_coil_syn.tst \
	coax_calc.tst \
	coax_syn.tst \
	coupled_microstrip_calc.tst \
	coupled_microstrip_syn.tst \
	ic_microstrip_calc.tst \
	ic_microstrip_syn.tst \
	microstrip_calc.tst \
	microstrip_syn.tst \
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


whatis: ${SCIMANS}  $(srcdir)/Makefile.am
	@echo "****************************************************"
	@echo "Generating the whatis table"
	@echo "****************************************************"
	@(for f in ${SCIMANS}; do ${AWK} 'BEGIN{s=0;} \
		/^\.SH/ {s=0}; \
		s==1{ \
			printf("%-26s - ",$$1); \
			for(i=3;i<=NF;i++){printf("%s ",$$i);} \
			printf("\n"); \
		} \
		/^\.SH[ \t]*NAME/ {s=1}' ${srcdir}/$$f ; done) | sort > whatis

whatis.htm: ${SCIMANS}  $(srcdir)/Makefile.am
	@echo "****************************************************"
	@echo "Generating the whatis.htm table"
	@echo "****************************************************"
	@echo "<html>" > $@
	@echo "<head>" >> $@
	@echo '<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">' >> $@
	@echo "<title>${PACKAGE}</title>" >> $@
	@echo "</head>" >> $@
	@echo '<body bgcolor="FFFFFF">' >> $@
	@echo "<table>" >> $@
	@(for f in ${SCIMANS}; do ${AWK} 'BEGIN{s=0;} \
		/^\.SH/ {s=0}; \
		s==1{ \
			printf("<TR><TD><A HREF=\"%s.htm\">%s</A></TD>",$$1,$$1); \
			printf("<TD>"); \
			for(i=3;i<=NF;i++){printf("%s ",$$i);} \
			printf("</TD></TR>\n"); \
		} \
		/^\.SH[ \t]*NAME/ {s=1}' ${srcdir}/$$f ; done) | sort >> $@
	@echo "</table></body></html>" >> $@

SUFFIXES=

include $(srcdir)/sciman.mk
