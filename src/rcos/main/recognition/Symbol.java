package rcos.main.recognition;

import java.util.List;
import android.graphics.PointF;
import android.graphics.Matrix;

class Symbol {
	public Symbol() {
		_transform = new Matrix();
		_isNormalized = false;
	}

	private List<PointF> _points;
	private boolean _isNormalized;
	private Size _resolution;
	private Character _char;
    private Matrix _transform;
	
	public Character getChar() {
		return _char;
	}
	
	public void setChar(char c) {
		_char = c;
	}

	public List<PointF> getPoints() { return _points; }

	public void setPoints(List<PointF> points) { _points = points; }

	public Matrix getTransform() {
		return _transform;
	}
	
	public void setTransform(Matrix transform) {
		_transform = transform;		
	}

	public double getMinDistance(Symbol other) {
		if (_isNormalized && !other.IsNormalizedAtResolution(_resolution))
			other.NormalizeToResolution(_resolution);

		double total = 0.0;
		List<PointF> biggerPoints;
		Matrix bigTrans;
		List<PointF> smallerPoints;
		Matrix smallTrans;

		if (other.getPoints().size() > getPoints().size()) {
			biggerPoints = other.getPoints();
			bigTrans = other.getTransform();
			smallerPoints = getPoints();
			smallTrans = getTransform();
		} else {
			biggerPoints = getPoints();
			bigTrans = getTransform();
			smallerPoints = other.getPoints();
			smallTrans = other.getTransform();
		}

		// Just do an exaustive search for now
		for (PointF p : biggerPoints) {
			double curDist = Double.POSITIVE_INFINITY;
			PointF otherPoint = PointMath.transformPoint(p, bigTrans);

			for (PointF p2 : smallerPoints) {
				PointF curPoint = PointMath.transformPoint(p2, smallTrans);
				double dist = PointMath.getSquaredDistance(otherPoint, curPoint) / biggerPoints.size();
				if (dist < curDist)
					curDist = dist;
			}

			total += curDist;
		}

		return total;
	}

	public boolean IsNormalizedAtResolution(Size resolution) {
		return _isNormalized && (resolution.equals(_resolution));
	}

	public void NormalizeToResolution(Size resolution) {
		_resolution = resolution;
		double minX = Double.MAX_VALUE;
		double minY = Double.MAX_VALUE;
		double maxX = Double.MIN_VALUE;
		double maxY = Double.MIN_VALUE;

		List<PointF> points = getPoints();

		// Get extremes
		for (PointF k : points) {
			minX = Math.min(k.x, minX);
			minY = Math.min(k.y, minY);
			maxX = Math.max(k.x, maxX);
			maxY = Math.max(k.y, maxY);
		}

		double dx = maxX - minX;
		double dy = maxY - minY;
		double scaleX = resolution.Width / dx;
		double scaleY = resolution.Height / dy;
		double uniformScaleFactor = Math.max(scaleX, scaleY);

		Matrix transform = new Matrix();
		transform.setScale((float)uniformScaleFactor, (float)uniformScaleFactor);
		setTransform(transform);

		// Normalize
		for (int i = 0; i < points.size(); i++ ) {
			PointF k = points.get(i);
			k.x -= minX;
			k.y -= minY;
			points.add(i, k);
		}

		_isNormalized = true;
	}
}

