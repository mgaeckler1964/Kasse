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

#ifndef LieferungFrmH
#define LieferungFrmH
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
//---------------------------------------------------------------------------
class TLieferungForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel2;
	TPanel *Panel3;
	TDBNavigator *DBNavigator1;
	TPanel *Panel4;
	TDBNavigator *DBNavigator2;
	TDataSource *DataSourceLieferung;
	TTable *TableLieferung;
	TDBGrid *DBGridLieferung;
	TDataSource *DataSourceLieferungArtikel;
	TTable *TableLieferungArtikel;
	TDBGrid *DBGridLieferungArtikel;
	TIntegerField *TableLieferungArtikelLIEFERUNG;
	TIntegerField *TableLieferungArtikelARTIKEL;
	TIntegerField *TableLieferungArtikelMENGE;
	TTable *TableArtikel;
	TStringField *TableLieferungArtikelAT_BEZ;
	TDateField *TableLieferungDATUM;
	TEdit *EditNumbers;
	TIntegerField *TableLieferungID;
	TIntegerField *TableLieferungArtikelID;
	TBooleanField *TableLieferungINVENTUR;
	void __fastcall TableLieferungAfterInsert(TDataSet *DataSet);
	void __fastcall TableLieferungAfterPost(TDataSet *DataSet);
	void __fastcall EditNumbersKeyPress(TObject *Sender, char &Key);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TableLieferungArtikelAfterPost(TDataSet *DataSet);
	void __fastcall EditNumbersEnter(TObject *Sender);
	void __fastcall EditNumbersExit(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall TableLieferungArtikelAfterInsert(TDataSet *DataSet);
private:	// User declarations
public:		// User declarations
	__fastcall TLieferungForm(TComponent* Owner);
	void AddInventur( int artikelID, int count );
};
//---------------------------------------------------------------------------
extern PACKAGE TLieferungForm *LieferungForm;
//---------------------------------------------------------------------------
#endif
