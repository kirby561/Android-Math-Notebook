package rcos.main;

import java.util.ArrayList;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.os.Bundle;
import android.util.Log;
import rcos.main.recognition.Symbol;
import rcos.main.recognition.LipiTKJNIInterface;
import rcos.main.recognition.LipitkResult;

// A page contains what strokes to draw,
//    and all the information about that
//    notbook page.
public class Page {
	private static final long RecognitionTimeout = 400; // milliseconds

	private ArrayList<Stroke> _strokes;
	private Stroke[] _recognitionStrokes;
	public Object StrokesLock = new Object();

	private Stroke[] _background;
	private ArrayList<Symbol> _symbols;
	private Timer _timer;
	private LipiTKJNIInterface _recognizer;

	public Page(LipiTKJNIInterface recognizer) {
		_strokes = new ArrayList<Stroke>();
		_symbols = new ArrayList<Symbol>();
		_background = getDefaultBackground();
		_timer = null;
		_recognizer = recognizer;
	}

	public Page(Bundle page) {		
		_strokes = new ArrayList<Stroke>();

		// Restore the strokes
		int count = page.getInt("StrokeCount");
		for (int i = 0; i < count; i++) 
			_strokes.add(new Stroke(page.getBundle("Stroke" + i)));

		// Bundle the background
		int backgroundCount = page.getInt("BackgroundCount");
		_background = new Stroke[backgroundCount];
		for (int i = 0; i < _background.length; i++)
			_background[i] = new Stroke(page.getBundle("BackgroundStroke" + i));
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
		redLine.addPoint(new PointF(75,0));
		redLine.addPoint(new PointF(75,11.5f*72));
		redLine.setPaint(red);
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
			blueStroke.addPoint(p1);
			blueStroke.addPoint(p2);
			blueStroke.setPaint(blue);
			result[i+1] = blueStroke;
		}

		return result;
	}

	public Bundle bundle() {
		Bundle result = new Bundle();

		// Bundle the strokes
		result.putInt("StrokeCount", _strokes.size());
		for (int i = 0; i < _strokes.size(); i++) 
			result.putBundle("Stroke" + i, _strokes.get(i).bundle());

		// Bundle the background
		result.putInt("BackgroundCount", _background.length);
		for (int i = 0; i < _background.length; i++)
			result.putBundle("BackgroundStroke" + i, _background[i].bundle());

		return result;
	}

	public void addStroke(Stroke stroke) {
		synchronized (StrokesLock) {
			_strokes.add(stroke);

			// Start a timer for recognition (or restart it if it's going)
			if (_timer != null) {
				_timer.cancel();
				_timer.purge();
				_timer = null;

				for (Stroke s : _recognitionStrokes)
					_strokes.add(s);
			}
			
			_recognitionStrokes = new Stroke[_strokes.size()];
			for (int s = 0; s < _strokes.size(); s++)
				_recognitionStrokes[s] = _strokes.get(s);
			_strokes.clear();

			TimerTask task = new TimerTask() {
				@Override
				public void run() {
					synchronized (StrokesLock) {
						// Recognize our current strokes
						LipitkResult[] results = _recognizer.recognize(_recognitionStrokes);
						
						for (LipitkResult result : results) {
							Log.e("jni", "ShapeID = " + result.Id + " Confidence = " + result.Confidence);			
						}
						
						// ?? Replace this with the symbol function: getSymbol(results[0].Id)
						String configFileDirectory = _recognizer.getLipiDirectory() + "/projects/alphanumeric/config/";
						String character = _recognizer.getSymbolName(results[0].Id, configFileDirectory);
						 
						// Make a symbol out of these strokes
						Symbol s = new Symbol(_recognitionStrokes, character);
						_symbols.add(s);
					}
				}		
			};
			
			_timer = new Timer();
			_timer.schedule(task, RecognitionTimeout);
		}
	}

	public ArrayList<Symbol> getSymbols() {
		return _symbols;		
	}

	public Stroke[] getBackground() {
		return _background;
	}

	public ArrayList<Stroke> getStrokes() {
		return _strokes;
	}
}
