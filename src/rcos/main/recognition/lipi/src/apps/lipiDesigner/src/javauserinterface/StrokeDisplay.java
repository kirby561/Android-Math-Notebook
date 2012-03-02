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
 * FILE DESCR	: StrokeDisplay  is a class which displays pen-point strokes in the display area.
 * CONTENTS		:
 *			clean
 *			addPoints
 *			paint
 *			getLipiTKStroke
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.awt.Color;
import java.awt.Graphics;
import java.util.Vector;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class StrokeDisplay extends JPanel
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: StrokeDisplay
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: Creates and initializes the data structures required for storing the pen-points
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public StrokeDisplay()
	{
		lipitkStroke = new LipiTKStroke();
		xCoordinate = new Vector();
		yCoordinate = new Vector();
		setBackground(Color.white);
		title = new JLabel("");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE		 	: 18-08-06
	 * NAME			: clean
	 * DESCRIPTION	: Resets the data structures
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void clean()
	{
		lipitkStroke = new LipiTKStroke();
		xCoordinate = new Vector();
		yCoordinate = new Vector();
		Graphics g = getGraphics();
		g.setColor(getBackground());
		g.fillRect(0, 0, getSize().width, getSize().height);
		title.setForeground(Color.black);
		repaint();
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: addPoints
	 * DESCRIPTION	: This function will adds the pen-points properly
	 * ARGUMENTS	: Xpoint specifies the X-coordinate point
	 *			      Ypoint specifies the Y-coordinate point
	 *			      flag indicates whether the current point first point or not
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void addPoints(int Xpoint,int Ypoint,boolean flag)
	{
		lastX = Xpoint;
		lastY = Ypoint;
		lipitkStroke.addPoints(lastX, lastY, flag);
		xCoordinate.add(new Integer(lastX));
		yCoordinate.add(new Integer(lastY));
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: paint
	 * DESCRIPTION	: This function paints the User Interface
	 * ARGUMENTS	: g - specifies the graphics reference
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void paint(Graphics g)
	{
		super.paint(g);
		lipitkStroke.findLipiTKStroke(g);
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getLipiTKStroke
	 * DESCRIPTION	: This function returns the LipiTKStroke reference
	 * ARGUMENTS	: None
	 * RETURNS		: lipitkStroke - reference to LipiTKStroke
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static LipiTKStroke getLipiTKStroke()
	{
		return lipitkStroke;
	}

	private Vector xCoordinate;
	private Vector yCoordinate;
	public int lastX;
	public int lastY;
	public static LipiTKStroke lipitkStroke ;
	private JLabel title;
}