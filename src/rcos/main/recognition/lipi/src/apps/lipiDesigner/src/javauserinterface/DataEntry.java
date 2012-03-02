/*****************************************************************************************
 * Copyright (c) 2009 Hewlett-Packard Development Company, L.P.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all 
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *****************************************************************************************/
/******************************************************************************
 * SVN MACROS
 *
 * $LastChangedDate: 2009-03-20 16:33:02 +0530 (Fri, 20 Mar 2009) $
 * $Revision: 743 $
 * $Author: mnab $
 *
 ******************************************************************************/
/************************************************************************
 * FILE DESCR	: DataEntry  is a class which creates the Writing area to enter the data sample
 * CONTENTS		:
 *			clean
 *			actionPerformed
 *			mousePressed
 *			mouseDragged
 *          getLipiTKStroke
 *          mouseReleased
 *          mouseClicked
 *          mouseEntered
 *          mouseMoved
 *          mouseExited
 *			paintComponent
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.Vector;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class DataEntry extends JPanel  implements MouseListener, MouseMotionListener, ActionListener
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: DataEntry
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: Creates the data structures required to operate on points
	 *			      Adds the MouseListener functions to this class
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public DataEntry()
	{
		LipiTkUI.DEBUG("Enter Into DataEntry Class::Constructor Function");
		try
		{
			lipiTKStroke = new LipiTKStroke();
			xCoordinate = new Vector();
			yCoordinate = new Vector();
			addMouseListener(this);
			addMouseMotionListener(this);
			setBackground(Color.white);
			title = new JLabel("");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in DataEntry class from Constructor function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From DataEntry Class::Constructor Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: clean
	 * DESCRIPTION	: This function clears all the data structures
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void clean()
	{
		LipiTkUI.DEBUG("Enter Into DataEntry Class::Clean Function");
		try
		{
			lipiTKStroke = new LipiTKStroke();
			xCoordinate = new Vector();
			yCoordinate = new Vector();
			Graphics g = getGraphics();
			g.setColor(getBackground());
			g.fillRect(0, 0, getSize().width, getSize().height);
			title.setForeground(Color.black);
			repaint();
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in DataEntry class from Clean Function"+ ex.toString());
		}
		LipiTkUI.DEBUG("Exit From DataEntry Class::Clean Function");
	}

	public void actionPerformed(ActionEvent actionevent)
	{
		Graphics g = getGraphics();
		g.setColor(getBackground());
		g.fillRect(0, 0, getSize().width, getSize().height);
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: mousePressed
	 * DESCRIPTION	: This function gets invoked for the first time when the user pressesed the mouse on writing area
	 * ARGUMENTS	: mouseevent - contains the reference to mouseEvent
	 * RETURNS		: None
	 * NOTES		: throws Standard exception if any exception occured
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void mousePressed(MouseEvent mouseevent)
	{
		lastX = mouseevent.getX();
		lastY = mouseevent.getY();
		lipiTKStroke.addPoints(lastX, lastY, true);
		xCoordinate.add(new Integer(lastX));
		yCoordinate.add(new Integer(lastY));
		Graphics graphics = getGraphics();
		Graphics2D graphics2D = (Graphics2D)graphics;
		graphics2D.setStroke(basicStroke);
		graphics2D.drawLine(lastX, lastY, lastX, lastY);
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: mouseDragged
	 * DESCRIPTION	: This function gets invoked every time when user drags on the writing area
	 * ARGUMENTS	: mouseevent - contains the reference to mouseEvent
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void mouseDragged(MouseEvent mouseevent)
	{
		Graphics graphics = getGraphics();
		int i = mouseevent.getX();
		int j = mouseevent.getY();
		lipiTKStroke.addPoints(lastX, lastY, false);
		Graphics2D graphics2D = (Graphics2D)graphics;
		graphics2D.setStroke(basicStroke);
		graphics2D.drawLine(lastX, lastY, i, j);
		lastX = i;
		lastY = j;
	}

	public static LipiTKStroke getLipiTKStroke()
	{
		return lipiTKStroke;
	}

	public void mouseReleased(MouseEvent mouseevent)
	{
	}

	public void mouseClicked(MouseEvent mouseevent)
	{
	}

	public void mouseEntered(MouseEvent mouseevent)
	{
	}

	public void mouseMoved(MouseEvent mouseevent)
	{
	}

	public void mouseExited(MouseEvent mouseevent)
	{
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: paintComponent
	 * DESCRIPTION	: 
	 * ARGUMENTS	: Graphics
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		lipiTKStroke.findLipiTKStroke(g);
	}

	private Vector xCoordinate;
	private Vector yCoordinate;
	public int lastX;
	public int lastY;
	private static LipiTKStroke lipiTKStroke;
	private JLabel title;
	private static final BasicStroke basicStroke = new BasicStroke(2.0F);
}