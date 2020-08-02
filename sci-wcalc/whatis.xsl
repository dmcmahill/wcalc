<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent="yes"  />
<xsl:strip-space elements="*"/>
<xsl:param name="path"/>
<xsl:param name="suffix">htm</xsl:param>

<!-- We use upper case on the A HREF bit because the scilab help
     browser is very fragile in its parsing of whatis.htm to generate
     a tree list -->
<xsl:template match="TOP/MAN">
<dd><A HREF="{$path}{NAME}.{$suffix}"><xsl:value-of select="NAME"/></A> - <xsl:value-of select="SHORTDESCR"/></dd>
</xsl:template>

</xsl:stylesheet>

