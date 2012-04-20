package rcos.main;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;
import android.widget.Toast;

//
//  AndroidMathNotebookActivity
//
//  The main activity of the application.
//
public class AndroidMathNotebookActivity extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        // Load GUI elements
        _canvas = (NotesCanvas)this.findViewById(R.id.Canvas);
        _btnFreehand = (ImageButton)this.findViewById(R.id.btnFreehand);
        _btnRecognition = (ImageButton)this.findViewById(R.id.btnRecognition);
        _btnMath = (ImageButton)this.findViewById(R.id.btnMath);
        _btnUndo = (ImageButton)this.findViewById(R.id.btnUndo);
        _btnRedo = (ImageButton)this.findViewById(R.id.btnRedo);
        
        setEventHandlers();
    }
    
    // Sets all the event handlers for this activity
    private void setEventHandlers() {
        _btnFreehand.setOnClickListener(onBtnFreehandClicked);
        _btnRecognition.setOnClickListener(onBtnRecognitionClicked);
        _btnMath.setOnClickListener(onBtnMathClicked);
        _btnUndo.setOnClickListener(onBtnUndoClicked);
        _btnRedo.setOnClickListener(onBtnRedoClicked);
    }
    
    private void resetModeButtonStates() {
    	_btnFreehand.setImageResource(R.drawable.freehand);
    	_btnRecognition.setImageResource(R.drawable.recognition);
    	_btnMath.setImageResource(R.drawable.math);
    }
    
    // Event handlers
    private View.OnClickListener onBtnFreehandClicked = new View.OnClickListener() {
        public void onClick(View v) {
        	resetModeButtonStates();
            _btnFreehand.setImageResource(R.drawable.freehand_active);
            _canvas.setMode(NotesCanvas.FreehandMode);
        }
    };
    
    private View.OnClickListener onBtnRecognitionClicked = new View.OnClickListener() {
        public void onClick(View v) {
        	resetModeButtonStates();
        	_btnRecognition.setImageResource(R.drawable.recognition_active);
        	_canvas.setMode(NotesCanvas.RecognitionMode);
        }
    };
    
    private View.OnClickListener onBtnMathClicked = new View.OnClickListener() {
		public void onClick(View v) {
        	resetModeButtonStates();
        	_btnMath.setImageResource(R.drawable.math_active);
        	_canvas.setMode(NotesCanvas.MathMode);
		}
    };
    
    private View.OnClickListener onBtnUndoClicked = new View.OnClickListener() {
		public void onClick(View v) {
        	_canvas.undo();
		}
    };
    
    private View.OnClickListener onBtnRedoClicked = new View.OnClickListener() {
		public void onClick(View v) {
        	_canvas.redo();
		}
    };
    
    private void onMnuExitClicked() {
        	// ?? Should save current document here first
        	
        	finish();
    }
    
    private void onMnuSettingsClicked() {
        	// Start the settings activity
            Intent myIntent = new Intent(getApplicationContext(), SettingsActivity.class);
            startActivityForResult(myIntent, 0);
    }

    @Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		_canvas.stop();
		
		super.onPause();
	}

	@Override
	protected void onRestoreInstanceState(Bundle savedInstanceState) {
		_canvas.restoreState(savedInstanceState);	
		
		super.onRestoreInstanceState(savedInstanceState);
	}

	@Override
	protected void onResume() {
		super.onResume();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		_canvas.saveState(outState);
		
		super.onSaveInstanceState(outState);
	}

	@Override
	protected void onStart() {
		
		super.onStart();
	}

	@Override
	protected void onStop() {
		
		super.onStop();
	}

	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
    
        return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.mnuSettings:    
            	onMnuSettingsClicked();
            	break;
            case R.id.mnuExit:     
            	onMnuExitClicked();
                break;
            case R.id.mnuSave: 
            	Toast.makeText(this, "Not yet implemented!", Toast.LENGTH_LONG).show();   
            	break;            
            case R.id.mnuLoad: 
                Toast.makeText(this, "Not yet implemented!", Toast.LENGTH_LONG).show();   
                break;
        }
        return true;
    }
    
    private NotesCanvas _canvas;
    private ImageButton _btnFreehand;
    private ImageButton _btnRecognition;
    private ImageButton _btnMath;
    private ImageButton _btnUndo;
    private ImageButton _btnRedo;
}