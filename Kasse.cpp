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

#include <gak/logfile.h>

#pragma hdrstop
USERES("Kasse.res");
USEFORM("KasseMainFrm.cpp", KasseForm);
USEFORM("ArtikelFrm.cpp", ArtikelForm);
USEFORM("MitarbeiterFrm.cpp", MitarbeiterForm);
USEFORM("VerkaufFrm.cpp", VerkaufForm);
USEFORM("LieferungFrm.cpp", LieferungForm);
USEFORM("BuchungFrm.cpp", BuchungenForm);
USEFORM("PartyFrm.cpp", PartyForm);
USEFORM("CompleteFrm.cpp", CompleteForm);
USELIB("..\..\Object\gaklib\gaklib_bcb.lib");
USEFORM("..\GAKLIB\Repository\Config.cpp", ConfigDataModule); /* TDataModule: File Type */
USEFORM("ConfigFrm.cpp", ConfigForm);
USEUNIT("WebServer.cpp");
USEFORM("CheckMoneyFrm.cpp", CheckMoneyForm);
USEFORM("..\GAKLIB\Repository\AboutFrm.cpp", AboutProgramForm);
USELIB("..\..\Object\winlib\winlibBCB.lib");
USEFORM("..\GAKLIB\Repository\StatusFrm.cpp", StatusForm);
//---------------------------------------------------------------------------
#include "MitarbeiterFrm.h"
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	doEnterFunction( "WinMain" );

	try
	{
		Application->Initialize();
		Application->Title = "GAK Kassenterminal";
		Application->CreateForm(__classid(TKasseForm), &KasseForm);
		Application->CreateForm(__classid(TConfigForm), &ConfigForm);
		Application->CreateForm(__classid(TPartyForm), &PartyForm);
		Application->CreateForm(__classid(TCompleteForm), &CompleteForm);
		Application->CreateForm(__classid(TCheckMoneyForm), &CheckMoneyForm);
		Application->CreateForm(__classid(TArtikelForm), &ArtikelForm);
		Application->CreateForm(__classid(TMitarbeiterForm), &MitarbeiterForm);
		Application->CreateForm(__classid(TLieferungForm), &LieferungForm);
		Application->CreateForm(__classid(TBuchungenForm), &BuchungenForm);
		Application->CreateForm(__classid(TVerkaufForm), &VerkaufForm);
		Application->CreateForm(__classid(TAboutProgramForm), &AboutProgramForm);
		Application->CreateForm(__classid(TStatusForm), &StatusForm);
		try
		{
			MitarbeiterForm->FindKasse();
		}
		catch( ... )
		{
			Application->MessageBox( "Kann Kasse nicht erstellen", "Kasse", MB_OK|MB_ICONERROR);
		}
		try
		{
			MitarbeiterForm->FindInventur();
		}
		catch( ... )
		{
			Application->MessageBox( "Kann Inventur nicht erstellen", "Kasse", MB_OK|MB_ICONERROR);
		}


		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
