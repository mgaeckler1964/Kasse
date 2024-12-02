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
#include <fstream.h>

#include <vcl.h>
#include <vcl/printers.hpp>
#include <gak/csv.h>

#include <gak/vcl_tools.h>
#include <winlib/barcode.h>

#pragma hdrstop

#include "ArtikelFrm.h"
#include "LieferungFrm.h"
#include "KasseMainFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TArtikelForm *ArtikelForm;
//---------------------------------------------------------------------------
__fastcall TArtikelForm::TArtikelForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TArtikelForm::artikelExport( void )
{
	if( SaveDialog->Execute() )
	{
		if( !TableArtikel->Active )
			TableArtikel->Open();

		exportTable2CSV( TableArtikel, SaveDialog->FileName.c_str() );
	}
}
//---------------------------------------------------------------------------
void TArtikelForm::artikelImport( void )
{
	if( OpenDialog->Execute() )
	{
		ifstream fp( OpenDialog->FileName.c_str() );
		if( fp.is_open() )
		{
			FieldSet	  	record;
			ArrayOfStrings	fieldNames;
			STRING		  	idSTR, verkaufsPreisSTR, partyPreisSTR;
			int			  	id;
			double		  	verkaufsPreis, partyPreis;

			TableArtikelCode->Close();
			TableArtikel->Close();

			readCSVLine( fp, &fieldNames );

			while( !fp.eof() && !fp.fail() )
			{
				readFIELD_SET( fp, fieldNames, &record );

				idSTR = (STRING)record["ID"];
				verkaufsPreisSTR = (STRING)record["verkaufsPreis"];
				partyPreisSTR = (STRING)record["partyPreis"];

				id = atoi( idSTR );
				verkaufsPreis = atof( verkaufsPreisSTR );
				partyPreis = atof( partyPreisSTR );

				if( id && verkaufsPreis )
				{
					QueryArtikelImport->Params->Items[0]->AsFloat = verkaufsPreis;
					QueryArtikelImport->Params->Items[1]->AsFloat = partyPreis;
					QueryArtikelImport->Params->Items[2]->AsInteger = id;

					QueryArtikelImport->ExecSQL();
				}
			}

			KasseForm->restartDatabase();
			Show();
			FormActivate( this );
		}
	}
}
//---------------------------------------------------------------------------
int TArtikelForm::FindArtikel( const AnsiString &code )
{
	int artikelID = -1;

	QueryArtikelCode->Params->Items[0]->AsString = code;
	QueryArtikelCode->Open();
	if( !QueryArtikelCode->Eof )
		artikelID = QueryArtikelCodeARTIKEL->AsInteger;
	QueryArtikelCode->Close();

	return artikelID;
}

//---------------------------------------------------------------------------
bool TArtikelForm::locateArtikel( int artikelID )
{
	bool	found;
	TableArtikel->DisableControls();
	if(TableArtikelID->AsInteger != artikelID )
	{
		if( TableArtikel->IndexName != "" || !TableArtikel->Active )
		{
			TableArtikelCode->Close();
			TableArtikel->Close();
			TableArtikel->IndexName = "";
			TableArtikel->Open();
		}
		TableArtikel->SetKey();
		TableArtikel->FieldByName("ID")->AsInteger = artikelID;
		TableArtikel->GotoNearest();
	}
	found = (TableArtikelID->AsInteger == artikelID);
	TableArtikel->EnableControls();

	return found;
}

