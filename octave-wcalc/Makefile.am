## -*- makefile -*-
## $Id: Makefile.am,v 1.6 2004/09/09 12:47:07 dan Exp $
##

## Copyright (c) 2002, 2004 Dan McMahill
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

# to find the mex source files
vpath %.c $(top_srcdir)/sci-wcalc
vpath %.man $(top_srcdir)/sci-wcalc

# list of all the mex files
include $(top_srcdir)/sci-wcalc/mex.mk

# the 'mex' command to compile the mex files
MEX=	@OCTMEX@

# the suffix for the compiled mex file
MEXSFX=	oct

octfiledir=	@SITEOCT@/${PACKAGE}
fcnfiledir=	@SITEOCTM@/${PACKAGE}
octfile_DATA=	${MEX_FILES}
fcnfile_DATA=	${MEX_MANS}

noinst_SCRIPTS=		runoctave

MEX_FILES=	${MEX_SRCS:.c=.${MEXSFX}}
MEX_MANS=	${MEX_SRCS:.c=.m}
MEX_TMP_CXX_FILES=	${addprefix mex_,${MEX_SRCS:.c=.cc}}
MEX_TMP_OBJ_FILES=	${MEX_TMP_CXX_FILES:.cc=.o}

AM_LDFLAGS=	${DMALLOC_LIBS} ${EFENCE_LIBS} @WCALC_LIBS@

# various MEX flags
MEXLDFLAGS=	@WCALC_OCTLIBS@ ${DMALLOC_LIBS} ${EFENCE_LIBS}
MEXINCLUDES=	-I$(top_srcdir) -I$(top_srcdir)/include @WCALC_CFLAGS@


all-local:	${MEX_FILES} runoctave ${MEX_MANS}

clean-local:
	-rm alert.o ${MEX_FILES} runoctave ${MEX_MANS} ${MEX_TMP_CXX_FILES} ${MEX_TMP_OBJ_FILES}

runoctave:	$(srcdir)/runoctave.in
	sed "s;@BUILDDIR@;`pwd`;g" $(srcdir)/runoctave.in > $@
	chmod 755 $@


EXTRA_DIST=	runoctave.in copyright.m

# All the new suffix rules
SUFFIXES= .c .${MEXSFX} .m .cat .man

.c.${MEXSFX} :
	$(MEX) $(MEXINCLUDES) $< $(srcdir)/alert.c $(MEXLDFLAGS)

.cat.m :
	@echo "****************************************************"
	@echo "Generating Octave help file: $*.m"
	@echo "****************************************************"
	${AWK} 'BEGIN{s=0;} \
		/^\.SH/ {s=0;t=3;} \
		s==1{ \
			if(t==3)printf("%%%-16s ",toupper($$1)); \
			for(i=t;i<=NF;i++){printf("%s ",$$i);} \
			t=1; \
		} \
		/^\.SH[ \t]*NAME/ {s=1} \
		END{printf("\n");}' $(top_srcdir)/sci-wcalc/$*.man > $@
	echo "%" >> $@
	cat $< | \
		col -b -x | \
		${AWK} '/(^[ \t]*[0-9]+$)|(^Dan.*[0-9]+$)/ {cnt=3;next;} \
				cnt==0{print "% " $$0} \
				cnt > 0{cnt--}' |\
		sed 's;//;%;g' \
		>> $@
	echo " " >> $@
	cat $(srcdir)/copyright.m >> $@

include $(top_srcdir)/sci-wcalc/sciman.mk

