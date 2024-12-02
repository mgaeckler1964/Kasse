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

#ifndef VerkaufFrmH
#define VerkaufFrmH
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
#include <ComCtrls.hpp>

#include <gak/vcl_tools.h>

//---------------------------------------------------------------------------
class TVerkaufForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel2;
	TPanel *Panel3;
	TDBNavigator *DBNavigator1;
	TPanel *Panel4;
	TDBNavigator *DBNavigator2;
	TDataSource *DataSourceVerkauf;
	TTable *TableVerkauf;
	TDBGrid *DBGridVerkauf;
	TDataSource *DataSourceVerkaufArtikel;
	TTable *TableVerkaufArtikel;
	TDBGrid *DBGridVerkaufArtikel;
	TIntegerField *TableVerkaufArtikelARTIKEL;
	TIntegerField *TableVerkaufArtikelMENGE;
	TStringField *TableVerkaufArtikelAT_BEZ;
	TEdit *EditNumbers;
	TIntegerField *TableVerkaufMITARBEITER;
	TCurrencyField *TableVerkaufGESAMTPREIS;
	TIntegerField *TableVerkaufArtikelVERKAUF;
	TDateTimeField *TableVerkaufDATUM;
	TDBText *DBText1;
	TQuery *QueryArtikel;
	TIntegerField *QueryArtikelID;
	TStringField *QueryArtikelBEZEICHNUNG;
	TStringField *QueryArtikelBEZ_PREIS;
	TCurrencyField *QueryArtikelVERKAUFSPREIS;
	TQuery *QueryMitarbeiter;
	TStringField *TableVerkaufMA_BEZ;
	TCheckBox *CheckBoxParty;
	TCurrencyField *QueryArtikelPARTYPREIS;
	TBooleanField *TableVerkaufPARTYMODE;
	TBooleanField *TableVerkaufFERTIG;
	TCheckBox *CheckBoxFilter;
	TIntegerField *TableVerkaufID;
	TIntegerField *TableVerkaufArtikelID;
	TPageControl *PageControl;
	TTabSheet *TabSheetTop;
	TQuery *QueryTopSeller;
	TStringField *QueryTopSellerBEZEICHNUNG;
	TTabSheet *TabSheetPrivate;
	TCheckBox *CheckBoxKonto;
	TQuery *QueryGroups;
	TStringField *QueryGroupsGRUPPEN;
	TStringField *QueryTopSellerGRUPPEN;
	TFloatField *QueryTopSellerMENGE;
	TStringField *QueryTopSellerCODE;
	TCurrencyField *QueryTopSellerVERKAUFSPREIS;
	TCurrencyField *QueryTopSellerPARTYPREIS;
	TTimer *TimerClientOrder;
	void __fastcall TableVerkaufAfterInsert(TDataSet *DataSet);
	void __fastcall EditNumbersSetFocus(TDataSet *DataSet);
	void __fastcall EditNumbersKeyPress(TObject *Sender, char &Key);
	void __fastcall TableVerkaufArtikelAfterInsert(TDataSet *DataSet);
	void __fastcall TableVerkaufArtikelAfterPost(TDataSet *DataSet);
	void __fastcall TableVerkaufArtikelBeforeDelete(TDataSet *DataSet);
	void __fastcall TableVerkaufArtikelAfterScroll(TDataSet *DataSet);
	void __fastcall TableVerkaufArtikelAfterRefresh(TDataSet *DataSet);
	void __fastcall QueryArtikelCalcFields(TDataSet *DataSet);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall CheckBoxPartyClick(TObject *Sender);
	void __fastcall DBGridVerkaufDrawColumnCell(TObject *Sender,
          const TRect &Rect, int DataCol, TColumn *Column,
          TGridDrawState State);
	void __fastcall EditNumbersChange(TObject *Sender);
	void __fastcall ProtectCompleted(TDataSet *DataSet);
	void __fastcall CheckBoxFilterClick(TObject *Sender);
	void __fastcall TableVerkaufAfterOpen(TDataSet *DataSet);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EditNumbersEnter(TObject *Sender);
	void __fastcall EditNumbersExit(TObject *Sender);
	void __fastcall CheckBoxKontoClick(TObject *Sender);
	void __fastcall TimerClientOrderTimer(TObject *Sender);
	void __fastcall TableVerkaufArtikelBeforePost(TDataSet *DataSet);
	void __fastcall TableVerkaufAfterScroll(TDataSet *DataSet);
private:	// User declarations
	double					maxKredit, endKontenstand;
	gak::STRING				topSellerCSV;
	gak::Array<AnsiString>	codes;
	void __fastcall ButtonCodeClick(TObject *Sender);
	int	lastArtikelID, lastArtikelCount;
	void FindClientOrders( void );
public:		// User declarations
	__fastcall TVerkaufForm(TComponent* Owner);
	TDateTime	startDate, endDate;

	void GotoNumbers( void )
	{
		EditNumbers->SetFocus();
		EditNumbers->Color = static_cast<TColor>(0x55FF55);
	}
	const gak::STRING &getTopSellerCSV( void ) const
	{
		return topSellerCSV;
	}
	bool isCompleted( void )
	{
		return gak::vcl::isEmpty( TableVerkauf )
			|| TableVerkaufFERTIG->AsBoolean
			|| TableVerkaufDATUM->AsDateTime < (TDateTime::CurrentDateTime() - 1.0/24.0)
		;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TVerkaufForm *VerkaufForm;
//---------------------------------------------------------------------------
#endif
