package rcos.main;

import java.util.Stack;

import rcos.main.commands.Command;

public class CommandManager {
	private int _maxUndos;
	private Stack<Command> _commands;
	private Stack<Command> _redoStack;
	
	public CommandManager(int maxUndos) {
		_maxUndos = maxUndos;
		_commands = new Stack<Command>();
		_redoStack = new Stack<Command>();
		_commands.setSize(_maxUndos);
	}
	
	public void setMaxUndos(int maxUndos) {
		_maxUndos = maxUndos;
		_commands.setSize(maxUndos);		
	}
	
	public int getMaxUndos() {
		return _maxUndos;
	}
	
	public void doCommand(Command command) {
		_commands.push(command);
		command.doCommand();
		_redoStack.clear();
	}
	
	public void undo() {
		boolean undoable = false;
		do {
			if (_commands.size() == 0)
				return;
		
			Command command = _commands.pop();
			command.undo();
			undoable = command.isUndoable();
			_redoStack.push(command);
		} while (!undoable);
	}
	
	public void redo() {
		boolean undoable = false;
		do {
			if (_redoStack.size() == 0)
				return;
		
			Command command = _redoStack.pop();
			_commands.push(command);
			command.doCommand();
			undoable = command.isUndoable();
		} while (!undoable);
	}
}
