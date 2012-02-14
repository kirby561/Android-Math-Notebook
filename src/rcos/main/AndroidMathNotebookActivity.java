package rcos.main;

import android.app.Activity;
import android.os.Bundle;

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
        
        _canvas = (NotesCanvas)this.findViewById(R.id.Canvas);
        _canvas.invalidate();
    }
    
    private NotesCanvas _canvas;
}