<?xml version="1.0"?>
<!-- $Id: htmlpage.xsl,v 1.1 2005/10/19 00:25:57 dan Exp $ -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html" indent="yes"  />
<xsl:strip-space elements="*"/>
<xsl:param name="program">scilab</xsl:param>
<xsl:param name="header">yes</xsl:param>

<xsl:template match="MAN">
<xsl:if test="$header = 'yes'">
<html>
<head>
<!-- Copyright (c) 2001, 2002, 2004, 2005
        Dan McMahill  ALL RIGHTS RESERVED. -->
<!-- Do not edit directly.  Edit the xml file instead -->
	<link rev="made" href="mailto:danmc@sourceforge.net"></link>
	<link rel="shortcut icon" href="favicon.ico" type="image/x-icon"></link>
	<title><xsl:value-of select="NAME"/></title>
	<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"></meta>
	<meta name="DESCRIPTION" content="Wcalc:  A transmission line calculator"></meta>
	<meta name="KEYWORDS" content=""></meta>
</head>

<body bgcolor="#ffffff" text="#000000" link="#3535c5" vlink="#700080">
</xsl:if>

<xsl:value-of select="NAME"/> - <xsl:value-of select="SHORTDESCR"/>

<h2>SYNOPSIS</h2>

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

<h2>PARAMETERS</h2>
<h3>INPUT PARAMETERS</h3>
<ul>
<xsl:for-each select="PARAMETERS/RHS">
<li>
<xsl:value-of select="@name"/>
: <xsl:apply-templates/>
</li>
</xsl:for-each>
</ul>

<h3>OUTPUT VALUES</h3>
<ul>
<xsl:for-each select="PARAMETERS/LHS">
<li>
<xsl:value-of select="@name"/>
: <xsl:value-of select="."/>
</li>
</xsl:for-each>
</ul>

<h2>DESCRIPTION</h2>
<p>
<xsl:value-of select="DESCRIPTION"/>
</p>

<h2>EXAMPLE</h2>
<pre>
<xsl:if test="$program = 'stdio'">
<xsl:for-each select="EXAMPLE/STDEX"><xsl:value-of select="."/></xsl:for-each>
</xsl:if>

<xsl:if test="$program != 'stdio'">
	<xsl:for-each select="EXAMPLE/SCIEX">
		<xsl:for-each select="COMMENT">
			<xsl:if test="$program = 'stdio'" >
				<xsl:text> * </xsl:text>
			</xsl:if>

			<xsl:if test="$program = 'scilab'">
				<xsl:text> // </xsl:text>
			</xsl:if>

			<xsl:if test="$program = 'octave'">
				<xsl:text> % </xsl:text>
				</xsl:if>

			<xsl:if test="$program = 'matlab'">
				<xsl:text> % </xsl:text>
			</xsl:if>
			<xsl:value-of select="."/>
			<xsl:text>
</xsl:text>
		</xsl:for-each>

		<xsl:value-of select="./SCICMD"/> 
		<xsl:text>
</xsl:text>
	</xsl:for-each>
</xsl:if>
</pre>

<h2>SEE ALSO</h2>
<p>
<xsl:value-of select="SEE_ALSO"/>
</p>

<h2>AUTHOR</h2>
<p>
<xsl:value-of select="AUTHOR"/>
</p>

<h2>BUGS</h2>
<p>
<xsl:value-of select="BUGS"/>
</p>

<xsl:if test="$header = 'yes'">
</body>
</html>
</xsl:if>
</xsl:template>

<xsl:template match="*"/>

<xsl:template match="ENUM">
	<ul>
	<xsl:for-each select="ITEM">
		<li> <xsl:value-of select="@name"/> : <xsl:value-of select="."/> </li>
	</xsl:for-each>
	</ul>
</xsl:template>


</xsl:stylesheet>

