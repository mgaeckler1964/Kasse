<?xml version="1.0" encoding="iso-8859-1" ?>
<setup>
<application title="CRESD Kassenterminal" version="3.1.0.116" path="CRESD\Kasse" registry="CRESD\Kasse" />
<bde AliasName="KASSE" RESOURCE="0007" DataPath="" dbVersion="6" />
<icons Group="Kasse">
<icon title="Kasse" target="PROGRAM\Kasse.exe" main="true" dbUpd="" />
<icon title="Liesmich" target="PROGRAM\Liesmich.txt" main="" dbUpd="" />
<icon title="Kassen DB Administration" target="PROGRAM\KasseDB.exe" main="" dbUpd="true" />
<icon title="BDE Administration" target="BDE\bdeadmin.exe" main="" dbUpd="" />
<icon title="Handbuch" target="PROGRAM\Kasse.pdf" main="" dbUpd="" />
</icons>
<source>
<file source="..\..\Object\Kasse.exe" />
<file source="..\..\Object\kasseDB.exe" />
<file source="..\..\Object\srxViewer.exe" />
<file source="reports" />
<file source="party.xsl" />
<file source="..\..\Object\srxViewer.DEU" />
<file source="Liesmich.txt" />
<file source="..\Internet\web\cresd\edv\ger\Products\Kasse.pdf" />
<file source="..\..\Object\java\CresdKasse.apk" />
</source>
</setup>