<?xml version="1.0"?>
<!-- $Id: manpage.xsl,v 1.3 2005/10/20 22:24:28 dan Exp $ -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" indent="yes"  />
<xsl:strip-space elements="*"/>
<xsl:param name="program">scilab</xsl:param>
<xsl:param name="section">n</xsl:param>

<xsl:template match="TOP">
<xsl:apply-templates/>
</xsl:template>

<xsl:template match="MAN">
<xsl:text>
.\" Copyright (c), 2005 Dan McMahill
.\" Do not edit this directly.  Edit the XML source file instead
.\"
</xsl:text>
.TH <xsl:value-of select="NAME"/> "<xsl:value-of select="$section"/>" "" "Wcalc" "Wcalc Commands"
.B <xsl:value-of select="NAME"/>
- <xsl:value-of select="SHORTDESCR"/>
.SH SYNOPSIS
<xsl:if test="$program != 'stdio'">
[<xsl:for-each select="PARAMETERS/LHS">
<xsl:value-of select="@name"/>
<xsl:if test="position() != last()">, </xsl:if>
</xsl:for-each><xsl:text>] = 
</xsl:text>
<xsl:value-of select="NAME"/>(<xsl:for-each select="PARAMETERS/RHS">
<xsl:value-of select="@name"/>
<xsl:if test="position() != last()">, </xsl:if>
</xsl:for-each>)
</xsl:if>

<xsl:if test="$program = 'stdio'">
<xsl:value-of select="NAME"/><xsl:text> </xsl:text>
<xsl:for-each select="PARAMETERS/RHS">
<xsl:value-of select="@name"/><xsl:text> </xsl:text>
</xsl:for-each>
</xsl:if>

.SH INPUT PARAMETERS
<xsl:for-each select="PARAMETERS/RHS">
.TP 10
<xsl:value-of select="@name"/>
: <xsl:apply-templates/>
</xsl:for-each>

.SH OUTPUT VALUES
<xsl:for-each select="PARAMETERS/LHS">
.TP 10
<xsl:value-of select="@name"/>
: <xsl:value-of select="."/>
</xsl:for-each>
.SH DESCRIPTION
<xsl:for-each select="DESCRIPTION">
<xsl:apply-templates/>
</xsl:for-each>
.SH EXAMPLE
.nf
<xsl:if test="$program = 'stdio'">
<xsl:for-each select="EXAMPLE/STDEX"><xsl:value-of select="."/></xsl:for-each>
</xsl:if>
<xsl:if test="$program != 'stdio'">
<xsl:for-each select="EXAMPLE/SCIEX">
<xsl:for-each select="COMMENT">
<xsl:if test="$program = 'stdio'" >
<xsl:text> .TP * </xsl:text>
</xsl:if>
<xsl:if test="$program = 'scilab'">
<xsl:text>
.TP
 // </xsl:text>
</xsl:if>
<xsl:if test="$program = 'octave'">
<xsl:text> .TP % </xsl:text>
</xsl:if>
<xsl:if test="$program = 'matlab'">
<xsl:text> .TP % </xsl:text>
</xsl:if>
<xsl:value-of select="."/>
</xsl:for-each>
<xsl:text>
</xsl:text>
<xsl:value-of select="./SCICMD"/> 
</xsl:for-each>
</xsl:if>
.fi
.SH SEE ALSO
stdio-wcalc(1),
<xsl:for-each select="SEE_ALSO/ALSO">
<xsl:value-of select="."/>(<xsl:value-of select="$section"/>)<xsl:if test="position() != last()">, </xsl:if>
</xsl:for-each>
.SH AUTHOR
<xsl:value-of select="AUTHOR"/>
.SH BUGS
<xsl:value-of select="BUGS"/>
</xsl:template>

<xsl:template match="*"/>

<xsl:template match="ENUM">
<xsl:text>
.RS</xsl:text>
<xsl:for-each select="ITEM">
.TP 10
<xsl:value-of select="@name"/>
: <xsl:value-of select="."/>
</xsl:for-each>
.RE
</xsl:template>

<xsl:template match="VERB">
.NF

<xsl:value-of select="."/>

.FI
</xsl:template>

</xsl:stylesheet>

