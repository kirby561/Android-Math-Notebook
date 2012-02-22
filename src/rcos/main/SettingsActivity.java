package rcos.main;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

//
//  AndroidMathNotebookActivity
//
//  The main activity of the application.
//
public class SettingsActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.settings);
        
        Spinner spinner = (Spinner) findViewById(R.id.cmbControlStyle);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(
        		this.getApplicationContext(), R.array.control_style_options, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);
        
        setEventHandlers();
    }
    
    // Sets all the event handlers for this activity
    private void setEventHandlers() {

    }
    
    
}