package rcos.main.recognition;

import java.util.HashMap;
import java.util.Map;

class RecognitionEngine {
	public RecognitionEngine () {
		_symbols = new HashMap<Character, Symbol>();
		_resolution = new Size(100, 100);
	}

	private Map<Character, Symbol> _symbols;
	
	private Size _resolution;
	
	// A constraint for the max X or max Y value to be stored
	// Symbols will be resized to fit inside this "box", with the aspect
	// ratio unchanged
	public Size getResolution() {
		return _resolution;
	}
	
	public void setResolution(Size res) {
		_resolution = res;
	}
	
	public void TrainSymbol(Symbol s) {
		if (!s.IsNormalizedAtResolution(getResolution()))
			s.NormalizeToResolution(getResolution());
		_symbols.put(s.getChar(), s);
	}

	public Character RecognizeSymbol(Symbol s) {
		double smallest = Double.POSITIVE_INFINITY;
		Symbol match = null;

		for (Symbol databaseSymbol : _symbols.values()) {
			double dist = databaseSymbol.getMinDistance(s);

			if (dist < smallest) {
				smallest = dist;
				match = databaseSymbol;
			}
		}

		return match.getChar();
	}
}
