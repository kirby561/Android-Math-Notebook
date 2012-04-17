package rcos.main.recognition;

import android.graphics.PointF;
import rcos.main.Stroke;

public class LipiTKJNIInterface {
	private String _lipiDirectory;
	private String _project;
	
	static
	{
		try {
			System.loadLibrary("lipitk");	
			System.loadLibrary("jnilink");	
		} catch (Exception ex) {
			System.out.println(ex.getMessage());
		}
	}
	
	//  Initializes the interface with a directory to look for projects in
	//  	the name of the project to use for recognition, and the name
	//  	of the ShapeRecognizer to use.
	public LipiTKJNIInterface(String lipiDirectory, String project) {
		_lipiDirectory = lipiDirectory;
		_project = project;	
	}
	
	public void initialize() {
			initializeNative(_lipiDirectory, _project);
			
			Stroke[] strokes = new Stroke[2];
			Stroke s = new Stroke();
			s.addPoint(new PointF(0.0f,1.0f));
			s.addPoint(new PointF(0.5f,0.0f));
			s.addPoint(new PointF(1.0f,1.0f));
			
			Stroke s2 = new Stroke();
			s2.addPoint(new PointF(0.0f,0.5f));
			s2.addPoint(new PointF(1.0f,0.5f));
			
			strokes[0] = s;
			strokes[1] = s2;
			
			recognizeNative(strokes, strokes.length);
	}
	
	public LipitkResult[] recognize(Stroke[] strokes) {
		
		
		return null;
	}
	
	// Initializes the LipiTKEngine in native code
	private native void initializeNative(String lipiDirectory, String project);
	
	// Returns a list of results when recognizing the given list of strokes
	private native LipitkResult[] recognizeNative(Stroke[] strokes, int numJStrokes);
								  
}
