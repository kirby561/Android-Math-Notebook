package rcos.main;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.Point;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

//
//  NotesCanvas
//
//  The main drawing surface in the application.
//  Handles touch events and draws strokes at the moment.
//
public class NotesCanvas extends SurfaceView implements SurfaceHolder.Callback {	
	private DrawingThread _drawingThread;
	private ArrayList<Stroke> _strokes;
	private Stroke _currentStroke;
	
	private void initialize() {
		// Initialize the drawing thread and the strokes array
		_drawingThread = new DrawingThread(getHolder(), this);
        _strokes = new ArrayList<Stroke>();
        _currentStroke = null;
        
        // Register for surface callbacks
		getHolder().addCallback(this);
        setFocusable(true);
	}
	
	public NotesCanvas(Context context) {
		super(context);
		initialize();
	}

	public NotesCanvas(Context context, AttributeSet attrs) {
		super(context, attrs);
		initialize();
	}

	public NotesCanvas(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		initialize();
	}

	public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
		
	}

	public void surfaceCreated(SurfaceHolder holder) {
		// Start the drawing thread
		_drawingThread.setRunning(true);
        _drawingThread.start();
	}

	public void surfaceDestroyed(SurfaceHolder holder) {
		// Clean up the drawing thread
        boolean retry = true;
        _drawingThread.setRunning(false);
        while (retry) {
                try {
                    _drawingThread.join();
                    retry = false;
                } catch (InterruptedException e) {
                	// Keep trying
                }
        }
	}
	
    @Override
	public boolean onTouchEvent(MotionEvent event) {
		int count = event.getPointerCount();
		
		// If only one pointer is down
		if (count == 1) {
			// Get the action (up/down/move etc..)
			int action = event.getActionMasked();
			
			// Get the point it occurred at
			Point p = new Point((int)Math.round(event.getX()), (int)Math.round(event.getY()));
			
			synchronized(_strokes) {
				// If there is no current stroke,
				// this must be an ACTION_DOWN, so create
				// one and add it to the list
				if (_currentStroke == null) {
					_currentStroke = new Stroke();
					_strokes.add(_currentStroke);
				}
				
				// Add the point to the current stroke
				_currentStroke.Points.add(p);
				
				// If this was an ACTION_UP, clear the current stroke
				if (action == MotionEvent.ACTION_UP) 
					_currentStroke = null;				
			}

			return true;
		}
		
		return false;
	}

	@Override
    public void onDraw(Canvas canvas) {
		synchronized(_strokes) {
			// Just draw all the strokes we're keeping track of for now
			for (Stroke stroke : _strokes) {
				if (stroke.Points.size() < 2)
					continue;
    		
				for (int pIndex = 0; pIndex < stroke.Points.size() - 1; pIndex++) {
					Point p1 = stroke.Points.get(pIndex);
					Point p2 = stroke.Points.get(pIndex + 1);
					canvas.drawLine(p1.x, p1.y, p2.x, p2.y, stroke.Paint);
				}
			}
		}
    }

}
