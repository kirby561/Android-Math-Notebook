package rcos.main.recognition;

import android.graphics.Matrix;
import android.graphics.PointF;
import android.graphics.RectF;

public class PointMath {
    public static double getDistance(PointF p1, PointF p2) {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return Math.sqrt(dx * dx + dy * dy);
    }

    public static double getSquaredDistance(PointF p1, PointF p2) {
        double dx = p1.x - p2.x;
        double dy = p1.y - p2.y;
        return dx * dx + dy * dy;
    }
    
    public static PointF transformPoint(PointF p, Matrix transform) {
    	float[] point = new float[] {p.x, p.y};
    	transform.mapPoints(point);
    	return new PointF(point[0], point[1]);
    }
    
    // Returns true if the line segment defined by the given two points intersects the given rectangle.
    public static boolean lineSegmentIntersectsRect(PointF p1, PointF p2, RectF r2) {
    	RectF line = new RectF(Math.min(p1.x, p2.x), Math.min(p1.y, p2.y), Math.max(p1.x, p2.x), Math.max(p1.y, p2.y));
    	
    	// If the bounding box of the line segment intersects the rectangle or one contains the other,
    	//    the line segment intersects the rectangle.
    	return RectF.intersects(line, r2) || line.contains(r2) || r2.contains(line);
    }
}