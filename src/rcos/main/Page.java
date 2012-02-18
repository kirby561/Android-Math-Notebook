package rcos.main;

import java.util.ArrayList;

import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PointF;

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
