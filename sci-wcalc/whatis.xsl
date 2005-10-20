<?xml version="1.0"?>
<!-- $Id: whatis.xsl,v 1.1 2005/10/20 00:46:04 dan Exp $ -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent="yes"  />
<xsl:strip-space elements="*"/>

<xsl:template match="MAN">
<tr><td><a href="{NAME}.htm"><xsl:value-of select="NAME"/></a></td><td><xsl:value-of select="SHORTDESCR"/></td></tr>
</xsl:template>

</xsl:stylesheet>

