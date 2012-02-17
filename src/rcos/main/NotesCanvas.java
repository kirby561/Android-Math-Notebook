package rcos.main;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.PointF;
import android.graphics.Rect;
import android.hardware.Camera.Size;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Display;
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
	private Stroke[] _background;
	private Matrix _viewTransform = new Matrix();
	private Matrix _invViewTransform = new Matrix();
	private int _mode;

	// Vars for touch events
	private float _origDistance;
	private Matrix _origTransform;
	private Matrix _origInvTransform;
	private Stroke _currentStroke;
	private PointF _zoomCenter;
	private PointF _lastSpot;
	private int _gestureState;

	// Modes
	public final int FreehandMode = 0;
	public final int RecognitionMode = 1;
	public final int MathMode = 2;
	public final int EraseMode = 3;

	// Gesture States
	public final int None = 0;
	public final int Drawing = 1;
	public final int Panning = 2;
	public final int Zooming = 3;

	// Constants
	public final float TranslationPressureThreshold = 0.07f;
	public final int DrawingPointThreshold = 5;

	public Object DrawingLock = new Object();

	public void setMode(int mode) {
		_mode = mode;
	}

	public int getMode() {
		return _mode;
	}

	private void initialize() {
		// Initialize the drawing thread and the strokes array
		_drawingThread = new DrawingThread(getHolder(), this);
		_strokes = new ArrayList<Stroke>();
		_currentStroke = null;
		_gestureState = None;

		_background = getDefaultBackground();

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

	// Returns a list of strokes that creates a standard
	// 8.5x11 inch notebook page
	private Stroke[] getDefaultBackground() {
		// Just draw normal 8.5x11" page at 72 DPI
		Stroke[] result = new Stroke[40];

		// Red line
		Paint red = new Paint();
		red.setStrokeWidth(4);
		red.setColor(Color.RED);
		Stroke redLine = new Stroke();
		redLine.Points.add(new PointF(75,0));
		redLine.Points.add(new PointF(75,11.5f*72));
		redLine.Paint = red;
		result[0] = redLine;

		// Blue lines
		int offset = 88;
		int spacing = 18;
		Paint blue = new Paint();
		blue.setStrokeWidth(1);
		blue.setARGB(255, 122, 195, 240);
		for (int i = 0; i < 39; i++) {
			int y = offset + i * spacing;
			PointF p1 = new PointF(0, y);
			PointF p2 = new PointF(792, y);
			Stroke blueStroke = new Stroke();
			blueStroke.Points.add(p1);
			blueStroke.Points.add(p2);
			blueStroke.Paint = blue;
			result[i+1] = blueStroke;
		}

		return result;
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

	// Updates the inverse if applicable
	public void updateInverseViewTransform() {
		if (_invViewTransform == null) {
			_invViewTransform = new Matrix();
			_viewTransform.invert(_invViewTransform);
		}
	}

	// Transforms p using the inverse view transform and
	// returns the new location
	private PointF getInvViewTransform(PointF p) {
		updateInverseViewTransform();
		float[] pArray = new float[2];
		pArray[0] = p.x;
		pArray[1] = p.y;
		_invViewTransform.mapPoints(pArray);

		return new PointF(pArray[0], pArray[1]);
	}

	// Transforms p using the view transform and
	// returns the new location
	private PointF getViewTransform(PointF p) {
		float[] pArray = new float[2];
		pArray[0] = p.x;
		pArray[1] = p.y;
		_viewTransform.mapPoints(pArray);

		return new PointF(pArray[0], pArray[1]);
	}

	// Applies the given transform to the given point
	private PointF applyTransform(Matrix transform, PointF point) {
		float points[] = new float[2];

		points[0] = point.x;
		points[1] = point.y;

		transform.mapPoints(points);

		return new PointF(points[0], points[1]);
	}

	// Applies the inverse of the given transform to the given point
	private PointF applyInvTransform(Matrix transform, PointF point) {
		float points[] = new float[2];

		points[0] = point.x;
		points[1] = point.y;

		Matrix inv = new Matrix();
		transform.invert(inv);

		inv.mapPoints(points);

		return new PointF(points[0], points[1]);
	}

	// Returns the distance between points p1 and p2
	private float getDistance(PointF p1, PointF p2) {
		float dx = p1.x - p2.x;
		float dy = p1.y - p2.y;

		return (float)Math.sqrt(dx * dx + dy * dy);
	}

	// Returns the difference of p2 - p1
	private PointF getDifference(PointF p1, PointF p2) {
		return new PointF(p2.x - p1.x, p2.y - p1.y);
	}

	// Returns the center of points p1 and p2
	private PointF getCenter(PointF p1, PointF p2) {
		PointF result = new PointF();
		result.x = Math.round((p1.x + p2.x) / 2.0f);
		result.y = Math.round((p1.y + p2.y) / 2.0f);
		return result;
	}

	// Changes the view transform's values to the given ones
	//     and flags the inverse as being dirty.
	public void updateViewTransform(float[] vals) {
		_viewTransform.setValues(vals);
		_invViewTransform = null;
	}

	// Copies the given transform's values into the current 
	//     view transform and flags the inverse as dirty.
	public void updateViewTransform(Matrix viewTransform) {
		_viewTransform = new Matrix(viewTransform);
		_invViewTransform = null;
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int action = event.getActionMasked();
		int pointerCount = event.getPointerCount();

		switch (action) {
		case MotionEvent.ACTION_DOWN:
			synchronized (DrawingLock) {
				updateInverseViewTransform();

				float x = event.getX();
				float y = event.getY();

				// Get the point it occurred at
				PointF p = new PointF(x, y);

				// Put it in page coordinates
				p = getInvViewTransform(p);

				// Check for the width of the pointer
				// to see if we should start panning
				float pressure = event.getPressure();
				if (pressure > TranslationPressureThreshold) {
					_gestureState = Panning;
					_lastSpot = p;
				} else {

					_currentStroke = new Stroke();
					_gestureState = Drawing;

					// Add the point to the current stroke
					_currentStroke.Points.add(p);
				}
			}
			break;
		case MotionEvent.ACTION_MOVE:
			synchronized(DrawingLock) {				
				updateInverseViewTransform();

				// If we are drawing, there's a possibility we
				// might want to pan instead.  Check if the size of
				// their finger has changed since the start and
				// cancel the current stroke/start panning if it is
				// above the threshold.
				float pressure = event.getPressure();

				if (_gestureState == Drawing && pressure > TranslationPressureThreshold) {
					float x = event.getX();
					float y = event.getY();

					// Get the point it occurred at
					PointF p = new PointF(x, y);

					// Put it in page coordinates
					p = getInvViewTransform(p);

					// Set the new state
					_gestureState = Panning;
					_lastSpot = p;
					_currentStroke = null;
				} else if (_gestureState == Zooming && pointerCount == 2) {

					PointF p1 = new PointF(event.getX(0), event.getY(0));
					PointF p2 = new PointF(event.getX(1), event.getY(1));

					// Put these in page coordinates
					p1 = applyTransform(_origInvTransform, p1);
					p2 = applyTransform(_origInvTransform, p2);

					float dx = p2.x - p1.x;
					float dy = p2.y - p1.y;
					float dist = (float)Math.sqrt(dx * dx + dy * dy);
					float ratio = 1;

					if (_origDistance <= 0) {
						_zoomCenter = new PointF((p1.x + p2.x) / 2, (p1.y + p2.y)/ 2);
						_origDistance = dist;
					} else {
						ratio = dist / _origDistance;
					}

					float[] vals = new float[9];
					_origTransform.getValues(vals);

					float initScale = vals[Matrix.MSCALE_X];
					float initTransX = vals[Matrix.MTRANS_X];
					float initTransY = vals[Matrix.MTRANS_Y];

					vals[Matrix.MSCALE_X] = initScale * ratio;
					vals[Matrix.MSCALE_Y] = initScale * ratio;
					vals[Matrix.MTRANS_X] =  initTransX - initScale * (_zoomCenter.x * ratio - _zoomCenter.x);
					vals[Matrix.MTRANS_Y] =  initTransY - initScale * (_zoomCenter.y * ratio - _zoomCenter.y);

					updateViewTransform(vals);

				} else if (_gestureState == Drawing) {

					float x = event.getX();
					float y = event.getY();

					// Get the point it occurred at
					PointF p = new PointF(x, y);

					// Put it in page coordinates
					p = getInvViewTransform(p);

					// Add the point to the current stroke
					_currentStroke.Points.add(p);

				} else if (_gestureState == Panning) {
					float x = event.getX();
					float y = event.getY();

					// Get the point it occurred at
					PointF p = new PointF(x, y);

					// Put it in page coordinates
					p = getInvViewTransform(p);

					PointF diff = getDifference(_lastSpot, p);
					_lastSpot = p;

					_viewTransform.preTranslate(diff.x, diff.y);
					_invViewTransform = null;
				}
			}

			break;
		case MotionEvent.ACTION_UP:
			synchronized (DrawingLock) {
				updateInverseViewTransform();
				
				if (_gestureState == Drawing) {
					float x = event.getX();
					float y = event.getY();

					// Get the point it occurred at
					PointF p = new PointF(x, y);

					// Put it in page coordinates
					p = getInvViewTransform(p);

					// Add the point to the current stroke
					_currentStroke.Points.add(p);

					_strokes.add(_currentStroke);
				} else if (_gestureState == Panning) {
					float x = event.getX();
					float y = event.getY();

					// Get the point it occurred at
					PointF p = new PointF(x, y);

					// Put it in page coordinates
					p = getInvViewTransform(p);

					PointF diff = getDifference(_lastSpot, p);
					_lastSpot = null;

					_viewTransform.preTranslate(diff.x, diff.y);
					_invViewTransform = null;
				}
			}

			_gestureState = None;
			break;

		case MotionEvent.ACTION_POINTER_DOWN:
			synchronized(DrawingLock) {
				updateInverseViewTransform();

				// 2 Fingers down, we're zooming
				_gestureState = Zooming;
				_origDistance = -1;
				_origTransform = new Matrix(_viewTransform);
				_origInvTransform = new Matrix(_invViewTransform);
			}
			break;
		case MotionEvent.ACTION_POINTER_UP:
			if (pointerCount < 2) {
				// Done zooming
				_gestureState = None;
			}
			break;
		}

		return true;


		/*
		int count = event.getPointerCount();

		// If only one pointer is down
		if (count == 1) {
			// Get the action (up/down/move etc..)
			int action = event.getActionMasked();

			synchronized(DrawingLock) {
				updateInverseViewTransform();

				float[] point = {event.getX(), event.getY()};

				float x = event.getX();
				float y = event.getY();

				// Get the point it occurred at
				PointF p = new PointF(x, y);

				// Put it in page coordinates
				p = getInvViewTransform(p);

				// Two fingers close together could be
				// a translation
//				float size = event.getSize();
//				if (size > TranslationPressureThreshold && _currentStroke == null) {
//					if (_lastSpot == null)
//						_lastSpot = new PointF(x, y);
//					PointF spot = new PointF(x, y);
//					PointF diff = getDifference(_lastSpot, spot);
//					
//					_viewTransform.preTranslate(diff.x, diff.y);
//					
//					if (action == MotionEvent.ACTION_UP)
//						_lastSpot = null;	
//					else
//						_lastSpot = spot;
//				} else {
					if (_lastSpot != null) {
						_lastSpot = null;
						return true;
					}

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
					if (action == MotionEvent.ACTION_UP) {
						_currentStroke = null;	
					}

					_origCenter = null;
				}
//			}	

			return true;
		} else if (count == 2) {
			// Manipulate the view
			synchronized(DrawingLock) {
				// Destroy any current lines being drawn -- we're moving the view now
				if (_currentStroke != null) {
					_strokes.remove(_currentStroke);
					_currentStroke = null;
				}


				float x1 = event.getX(0);
				float y1 = event.getY(0);
				float x2 = event.getX(1);
				float y2 = event.getY(1);

				PointF[] curPoints = new PointF[2];
				if (_origTransform == null) {
					curPoints[0] = applyInvTransform(_viewTransform, new PointF(x1, y1));
					curPoints[1] = applyInvTransform(_viewTransform, new PointF(x2, y2));
				} else {
					curPoints[0] = applyInvTransform(_origTransform, new PointF(x1, y1));
					curPoints[1] = applyInvTransform(_origTransform, new PointF(x2, y2));
				}

				// First two pointer event?
				if (_origCenter == null) {
					_origCenter = getCenter(curPoints[0], curPoints[1]);
					_origDistance = getDistance(curPoints[0], curPoints[1]);
					_origTransform = new Matrix();
					_origTransform.set(_viewTransform);
					return true;
				}

				// Calculate distance and center
				PointF center = getCenter(curPoints[0], curPoints[1]);
				float distance = getDistance(curPoints[0], curPoints[1]);

				// Calculate the new scale
				float scale = (float)distance / _origDistance;

				// Calculate translation
				//PointF translation = getDifference(_origCenter, center);

				// Update the view transform
				_viewTransform.set(_origTransform);
				//_viewTransform.preTranslate(translation.x, translation.y);
				_viewTransform.preScale(scale, scale);

				// Make a new inverse be calculated when needed
				_invViewTransform = null;
			}

			return true;
		} else {
			_origCenter = null;
		}

		return false;

		 */
	}

	// Draws the given stroke on the given canvas
	// uses the view transform to map the points to where 
	// they should be on the canvas
	private void drawStroke(Canvas canvas, Stroke stroke) {
		if (stroke.Points.size() < 2)
			return;

		for (int pIndex = 0; pIndex < stroke.Points.size() - 1; pIndex++) {
			float points[] = new float[4];
			PointF p1 = stroke.Points.get(pIndex);
			PointF p2 = stroke.Points.get(pIndex + 1);

			points[0] = p1.x;
			points[1] = p1.y;
			points[2] = p2.x;
			points[3] = p2.y;

			_viewTransform.mapPoints(points);

			canvas.drawLine(points[0], points[1], points[2], points[3], stroke.Paint);
		}
	}

	@Override
	public void onDraw(Canvas canvas) {		
		// Draw the data
		synchronized(DrawingLock) {			
			// Draw the background
			Paint whiteFill = new Paint();
			whiteFill.setStyle(Style.FILL_AND_STROKE);
			whiteFill.setColor(Color.WHITE);
			canvas.drawRect(new Rect(0,0,canvas.getWidth(), canvas.getHeight()), whiteFill);
			for (Stroke stroke : _background)
				drawStroke(canvas, stroke);

			// Just draw all the strokes we're keeping track of for now
			for (Stroke stroke : _strokes)
				drawStroke(canvas, stroke);

			if (_gestureState == Drawing)
				drawStroke(canvas, _currentStroke);
		}

	}

}
