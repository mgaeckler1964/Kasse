<?xml version="1.0" encoding="iso-8859-1" ?>
<setup>
<application title="CRESD Kassenterminal" version="3.1.2.129" author="gak" />
<bde AliasName="KASSE" RESOURCE="0007" DataPath="" dbVersion="5" />
<icons Group="Kasse">
<icon title="Kasse" target="PROGRAM\Kasse.exe" main="true" dbUpd="" />
<icon title="Liesmich" target="PROGRAM\Liesmich.txt" main="" dbUpd="" />
<icon title="Kassen DB Administration" target="PROGRAM\KasseDB.exe" main="" dbUpd="true" />
<icon title="BDE Administration" target="BDE\bdeadmin.exe" main="" dbUpd="" />
<icon title="Handbuch" target="PROGRAM\Kasse.pdf" main="" dbUpd="" />
</icons>
<source>
<group name="PROGRAM" destination="">
<file source="..\..\Object\Kasse.exe" />
<file source="..\..\Object\kasseDB.exe" />
<file source="..\..\Object\srxViewer.exe" />
<file source="reports" />
<file source="party.xsl" />
<file source="..\..\Object\srxViewer.DEU" />
<file source="Liesmich.txt" />
<file source="Mobil\bin\GakKasse.apk" />
<file source="Kasse.pdf" />
</group>
</source>
</setup>