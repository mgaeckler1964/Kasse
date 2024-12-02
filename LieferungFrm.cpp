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

#include "LieferungFrm.h"
#include "ArtikelFrm.h"
#include "KasseMainFrm.h"

//---------------------------------------------------------------------------
using namespace gak;
using namespace vcl;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLieferungForm *LieferungForm;
//---------------------------------------------------------------------------
__fastcall TLieferungForm::TLieferungForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::TableLieferungAfterInsert(
	  TDataSet *)
{
	setNewMaxValue( TableLieferung, TableLieferungID );
	TableLieferungDATUM->AsDateTime = TDateTime::CurrentDate();
	TableLieferung->Post();
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::TableLieferungAfterPost(TDataSet *)
{
	TableLieferung->FlushBuffers();
	TableLieferungArtikel->FlushBuffers();

	EditNumbers->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::EditNumbersKeyPress(TObject *,
	  char &Key)
{
	if( (Key == '\r' || Key == '\n') && TableArtikel->Active )
	{
		try
		{
			AnsiString	NumberText = EditNumbers->Text.Trim();
			if( TableLieferungArtikel->State != dsEdit && TableLieferungArtikel->State != dsInsert )
				TableLieferungArtikel->Edit();

			int artikelID = ArtikelForm->FindArtikel( NumberText );
			if( artikelID > 0 )
			{
				if( !TableLieferungArtikelARTIKEL->IsNull )
					TableLieferungArtikel->Insert();
				TableLieferungArtikelARTIKEL->AsInteger = artikelID;
			}
			else
			{
				bool	noUnitTest = false;
				if( isalpha( NumberText[1] ) )
				{
					noUnitTest = true;
					NumberText = NumberText.SubString( 2,NumberText.Length() ).Trim();
				}

				int menge = atoi( NumberText.c_str() );
				if( menge > 0 && menge < 1000 )
				{
					if( !noUnitTest )
					{
						artikelID = TableLieferungArtikelARTIKEL->AsInteger;
						if( artikelID )
							menge *= ArtikelForm->FindArtikelUnit( artikelID );
					}

					TableLieferungArtikelMENGE->AsInteger = menge;
					TableLieferungArtikel->Post();
				}
				else
					Application->MessageBox(
						"Artikel nicht gefunden", "Fehler",
						MB_ICONERROR
					);
			}
		}
		catch( ... )
		{
			EditNumbers->Text = "";
			throw;
		}

		EditNumbers->Text = "";

	}
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::FormActivate(TObject *)
{
	if( isDBopen() )
	{
		TableLieferung->Open();
		TableLieferungArtikel->Open();

		TableArtikel->Close();
		TableArtikel->Open();
	}

	WindowState = wsMaximized;
}
//---------------------------------------------------------------------------


void __fastcall TLieferungForm::TableLieferungArtikelAfterPost(
	  TDataSet *)
{
	TableLieferungArtikel->FlushBuffers();	
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::EditNumbersEnter(TObject *)
{
	EditNumbers->Color = (TColor)0x55FF55;
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::EditNumbersExit(TObject *)
{
	EditNumbers->Color = (TColor)0x0000FF;
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::FormShow(TObject *)
{
	if( isDBopen() )
	{
		TableArtikel->Open();
		TableLieferung->Open();
		TableLieferungArtikel->Open();
	}

	EditNumbers->SetFocus();	
}
//---------------------------------------------------------------------------

void __fastcall TLieferungForm::TableLieferungArtikelAfterInsert(
      TDataSet *)
{
	setNewMaxValue( TableLieferungArtikel, TableLieferungArtikelID );
}
//---------------------------------------------------------------------------

void TLieferungForm::AddInventur( int artikelID, int count )
{
	TableLieferung->Open();
	TableLieferungArtikel->Open();

	if( !TableLieferungINVENTUR->AsBoolean || double(TableLieferungDATUM->AsDateTime - TDateTime::CurrentDateTime()) > 1 )
	{
		TableLieferung->Insert();
		TableLieferung->Edit();
		TableLieferungINVENTUR->ReadOnly = false;
		TableLieferungINVENTUR->AsBoolean = true;
		TableLieferungINVENTUR->ReadOnly = true;
		TableLieferung->Post();
	}

	TableLieferungArtikel->Insert();
	TableLieferungArtikelARTIKEL->AsInteger = artikelID;
	TableLieferungArtikelMENGE->AsInteger = count;
	TableLieferungArtikel->Post();
}
//---------------------------------------------------------------------------


