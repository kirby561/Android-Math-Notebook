package rcos.main.recognition;

// Simple structure to keep track of a
// double precision size
public class Size {
	public Size(double width, double height) { 
		Width = width; 
		Height = height; 
	}
	
	@Override
	public boolean equals(Object o) {
		if (o instanceof Size) {
			Size other = (Size)o;
			return other.Width == Width && other.Height == Height;
		}
		return false;
	}

	double Width;
	double Height;
}
