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

#include <memory.h>

#include <vcl.h>

#include <gak/gaklib.h>
#include <gak/array.h>
#include <gak/directory.h>
#include <gak/map.h>
#pragma hdrstop

#include "KasseMainFrm.h"
#include "VerkaufFrm.h"
#include "BuchungFrm.h"
#include "MitarbeiterFrm.h"
#include "ArtikelFrm.h"
#include "LieferungFrm.h"
#include "PartyFrm.h"
#include "Config.h"
#include "ConfigFrm.h"
#include "AboutFrm.h"
#include "WebServer.h"
#include "CheckMoneyFrm.h"
#include "StatusFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace gak;
TKasseForm *KasseForm;
//---------------------------------------------------------------------------
__fastcall TKasseForm::TKasseForm(TComponent* Owner)
	: TForm(Owner)
{
	adminMode = false;
}
//---------------------------------------------------------------------------
void TKasseForm::checkDbActivity( void )
{
	if( VerkaufForm->TableVerkauf->State > dsBrowse || VerkaufForm->TableVerkaufArtikel->State > dsBrowse )
	{
		VerkaufForm->BringToFront();
		VerkaufForm->SetFocus();
		throw Exception( "Bitte erst Verkaufsvorgang abschließen" );
	}
	if( LieferungForm->TableLieferung->State > dsBrowse || LieferungForm->TableLieferungArtikel->State > dsBrowse )
	{
		LieferungForm->BringToFront();
		LieferungForm->SetFocus();
		throw Exception( "Bitte erst Lieferung abschließen" );
	}

	if( ArtikelForm->TableArtikel->State > dsBrowse || ArtikelForm->TableArtikelCode->State > dsBrowse )
	{
		ArtikelForm->BringToFront();
		ArtikelForm->SetFocus();
		throw Exception( "Bitte erst Artikel abschließen" );
	}
	if( MitarbeiterForm->TableMitarbeiter->State > dsBrowse )
	{
		MitarbeiterForm->BringToFront();
		MitarbeiterForm->SetFocus();
		throw Exception( "Bitte erst Mitarbeiter abschließen" );
	}
	if( BuchungenForm->TableBuchung->State > dsBrowse )
	{
		BuchungenForm->BringToFront();
		BuchungenForm->SetFocus();
		throw Exception( "Bitte erst Buchungen abschließen" );
	}


}
//---------------------------------------------------------------------------
void __fastcall TKasseForm::Verkauf1Click(TObject *Sender)
{
	VerkaufForm->BringToFront();
	VerkaufForm->SetFocus();
	if( Sender == NeuerVerkauf1 )
	{
		VerkaufForm->TableVerkauf->Insert();
	}
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::Buchungen1Click(TObject *)
{
	BuchungenForm->BringToFront();
	BuchungenForm->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::Mitarbeiter1Click(TObject *)
{
	MitarbeiterForm->BringToFront();
	MitarbeiterForm->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::Artikel1Click(TObject *)
{
	ArtikelForm->BringToFront();
	ArtikelForm->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::Lieferungen1Click(TObject *)
{
	LieferungForm->BringToFront();
	LieferungForm->SetFocus();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::Ende1Click(TObject *)
{
	Close();
//	Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::ReportClick(TObject *Sender)
{
	STRING theReport = ConfigDataModule->GetValue( "reportPath", "reports\\" );
	theReport += ((TMenuItem*)Sender)->Caption.c_str();

	theReport = makeFullPath( (STRING)Application->ExeName.c_str(), theReport );

	STRING theReportCmd = "srxViewer.exe KASSE \"";
	theReportCmd += theReport;
	theReportCmd += '\"';

	WinExec( theReportCmd, SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::PartyAuswertungClick(TObject *)
{
	TDateTime	startDate = VerkaufForm->startDate;
	TDateTime	endDate = VerkaufForm->endDate;

	if( endDate <= startDate )
		endDate = TDateTime::CurrentDateTime();

	PartyForm->EditFromDate->Text = startDate.DateTimeString();
	PartyForm->EditToDate->Text = endDate.DateTimeString();
	PartyForm->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TKasseForm::AppWindowProc(TMessage &msg)
{
	if( msg.Msg == WM_ENDSESSION )
	{
		theDatabase->Close();
		Session->Close();
	}

	WndProc( msg );
}
//---------------------------------------------------------------------------
void __fastcall TKasseForm::FormCreate(TObject *)
{
	STRING kasseXML = makeFullPath( Application->ExeName.c_str(), "KASSE.XML" );
	adminMode = !strAccess( kasseXML, 06 );

	//adminMode = false;

	char	theUserName[128];
	DWORD	size = sizeof( theUserName );
	GetUserName( theUserName, &size );
	actUserName = theUserName;

	WindowProc = AppWindowProc;
	Application->OnActivate = FormActivate;
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::FormShow(TObject *)
{
	STRING	privDir = getTempPath() + "\\KASSE";

	makeDirectory( privDir );

	Session->PrivateDir = (const char *)privDir;
	try
	{
		theDatabase->Open();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
		Application->Terminate();
		throw;
	}
	catch( ... )
	{
		Application->MessageBox(
			"Datenhbank kann nicht geöffnet werden", "Kasse", MB_OK|MB_ICONSTOP
		);
		Application->Terminate();
		throw;
	}

	Application->CreateForm(__classid(TConfigDataModule), &ConfigDataModule);
	if( ConfigDataModule->GetDBVersion( theDatabase ) != 6 )
	{
		Application->MessageBox(
			"Falsche Datenbankversion", "Kasse", MB_OK|MB_ICONSTOP
		);
		Application->Terminate();
//		exit( -1 );
	}

	DirectoryList	reports;
	ConfigDataModule->setDatabase( theDatabase );
	STRING 	reportPath = ConfigDataModule->GetValue("reportPath","reports\\");

	reportPath = makeFullPath(STRING(Application->ExeName.c_str()), reportPath);

	STRING	oldPath = getcwd();
	setcwd( reportPath );
	reports.findFiles( "*.srx" );
	reports.checkAccess( DirectoryList::READ );
	setcwd( oldPath );

	MenuBerichte->AutoHotkeys = maManual;
	for(
		DirectoryList::iterator it = reports.begin(), endIT = reports.end();
		it != endIT;
		++it
	)
	{
		TMenuItem *newMenu = new TMenuItem( this );
		newMenu->AutoHotkeys = maManual;
		newMenu->Caption = (const char *)it->fileName;
		newMenu->OnClick = ReportClick;

		MenuBerichte->Add( newMenu );
	}

	PartyAuswertung->Visible = isAdmin();
	PartyAuswertung->Enabled = isAdmin();
	Configuration->Visible = isAdmin();
	Configuration->Enabled = isAdmin();
	Abschluss->Visible = isAdmin();
	Abschluss->Enabled = isAdmin();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::FormActivate(TObject *)
{
	WindowState = wsMaximized;
	if( VerkaufForm )
	{
		VerkaufForm->BringToFront();
		VerkaufForm->SetFocus();
		VerkaufForm->GotoNumbers();
		VerkaufForm->WindowState = wsMaximized;
	}
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::FormClose(TObject *,
	  TCloseAction &)
{
	theDatabase->Close();
	Session->Close();
}
//---------------------------------------------------------------------------


void __fastcall TKasseForm::ConfigurationClick(TObject *)
{
	STRING organisation = ConfigDataModule->GetValue( "Organisation", "" );
	ConfigForm->EditOrganisation->Text = (const char *)organisation;

	STRING reportPath = ConfigDataModule->GetValue( "reportPath", "reports\\" );
	ConfigForm->EditReports->Text = (const char *)reportPath;

	int enableServer = ConfigDataModule->GetValue( "enableServer", 0 );
	ConfigForm->CheckBoxEnableServer->Checked = enableServer != 0;

	int serverPort = ConfigDataModule->GetValue( "serverPort", 8080 );
	STRING	serverPortStr = formatNumber( serverPort );
	ConfigForm->EditServerPort->Text = (const char *)serverPortStr;

	STRING password = ConfigDataModule->GetValue( "password", "" );
	ConfigForm->EditPassword1->Text = (const char *)password;
	ConfigForm->EditPassword2->Text = (const char *)password;

	if( enableServer )
	{
		kasseServer.stopServer( false );
	}

	if( ConfigForm->ShowModal() == mrOk )
	{
		organisation = ConfigForm->EditOrganisation->Text.c_str();
		ConfigDataModule->SetValue( "Organisation", organisation );

		reportPath = ConfigForm->EditReports->Text.c_str();
		if( !reportPath.isEmpty() && !reportPath.endsWith( DIRECTORY_DELIMITER ) )
			reportPath += DIRECTORY_DELIMITER;

		ConfigDataModule->SetValue( "reportPath", reportPath );

		enableServer = ConfigForm->CheckBoxEnableServer->Checked ? 1 : 0;
		ConfigDataModule->SetValue( "enableServer", enableServer );

		serverPort = atoi( ConfigForm->EditServerPort->Text.c_str() );
		ConfigDataModule->SetValue( "serverPort", serverPort );

		password = ConfigForm->EditPassword1->Text.c_str();
		ConfigDataModule->SetValue( "password", password );
	}

	if( enableServer )
	{
		KassenServer::setPassword( password );
		kasseServer.startServer( (unsigned short)serverPort, 20 );
	}
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::ArtikelExport1Click(TObject *)
{
	checkDbActivity();

	ArtikelForm->artikelExport();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::ArtikelImport1Click(TObject *)
{
	checkDbActivity();

	if( VerkaufForm->isCompleted() )
		ArtikelForm->artikelImport();
	else
	{
		VerkaufForm->BringToFront();
		VerkaufForm->SetFocus();
		throw Exception( "Bitte erst Verkaufsvorgang abschließen" );
	}
}
//---------------------------------------------------------------------------
void __fastcall TKasseForm::KassenprfungClick(TObject *)
{
	checkDbActivity();

	CheckMoneyForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::InventurClick(TObject *)
{
	checkDbActivity();

	if(
		Application->MessageBox(
			"Die Funktion überträgt Inventurdaten in die Buchungsliste. Fortfahren?",
			"Inventur",
			MB_YESNO|MB_ICONQUESTION ) == IDYES
		)
	{
		ArtikelForm->BringToFront();
		ArtikelForm->SetFocus();
		ArtikelForm->Inventur();
	}
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::InfoClick(TObject *)
{
	AboutProgramForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TKasseForm::AbschlussClick(TObject *)
{
	checkDbActivity();

	PairMap< int, int >	artikelMengen;
	PairMap< int, int >	verkaufsBetrag;

	unsigned short	dummy, year;
	TDateTime now = TDateTime::CurrentDate();
	now.DecodeDate( &year, &dummy, &dummy );
	TDateTime endDate = ::EncodeDate( year-1, 1, 1 );
	AnsiString  question = "Löschen aller Datensätze bis ";
	question += endDate.DateTimeString();
	question += '?';

	if(
		Application->MessageBox(
			question.c_str(), "Kasse", MB_YESNO|MB_ICONQUESTION ) == IDYES
	)
	{
		StatusForm->Show();

		size_t verkaeufe = 0;
		size_t verkaeufeArtikel = 0;
		size_t lieferungen = 0;
		size_t lieferungenArtikel = 0;
		size_t buchungen = 0;

		std::auto_ptr<TQuery> selectMaster( new TQuery( NULL ) );
		std::auto_ptr<TQuery> selectDetail( new TQuery( NULL ) );
		std::auto_ptr<TQuery> deleteSQL( new TQuery( NULL ) );

		selectMaster->DatabaseName = "kasseDB";
		selectDetail->DatabaseName = "kasseDB";
		deleteSQL->DatabaseName = "kasseDB";

		/*
			clean tables VERKAUF and VERKAUF_ARTIKEL
			========================================
		*/
		selectMaster->SQL->Add( "select * from VERKAUF where DATUM < :endDate" );
		selectDetail->SQL->Add( "select * from VERKAUF_ARTIKEL where VERKAUF = :theID" );
		deleteSQL->SQL->Add( "delete from VERKAUF_ARTIKEL where VERKAUF = :theID" );

		selectMaster->Params->Items[0]->AsDateTime = endDate;
		for(
			selectMaster->Open();
			!selectMaster->Eof;
			selectMaster->Next()
		)
		{
			verkaeufe++;
			int mitarbeiterID = selectMaster->FieldByName( "MITARBEITER" )->AsInteger;
			if( verkaufsBetrag.hasElement( mitarbeiterID ) )
				verkaufsBetrag[mitarbeiterID] += int(selectMaster->FieldByName( "GESAMTPREIS" )->AsFloat * 100 + 0.5);
			else
				verkaufsBetrag[mitarbeiterID] = int(selectMaster->FieldByName( "GESAMTPREIS" )->AsFloat * 100 + 0.5);
			deleteSQL->Params->Items[0]->AsInteger = selectMaster->Fields->Fields[0]->AsInteger;
			selectDetail->Params->Items[0]->AsInteger = selectMaster->Fields->Fields[0]->AsInteger;
			for(
				selectDetail->Open();
				!selectDetail->Eof;
				selectDetail->Next()
			)
			{
				int artikelID = selectDetail->FieldByName("ARTIKEL")->AsInteger;
				int menge = selectDetail->FieldByName("MENGE")->AsInteger;
				if( artikelMengen.hasElement( artikelID ) )
					artikelMengen[artikelID] += menge;
				else
					artikelMengen[artikelID] = menge;

				verkaeufeArtikel += menge;
			}
			selectDetail->Close();
			deleteSQL->ExecSQL();

			if( !(verkaeufe %100) )
			{
				STRING status = formatNumber( verkaeufe ) + '/' + formatNumber( verkaeufeArtikel ) + " gelöscht.";
				StatusForm->setStatus( "Verkäufe", status );
			}
		}
		selectMaster->Close();

		deleteSQL->SQL->Clear();
		deleteSQL->SQL->Add( "delete from VERKAUF where DATUM < :endDate" );
		deleteSQL->Params->Items[0]->AsDateTime = endDate;
		deleteSQL->ExecSQL();


		/*
			clean tables LIEFERUNG and LIEFERUNG_ARTIKEL
			============================================
		*/
		selectMaster->SQL->Clear();
		selectDetail->SQL->Clear();
		deleteSQL->SQL->Clear();

		selectMaster->SQL->Add( "select * from LIEFERUNG where DATUM < :endDate" );
		selectDetail->SQL->Add( "select * from LIEFERUNG_ARTIKEL where LIEFERUNG = :theID" );
		deleteSQL->SQL->Add( "delete from LIEFERUNG_ARTIKEL where LIEFERUNG = :theID" );

		selectMaster->Params->Items[0]->AsDateTime = endDate;
		for(
			selectMaster->Open();
			!selectMaster->Eof;
			selectMaster->Next()
		)
		{
			lieferungen++;
			deleteSQL->Params->Items[0]->AsInteger = selectMaster->Fields->Fields[0]->AsInteger;
			selectDetail->Params->Items[0]->AsInteger = selectMaster->Fields->Fields[0]->AsInteger;
			for(
				selectDetail->Open();
				!selectDetail->Eof;
				selectDetail->Next()
			)
			{
				int artikelID = selectDetail->FieldByName("ARTIKEL")->AsInteger;
				int menge = selectDetail->FieldByName("MENGE")->AsInteger;
				if( artikelMengen.hasElement( artikelID ) )
					artikelMengen[artikelID] -= menge;
				else
					artikelMengen[artikelID] = -menge;

				lieferungenArtikel += menge;
			}
			selectDetail->Close();
			deleteSQL->ExecSQL();

			if( !(lieferungen %100) )
			{
				STRING status = formatNumber( lieferungen ) + '/' + formatNumber( lieferungenArtikel ) + " gelöscht.";
				StatusForm->setStatus( "Lieferungen", status );
			}
		}
		selectMaster->Close();

		deleteSQL->SQL->Clear();
		deleteSQL->SQL->Add( "delete from LIEFERUNG where DATUM < :endDate" );
		deleteSQL->Params->Items[0]->AsDateTime = endDate;
		deleteSQL->ExecSQL();

		/*
			clean table BUCHUNG
			===================
		*/
		selectMaster->SQL->Clear();

		selectMaster->SQL->Add( "select * from BUCHUNG where DATUM < :endDate" );

		selectMaster->Params->Items[0]->AsDateTime = endDate;
		for(
			selectMaster->Open();
			!selectMaster->Eof;
			selectMaster->Next()
		)
		{
			buchungen++;
			int sollID = selectMaster->FieldByName( "SOLL_KONTO" )->AsInteger;
			int habenID = selectMaster->FieldByName( "HABEN_KONTO" )->AsInteger;
			int betrag = int(selectMaster->FieldByName( "BETRAG" )->AsFloat * 100.0 + 0.5);

			if( verkaufsBetrag.hasElement( habenID ) )
				verkaufsBetrag[habenID] += betrag;
			else
				verkaufsBetrag[habenID] = betrag;

			if( verkaufsBetrag.hasElement( sollID ) )
				verkaufsBetrag[sollID] -= betrag;
			else
				verkaufsBetrag[sollID] = -betrag;

			if( !(buchungen %100) )
			{
				STRING status = formatNumber( buchungen )+ " gelöscht.";
				StatusForm->setStatus( "Buchungen", status );
			}
		}
		selectMaster->Close();

		deleteSQL->SQL->Clear();
		deleteSQL->SQL->Add( "delete from BUCHUNG where DATUM < :endDate" );
		deleteSQL->Params->Items[0]->AsDateTime = endDate;
		deleteSQL->ExecSQL();

		// update artikel
		selectMaster->SQL->Clear();
		selectMaster->RequestLive = true;
		selectMaster->SQL->Add( "select * from ARTIKEL" );
		for(
			selectMaster->Open();
			!selectMaster->Eof;
			selectMaster->Next()
		)
		{
			if(
				artikelMengen.hasElement(
					selectMaster->Fields->Fields[0]->AsInteger
				)
			)
			{
				selectMaster->Edit();
				TField	*bestandField = selectMaster->FieldByName( "BESTAND" );
				int bestand = bestandField->AsInteger;
				bestand -= artikelMengen[selectMaster->Fields->Fields[0]->AsInteger];
				bestandField->AsInteger = bestand;

				selectMaster->Post();
			}
		}
		selectMaster->Close();

		// update mitarbeiter
		selectMaster->SQL->Clear();
		selectMaster->RequestLive = true;
		selectMaster->SQL->Add( "select * from MITARBEITER" );
		for(
			selectMaster->Open();
			!selectMaster->Eof;
			selectMaster->Next()
		)
		{
			if(
				verkaufsBetrag.hasElement(
					selectMaster->Fields->Fields[0]->AsInteger
				)
			)
			{
				selectMaster->Edit();
				TField	*bestandField = selectMaster->FieldByName( "KONTOSTAND" );
				double bestand = bestandField->AsFloat;
				bestand += double(verkaufsBetrag[selectMaster->Fields->Fields[0]->AsInteger])/100.0;
				bestandField->AsFloat = bestand;

				selectMaster->Post();
			}
		}
		selectMaster->Close();

		StatusForm->Close();

		STRING status = formatNumber( verkaeufe ) + '/' + formatNumber( verkaeufeArtikel ) + " Verkäufe/Artikel gelöscht.\n" +
			formatNumber( lieferungen ) + '/' + formatNumber( lieferungenArtikel ) + " Lieferungen/Artikel  gelöscht.\n" +
			formatNumber( buchungen )+ " Buchungen gelöscht."
		;

		status += formatNumber( sizeof( selectMaster ) ) + 'x' + formatNumber( sizeof( selectDetail ) );
		Application->MessageBox( (const char *)status, "Kassenabschluß", MB_OK|MB_ICONINFORMATION );

		restartDatabase();
		if( !VerkaufForm->Active )
		{
			VerkaufForm->BringToFront();
			VerkaufForm->SetFocus();
		}
		else
			VerkaufForm->FormActivate( this );
	}
}
//---------------------------------------------------------------------------




