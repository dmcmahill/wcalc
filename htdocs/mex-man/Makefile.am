## $Id: Makefile.am,v 1.2 2001/10/30 23:52:53 dan Exp $

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

vpath %.man $(top_srcdir)/sci-wcalc

include $(top_srcdir)/sci-wcalc/mex.mk
#MEX_SRCS=air_coil_calc.c

pkgimgdatadir= @HTMLDIR@/mex-man
pkgimgdata_DATA= ${SCIMANHTML}

# the include= is a ':' seperated search path for SSI directives
SHTML_INCLUDE_PATH= $(srcdir):$(top_srcdir)/htdocs

SCIMANSHTML=	${MEX_SRCS:.c=.shtml}
SCIMANHTML=	${MEX_SRCS:.c=.html}
SCIMANS=	${MEX_SRCS:.c=.man}
SCICATS=	${SCIMANS:.man=.cat}

all-local:	whatis.incl

clean-local:	
	-rm ${SCIMANHTML} ${SCIMANSHTML} whatis.incl ${BUILT_SOURCES}


BUILT_SOURCES= left_column.incl main_footer.incl
EXTRA_DIST=	man_start.incl man_end.incl

CP_INCL=	sed -e 's;a href=";a href="../;g' \
		    -e 's;img src=";img src="../;g' 

left_column.incl: $(srcdir)/../left_column.incl
	$(CP_INCL) $< > $@

main_footer.incl: $(srcdir)/../main_footer.incl
	$(CP_INCL) $< > $@

whatis.incl: ${SCIMANS} Makefile $(top_srcdir)/sci-wcalc/mex.mk
	@echo "****************************************************"
	@echo "Generating the HTML whatis table"
	@echo "****************************************************"
	rm -f $@
	for man in ${SCIMANS} ; do \
	${AWK} 'BEGIN{s=0;} \
		/^\.SH/ {s=0}; \
		s==1{ \
			printf("<tr><td><a href=\"mex-man/%s.html\">%s</a></td><td>",$$1,$$1); \
			for(i=3;i<=NF;i++){printf("%s ",$$i);} \
			printf("</td></tr>\n"); \
		} \
		/^\.SH[ \t]*NAME/ {s=1}' $(top_srcdir)/sci-wcalc/$$man | sort >> $@ ; \
	done

SUFFIXES=	.shtml .html .cat

.cat.shtml :
	sed 's;@fname@;$*;g' $(srcdir)/man_start.incl > $@
	echo "<pre>" >> $@
	cat $< | col -b | \
		${AWK} '/(^[ \t]*[0-9]+$)|(^Dan.*[0-9]+$)/ {cnt=3;next;} \
			cnt==0{print  $$0} \
			cnt > 0{cnt--}' | \
		sed 's;//;%;g' >> $@
	echo "</pre>" >> $@
	cat $(srcdir)/man_end.incl >> $@

include $(top_srcdir)/sci-wcalc/sciman.mk
include $(top_srcdir)/htdocs/shtml.mk
