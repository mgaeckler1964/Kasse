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
#pragma hdrstop

#include "BuchungFrm.h"
#include "MitarbeiterFrm.h"
#include "CheckMoneyFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCheckMoneyForm *CheckMoneyForm;
//---------------------------------------------------------------------------
__fastcall TCheckMoneyForm::TCheckMoneyForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCheckMoneyForm::EditIstbestandKeyPress(TObject *,
	  char &Key)
{
	if( Key != ',' && ( Key < '0' || Key > '9' ) && Key != '\b' )
		Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCheckMoneyForm::FormShow(TObject *)
{
	double	endKontenstand;

	MitarbeiterForm->FindKasse( &endKontenstand );
	EditSollBestand->Text = AnsiString::FloatToStrF(
		endKontenstand, AnsiString::sffCurrency, 15, 2
	);
	ButtonOK->Enabled = false;
	EditIstbestand->Text = "";
	EditIstbestand->SetFocus();

}
//---------------------------------------------------------------------------
void __fastcall TCheckMoneyForm::ButtonOKClick(TObject *)
{
	double	sollKontenstand;
	double	istKontenstand = EditIstbestand->Text.ToDouble();

	int kasseID =  MitarbeiterForm->FindKasse( &sollKontenstand );
	if( sollKontenstand != istKontenstand )
	{
		int inventurID =  MitarbeiterForm->FindInventur();
		double difference = sollKontenstand - istKontenstand;

		if( difference > 0 )
			BuchungenForm->createBooking( kasseID, inventurID, difference );
		else
			BuchungenForm->createBooking( inventurID, kasseID, -difference );

		BuchungenForm->BringToFront();
		BuchungenForm->SetFocus();
	}

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TCheckMoneyForm::EditIstbestandChange(TObject *)
{
	if( EditIstbestand->Text > "" )
		ButtonOK->Enabled = true;
	else
		ButtonOK->Enabled = false;
}
//---------------------------------------------------------------------------

