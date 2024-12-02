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

#ifndef KasseMainFrmH
#define KasseMainFrmH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <AppEvnts.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class TKasseForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu;
	TMenuItem *Datei1;
	TMenuItem *Ende1;
	TMenuItem *Aufgaben1;
	TMenuItem *Verkauf1;
	TMenuItem *Buchungen1;
	TMenuItem *Mitarbeiter1;
	TMenuItem *Artikel1;
	TMenuItem *Lieferungen1;
	TMenuItem *N1;
	TMenuItem *NeuerVerkauf1;
	TMenuItem *PartyAuswertung;
	TDatabase *theDatabase;
	TMenuItem *MenuBerichte;
	TMenuItem *N2;
	TMenuItem *Configuration;
	TMenuItem *N3;
	TMenuItem *ArtikelExport1;
	TMenuItem *ArtikelImport1;
	TMenuItem *Info;
	TMenuItem *N4;
	TMenuItem *Kassenprfung;
	TMenuItem *Inventur;
	TMenuItem *Abschluss;
	void __fastcall Verkauf1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall Buchungen1Click(TObject *Sender);
	void __fastcall Mitarbeiter1Click(TObject *Sender);
	void __fastcall Artikel1Click(TObject *Sender);
	void __fastcall Lieferungen1Click(TObject *Sender);
	void __fastcall Ende1Click(TObject *Sender);
	void __fastcall ReportClick(TObject *Sender);
	void __fastcall PartyAuswertungClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall AppWindowProc(TMessage &msg);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ConfigurationClick(TObject *Sender);
	void __fastcall ArtikelExport1Click(TObject *Sender);
	void __fastcall ArtikelImport1Click(TObject *Sender);
	void __fastcall KassenprfungClick(TObject *Sender);
	void __fastcall InventurClick(TObject *Sender);
	void __fastcall InfoClick(TObject *Sender);
	void __fastcall AbschlussClick(TObject *Sender);
private:	// User declarations
	bool			adminMode;
	gak::CI_STRING	actUserName;

	void checkDbActivity( void );
public:		// User declarations
	__fastcall TKasseForm(TComponent* Owner);

	bool isAdmin( void )
	{
		return adminMode;
	}
	bool isDBopen( void )
	{
		return theDatabase->Connected;
	}
	void restartDatabase( void )
	{
		theDatabase->Close();
		Session->Close();
		Session->Open();
		theDatabase->Open();
	}
	const gak::CI_STRING &getActUser() const
	{
		return actUserName;
	}
};
//---------------------------------------------------------------------------
extern PACKAGE TKasseForm *KasseForm;
//---------------------------------------------------------------------------
inline	bool isDBopen( void )
{
	return KasseForm->isDBopen();
}
//---------------------------------------------------------------------------
#endif
