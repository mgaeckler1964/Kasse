/*
		Project:		KASSE
		Module:			
		Description:	
		Author:			Martin G�ckler
		Address:		HoFmannsthalweg 14, A-4030 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 1988-2024 Martin G�ckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin G�ckler, Austria, Linz ``AS IS''
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

#ifndef PartyFrmH
#define PartyFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TPartyForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel2;
	TDBGrid *DBGrid1;
	TDataSource *DataSourceParty;
	TQuery *QueryParty;
	TStringField *QueryPartyBEZEICHNUNG;
	TCurrencyField *QueryPartyPARTYPREIS;
	TCurrencyField *QueryPartyVERKAUFSPREIS;
	TButton *ButtonQueryStart;
	TEdit *EditFromDate;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EditToDate;
	TLabel *LabelResult;
	TFloatField *QueryPartyMENGE;
	TButton *ButtonExport;
	void __fastcall ButtonQueryStartClick(TObject *Sender);
	void __fastcall QueryPartyAfterOpen(TDataSet *DataSet);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ButtonExportClick(TObject *Sender);
private:	// User declarations
	double	sumPartyPreis, sumVerkaufspreis, differenz;
public:		// User declarations
	__fastcall TPartyForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPartyForm *PartyForm;
//---------------------------------------------------------------------------
#endif
