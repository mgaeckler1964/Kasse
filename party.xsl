<?xml version="1.0" encoding="iso-8859-1" ?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:output method="html" indent="yes" encoding="iso-8859-1" />

	<xsl:template match='/|*'>
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match='PARTY'>
		<html>
			<head><title>Partyauswertung</title></head>
			<body>
				<table>
					<tr>
						<th align="left">Artikel</th>
						<th align="right">Menge</th>
						<th align="right">Partypreis</th>
						<th align="right">Standardpreis</th>
					</tr>
					<xsl:for-each select="ARTIKEL">
						<tr>
							<td align="left">
								<xsl:value-of select="@bezeichnung" />
							</td>
							<td align="right">
								<xsl:value-of select="@menge" />
							</td>
							<td align="right">
								<xsl:value-of select="@partypreis" />
							</td>
							<td align="right">
								<xsl:value-of select="@verkaufspreis" />
							</td>
						</tr>
					</xsl:for-each>
				</table>
				<hr />
				<table>
					<tr><th align="left">Umsatz</th><td align="right"><xsl:value-of select="@Umsatz" /></td></tr>
					<tr><th align="left">Betreiber</th><td align="right"><xsl:value-of select="@Betreiber" /></td></tr>
					<tr><th align="left">Veranstalter</th><td align="right"><xsl:value-of select="@Veranstalter" /></td></tr>
				</table>
			</body>
		</html>
	</xsl:template>

</xsl:stylesheet>