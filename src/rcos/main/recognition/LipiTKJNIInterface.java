package rcos.main.recognition;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;

import android.graphics.PointF;
import android.util.Log;
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
	
	public String getSymbolName(int id,String project_config_dir)
	{
		String line;
		int temp;
		String [] splited_line= null;
		try
		{
			File map_file = new File(project_config_dir+"unicodeMapfile_alphanumeric.ini");
			BufferedReader readIni = new BufferedReader(new FileReader(map_file));
			readIni.readLine();
			readIni.readLine();
			readIni.readLine();
			readIni.readLine();
			while((line=readIni.readLine())!=null)
			{
				splited_line = line.split(" ");
				Log.d("JNI_LOG","split 0="+splited_line[0]);
				Log.d("JNI_LOG","split 1="+splited_line[1]);
				splited_line[0] = splited_line[0].substring(0, splited_line[0].length()-1); //trim out = sign
				if(splited_line[0].equals((new Integer(id)).toString()))
				{
					splited_line[1] = splited_line[1].substring(2);
					temp = Integer.parseInt(splited_line[1], 16);
					return String.valueOf((char)temp);
				}
			}
		}
		catch(Exception ex)
		{
			Log.d("JNI_LOG","Exception in getSymbolName Function"+ex.toString());
			return "-1";
		}
		return "0";
	}
	
	public void initialize() {
		initializeNative(_lipiDirectory, _project);
	}
	
	public LipitkResult[] recognize(Stroke[] strokes) {
		LipitkResult[] results = recognizeNative(strokes, strokes.length);
		
		for (LipitkResult result : results)
			Log.d("jni", "ShapeID = " + result.Id + " Confidence = " + result.Confidence);			
		
		return results;
	}
	
	// Initializes the LipiTKEngine in native code
	private native void initializeNative(String lipiDirectory, String project);
	
	// Returns a list of results when recognizing the given list of strokes
	private native LipitkResult[] recognizeNative(Stroke[] strokes, int numJStrokes);
								  
}
