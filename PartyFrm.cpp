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
#include <memory>

#include <fstream>

#include <vcl.h>

#include <gak/vcl_tools.h>
#include <gak/directory.h>
#include <gak/fcopy.h> 

#pragma hdrstop

#include "PartyFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPartyForm *PartyForm;
//---------------------------------------------------------------------------
__fastcall TPartyForm::TPartyForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPartyForm::ButtonQueryStartClick(TObject *)
{
	QueryParty->Close();
	TDateTime startDate( EditFromDate->Text );
	TDateTime endDate( EditToDate->Text );
	QueryParty->ParamByName( "startDatum" )->AsDateTime = startDate;
	QueryParty->ParamByName( "endDatum" )->AsDateTime = endDate;
	QueryParty->Open();
}
//---------------------------------------------------------------------------
void __fastcall TPartyForm::QueryPartyAfterOpen(TDataSet *)
{
	char	buffer[1024];

	sumPartyPreis = sumVerkaufspreis = 0;

	for( QueryParty->First(); !QueryParty->Eof; QueryParty->Next() )
	{
		sumPartyPreis += QueryPartyPARTYPREIS->AsFloat;
		sumVerkaufspreis += QueryPartyVERKAUFSPREIS->AsFloat;
	}
	differenz = sumPartyPreis - sumVerkaufspreis;

	sprintf(
		buffer,
		"Umsatz: %.2f €, Betreiber: %.2f €, Veranstalter: %.2f €",
		sumPartyPreis, sumVerkaufspreis, differenz
	);

	LabelResult->Caption = buffer;
}
//---------------------------------------------------------------------------

void __fastcall TPartyForm::FormClose(TObject *, TCloseAction &)
{
	QueryParty->Close();
}
//---------------------------------------------------------------------------

void __fastcall TPartyForm::ButtonExportClick(TObject *Sender)
{
	if( !QueryParty->Active )
		ButtonQueryStartClick( Sender );

	STRING	xmlName = getenv( "TEMP" );
	STRING	xslName = xmlName;
	xmlName += DIRECTORY_DELIMITER_STRING "partyKasse.xml";
	xslName += DIRECTORY_DELIMITER_STRING "party.xsl";

	std::auto_ptr<xml::Any>	theResult(
		exportTable2XML( QueryParty, "PARTY", "ARTIKEL", 2 )
	);

	theResult->setFloatAttribute(
		"Umsatz",
		sumPartyPreis, 2
	);
	theResult->setFloatAttribute(
		"Betreiber",
		sumVerkaufspreis, 2
	);
	theResult->setFloatAttribute(
		"Veranstalter",
		differenz, 2
	);

	fcopy( STRING("party.xsl"), xslName );
	STRING	xmlResult = theResult->generateDoc( "", "party.xsl" );

	std::ofstream	xmlFile( xmlName );
	xmlFile << xmlResult;
	xmlFile.close();

	ShellExecute( NULL, NULL, xmlName, NULL, NULL, SW_SHOWNORMAL );
}
//---------------------------------------------------------------------------


