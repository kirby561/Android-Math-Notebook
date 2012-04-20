package rcos.main.commands;

public interface Command {
	void doCommand();
	void undo();
	boolean isUndoable();
}
