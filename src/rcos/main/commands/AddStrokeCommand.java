package rcos.main.commands;

import rcos.main.Page;
import rcos.main.Stroke;

public class AddStrokeCommand implements Command {
	private Page _page;
	private Stroke _stroke;
	private boolean _undoable = true;
	
	public AddStrokeCommand(Stroke stroke, Page page) {
		_page = page;
		_stroke = stroke;
	}
	
	public AddStrokeCommand(Stroke stroke, Page page, boolean isUndoable) {
		_page = page;
		_stroke = stroke;
		_undoable = isUndoable;
	}
	
	public void doCommand() {
		_page.addStroke(_stroke);
	}

	public void undo() {
		_page.removeStroke(_stroke);
	}

	public boolean isUndoable() {
		return _undoable;
	}
	
}
