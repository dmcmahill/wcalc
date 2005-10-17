<?xml version="1.0"?>
<!-- $Id$ -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="text" indent="yes"  />
<xsl:strip-space elements="*"/>
<xsl:param name="program">scilab</xsl:param>
<xsl:variable name="comment">XXX</xsl:variable>

<xsl:template match="MAN">
<xsl:if test="$program = 'stdio'" >
	STDIO  <xsl:variable name="comment">bar</xsl:variable>
</xsl:if>
<xsl:if test="$program = 'scilab'">
	SCILAB <xsl:variable name="comment">ppc</xsl:variable>
</xsl:if>
<xsl:if test="$program = 'octave'">
	OCTAVE <xsl:variable name="comment">rep</xsl:variable>
</xsl:if>
<xsl:if test="$program = 'matlab'">
	MATLAB <xsl:variable name="comment">rep</xsl:variable>
</xsl:if>
comment is:  <xsl:value-of select="$comment"/>
.\" Copyright (c), 2005 Dan McMahill
.\" Do not edit this directly.  Edit the XML source file instead
.\"
.TH <xsl:value-of select="NAME"/> 1
.B <xsl:value-of select="NAME"/>
\- <xsl:value-of select="SHORTDESCR"/>
.SH SYNOPSIS
[<xsl:for-each select="PARAMETERS/LHS">
<xsl:value-of select="@name"/>
<xsl:if test="position() != last()">, </xsl:if>
</xsl:for-each>] = 
<xsl:value-of select="NAME"/>(<xsl:for-each select="PARAMETERS/RHS">
<xsl:value-of select="@name"/>
<xsl:if test="position() != last()">, </xsl:if>
</xsl:for-each>)

for stdio:\ 

<xsl:value-of select="NAME"/>\ 
<xsl:for-each select="PARAMETERS/RHS"> <xsl:value-of select="@name"/>\ </xsl:for-each>


.SH INPUT PARAMETERS
<xsl:for-each select="PARAMETERS/RHS">
.TP 10
<xsl:value-of select="@name"/>
: <xsl:value-of select="."/>
</xsl:for-each>
<xsl:apply-templates/>
.SH OUTPUT VALUES
<xsl:for-each select="PARAMETERS/LHS">
.TP 10
<xsl:value-of select="@name"/>
: <xsl:value-of select="."/>
</xsl:for-each>
.SH DESCRIPTION
<xsl:value-of select="DESCRIPTION"/>
.SH EXAMPLE
.nf
<xsl:if test="$program = 'stdio'">
<xsl:for-each select="EXAMPLE/STDEX"><xsl:value-of select="."/></xsl:for-each>
</xsl:if>
scilab example
<xsl:for-each select="EXAMPLE/SCIEX">
\ <xsl:value-of select="./SCICMD"/> <xsl:value-of select="$comment"/> <xsl:value-of select="./COMMENT"/>
</xsl:for-each>
.fi
.SH SEE ALSO
<xsl:value-of select="SEE_ALSO"/>
.SH AUTHOR
<xsl:value-of select="AUTHOR"/>
.SH BUGS
<xsl:value-of select="BUGS"/>
</xsl:template>

<xsl:template match="*"/>

<xsl:template match="PARAMETERS/RHS/ENUM">
.RS
<xsl:for-each select="ITEM">
.TP 10
<xsl:value-of select="@name"/>
: <xsl:value-of select="."/>
</xsl:for-each>
.RE
</xsl:template>


</xsl:stylesheet>

