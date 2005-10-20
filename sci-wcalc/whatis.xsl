<?xml version="1.0"?>
<!-- $Id: whatis.xsl,v 1.2 2005/10/20 01:05:30 dan Exp $ -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent="yes"  />
<xsl:strip-space elements="*"/>
<xsl:param name="path"/>
<xsl:param name="suffix">htm</xsl:param>

<xsl:template match="MAN">
<tr><td><a href="{$path}{NAME}.{$suffix}"><xsl:value-of select="NAME"/></a></td><td><xsl:value-of select="SHORTDESCR"/></td></tr>
</xsl:template>

</xsl:stylesheet>

