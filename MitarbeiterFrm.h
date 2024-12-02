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

#ifndef MitarbeiterFrmH
#define MitarbeiterFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>

#include <gak/CI_STRING.h>

//---------------------------------------------------------------------------
class TMitarbeiterForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TDataSource *DataSource;
	TTable *TableMitarbeiter;
	TDBGrid *DBGrid1;
	TDBNavigator *DBNavigator1;
	TStringField *TableMitarbeiterCODE;
	TStringField *TableMitarbeiterNAME;
	TCurrencyField *TableMitarbeiterKONTOSTAND;
	TQuery *QuerySoll;
	TCurrencyField *TableMitarbeiterEND_KONTOSTAND;
	TIntegerField *QuerySollSOLL_KONTO;
	TCurrencyField *QuerySollBETRAG;
	TQuery *QueryHaben;
	TIntegerField *QueryHabenHABEN_KONTO;
	TCurrencyField *QueryHabenBETRAG;
	TQuery *QueryVerkauf;
	TIntegerField *QueryVerkaufMITARBEITER;
	TCurrencyField *QueryVerkaufGESAMTPREIS;
	TPaintBox *PaintBoxBarCode;
	TButton *ButtonCreateEAN8;
	TButton *ButtonPrint;
	TPrintDialog *PrintDialog;
	TIntegerField *TableMitarbeiterID;
	TCurrencyField *TableMitarbeiterMAX_KREDIT;
	TStringField *TableMitarbeiterUSERNAME;
	void __fastcall TableMitarbeiterCalcFields(TDataSet *DataSet);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TableMitarbeiterAfterScroll(TDataSet *DataSet);
	void __fastcall PaintBoxBarCodePaint(TObject *Sender);
	void __fastcall ButtonCreateEAN8Click(TObject *Sender);
	void __fastcall ButtonPrintClick(TObject *Sender);
	void __fastcall TableMitarbeiterAfterPost(TDataSet *DataSet);
	void __fastcall TableMitarbeiterAfterInsert(TDataSet *DataSet);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TableMitarbeiterBeforeDelete(TDataSet *DataSet);
	void __fastcall TableMitarbeiterBeforeOpen(TDataSet *DataSet);
private:	// User declarations
	char				barCode[128];
	int					kassenID, inventurID;
	double calcAccountValue();
public:		// User declarations
	__fastcall TMitarbeiterForm(TComponent* Owner);
	bool isProtected( int mitarbeiterID );
	bool FindMitarbeiter(
		int mitarbeiterID,
		double *maxKredit, double *endKontenstand, gak::CI_STRING *osName
	);
	int FindMitarbeiter( const AnsiString &code, double *maxKredit, double *endKontenstand );
	int FindCreateAccount( const AnsiString &code, const AnsiString &name, double *endKontenstand );
	int FindKasse( double *endKontenstand )
	{
		return kassenID = FindCreateAccount( "", "Kasse", endKontenstand );
	}
	int FindKasse( void )
	{
		if( !kassenID )
		{
			double	dummy;
			kassenID = FindKasse( &dummy );
		}
		return kassenID;
	}
	int FindInventur( void )
	{
		if( !inventurID )
		{
			double	dummy;
			inventurID = FindCreateAccount( "1", "Inventur", &dummy );
		}
		return inventurID;
	}
	void CloseQueries( void )
	{
		QueryHaben->Close();
		QuerySoll->Close();
		QueryVerkauf->Close();
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TMitarbeiterForm *MitarbeiterForm;
//---------------------------------------------------------------------------
#endif
