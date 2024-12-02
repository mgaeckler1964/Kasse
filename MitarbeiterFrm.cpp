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

#include <stdio.h>

#include <io.h>
#include <vcl.h>
#include <vcl/printers.hpp>

#include <gak/vcl_tools.h>

#include <winlib/barcode.h>

#pragma hdrstop

#include "KasseMainFrm.h"
#include "MitarbeiterFrm.h"
#include "Config.h"
//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMitarbeiterForm *MitarbeiterForm;
//---------------------------------------------------------------------------
static	PairMap<int,bool,PODallocator<KeyValuePair<int, bool> > >	protAccountCache;
//---------------------------------------------------------------------------
__fastcall TMitarbeiterForm::TMitarbeiterForm(TComponent* Owner)
	: TForm(Owner)
{
	kassenID = inventurID = 0;
}
//---------------------------------------------------------------------------
bool TMitarbeiterForm::isProtected( int mitarbeiterID )
{
	bool isProtected;
	bool adminMode = KasseForm->isAdmin();
	if( !adminMode )
	{
		if( !protAccountCache.hasElement( mitarbeiterID ) )
		{
			double		dummy;
			CI_STRING	osName;
			bool found = FindMitarbeiter( mitarbeiterID, &dummy, &dummy, &osName );

			isProtected = found && !osName.isEmpty() && osName != KasseForm->getActUser();

			protAccountCache[mitarbeiterID] = isProtected;
		}
		else
			isProtected = protAccountCache[mitarbeiterID];
	}
	else
		isProtected = false;

	return isProtected;
}
//---------------------------------------------------------------------------
bool TMitarbeiterForm::FindMitarbeiter(
	int mitarbeiterID,
	double *maxKredit, double *endKontenstand, CI_STRING *osName
)
{
	if( !TableMitarbeiter->Active )
		TableMitarbeiter->Open();

	bool found = TableMitarbeiterID->AsInteger == mitarbeiterID;

	if( !found )
	{
		TableMitarbeiter->DisableControls();
		for(
			TableMitarbeiter->First();
			!TableMitarbeiter->Eof;
			TableMitarbeiter->Next()
		)
		{
			if( TableMitarbeiterID->AsInteger == mitarbeiterID )
			{
				found = true;
				break;
			}
		}

		TableMitarbeiter->EnableControls();
	}

	if( found )
	{
		TableMitarbeiter->Refresh();
		*maxKredit = TableMitarbeiterMAX_KREDIT->IsNull
			? -1
			: TableMitarbeiterMAX_KREDIT->AsFloat
		;
		*endKontenstand = TableMitarbeiterEND_KONTOSTAND->IsNull
			? calcAccountValue()
			: TableMitarbeiterEND_KONTOSTAND->AsFloat
		;
		*osName = TableMitarbeiterUSERNAME->AsString.c_str();
	}
	return found;
}
//---------------------------------------------------------------------------
int TMitarbeiterForm::FindMitarbeiter( const AnsiString &code, double *maxKredit, double *endKontenstand )
{
	int mitarbeiterID = -1;

	if( !TableMitarbeiter->Active || TableMitarbeiterCODE->AsString != code )
	{
		TableMitarbeiter->DisableControls();

		if( TableMitarbeiter->IndexName != "MA_CODE_IDX"
		||  !TableMitarbeiter->Active )
		{
			TableMitarbeiter->Close();
			TableMitarbeiter->IndexName = "MA_CODE_IDX";
			TableMitarbeiter->Open();
		}
		TableMitarbeiter->SetKey();
		TableMitarbeiter->FieldByName("CODE")->AsString = code;
		TableMitarbeiter->GotoNearest();

		TableMitarbeiter->EnableControls();
	}
	if(TableMitarbeiterCODE->AsString == code )
	{
		TableMitarbeiter->Refresh();
		mitarbeiterID = TableMitarbeiterID->AsInteger;
		*maxKredit = TableMitarbeiterMAX_KREDIT->IsNull
			? -1
			: TableMitarbeiterMAX_KREDIT->AsFloat
		;
		*endKontenstand = TableMitarbeiterEND_KONTOSTAND->IsNull
			? calcAccountValue()
			: TableMitarbeiterEND_KONTOSTAND->AsFloat
		;
	}

	return mitarbeiterID;
}

