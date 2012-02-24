package rcos.main;

import java.io.Serializable;
import java.util.ArrayList;
import android.graphics.Paint;
import android.graphics.PointF;
import android.graphics.RectF;
import android.os.Bundle;
import android.os.Parcelable;

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
	
	/// Debundles the given stroke
	public Stroke(Bundle stroke) {
		this();
		
		// Restore the points from the bundle
		int pointCount = stroke.getInt("PointCount");
		for (int i = 0; i < pointCount; i++) {
			float x = stroke.getFloat("PointX" + i);
			float y = stroke.getFloat("PointY" + i);
			addPoint(new PointF(x,y));
		}
		
		// Restore the Paint from the bundle
		_paint.setColor(stroke.getInt("PaintColor"));
		_paint.setStrokeWidth(stroke.getFloat("PaintStrokeWidth"));
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
	
	/// Bundles this stroke
	public Bundle bundle() {
		Bundle result = new Bundle();
		
		// Write the points into the bundle
		result.putInt("PointCount", getNumberOfPoints());
		for (int i = 0; i < getNumberOfPoints(); i++) {
			result.putFloat("PointX" + i, _points.get(i).x);
			result.putFloat("PointY" + i, _points.get(i).y);
		}
		
		// Write the Paint into the bundle
		result.putInt("PaintColor", _paint.getColor());
		result.putFloat("PaintStrokeWidth", _paint.getStrokeWidth());
		
		return result;
	}
	
	// The list of points in this stroke
	private ArrayList<PointF> _points;
	
	private RectF _boundingBox = null;
	
	// The paint to use when drawing this stroke
	private Paint _paint;
}
