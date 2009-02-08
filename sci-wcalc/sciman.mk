## -*- Makefile -*-
## $Id: sciman.mk,v 1.12 2009/01/11 04:22:24 dan Exp $
##

## Copyright (c) 2001, 2004, 2005, 2009 Dan McMahill
## All rights reserved.
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

WC_MANS=	${MEX_SRCS:.c=.man}
WC_CATS=	${MEX_SRCS:.c=.cat}
WC_HTML=	${MEX_SRCS:.c=.htm}
WC_XML=	${MEX_SRCS:.c=.xml}

SUFFIXES+= .cat .man .htm .xml .txt .m

GROFF=		@GROFF@
NEQN=		@NEQN@
TBL=		@TBL@
W3M=		@W3M@
XSLTPROC=	@XSLTPROC@

.man.cat : 
if MISSING_NROFF
	@echo "****************************************************"
	@echo "WARNING:  groff/tbl/neqn were not found on your system but"
	@echo "          $@ is out of date and needs to be"
	@echo "          rebuilt.  Changes to $<"
	@echo "          will be ignored."
	@echo "****************************************************"
else
	@echo "****************************************************"
	@echo "Processing $*.man to produce $*.cat"
	@echo "****************************************************"
	cat $< | ${TBL} | ${NEQN} -Tascii | ${GROFF} -man -Tascii | \
		${AWK} 'BEGIN{s=0;t=0;} \
		     /^ *$$/ {s=1;next;} \
		     s==1 { \
			s=0; \
			if(t){printf("\n");} \
			t=1; \
		     } \
		     {print;}' > $@
endif

.xml.man :
if MISSING_XSLT
	@echo "****************************************************"
	@echo "WARNING:  xsltproc and/or w3m were not found on your"
	@echo "          system but $@ is out of date and needs"
	@echo "          to be rebuilt.  Changes to $<"
	@echo "          will be ignored."
	@echo "****************************************************"
else
	${XSLTPROC} --stringparam program "${TARGETPROGRAM}" \
		--stringparam section "${MANSEC}" $(top_srcdir)/sci-wcalc/manpage.xsl $< > $@
endif

.xml.htm :
if MISSING_XSLT
	@echo "****************************************************"
	@echo "WARNING:  xsltproc and/or w3m were not found on your"
	@echo "          system but $@ is out of date and needs"
	@echo "          to be rebuilt.  Changes to $<"
	@echo "          will be ignored."
	@echo "****************************************************"
else
	${XSLTPROC} --stringparam program "${TARGETPROGRAM}" \
		--stringparam suffix "${HTMLSUFFIX}" \
		$(top_srcdir)/sci-wcalc/htmlpage.xsl $< > $@
endif

.htm.txt :
if MISSING_W3M
	@echo "****************************************************"
	@echo "WARNING:  xsltproc and/or w3m were not found on your"
	@echo "          system but $@ is out of date and needs"
	@echo "          to be rebuilt.  Changes to $<"
	@echo "          will be ignored."
	@echo "****************************************************"
else
	${W3M} -dump $< > $@
endif

##
## Used by octave-wcalc and mex-wcalc
##  TARGETPROGRAM will be set by the top level Makefile.am in one of those
##  subdirectories

.txt.m : 
	@echo "****************************************************"
	@echo "Generating ${TARGETPROGRAM} help file: $*.m"
	@echo "****************************************************"
	${AWK} 'BEGIN{s=0;} \
		s==0{ \
			printf("%%%-16s ",toupper($$1)); \
			for(i=2;i<=NF;i++){printf("%s ",$$i);} \
			printf("\n"); \
			s=1; \
			next; \
		} \
		{print "%  " $$0} \
		END{printf("\n\n");}' $< > $@
	cat $(srcdir)/copyright.m >> $@

