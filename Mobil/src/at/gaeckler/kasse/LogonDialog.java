/*
		Project:		KASSE
		Module:			LogonDialog.java
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

package at.gaeckler.kasse;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

public class LogonDialog extends DialogFragment 
{
	public interface KasseLogonListener
	{
		public void onKasseLogon( String serverName, int port, String password );
		public void onKasseCancel();
	}

	EditText			serverName, serverPort, password;
	Button				buttonOK;
	KasseLogonListener	myListener;
	SharedPreferences	settings;

	@Override
	public Dialog onCreateDialog(Bundle savedInstanceState) 
	{
	    AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
	    // Get the layout inflater
	    LayoutInflater inflater = getActivity().getLayoutInflater();

	    // Inflate and set the layout for the dialog
	    // Pass null as the parent view because its going in the dialog layout
        View view = inflater.inflate(R.layout.kasse_logon, null);
	    builder.setView( view )
	    // Add action buttons
	    .setPositiveButton("OK", new DialogInterface.OnClickListener() 
	    {
    		@Override
    		public void onClick(DialogInterface dialog, int id)
    		{
    			int port = 81;
    			
    			try
    			{
    				port = Integer.parseInt(serverPort.getText().toString());
    			}
    			catch( Exception e )
    			{
    				
    			}
    			
    		    SharedPreferences.Editor editor = settings.edit();
    		    editor.putString("serverName", serverName.getText().toString());
    		    editor.putString("serverPort", serverPort.getText().toString());
    		
    		    // Commit the edits!
    		    editor.commit();
    			myListener.onKasseLogon(serverName.getText().toString(), port, password.getText().toString());
    			
    		}
	    })
	    .setNegativeButton("Abbruch", new DialogInterface.OnClickListener() 
	    {
	    	public void onClick(DialogInterface dialog, int id) 
	    	{
	    		myListener.onKasseCancel();
	    		LogonDialog.this.getDialog().cancel();
	    	}
	    });      

	    serverName = (EditText) view.findViewById(R.id.serverName);
        serverPort = (EditText) view.findViewById(R.id.serverPort);
        password = (EditText) view.findViewById(R.id.password);

        serverName.setText( settings.getString("serverName", "") );
        serverPort.setText( settings.getString("serverPort", "") );

        return builder.create();
	}
	@Override
	public void onAttach( Activity activity )
	{
		super.onAttach(activity);
        // Verify that the host activity implements the callback interface
        settings = activity.getSharedPreferences("KasseConfig", 0);

        try
        {
            // Instantiate the NoticeDialogListener so we can send events to the host
            myListener = (KasseLogonListener) activity;
        }
        catch (ClassCastException e)
        {
            // The activity doesn't implement the interface, throw exception
            throw new ClassCastException(activity.toString()  + " must implement KasseLogonListener");
        }
		
	}

}
