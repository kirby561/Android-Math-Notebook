package rcos.main;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.View;
import android.widget.ImageButton;

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
        
        setEventHandlers();
    }
    
    // Sets all the event handlers for this activity
    private void setEventHandlers() {
        _btnFreehand.setOnClickListener(onBtnFreehandClicked);
        _btnRecognition.setOnClickListener(onBtnRecognitionClicked);
        _btnMath.setOnClickListener(onBtnMathClicked);
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
    
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);
        return true;
    }
    
    private NotesCanvas _canvas;
    private ImageButton _btnFreehand;
    private ImageButton _btnRecognition;
    private ImageButton _btnMath;
}