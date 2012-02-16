package rcos.main.recognition;

import android.graphics.Matrix;
import android.graphics.PointF;

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
}