package rcos.main;
import android.graphics.Canvas;
import android.view.SurfaceHolder;
 
//
//  DrawingThread
//
// Draws the canvas constantly
// in a background thread
//
public class DrawingThread extends Thread {
    private SurfaceHolder _surfaceHolder;
    private NotesCanvas _canvas;
    private boolean _run = false;
 
    public DrawingThread(SurfaceHolder surfaceHolder, NotesCanvas panel) {
        _surfaceHolder = surfaceHolder;
        _canvas = panel;
    }
 
    public void setRunning(boolean run) {
        _run = run;
    }
 
    @Override
    public void run() {
        Canvas c;
        while (_run) {
            c = null;
            try {
                c = _surfaceHolder.lockCanvas(null);
                synchronized (_surfaceHolder) {
                    _canvas.onDraw(c);
                }
            } finally {
                if (c != null) {
                    _surfaceHolder.unlockCanvasAndPost(c);
                }
            }
        }
    }
}