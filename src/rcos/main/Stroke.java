package rcos.main;

import java.util.ArrayList;
import android.graphics.Paint;
import android.graphics.Point;

//
// Stroke
//
// Keeps track of a list of points
// and how they should be drawn
//
public class Stroke {
	public Stroke() {
		Points = new ArrayList<Point>();
		Paint = new Paint();
		
		// Just use a random color for now
		Paint.setARGB(255, getRandColor(), getRandColor(), getRandColor());
	}
	
	private int getRandColor() {
		int rand = (int)Math.round(Math.random() * 255);
		return rand;
	}
	
	// The list of points in this stroke
	public ArrayList<Point> Points;
	
	// The paint to use when drawing this stroke
	public Paint Paint;
}
