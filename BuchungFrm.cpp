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

#include <gak/vcl_tools.h>

#pragma hdrstop

#include "BuchungFrm.h"
#include "MitarbeiterFrm.h"
#include "KasseMainFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBuchungenForm *BuchungenForm = NULL;
//---------------------------------------------------------------------------
__fastcall TBuchungenForm::TBuchungenForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBuchungenForm::FormActivate(TObject *)
{
	TableBuchung->Close();
	QueryMitarbeiter->Close();

	if( isDBopen() )
	{
		QueryMitarbeiter->Open();
		TableBuchung->Open();
		TableBuchung->Last();
	}

	WindowState = wsMaximized;
}
//---------------------------------------------------------------------------
void __fastcall TBuchungenForm::TableBuchungAfterInsert(TDataSet *)
{
	setNewMaxValue( TableBuchung, TableBuchungID );
	TableBuchungDATUM->AsDateTime = TDateTime::CurrentDateTime();
}
//---------------------------------------------------------------------------
void __fastcall TBuchungenForm::TableBuchungAfterPost(TDataSet *)
{
	TableBuchung->FlushBuffers();
	MitarbeiterForm->CloseQueries();
}
//---------------------------------------------------------------------------
void TBuchungenForm::createBooking( int sollKonto, int habenKonto, double betrag )
{
	TableBuchung->Open();
	TableBuchung->Insert();
	TableBuchungSOLL_KONTO->AsInteger = sollKonto;
	TableBuchungHABEN_KONTO->AsInteger = habenKonto;
	TableBuchungBETRAG->AsFloat = betrag;
	TableBuchung->Post();
}
//---------------------------------------------------------------------------

void __fastcall TBuchungenForm::TableBuchungBeforePost(TDataSet *)
{
	bool adminMode = KasseForm->isAdmin();
	if( !adminMode )
	{
		if( MitarbeiterForm->isProtected( TableBuchungSOLL_KONTO->AsInteger ) )
/*@*/		throw Exception( "Kein Zugriff auf das SOLL-Konto" );
		if( MitarbeiterForm->isProtected( TableBuchungHABEN_KONTO->AsInteger ) )
/*@*/		throw Exception( "Kein Zugriff auf das HABEN-Konto" );
	}
}
//---------------------------------------------------------------------------

void __fastcall TBuchungenForm::TableBuchungFilterRecord(TDataSet *, bool &Accept)
{
	Accept = !MitarbeiterForm->isProtected( TableBuchungSOLL_KONTO->AsInteger )
		  && !MitarbeiterForm->isProtected( TableBuchungHABEN_KONTO->AsInteger )
	;
}
//---------------------------------------------------------------------------