int TMitarbeiterForm::FindCreateAccount( const AnsiString &code, const AnsiString &name, double *endKontenstand )
{
	double	dummy;
	int accountID = FindMitarbeiter( code, &dummy, endKontenstand );
	if( accountID <= 0 )
	{
		TableMitarbeiter->Open();
		TableMitarbeiter->Insert();
		TableMitarbeiterCODE->AsString = code;
		TableMitarbeiterNAME->AsString = name;
		try
		{
			TableMitarbeiter->Post();
		}
		catch( ... )
		{
			TableMitarbeiter->Cancel();
			TableMitarbeiter->Close();
/*@*/		throw Exception( AnsiString("Kann Konto ") + name + " nicht ermitteln" );
		}

		accountID = TableMitarbeiterID->AsInteger;
		*endKontenstand = 0;
	}

	return accountID;
}

//---------------------------------------------------------------------------
double TMitarbeiterForm::calcAccountValue()
{
	int		mitarbeiterId = TableMitarbeiterID->AsInteger;
	double	kontostand = TableMitarbeiterKONTOSTAND->AsFloat;

	if( !QuerySoll->Active )
		QuerySoll->Open();

	for( QuerySoll->First(); !QuerySoll->Eof; QuerySoll->Next() )
	{
		if( QuerySollSOLL_KONTO->AsInteger == mitarbeiterId )
		{
			kontostand -= QuerySollBETRAG->AsFloat;
			break;
		}
	}

	if( !QueryHaben->Active )
		QueryHaben->Open();

	for( QueryHaben->First(); !QueryHaben->Eof; QueryHaben->Next() )
	{
		if( QueryHabenHABEN_KONTO->AsInteger == mitarbeiterId )
		{
			kontostand += QueryHabenBETRAG->AsFloat;
			break;
		}
	}

	if( !QueryVerkauf->Active )
		QueryVerkauf->Open();

	for( QueryVerkauf->First(); !QueryVerkauf->Eof; QueryVerkauf->Next() )
	{
		if( QueryVerkaufMITARBEITER->AsInteger == mitarbeiterId )
		{
			kontostand += QueryVerkaufGESAMTPREIS->AsFloat;
			break;
		}
	}

	return kontostand;
}

