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
#include <stdio.h>

#include <vcl.h>
#pragma hdrstop

#include "CompleteFrm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCompleteForm *CompleteForm;
//---------------------------------------------------------------------------
__fastcall TCompleteForm::TCompleteForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
int TCompleteForm::ShowModal( double sumValue )
{
	char buffer[128];

	sprintf( buffer, "Summe: %.2f �", sumValue );
	LabelSumme->Caption = buffer;

	this->sumValue = sumValue;
	EditGiven->Text = "";

	sprintf( buffer, "Zur�ck: %.2f �", -sumValue );
	LabelBack->Caption = buffer;

	return TForm::ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TCompleteForm::EditGivenChange(TObject *)
{
	char	buffer[128];
	double	givenValue = 0;
	double	returnValue;

	sscanf( EditGiven->Text.c_str(), "%lf", &givenValue );
	returnValue = givenValue - sumValue;
	sprintf( buffer, "Zur�ck: %.2f �", returnValue );

	LabelBack->Caption = buffer;
}
//---------------------------------------------------------------------------
void __fastcall TCompleteForm::FormShow(TObject *)
{
	SetFocus();
	SetFocusedControl( EditGiven );
}
//---------------------------------------------------------------------------

void __fastcall TCompleteForm::FormActivate(TObject *)
{
	SetFocusedControl( EditGiven );
}
//---------------------------------------------------------------------------

void __fastcall TCompleteForm::NoButtonFocus(TObject *)
{
	SetFocusedControl( EditGiven );
}
//---------------------------------------------------------------------------

