package rcos.main;

import java.util.ArrayList;
import android.graphics.Paint;
import android.graphics.PointF;
import android.graphics.RectF;

//
// Stroke
//
// Keeps track of a list of points
// and how they should be drawn
//
public class Stroke {
	public Stroke() {
		_points = new ArrayList<PointF>();
		_paint = new Paint();
		
		// Just use a random color for now
		_paint.setARGB(255, getRandColor(), getRandColor(), getRandColor());
	}
	
	private int getRandColor() {
		int rand = (int)Math.round(Math.random() * 255);
		return rand;
	}
	
	// Adds the given point to this stroke
	public void addPoint(PointF point) {
		_points.add(point);
		
		addPointToBoundingBox(point);
	}
	
	// Expands the bounding box to accommodate the given point if necessary
	private void addPointToBoundingBox(PointF point) {
		if (_boundingBox == null) {
			_boundingBox = new RectF(point.x, point.y, point.x, point.y);
			return;
		}
		
		// Expand the bounding box to include it, if necessary
		_boundingBox.union(point.x, point.y);		
	}

	public ArrayList<PointF> getPoints() {
		return _points;
	}
	
	public int getNumberOfPoints() {
		return _points.size();
	}
	
	public PointF getPointAt(int index) {
		return _points.get(index);
	}
	
	public RectF getBoundingBox() {
		return _boundingBox;
	}
	
	public Paint getPaint() {
		return _paint;
	}
	
	public void setPaint(Paint p) {
		_paint = p;
	}
	
	// The list of points in this stroke
	private ArrayList<PointF> _points;
	
	private RectF _boundingBox = null;
	
	// The paint to use when drawing this stroke
	private Paint _paint;
}
