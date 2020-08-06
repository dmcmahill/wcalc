##

## Copyright (c) 2001, 2002, 2020 Dan McMahill
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
## 


# the include_path= is a ':' seperated search path for SSI directives
SHTML2HTML  = ${AWK} -f $(top_srcdir)/utils/shtml2html include_path=$(SHTML_INCLUDE_PATH) $(SHTML2HTML_SF)

# All the new suffix rules

SUFFIXES+= .shtml .html

.shtml.html :
if AWK_GENSUB
	( ( ( $(SHTML2HTML) $<  ; echo $$? >&4 ) | \
		sed 's;/cgi-bin/;${CGIPATH};g' > $@) 4>&1 | \
		(read a ; exit $$a))
else
	@echo "WARNING:  your awk (${AWK}) does not include the gensub()"
	@echo "          function.  This prevents the rebuilding of the"
	@echo "          .html files from the .shtml files.  If you need"
	@echo "          this functionality, you will need to install gawk."
	@echo "          By setting the variable AWK in your configure"
	@echo "          environment, you can force configure to find a"
	@echo "          particular awk program."
endif

