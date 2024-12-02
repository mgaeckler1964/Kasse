/*
		Project:		KASSE
		Module:			
		Description:	
		Author:			Martin Gäckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Austria, Linz ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/

//---------------------------------------------------------------------------

#include <vcl.h>


#pragma hdrstop

#include "kasseDbFrm.h"
//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "dbConvertFrm"
#pragma resource "*.dfm"
TKasseDbConvertForm *KasseDbConvertForm;
//---------------------------------------------------------------------------
#pragma option -RT-
class ArtikelIdMap
{
	public:
	typedef int key_type;

	int		oldID, newID;
	STRING	barCode;

	int getKey( void ) const
	{
		return oldID;
	}
	void setKey( int oldID )
	{
		this->oldID = oldID;
	}
};

static Map<ArtikelIdMap>	artikelMapping;

class KassenDbSchema : public DatabaseSchema
{

	STRING	lastArticel;
	int		lastArtikelID;

	virtual TDataSet *openTable( const STRING &tableName );
	virtual bool checkFieldsBeforeInsert( TDataSet *sourceTable, const STRING &tableName );
	virtual void checkFieldsBeforePost( TDataSet *destTable, const STRING &tableName );
	virtual void closeTable( TDataSet *theTable, const STRING &tableName );
};
#pragma option -RT.

TDataSet *KassenDbSchema::openTable( const STRING &tableName )
{
	if( getDbVersion() >= 3 || tableName != "ARTIKEL" )
		return DatabaseSchema::openTable( tableName );

	TQuery	*theQuery;

	theQuery = new TQuery( NULL );
	theQuery->DatabaseName = getDatabase()->DatabaseName;
	theQuery->SQL->Add( "select * from ARTIKEL order by BEZEICHNUNG" );
	theQuery->Open();

	lastArticel = "";
	lastArtikelID = -1;
	artikelMapping.clear();

	return theQuery;
}

void KassenDbSchema::closeTable( TDataSet *theTable, const STRING &tableName )
{
	DatabaseSchema::closeTable( theTable, tableName );

	std::size_t	numMappings = artikelMapping.size();

	if( numMappings > 0
	&& getDbVersion() >= 3
	&& tableName == "ARTIKEL" )
	{
		theTable = openTable( "ARTIKEL_CODE" );
		TField	*idField = theTable->FieldByName( "ID" );
		TField	*artikelField = theTable->FieldByName( "ARTIKEL" );
		TField	*codeField = theTable->FieldByName( "CODE" );

		std::size_t i=1;
		for(
			Map<ArtikelIdMap>::const_iterator it = artikelMapping.cbegin(),
				endIT = artikelMapping.cend();
			it != endIT;
			++it, ++i
		)
		{
			const ArtikelIdMap	&theMapping = *it;
			if( !theMapping.barCode.isEmpty() )
			{
				theTable->Insert();
				idField->AsInteger = i;
				artikelField->AsInteger = theMapping.newID;
				codeField->AsString = static_cast<const char *>(
					theMapping.barCode)
				;
				theTable->Post();
			}
		}

		closeTable( theTable, "ARTIKEL_CODE" );
	}
}

bool KassenDbSchema::checkFieldsBeforeInsert(
	TDataSet *sourceTable, const STRING &tableName
)
{
	if( tableName == "VERKAUF" )
		return sourceTable->FieldByName( "GESAMTPREIS" )->AsFloat != 0.0;
  
	if( getDbVersion() >= 3 || tableName != "ARTIKEL" )
/***/	return true;

	bool ok;

	STRING nextArticel = sourceTable->FieldByName( "BEZEICHNUNG" )->AsString.c_str();
	STRING barCode = sourceTable->FieldByName( "CODE" )->AsString.c_str();
	int oldID = sourceTable->FieldByName( "ID" )->AsInteger;

	if( lastArticel == nextArticel )
		ok = false;
	else
	{
		lastArticel = nextArticel;
		lastArtikelID = oldID;
		ok = true;
	}

	ArtikelIdMap	&theMapping = artikelMapping[oldID];
	theMapping.newID = lastArtikelID;
	theMapping.barCode = barCode;

	return ok;
}

void KassenDbSchema::checkFieldsBeforePost( TDataSet *destTable, const STRING &tableName )
{
	if( artikelMapping.size()
	&& (tableName == "LIEFERUNG_ARTIKEL" || tableName == "VERKAUF_ARTIKEL") )
	{
		TField				*theField = destTable->FieldByName( "ARTIKEL" );
		int					oldID = theField->AsInteger;
		const ArtikelIdMap	&theMapping = artikelMapping[oldID];

		if( theMapping.newID != oldID )
			theField->AsInteger = theMapping.newID;
	}
}

//---------------------------------------------------------------------------
__fastcall TKasseDbConvertForm::TKasseDbConvertForm(TComponent* Owner)
	: TDbConvertForm(Owner)
{
}
//---------------------------------------------------------------------------
const char *TKasseDbConvertForm::getAliasName( void ) const
{
	return "KASSE";
}
//---------------------------------------------------------------------------
DatabaseSchema *TKasseDbConvertForm::createSchema( void ) const
{
	return new KassenDbSchema;
}


