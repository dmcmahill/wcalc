## -*- Makefile -*-
## $Id: sciman.mk,v 1.4 2003/03/03 03:25:20 dan Exp $
##

## Copyright (c) 2001, 2004 Dan McMahill
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

SUFFIXES+= .cat .man .htm

TBL=	@TBL@
NEQN=	@NEQN@
GROFF=	@GROFF@

.man.cat : 
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

.man.htm : 
	@echo "****************************************************"
	@echo "Processing $*.man to produce $*.htm"
	@echo "****************************************************"
	cat $< | ${TBL} | ${NEQN} -Thtml -Tps | ${GROFF} -man -Thtml| \
		${AWK} 'BEGIN{s=0;t=0;} \
		     /^ *$$/ {s=1;next;} \
		     s==1 { \
			s=0; \
			if(t){printf("\n");} \
			t=1; \
		     } \
		     {print;}' > $@