//---------------------------------------------------------------------------
double TArtikelForm::FindArtikelPrice( int artikelID, bool partyPreis )
{
	double	price = 0;

	if( locateArtikel( artikelID ) )
		price = partyPreis && TableArtikelPARTYPREIS->AsFloat ? TableArtikelPARTYPREIS->AsFloat : TableArtikelVERKAUFSPREIS->AsFloat;

	return price;
}
//---------------------------------------------------------------------------
int TArtikelForm::FindArtikelUnit( int artikelID )
{
	int	unit = 0;

	if( locateArtikel( artikelID ) )
		unit = TableArtikelEINHEIT->AsInteger;

	if( !unit )
		unit = 1;
	return unit;
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::FormActivate(TObject *)
{
	QueryVerkauf->Close();
	QueryLieferung->Close();

	TableArtikelCode->Close();
	TableArtikel->Close();
	TableArtikel->IndexName = "ARTIKEL_NAME_IDX";

	if( isDBopen() )
	{
		TableArtikel->Open();
		TableArtikelCode->Open();
	}

	WindowState = wsMaximized;
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::TableArtikelCalcFields(TDataSet *)
{
	TDateTime	firstTime = 0;

	int		verkaufsMenge = 0;
	double	numDays = 0;
	int		sollBestand = TableArtikelBESTAND->AsInteger;
	int		artikelId = TableArtikelID->AsInteger;

	if( !QueryVerkauf->Active )
		QueryVerkauf->Open();

	for( QueryVerkauf->First(); !QueryVerkauf->Eof; QueryVerkauf->Next() )
	{
		if( QueryVerkaufARTIKEL->AsInteger == artikelId )
		{
			firstTime = QueryVerkaufMIN_DATUM->AsDateTime;
			numDays = TDateTime::CurrentDateTime() - firstTime;
			verkaufsMenge = QueryVerkaufMENGE->AsInteger;
			sollBestand -= verkaufsMenge;
			break;
		}
	}
	if( !QueryLieferung->Active )
		QueryLieferung->Open();

	for( QueryLieferung->First(); !QueryLieferung->Eof; QueryLieferung->Next() )
	{
		if( QueryLieferungARTIKEL->AsInteger == artikelId )
		{
			sollBestand += QueryLieferungMENGE->AsInteger;
			break;
		}
	}
	TableArtikelSOLL_BESTAND->AsInteger = sollBestand;

	TableArtikelVERKAUF->AsFloat = numDays > 1
		? (double)verkaufsMenge / numDays
		: 0
	;
	TableArtikelRESTZEIT->AsFloat = TableArtikelVERKAUF->AsFloat
		? (double)TableArtikelSOLL_BESTAND->AsInteger / TableArtikelVERKAUF->AsFloat
		: 0
	;

	if( (double)firstTime )
		TableArtikelSTART->AsDateTime = firstTime;
	else
		TableArtikelSTART->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::PaintBoxBarCodePaint(TObject *)
{
	HDC hDC = PaintBoxBarCode->Canvas->Handle;

	if( TableArtikelCodeCODE->AsString.Length() == 13 )
		printEAN13Code(
			hDC,
			TableArtikelCodeCODE->AsString.c_str(),
			0, 0,PaintBoxBarCode->Width,PaintBoxBarCode->Height
		);
	else if( TableArtikelCodeCODE->AsString.Length() == 8 )
		printEAN8Code(
			hDC,
			TableArtikelCodeCODE->AsString.c_str(),
			0, 0,PaintBoxBarCode->Width,PaintBoxBarCode->Height
		);
}
//---------------------------------------------------------------------------


void __fastcall TArtikelForm::ButtonCreateEAN13Click(TObject *)
{
	char	barCode[14];

	if( TableArtikel->State == dsInsert )
		TableArtikel->Post();

	sprintf( barCode, "2666%08d", TableArtikelID->AsInteger );
	createEAN13Code( barCode );
	TableArtikelCode->Insert();
	TableArtikelCodeCODE->AsString = barCode;
	TableArtikelCode->Post();
	ButtonCreateEAN13->Enabled = false;
	PaintBoxBarCode->Invalidate();
}
//---------------------------------------------------------------------------

void TArtikelForm::printRecord(
	TCanvas *theCanvas,
	int x, int y,
	int widthRec, int heightRec,
	int widthBarCode, int heightBarCode
)
{
	HDC hDC = theCanvas->Handle;

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
		x+size.cy, y+size.cy, "Kultur-Projekt-München - Etikett"
	);
	theCanvas->TextOut( x+size.cy, y+2*size.cy, "Artikel" );
	theCanvas->TextOut(
		x+size.cy, y+4*size.cy, TableArtikelBEZEICHNUNG->AsString
	);

	if( TableArtikelCodeCODE->AsString.Length() == 13 )
		printEAN13Code(
			hDC,
			TableArtikelCodeCODE->AsString.c_str(),
			x+size.cy, y+6*size.cy, widthBarCode, heightBarCode
		);
	else if( TableArtikelCodeCODE->AsString.Length() == 8 )
		printEAN8Code(
			hDC,
			TableArtikelCodeCODE->AsString.c_str(),
			x+size.cy, y+6*size.cy, widthBarCode, heightBarCode
		);

	theCanvas->Font->Height = heightBarCode;
	theCanvas->TextOut(
		x+widthRec+size.cy, y+heightRec-heightBarCode,
		TableArtikelBEZEICHNUNG->AsString
	);
}

void __fastcall TArtikelForm::ButtonPrintClick( TObject *Sender )
{
	if( PrintDialog->Execute() )
	{
		TPrinter *thePrinter = Printer();

		thePrinter->BeginDoc();
		TCanvas	*theCanvas = thePrinter->Canvas;
		HDC hDC = theCanvas->Handle;

		int widthMM = GetDeviceCaps( hDC, HORZSIZE );
		int heightMM = GetDeviceCaps( hDC, VERTSIZE );
		int widthPX = GetDeviceCaps( hDC, HORZRES );
		int heightPX = GetDeviceCaps( hDC, VERTRES );

		int offsetX = GetDeviceCaps( hDC, PHYSICALOFFSETX	);
		int offsetY = GetDeviceCaps( hDC, PHYSICALOFFSETY	);

		int x = offsetX;
		int y = offsetY;

		int widthRec = (double)widthPX * 70 / (double)widthMM;
		int heightRec = (double)heightPX * 40 / (double)heightMM;

		int widthBarCode = (double)widthPX * 60.0 / (double)widthMM;
		int heightBarCode = (double)heightPX * 15.0 / (double)heightMM;
		int margin = (double)heightPX * 5.0 / (double)heightMM;

		if( Sender == ButtonPrint || Sender == MenuPrintAktuelles )
		{
			printRecord(
				theCanvas,
				x, y,
				widthRec, heightRec,
				widthBarCode, heightBarCode
			);
		}
		else
		{
			AnsiString currentGroup = TableArtikelGRUPPEN->AsString;
			for( TableArtikel->First(); !TableArtikel->Eof; TableArtikel->Next() )
			{
				bool doPrint = false;

				if( Sender == MenuPrintPrivate
				&& !strncmp( TableArtikelCodeCODE->AsString.c_str(), "2666", 4 ) )
					doPrint = true;
				else if( Sender == MenuPrintBestand
				&& TableArtikelBESTAND->AsInteger > 0 )
					doPrint = true;
				else if( Sender == MenuPrintBestandUndPrivat
				&& !strncmp( TableArtikelCodeCODE->AsString.c_str(), "2666", 4 )
				&& TableArtikelBESTAND->AsInteger > 0 )
					doPrint = true;
				else if( Sender == MenuAktuelleGruppe
				&& currentGroup == TableArtikelGRUPPEN->AsString )
					doPrint = true;

				if( doPrint )
				{
					int nextY = y + heightRec;

					if( nextY > heightPX )
					{
						thePrinter->NewPage();
						y = offsetY;
						nextY = heightRec;
					}
					printRecord(
						theCanvas,
						x, y,
						widthRec, heightRec,
						widthBarCode, heightBarCode
					);

					y = nextY + margin;
				}
			}
		}

		thePrinter->EndDoc();
	}
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::TableArtikelAfterPost(TDataSet *)
{
	TableArtikel->FlushBuffers();
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::TableArtikelAfterInsert(TDataSet *)
{
	setNewMaxValue( TableArtikel, TableArtikelID );
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::TableArtikelCodeAfterInsert(
	  TDataSet *)
{
	setNewMaxValue( TableArtikelCode, TableArtikelCodeID );
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::TableArtikelCodeAfterPost(TDataSet *)
{
	TableArtikelCode->FlushBuffers();
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::TableArtikelCodeAfterScroll(
	  TDataSet *)
{
	if( testEAN8Code( TableArtikelCodeCODE->AsString.c_str() )
	||  testEAN13Code( TableArtikelCodeCODE->AsString.c_str() ) )
		ButtonCreateEAN13->Enabled = false;
	else
		ButtonCreateEAN13->Enabled = true;
	PaintBoxBarCode->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TArtikelForm::TableArtikelCodeBeforeInsert(
	  TDataSet *)
{
	if( TableArtikel->State == dsInsert )
		TableArtikel->Post();	
}
//---------------------------------------------------------------------------



void __fastcall TArtikelForm::FormResize(TObject *)
{
	DBNavigatorArtikelCode->Left = DBGridBarCodes->Left;
}
//---------------------------------------------------------------------------

void TArtikelForm::Inventur( void )
{
	TableArtikel->Open();
	for(
		TableArtikel->First();
		!TableArtikel->Eof;
		TableArtikel->Next()
	)
	{
		if( !TableArtikelIST_BESTAND->IsNull )
		{
			int	sollBestand = TableArtikelSOLL_BESTAND->AsInteger;
			int	istBestand = TableArtikelIST_BESTAND->AsInteger;
			if( sollBestand != istBestand )
			{
				LieferungForm->AddInventur( TableArtikelID->AsInteger, istBestand - sollBestand  );
			}

			TableArtikel->Edit();
			TableArtikelIST_BESTAND->Clear();
			TableArtikel->Post();
		}

	}
}

//---------------------------------------------------------------------------


void __fastcall TArtikelForm::TableArtikelBeforeDelete(TDataSet *)
{
	if( TableArtikelSOLL_BESTAND->AsInteger )
		throw Exception( "Es gibt noch einen Warenbestand" );

}
//---------------------------------------------------------------------------

