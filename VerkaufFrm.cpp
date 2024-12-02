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

#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>

#include <vcl.h>

#include <gak/vcl_tools.h>
#include <gak/directory.h>
#include <gak/csv.h>
#include <gak/strFiles.h>
#include <gak/numericString.h>

#pragma hdrstop

#include "KasseMainFrm.h"

#include "VerkaufFrm.h"
#include "ArtikelFrm.h"
#include "MitarbeiterFrm.h"
#include "CompleteFrm.h"
#include "WebServer.h"
#include "Config.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TVerkaufForm *VerkaufForm = NULL;

//---------------------------------------------------------------------------
__fastcall TVerkaufForm::TVerkaufForm(TComponent* Owner)
	: TForm(Owner)
{
	startDate = endDate = TDateTime::CurrentDateTime();
}
//---------------------------------------------------------------------------
void TVerkaufForm::FindClientOrders( void )
{
	bool	partyMode = CheckBoxParty->Checked;
	STRING	tempPath = getenv( "TEMP" );
	tempPath += DIRECTORY_DELIMITER_STRING "order*.csv";

	DirectoryList	theOrders;
	theOrders.findFiles( tempPath );

	STRING		  	orderFile, eanCode, countStr;
	int			  	count;
	ArrayOfStrings	fields;

	if( theOrders.size() )
	{
		CheckBoxParty->Checked = true;
	}

	for(
		DirectoryList::iterator it = theOrders.begin(), endIT = theOrders.end();
		it != endIT;
		++it
	)
	{
		count = 0;
		orderFile = getenv( "TEMP" );
		orderFile += DIRECTORY_DELIMITER;
		orderFile += it->fileName;
		ifstream csv( orderFile );
		while( !csv.eof() )
		{
			readCSVLine( csv, &fields );
			eanCode = fields[0];
			countStr = fields[1];
			if( !eanCode.isEmpty() && !countStr.isEmpty() )
			{
				count = countStr.getValueN<unsigned>();
				if( count )
				{
					char cr = '\n';

					EditNumbers->Text = (const char *)eanCode;
					EditNumbersKeyPress( EditNumbers, cr );
					EditNumbers->Text = (const char *)countStr;
					EditNumbersKeyPress( EditNumbers, cr );
				}
			}
		}
		csv.close();
		remove( orderFile );

		if( count )
		{
			TableVerkauf->Edit();
			TableVerkaufFERTIG->AsBoolean = true;
			TableVerkauf->Post();
		}
	}

	if( theOrders.size() && !partyMode )
	{
		CheckBoxParty->Checked = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufAfterInsert(
	  TDataSet *)
{
	try
	{
		setNewMaxValue( TableVerkauf, TableVerkaufID );
		TableVerkaufDATUM->AsDateTime = TDateTime::CurrentDateTime();
		TableVerkaufGESAMTPREIS->AsFloat = 0;
		TableVerkaufMITARBEITER->AsInteger = MitarbeiterForm->FindKasse();
		TableVerkaufPARTYMODE->AsBoolean = CheckBoxParty->Checked;

		TableVerkauf->Post();
		TableVerkauf->Edit();

		maxKredit = -1;
	}
	catch( ... )
	{
		TableVerkauf->Cancel();
/*@*/	throw;
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::EditNumbersSetFocus(TDataSet *)
{
	TableVerkauf->FlushBuffers();
	TableVerkaufArtikel->FlushBuffers();

	GotoNumbers();
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::EditNumbersKeyPress(TObject *,
	  char &Key)
{
	if( (Key == '\r' || Key == '\n')
	&& QueryArtikel->Active
	&& EditNumbers->Text.Length() )
	{
		try
		{
			/*
				check for count first
			*/
			int menge = atoi( EditNumbers->Text.c_str() );
			if( menge > 0 && menge < 1000 )
			{
				if( TableVerkaufArtikel->State != dsEdit && TableVerkaufArtikel->State != dsInsert )
					TableVerkaufArtikel->Edit();
				TableVerkaufArtikelMENGE->AsInteger = menge;
			}
			else
			{
				/*
					check for artikel
				*/

				int artikelID = ArtikelForm->FindArtikel( EditNumbers->Text );
				if( artikelID > 0 )
				{
					if( isCompleted() )
						TableVerkauf->Insert();

					if( (TableVerkaufArtikel->Eof && TableVerkaufArtikel->Bof)
					|| TableVerkaufArtikelARTIKEL->AsInteger != artikelID )
						TableVerkaufArtikel->Insert();
					else if( TableVerkaufArtikel->State != dsEdit
					&& TableVerkaufArtikel->State != dsInsert )
						TableVerkaufArtikel->Edit();

					if( TableVerkaufArtikelARTIKEL->AsInteger == artikelID )
						TableVerkaufArtikelMENGE->AsInteger = TableVerkaufArtikelMENGE->AsInteger + 1;
					else
						TableVerkaufArtikelARTIKEL->AsInteger = artikelID;
				}
				else
				{
					/*
						An employee was found
					*/
					double lMaxKredit, lEndKontenstand;
					int mitarbeiterID = MitarbeiterForm->FindMitarbeiter(
						EditNumbers->Text,
						&lMaxKredit, &lEndKontenstand
					);
					if( mitarbeiterID > 0 )
					{
						double	thePrice = 0;
						if(CheckBoxKonto->Checked && !isCompleted() )
						{
							bool oldPartyMode = TableVerkaufPARTYMODE->AsBoolean;
							thePrice = TableVerkaufGESAMTPREIS->AsFloat;

							if( oldPartyMode )
							{
								thePrice = 0;
								for(
									TableVerkaufArtikel->First();
									!TableVerkaufArtikel->Eof;
									TableVerkaufArtikel->Next()
								)
								{
									thePrice += ArtikelForm->FindArtikelPrice(
										TableVerkaufArtikelARTIKEL->AsInteger,
										false
									) * TableVerkaufArtikelMENGE->AsInteger;
								}
							}
						}


						if( lMaxKredit >= 0	&& lEndKontenstand+thePrice > lMaxKredit )
							throw Exception( "Kreditrahmen überzogen" );

						if( TableVerkauf->State != dsEdit )
						{
							if( isCompleted() || !CheckBoxKonto->Checked )
							{
								TableVerkauf->Insert();
								thePrice = 0;
							}
							else
								TableVerkauf->Edit();
						}

						TableVerkaufMITARBEITER->AsInteger = mitarbeiterID;
						this->maxKredit = lMaxKredit;
						this->endKontenstand = lEndKontenstand;
						if( EditNumbers->Text.Length() )
							TableVerkaufPARTYMODE->AsBoolean = false;
						TableVerkaufGESAMTPREIS->AsFloat = thePrice;

						TableVerkauf->Post();
					}
					else
					{
						Application->MessageBox(
							"Artikel/Mitarbeiter nicht gefunden", "Fehler",
							MB_ICONERROR
						);
					}
				}
			}
			if( TableVerkaufArtikel->State == dsEdit || TableVerkaufArtikel->State == dsInsert )
				TableVerkaufArtikel->Post();
		}
		catch( ... )
		{
			EditNumbers->Text = "";
			CheckBoxKonto->Checked = false;
			throw;
		}

		EditNumbers->Text = "";
		CheckBoxKonto->Checked = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufArtikelAfterInsert(
	  TDataSet *)
{
	setNewMaxValue( TableVerkaufArtikel, TableVerkaufArtikelID );
	TableVerkaufArtikelMENGE->AsInteger = 1;
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufArtikelBeforePost(
	  TDataSet *)
{
	if( maxKredit >= 0 )
	{
		double	newPrice = TableVerkaufGESAMTPREIS->AsFloat;
		double	thePrice = ArtikelForm->FindArtikelPrice(
			TableVerkaufArtikelARTIKEL->AsInteger, TableVerkaufPARTYMODE->AsBoolean
		);
		int		oldCount = 0;

		if( lastArtikelID == TableVerkaufArtikelARTIKEL->AsInteger )
			oldCount = lastArtikelCount;

		newPrice += thePrice * (TableVerkaufArtikelMENGE->AsInteger - oldCount);

		if( endKontenstand + newPrice > maxKredit )
		{
			throw Exception( "Kreditrahmen überzogen" );
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufArtikelAfterPost(
	  TDataSet *)
{
	double	newPrice = TableVerkaufGESAMTPREIS->AsFloat;
	double	thePrice = ArtikelForm->FindArtikelPrice(
		TableVerkaufArtikelARTIKEL->AsInteger, TableVerkaufPARTYMODE->AsBoolean
	);
	int		oldCount = 0;

	if( lastArtikelID == TableVerkaufArtikelARTIKEL->AsInteger )
		oldCount = lastArtikelCount;

	newPrice += thePrice * (TableVerkaufArtikelMENGE->AsInteger - oldCount);
	if( TableVerkauf->State != dsInsert && TableVerkauf->State != dsEdit )
		TableVerkauf->Edit();
	TableVerkaufGESAMTPREIS->AsFloat = newPrice;
	TableVerkauf->Post();

	lastArtikelID = TableVerkaufArtikelARTIKEL->AsInteger;
	lastArtikelCount = TableVerkaufArtikelMENGE->AsInteger;

	TableVerkauf->FlushBuffers();
	TableVerkaufArtikel->FlushBuffers();
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufArtikelBeforeDelete(
	  TDataSet *DataSet )
{
	ProtectCompleted( DataSet );

	double	newPrice = TableVerkaufGESAMTPREIS->AsFloat;
	double	thePrice = ArtikelForm->FindArtikelPrice(
		TableVerkaufArtikelARTIKEL->AsInteger, TableVerkaufPARTYMODE->AsBoolean
	);

	newPrice -= thePrice * TableVerkaufArtikelMENGE->AsInteger;
	if( TableVerkauf->State != dsInsert && TableVerkauf->State != dsEdit )
		TableVerkauf->Edit();
	TableVerkaufGESAMTPREIS->AsFloat = newPrice;
	TableVerkauf->Post();
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufArtikelAfterScroll(
	  TDataSet *)
{
	lastArtikelID = TableVerkaufArtikelARTIKEL->AsInteger;
	lastArtikelCount = TableVerkaufArtikelMENGE->AsInteger;
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufArtikelAfterRefresh(
	  TDataSet *)
{
	lastArtikelID = TableVerkaufArtikelARTIKEL->AsInteger;
	lastArtikelCount = TableVerkaufArtikelMENGE->AsInteger;
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::QueryArtikelCalcFields(TDataSet *)
{
	char	buffer[32];

	AnsiString bezPreis = QueryArtikelBEZEICHNUNG->AsString;

	bezPreis += " (";
	sprintf(
		buffer,
		"%.2f",
		TableVerkaufPARTYMODE->AsBoolean && QueryArtikelPARTYPREIS->AsFloat
			? QueryArtikelPARTYPREIS->AsFloat
			: QueryArtikelVERKAUFSPREIS->AsFloat
	);
	bezPreis += buffer;
	bezPreis += ")";
	QueryArtikelBEZ_PREIS->AsString = bezPreis;
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::FormActivate(TObject *)
{
	if( !isDBopen() )
		return;

	TableVerkauf->Open();
	TableVerkaufArtikel->Open();
	TableVerkauf->Last();

	QueryArtikel->Close();
	QueryArtikel->Open();

	QueryMitarbeiter->Close();
	QueryMitarbeiter->Open();

	GotoNumbers();

	WindowState = wsMaximized;
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::CheckBoxPartyClick(TObject *)
{
	if( CheckBoxParty->Checked )
		startDate = TDateTime::CurrentDateTime();
	else
		endDate = TDateTime::CurrentDateTime();

	GotoNumbers();
}
//---------------------------------------------------------------------------



void __fastcall TVerkaufForm::DBGridVerkaufDrawColumnCell(TObject *,
	  const TRect &Rect, int DataCol, TColumn *Column,
	  TGridDrawState State)
{
	if( !State.Contains(gdFocused) )
	{
		if( TableVerkaufPARTYMODE->AsBoolean && isCompleted() )
		{
			if( DataCol )
				DBGridVerkauf->Canvas->Brush->Color = (TColor)0xBBFFBB;
			else
				DBGridVerkauf->Canvas->Brush->Color = (TColor)0xBBBBFF;
		}
		else if( TableVerkaufPARTYMODE->AsBoolean )
			DBGridVerkauf->Canvas->Brush->Color = (TColor)0xBBFFBB;
		else if( isCompleted() )
			DBGridVerkauf->Canvas->Brush->Color = (TColor)0xBBBBFF;
		else
			DBGridVerkauf->Canvas->Brush->Color = clWhite;
	}

	DBGridVerkauf->DefaultDrawColumnCell( Rect, DataCol, Column, State);
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::EditNumbersChange(TObject *)
{
	if( EditNumbers->Text == " " )
	{
		if( TableVerkaufGESAMTPREIS->AsFloat > 0
		&&  !isCompleted() )
		{
			if( CompleteForm->ShowModal( TableVerkaufGESAMTPREIS->AsFloat ) == mrOk )
			{
				TableVerkauf->Edit();
				TableVerkaufFERTIG->AsBoolean = true;
				TableVerkauf->Post();

				TableVerkauf->FlushBuffers();
				TableVerkaufArtikel->FlushBuffers();

				FindClientOrders();

				if( !KasseForm->theDatabase->IsSQLBased )
				{
					TableVerkaufArtikel->Close();
					TableVerkauf->Close();

					KasseForm->restartDatabase();

					TableVerkauf->Open();
					TableVerkaufArtikel->Open();
					TableVerkauf->Last();

					TableVerkauf->FlushBuffers();
					TableVerkaufArtikel->FlushBuffers();
				}
			}
		}
		EditNumbers->Text = "";
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::ProtectCompleted(TDataSet *)
{
	if( !isEmpty( TableVerkauf ) && isCompleted() )
	{
		GotoNumbers();

		throw Exception( "Nicht mehr möglich" );
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::CheckBoxFilterClick(TObject *)
{
	bool	filterEnabled = CheckBoxFilter->Checked;
	if( filterEnabled != TableVerkauf->Filtered )
	{

		TableVerkaufArtikel->Close();
		TableVerkauf->Close();
		TableVerkauf->Filtered = filterEnabled;
		if( filterEnabled )
		{
			TDateTime	now = TDateTime::CurrentDateTime();
			now -= 0.5;
			AnsiString	filter = "DATUM > '";
			filter += now.DateTimeString();
			filter += '\'';

			TableVerkauf->Filter = filter;
		}
		else
			TableVerkauf->Filter = "";

		TableVerkauf->Open();
		TableVerkaufArtikel->Open();
	}

	GotoNumbers();
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufAfterOpen(TDataSet *DataSet)
{
	DataSet->Last();
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::ButtonCodeClick(TObject *Sender)
{
	TButton	*codeButton = dynamic_cast<TButton *>( Sender );
	if( codeButton )
	{
		EditNumbers->Text = codes[codeButton->Tag];
		GotoNumbers();

		char tmp = '\n';
		EditNumbersKeyPress( EditNumbers, tmp );
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::FormShow(TObject *)
{
	if( !isDBopen() )
		return;

	doEnterFunction("TVerkaufForm::FormShow");
	TButton		*codeButton;
	int			left, top, buttonIdx = 0;
	const int buttonWidth = 165;
	const int buttonHeigth = 60;

	CheckBoxFilter->Checked = true;
	GotoNumbers();

	QueryTopSeller->Open();

	left = top = 0;
	while( !QueryTopSeller->Eof )
	{
		codeButton = new TButton( this );
		codeButton->Parent = TabSheetTop;
		codeButton->Left = left;
		codeButton->Top = top;
		codeButton->Width = buttonWidth;
		codeButton->Height = buttonHeigth;
		codeButton->Caption = QueryTopSellerBEZEICHNUNG->AsString;
		codeButton->Tag = buttonIdx;
		codes.addElement( QueryTopSellerCODE->AsString );

		codeButton->OnClick = ButtonCodeClick;
		buttonIdx++;

		left += buttonWidth;
		if( left + buttonWidth > TabSheetTop->Width )
		{
			left = 0;
			top += buttonHeigth;
			if( top + buttonHeigth > TabSheetTop->Height )
				break;
		}
		QueryTopSeller->Next();
	}

	QueryTopSeller->First();
	left = top = 0;
	while( !QueryTopSeller->Eof )
	{
		if( !strncmp( QueryTopSellerCODE->AsString.c_str(), "2666", 4 ) )
		{
			codeButton = new TButton( this );
			codeButton->Parent = TabSheetPrivate;
			codeButton->Left = left;
			codeButton->Top = top;
			codeButton->Width = buttonWidth;
			codeButton->Height = buttonHeigth;
			codeButton->Caption = QueryTopSellerBEZEICHNUNG->AsString;
			codeButton->Tag = buttonIdx;
			codes.addElement( QueryTopSellerCODE->AsString );
			codeButton->OnClick = ButtonCodeClick;
			buttonIdx++;

			left += buttonWidth;
			if( left + buttonWidth > TabSheetTop->Width )
			{
				left = 0;
				top += buttonHeigth;
				if( top + buttonHeigth > TabSheetTop->Height )
					break;
			}
		}
		QueryTopSeller->Next();
	}

	for(
		QueryGroups->Open();
		!QueryGroups->Eof;
		QueryGroups->Next()
	)
	{
		TTabSheet	*newSheet = new TTabSheet( this );
		newSheet->Parent = PageControl;
		newSheet->PageControl = PageControl;
		newSheet->Caption = QueryGroupsGRUPPEN->AsString;

		QueryTopSeller->First();
		left = top = 0;
		while( !QueryTopSeller->Eof )
		{
			if( QueryTopSellerGRUPPEN->AsString == QueryGroupsGRUPPEN->AsString )
			{
				codeButton = new TButton( this );
				codeButton->Parent = newSheet;
				codeButton->Left = left;
				codeButton->Top = top;
				codeButton->Width = buttonWidth;
				codeButton->Height = buttonHeigth;
				codeButton->Caption = QueryTopSellerBEZEICHNUNG->AsString;
				codeButton->Tag = buttonIdx;
				codes.addElement( QueryTopSellerCODE->AsString );

				codeButton->OnClick = ButtonCodeClick;
				buttonIdx++;

				left += buttonWidth;
				if( left + buttonWidth > TabSheetTop->Width )
				{
					left = 0;
					top += buttonHeigth;
					if( top + buttonHeigth > TabSheetTop->Height )
						break;
				}
			}
			QueryTopSeller->Next();
		}

	}
	topSellerCSV = getenv( "TEMP" );
	topSellerCSV += DIRECTORY_DELIMITER_STRING "topSeller.CSV";
	exportTable2CSV( QueryTopSeller, topSellerCSV, true );

	QueryTopSeller->Close();
	PageControl->ActivePage = TabSheetTop;

	ConfigDataModule->setDatabase( KasseForm->theDatabase );
	int enableServer = ConfigDataModule->GetValue( "enableServer", 0 );
	if( enableServer )
	{
		int serverPort = ConfigDataModule->GetValue( "serverPort", 8080 );
		STRING password = ConfigDataModule->GetValue( "password", "" );

		KassenServer::setPassword( password );
		kasseServer.startServer( (unsigned short)serverPort, 20 );
	}
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::EditNumbersEnter(TObject *)
{
	EditNumbers->Color = (TColor)0x55FF55;
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::EditNumbersExit(TObject *)
{
	EditNumbers->Color = (TColor)0x0000FF;
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::CheckBoxKontoClick(TObject *)
{
	GotoNumbers();
}
//---------------------------------------------------------------------------



void __fastcall TVerkaufForm::TimerClientOrderTimer(TObject *)
{
	if( isCompleted() )
		FindClientOrders();
}
//---------------------------------------------------------------------------

void __fastcall TVerkaufForm::TableVerkaufAfterScroll(TDataSet *)
{
	if( TableVerkauf->State == dsBrowse && !isCompleted() )
	{
		MitarbeiterForm->FindMitarbeiter(
			TableVerkaufMITARBEITER->AsInteger,
			&maxKredit,
			&endKontenstand
		);
	}
}
//---------------------------------------------------------------------------

