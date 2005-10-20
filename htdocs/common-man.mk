## $Id: Makefile.am,v 1.4 2005/10/20 02:30:43 dan Exp $

##
## Copyright (c) 2005 Dan McMahill
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


whatis.incl: ${SCIMANS} Makefile $(top_srcdir)/sci-wcalc/mex.mk $(top_srcdir)/sci-wcalc/whatis.xsl
	@echo "****************************************************"
	@echo "Generating the HTML whatis table"
	@echo "****************************************************"
	rm -f $@
	for f in ${WC_XML} ; do \
		${XSLTPROC} --stringparam suffix "html" \
			--stringparam path "${MPATH}/" \
			$(top_srcdir)/sci-wcalc/whatis.xsl \
			$(top_srcdir)/sci-wcalc/$$f >> $@ ; \
	done

SUFFIXES=	.shtml .html .cat

.xml.shtml :
	sed 's;@fname@;$*;g' $(srcdir)/man_start.incl > $@
	${XSLTPROC} --stringparam program "${TARGETPROGRAM}" \
		--stringparam header "no" $(top_srcdir)/sci-wcalc/htmlpage.xsl $< >> $@
	cat $(srcdir)/man_end.incl >> $@

