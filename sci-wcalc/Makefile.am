## $Id: Makefile.am,v 1.11 2001/10/24 03:02:53 dan Exp $
##

## Copyright (c) 2001 Dan McMahill
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

pkglib_LTLIBRARIES = libsci_wcalc.la

libsci_wcalc_la_SOURCES = \
	alert.c \
	libsci_wcalc.c \
	${MEX_SRCS}

libsci_wcalc_la_LIBADD = ../libwcalc/libwcalc.la 
libsci_wcalc_la_LDFLAGS = -avoid-version -export-dynamic -module

# note, we mangle the function name because with matlab, our function will have
# the same name as the filename, but in Scilab, we need to replace mexFunction with
# a more unique name
INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/libwcalc -I@SCI@/routines -DmexFunction=mex_$*

SCIMANS=	${MEX_SRCS:.c=.man}
SCICATS=	${SCIMANS:.man=.cat}

all-local:	${SCIMANS} load_wcalc.sce test_wcalc.sce

EXTRA_DIST= mex.mk sciman.mk ${SCIMANS}

BUILT_SOURCES= libsci_wcalc.c load_wcalc.sce test_wcalc.sce whatis ${SCICATS}

#
# Generate the entry point function that scilab uses to access the
# mex-functions
#
libsci_wcalc.c: $(srcdir)/mex.mk
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
load_wcalc.sce.in: $(srcdir)/mex.mk 
	@echo "****************************************************"
	@echo "Generating the scilab loader function in $@"
	@echo "****************************************************"
	@echo "scilab_functions=[..." > $@
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

load_wcalc.sce:	load_wcalc.sce.in
	sed  's;@libpath@;${pkglibdir}/;g' $< > $@
	@echo "****************************************************"
	@echo "* To use the scilab interface after installation   *"
	@echo "* execute the file \"load_wcalc.sce\" from within    *"
	@echo "* scilab.  This is done by:                        *"
	@echo "       --> exec(\"${pkglibdir}/load_wcalc.sce\");"
	@echo "* you may wish to add that command to your .scilab *"
	@echo "* file to have it loaded automatically on startup. *"
	@echo "****************************************************"

test_wcalc.sce:	load_wcalc.sce.in
	sed -e "s;@libpath@;`pwd`/.libs/;g" $< > $@
	@echo "****************************************************"
	@echo "* To test the scilab interface before installation *"
	@echo "* execute the file \"test_wcalc.sce\" from within    *"
	@echo "* scilab.  This is done by:                        *"
	@echo "       --> exec(\"`pwd`/test_wcalc.sce\");"
	@echo "****************************************************"


whatis: ${SCIMANS} 
	@echo "****************************************************"
	@echo "Generating the whatis table"
	@echo "****************************************************"
	@${AWK} 'BEGIN{s=0;} \
		/^\.SH/ {s=0}; \
		s==1{ \
			printf("%-16s - ",$$1); \
			for(i=3;i<=NF;i++){printf("%s ",$$i);} \
			printf("\n"); \
		} \
		/^\.SH[ \t]*NAME/ {s=1}' ${SCIMANS} | sort > whatis

SUFFIXES=

include $(srcdir)/sciman.mk
