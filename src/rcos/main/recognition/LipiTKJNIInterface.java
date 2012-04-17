package rcos.main.recognition;

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
	}
	
	public LipitkResult[] recognize(Stroke[] strokes) {
		
		
		return null;
	}
	
	// Initializes the LipiTKEngine in native code
	private native void initializeNative(String lipiDirectory, String project);
	
	// Returns a list of results when recognizing the given list of strokes
	private native LipitkResult[] recognizeNative(Stroke[] strokes, int numJStrokes);
}