//---------------------------------------------------------------------------
void __fastcall TMitarbeiterForm::TableMitarbeiterCalcFields(
	  TDataSet *)
{
	bool 		adminMode = KasseForm->isAdmin();
	CI_STRING	osName = TableMitarbeiterUSERNAME->AsString.c_str();
	bool		isProtected = !adminMode && !osName.isEmpty() && osName != KasseForm->getActUser();
	if( !isProtected )
		TableMitarbeiterEND_KONTOSTAND->AsFloat = calcAccountValue();
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::FormActivate(TObject *)
{
	doEnterFunction("TMitarbeiterForm::FormActivate");
	QuerySoll->Close();
	QueryHaben->Close();
	QueryVerkauf->Close();

	TableMitarbeiter->Close();
	TableMitarbeiter->IndexName = "MA_NAME_IDX";

	if( isDBopen() )
		TableMitarbeiter->Open();

	WindowState = wsMaximized;
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::TableMitarbeiterAfterScroll(
	  TDataSet *)
{
	doEnterFunction("TMitarbeiterForm::TableMitarbeiterAfterScroll");
	strcpy( barCode, TableMitarbeiterCODE->AsString.c_str() );
	if( testEAN8Code( barCode ) )
		ButtonCreateEAN8->Enabled = false;
	else
	{
		ButtonCreateEAN8->Enabled = true;
		barCode[0] = 0;
	}

	PaintBoxBarCode->Invalidate();
}

//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::PaintBoxBarCodePaint(TObject *)
{
	if( barCode[0] )
	{
		HDC hDC = PaintBoxBarCode->Canvas->Handle;
		printEAN8Code( hDC, barCode, 0, 0,PaintBoxBarCode->Width,PaintBoxBarCode->Height );
	}
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::ButtonCreateEAN8Click(TObject *)
{
	if( TableMitarbeiter->State == dsInsert )
	{
		TableMitarbeiterCODE->AsString = "Dummkopf";
		TableMitarbeiter->Post();
	}
	barCode[0] = '2';
	sprintf( barCode+1, "%06d", TableMitarbeiterID->AsInteger );
	createEAN8Code( barCode );
	TableMitarbeiter->Edit();
	TableMitarbeiterCODE->AsString = barCode;
	TableMitarbeiter->Post();
	ButtonCreateEAN8->Enabled = false;
	PaintBoxBarCode->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::ButtonPrintClick(TObject *)
{
	if( PrintDialog->Execute() )
	{
		STRING organisation = ConfigDataModule->GetValue(
			"Organisation", ""
		);

		TPrinter *thePrinter = Printer();

		thePrinter->BeginDoc();
		TCanvas	*theCanvas = thePrinter->Canvas;
		HDC hDC = theCanvas->Handle;

		int widthMM = GetDeviceCaps( hDC, HORZSIZE );
		int heightMM = GetDeviceCaps( hDC, VERTSIZE );
		int widthPX = GetDeviceCaps( hDC, HORZRES );
		int heightPX = GetDeviceCaps( hDC, VERTRES );

		int x = GetDeviceCaps( hDC, PHYSICALOFFSETX	);
		int y = GetDeviceCaps( hDC, PHYSICALOFFSETY	);

		int widthRec = (double)widthPX * 70 / (double)widthMM;
		int heightRec = (double)heightPX * 40 / (double)heightMM;

		int widthBarCode = (double)widthPX * 60.0 / (double)widthMM;
		int heightBarCode = (double)heightPX * 15.0 / (double)heightMM;

		theCanvas->Font->Height = heightBarCode/4;
		theCanvas->Pen->Color = clBlack;
		theCanvas->Pen->Width = 1;
		theCanvas->Pen->Mode = pmCopy;
		theCanvas->Pen->Style = psSolid;
		theCanvas->Brush->Color = clWhite;
		theCanvas->Brush->Style = bsSolid;

		theCanvas->Rectangle( x, y, x+widthRec, y+heightRec );
		TSize size = theCanvas->TextExtent( "W" );
		theCanvas->TextOut(
			x+size.cy, y+size.cy, (const char *)organisation
		);
		theCanvas->TextOut( x+size.cy, y+2*size.cy, "Mitarbeiter-/Kundenausweis" );
		theCanvas->TextOut(
			x+size.cy, y+4*size.cy, TableMitarbeiterNAME->AsString
		);

		printEAN8Code(
			hDC, barCode, x+size.cy, y+6*size.cy, widthBarCode, heightBarCode
		);

		thePrinter->EndDoc();
	}
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::TableMitarbeiterAfterPost(
	  TDataSet *)
{
	TableMitarbeiter->FlushBuffers();
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::TableMitarbeiterAfterInsert(
	  TDataSet *)
{
	setNewMaxValue( TableMitarbeiter, TableMitarbeiterID );
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::FormShow(TObject *)
{
	doEnterFunction("TMitarbeiterForm::FormShow");

	TableMitarbeiter->ReadOnly = !KasseForm->isAdmin();
	TableMitarbeiterKONTOSTAND->Visible = KasseForm->isAdmin();
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::TableMitarbeiterBeforeDelete(
      TDataSet *)
{
	if( TableMitarbeiterEND_KONTOSTAND->AsFloat )
		throw Exception( "Konto nicht ausgeglichen!" );
}
//---------------------------------------------------------------------------

void __fastcall TMitarbeiterForm::TableMitarbeiterBeforeOpen(TDataSet *)
{
	protAccountCache.clear();	
}
//---------------------------------------------------------------------------

