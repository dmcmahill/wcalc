## $Id: shtml.mk,v 1.1 2001/10/28 22:51:43 dan Exp $
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

# the include= is a ':' seperated search path for SSI directives
SHTML2HTML  = ${AWK} -f $(top_srcdir)/utils/shtml2html include=$(SHTML_INCLUDE_PATH)

# All the new suffix rules

SUFFIXES+= .shtml .html

.shtml.html :
	@AWK_GENSUB_YES@$(SHTML2HTML) $*.shtml | sed 's;/cgi-bin/;${CGIPATH};g' > $*.html
	@@AWK_GENSUB_NO@ echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@@AWK_GENSUB_NO@ echo "          function.  This prevents the rebuilding of the"
	@@AWK_GENSUB_NO@ echo "          .html files from the .shtml files.  If you need"
	@@AWK_GENSUB_NO@ echo "          this functionality, you will need to install gawk."
	@@AWK_GENSUB_NO@ echo "          By setting the variable AWK in your configure"
	@@AWK_GENSUB_NO@ echo "          environment, you can force configure to find a"
	@@AWK_GENSUB_NO@ echo "          particular awk program."

