<?xml version="1.0"?>
<!-- $Id: whatis.xsl,v 1.4 2005/10/21 12:55:44 dan Exp $ -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent="yes"  />
<xsl:strip-space elements="*"/>
<xsl:param name="path"/>
<xsl:param name="suffix">htm</xsl:param>

<xsl:template match="TOP/MAN">
<dd><a href="{$path}{NAME}.{$suffix}"><xsl:value-of select="NAME"/></a> - <xsl:value-of select="SHORTDESCR"/></dd>
</xsl:template>

</xsl:stylesheet>

