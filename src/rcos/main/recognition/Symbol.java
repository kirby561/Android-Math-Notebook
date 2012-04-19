package rcos.main.recognition;

import java.util.List;

import rcos.main.Stroke;
import android.graphics.PointF;
import android.graphics.Matrix;

public class Symbol {
	private Stroke[] _strokes;
	private String _character;
	
	public Symbol(Stroke[] strokes, String character) {
		_strokes = strokes;
		_character = character;		
	}
	
	public Stroke[] getStrokes() {
		return _strokes;
	}
	
	public String getCharacter() {
		return _character;
	}
}

