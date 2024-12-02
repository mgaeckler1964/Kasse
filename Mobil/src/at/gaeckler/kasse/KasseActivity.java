/*
		Project:		KASSE
		Module:			KasseActivity.java
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

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import android.app.ActionBar.LayoutParams;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.FragmentManager;
import android.content.DialogInterface;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.GridView;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;

class ArticelRecord
{
	public String	Name, Group, EanCode;
	public double	normalPrice, partyPrice;
	public int		count = 0;
	
	public String toString()
	{
		String result = Name;
		if( count > 0 )
		{
			result += " (";
			result += Integer.toString(count);
			result += ')';
		}
		return result;
	}
}

class KassenThread extends Thread
{
	String 	serverName;
	int 	port;
	String 	password;
	
	String	myURL;
	
	KassenThread( String serverName, int port, String password )
	{
		this.serverName = serverName;
		this.port = port;
		this.password = password;
		
		myURL = "http://" + serverName + ":" + Integer.toString(port) + "/" + password;
	}
}
class receiverThread extends KassenThread
{
	receiverThread(String serverName, int port, String password)
	{
		super(serverName, port, password);
	}

	List<ArticelRecord>	articles = new ArrayList<ArticelRecord>();
	List<String>		groups = new ArrayList<String>();
	
	private String[] parseCSVLine( String line )
	{
		List<String>	fields = new ArrayList<String>();
		boolean			inString = false;
		String			field = "";
		
		for( int i=0; i<line.length(); i++ )
		{
			char c=line.charAt( i );
			if( c == '"' )
				inString = !inString;
			else if( c == ',' && !inString )
			{
				fields.add( field );
				field = "";
			}
			else
				field = field + c;
		}
		fields.add( field );
		
		return (String[])fields.toArray( new String[0] );
	}

	public void run()
	{
		try
		{
			String [] fields;
			
			URL	url = new URL( myURL);
			HttpURLConnection con = (HttpURLConnection) url.openConnection();
			
			con.setReadTimeout( 20000 );
			InputStream iStream = con.getInputStream();
			InputStreamReader isReader = new InputStreamReader( iStream );
			BufferedReader br  = new BufferedReader( isReader );
			boolean first = true;

			String line;
			while( (line = br.readLine()) != null )
			{
				if( !line.equals("") )
				{
					if( !first )
					{
						fields = parseCSVLine( line );
						if( fields.length == 6 && !fields[0].equals("") && !fields[3].equals("") )
						{
							ArticelRecord newRec = new ArticelRecord();
							newRec.Name = fields[0];
							newRec.Group = fields[1];
							newRec.EanCode = fields[3];
							newRec.normalPrice = Double.parseDouble(fields[4]);
							newRec.partyPrice = Double.parseDouble(fields[5]);
							
							articles.add( newRec );
						}
					}
					else
						first = false;
				}
			}
			for( int i=0; i<articles.size(); i++ )
			{
				ArticelRecord newRec = articles.get( i );
				String groupName = newRec.Group;
				if( !groups.contains(groupName) )
					groups.add( groupName );
				
			}
		}
		catch( Exception e )
		{
			Log.d("MyApp",e.toString());
		}
	}

}

class senderThread extends KassenThread
{
	boolean						success = false;
	List<ArticelRecord>			orderArticles;
	
	public senderThread( String serverName, int port, String password, List<ArticelRecord> newOrderArticles )
	{
		super(serverName, port, password);
		orderArticles = newOrderArticles;
	}
	public void run()
	{
		try
		{
			String	result = "";
			for( int i=0; i<orderArticles.size(); i++)
			{
				ArticelRecord theArticle = orderArticles.get( i );
				
				result += theArticle.EanCode;
				result += ',';
				result += Integer.toString( theArticle.count );
				result += "\r\n";
			}
			byte[] buffer = result.getBytes();

			URL	url = new URL( myURL);
			HttpURLConnection con = (HttpURLConnection) url.openConnection();

			con.setRequestMethod("POST");
			con.setRequestProperty("User-Agent", "CresdKasse");
			con.setRequestProperty("Connection", "close");
			con.setRequestProperty("Content-Type", "application/kasse.order");
			con.setRequestProperty(
					"Content-Length",
					Integer.toString(buffer.length)
				 );
			con.setDoOutput( true );
			OutputStream os = con.getOutputStream();
			os.write( buffer );
			os.close();
			
			Log.d("MyApp","getResponse");
			int rc = con.getResponseCode();
			if (rc != 200 )
			{
				Log.d("MyApp","baeh");
				Log.d("MyApp",con.getResponseMessage());
			}
			else
				success = true;
		}
		catch( Exception e )
		{
			Log.d("MyApp",e.toString());
		}
	}
}

public class KasseActivity extends Activity implements LogonDialog.KasseLogonListener
{

	List<ArticelRecord>			articles;

	ArrayAdapter<ArticelRecord>	currentAdapter;
	List<ArticelRecord>			currentArticles;
	GridView 					currentArticlesGrid;

	List<String>				groups;
	TextView					textViewSum;

	ArrayAdapter<ArticelRecord>	orderAdapter;
	List<ArticelRecord>			orderArticles = null;
	ListView					listViewOrder;
	
	double						totalPrice = 0;
	
	Button						buttonFinish;
	
	LogonDialog					logonDialog;

	String 						serverName;
	int 						port;
	String 						password;

	private void showAlert( String errorText )
	{
    	AlertDialog.Builder builder = new AlertDialog.Builder( this );
    	builder.setMessage(errorText)
    		   .setTitle("Cresd KASSE")
    	       .setCancelable(false)
    	       .setNegativeButton("OK", new DialogInterface.OnClickListener() {
    	           public void onClick(DialogInterface dialog, int id) {
    	                dialog.cancel();
    	                finish();
    	           }
    	       });
    	AlertDialog alert = builder.create();
    	alert.show();
	}
    private void showLogonDialog() {
        FragmentManager fm = getFragmentManager();
        logonDialog		= new LogonDialog();
        logonDialog.show(fm, "kasse_logon");
    }

	void updateGridView( String group )
	{
		if( group.equals("Leer"))
			group = "";
		
		currentArticles = new ArrayList<ArticelRecord>();
		for( int i=0; i<articles.size(); i++ )
		{
			ArticelRecord article = articles.get(i);
			if( article.Group.equals(group))
				currentArticles.add(article);
		}
		currentAdapter = new ArrayAdapter<ArticelRecord>( this, android.R.layout.simple_list_item_1, currentArticles );
		currentArticlesGrid.setAdapter( currentAdapter );
	}
	void addOrder( int index )
	{
		if( orderArticles == null )
		{
			orderArticles = new ArrayList<ArticelRecord>();
			orderAdapter = new ArrayAdapter<ArticelRecord>( this, android.R.layout.simple_list_item_1, orderArticles );
			listViewOrder.setAdapter(orderAdapter);
		}
		ArticelRecord theArticle = currentArticles.get(index);
		if( !orderArticles.contains(theArticle) )
		{
			theArticle.count = 1;
			orderArticles.add( theArticle );
		}
		else
			theArticle.count++;
		
		totalPrice += theArticle.partyPrice;
		
		orderAdapter.notifyDataSetChanged();
		currentAdapter.notifyDataSetChanged();
		
		textViewSum.setText(Double.toString(totalPrice));
	}
	void removeOrder( int index )
	{
		ArticelRecord theArticle = orderArticles.get(index);

		totalPrice -= theArticle.partyPrice * theArticle.count;
		theArticle.count = 0;
		orderArticles.remove( index );

		orderAdapter.notifyDataSetChanged();
		currentAdapter.notifyDataSetChanged();
		
		textViewSum.setText(Double.toString(totalPrice));
	}
	void finishOrder( )
	{
		senderThread theSender = new senderThread( serverName, port, password, orderArticles );

		try
		{
			theSender.start();
			if( theSender.isAlive() )
				theSender.join(20000);
		}
		catch (InterruptedException e)
		{
			e.printStackTrace();
		}

		if( theSender.success )
		{
			for( int i=orderArticles.size()-1; i>=0; i-- )
			{
				ArticelRecord theArticle = orderArticles.get( i );
				theArticle.count = 0;
				orderArticles.remove( i );
			}
	
			totalPrice = 0;
			orderAdapter.notifyDataSetChanged();
			currentAdapter.notifyDataSetChanged();
			
			textViewSum.setText(Double.toString(totalPrice));
		}
		else
			showAlert( "Fehler beim Senden!" );
	}
	void loadArticles(String serverName, int port, String password) throws InterruptedException
	{
		receiverThread theReceiver = new receiverThread(serverName, port, password);

		theReceiver.start();
		if( theReceiver.isAlive() )
			theReceiver.join( 20000 );
		articles = theReceiver.articles;
		groups = theReceiver.groups;
	}
	@Override
	public void onKasseLogon( String serverName, int port, String password )
	{
		this.serverName = serverName;
		this.port = port;
		this.password = password;

		try
		{
			loadArticles(serverName, port, password);
		}
		catch( InterruptedException e )
		{
			showAlert( "Fehler beim Laden der Artikel" );
		}
	
		if( groups.size() == 0 )
		{
			showAlert( "Fehler beim Laden der Artikel. Nichts erhalten." );
		}
		else
		{
			LinearLayout groupButtonsLayout = (LinearLayout)findViewById(R.id.groupButtons);
			int buttonWidth=groupButtonsLayout.getWidth()/groups.size(); 
		
			String first = "";
			for( int i=0; i<groups.size(); i++ )
			{
				String groupName = groups.get(i);
				if( groupName.equals("") )
					groupName = "Leer";
				if( i==0 )
					first = groupName;
					
				Button myButton = new Button(this);
				LayoutParams lp = new LayoutParams(buttonWidth, groupButtonsLayout.getHeight());
				myButton.setId( i );
				myButton.setText(groupName);
				groupButtonsLayout.addView(myButton, lp);
				Log.d("MyApp",groupName);
				myButton.setOnClickListener( new Button.OnClickListener(){
					public void onClick( View v )
					{
						Button btn = (Button)v;
						updateGridView( (String) btn.getText() );
					}
				});
			}
			updateGridView( first );
		}
	}
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		currentArticlesGrid = (GridView)findViewById(R.id.currentArticlesGrid);
		currentArticlesGrid.setOnItemClickListener(new AdapterView.OnItemClickListener() 
		{
	        @Override
	        public void onItemClick(AdapterView<?> parent, View v, int position, long id) 
	        {
	            addOrder( position );
	        }
		});
		listViewOrder = (ListView)findViewById(R.id.listViewOrder);
		listViewOrder.setOnItemClickListener(new AdapterView.OnItemClickListener() 
		{
	        @Override
	        public void onItemClick(AdapterView<?> parent, View v, int position, long id) 
	        {
	            removeOrder( position );
	        }
		});
		textViewSum = (TextView)findViewById( R.id.textViewSum );
		textViewSum.setText("0.0");

		buttonFinish = (Button)findViewById( R.id.buttonFinish );
		buttonFinish.setOnClickListener( new Button.OnClickListener()
		{
			public void onClick( View v )
			{
				finishOrder();
			}
		});

		showLogonDialog();
		
		

	}
	@Override
	public void  onConfigurationChanged (Configuration newConfig)
	{
		super.onConfigurationChanged( newConfig );
		
	}
	@Override
	public void onKasseCancel() 
	{
		finish();
	}
}
