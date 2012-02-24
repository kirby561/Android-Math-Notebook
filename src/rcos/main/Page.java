package rcos.main;

import java.util.ArrayList;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;
import android.os.Bundle;

// A page contains what strokes to draw,
//    and all the information about that
//    notbook page.
public class Page {
	private ArrayList<Stroke> _strokes;
	private Stroke[] _background;
	
	public Page() {
		_strokes = new ArrayList<Stroke>();
		_background = getDefaultBackground();
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
		_strokes.add(stroke);
	}
	
	public Stroke[] getBackground() {
		return _background;
	}
	
	public ArrayList<Stroke> getStrokes() {
		return _strokes;
	}
}
