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

#ifndef ArtikelFrmH
#define ArtikelFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TArtikelForm : public TForm
{
__published:	// IDE-managed Components
	TTable *TableArtikel;
	TDataSource *DataSourceArtikel;
	TDBGrid *DBGridArtikel;
	TStringField *TableArtikelBEZEICHNUNG;
	TIntegerField *TableArtikelBESTAND;
	TCurrencyField *TableArtikelEINKAUFSPREIS;
	TCurrencyField *TableArtikelVERKAUFSPREIS;
	TPanel *Panel1;
	TDBNavigator *DBNavigator1;
	TIntegerField *TableArtikelSOLL_BESTAND;
	TQuery *QueryVerkauf;
	TIntegerField *QueryVerkaufARTIKEL;
	TFloatField *QueryVerkaufMENGE;
	TQuery *QueryLieferung;
	TIntegerField *QueryLieferungARTIKEL;
	TFloatField *QueryLieferungMENGE;
	TCurrencyField *TableArtikelPARTYPREIS;
	TPaintBox *PaintBoxBarCode;
	TButton *ButtonCreateEAN13;
	TButton *ButtonPrint;
	TPrintDialog *PrintDialog;
	TFloatField *TableArtikelVERKAUF;
	TFloatField *TableArtikelRESTZEIT;
	TPopupMenu *PopupMenuPrint;
	TMenuItem *MenuPrintAktuelles;
	TMenuItem *MenuPrintBestand;
	TMenuItem *MenuPrintPrivate;
	TMenuItem *MenuPrintBestandUndPrivat;
	TIntegerField *TableArtikelID;
	TStringField *TableArtikelGRUPPEN;
	TMenuItem *N1;
	TMenuItem *MenuAktuelleGruppe;
	TDBGrid *DBGridBarCodes;
	TTable *TableArtikelCode;
	TDataSource *DataSourceArtikelCode;
	TIntegerField *TableArtikelCodeID;
	TStringField *TableArtikelCodeCODE;
	TDBNavigator *DBNavigatorArtikelCode;
	TQuery *QueryArtikelCode;
	TIntegerField *TableArtikelCodeARTIKEL;
	TIntegerField *QueryArtikelCodeARTIKEL;
	TOpenDialog *OpenDialog;
	TSaveDialog *SaveDialog;
	TQuery *QueryArtikelImport;
	TIntegerField *TableArtikelIST_BESTAND;
	TDateTimeField *QueryVerkaufMIN_DATUM;
	TDateField *TableArtikelSTART;
	TIntegerField *TableArtikelEINHEIT;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TableArtikelCalcFields(TDataSet *DataSet);
	void __fastcall PaintBoxBarCodePaint(TObject *Sender);
	void __fastcall ButtonCreateEAN13Click(TObject *Sender);
	void __fastcall ButtonPrintClick(TObject *Sender);
	void __fastcall TableArtikelAfterPost(TDataSet *DataSet);
	void __fastcall TableArtikelAfterInsert(TDataSet *DataSet);
	void __fastcall TableArtikelCodeAfterInsert(TDataSet *DataSet);
	void __fastcall TableArtikelCodeAfterPost(TDataSet *DataSet);
	void __fastcall TableArtikelCodeAfterScroll(TDataSet *DataSet);
	void __fastcall TableArtikelCodeBeforeInsert(TDataSet *DataSet);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall TableArtikelBeforeDelete(TDataSet *DataSet);
private:	// User declarations
	void printRecord(
		TCanvas *theCanvas,
		int x, int y,
		int widthRec, int heightRec,
		int widthBarCode, int heightBarCode
	);
	bool locateArtikel( int artikelID );
public:		// User declarations
	__fastcall TArtikelForm(TComponent* Owner);
	int FindArtikel( const AnsiString &code );
	double FindArtikelPrice( int artikelID, bool partyPreis );
	int FindArtikelUnit( int artikelID );
	void artikelExport( void );
	void artikelImport( void );
	void Inventur( void );
};
//---------------------------------------------------------------------------
extern PACKAGE TArtikelForm *ArtikelForm;
//---------------------------------------------------------------------------
#endif
