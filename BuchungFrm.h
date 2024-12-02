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

#ifndef BuchungFrmH
#define BuchungFrmH
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
class TBuchungenForm : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TPanel *Panel1;
	TDataSource *DataSourceBuchung;
	TTable *TableBuchung;
	TDateTimeField *TableBuchungDATUM;
	TIntegerField *TableBuchungSOLL_KONTO;
	TIntegerField *TableBuchungHABEN_KONTO;
	TCurrencyField *TableBuchungBETRAG;
	TQuery *QueryMitarbeiter;
	TIntegerField *QueryMitarbeiterID;
	TStringField *QueryMitarbeiterNAME;
	TStringField *TableBuchungSOLL_KONTO_BEZ;
	TStringField *TableBuchungHABEN_KONTO_BEZ;
	TDBGrid *DBGrid1;
	TDBNavigator *DBNavigator1;
	TIntegerField *TableBuchungID;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall TableBuchungAfterInsert(TDataSet *DataSet);
	void __fastcall TableBuchungAfterPost(TDataSet *DataSet);
	void __fastcall TableBuchungBeforePost(TDataSet *DataSet);
	void __fastcall TableBuchungFilterRecord(TDataSet *DataSet, bool &Accept);
private:	// Anwender-Deklarationen
public:		// Anwender-Deklarationen
	__fastcall TBuchungenForm(TComponent* Owner);
	void createBooking( int sollKonto, int habenKonto, double money );
};
//---------------------------------------------------------------------------
extern PACKAGE TBuchungenForm *BuchungenForm;
//---------------------------------------------------------------------------
#endif
