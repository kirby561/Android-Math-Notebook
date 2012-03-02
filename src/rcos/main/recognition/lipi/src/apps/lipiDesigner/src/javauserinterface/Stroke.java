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
 * FILE DESCR	: A stroke in the Ink Sample is represented by this class
 * CONTENTS		:
 *			addPoints
 *			findStroke
 *			getPoints
 *			getStrokeLength
 *			getXPoint
 *			getYPoint
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.awt.BasicStroke;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.util.Vector;

public class Stroke
{
	private static final BasicStroke basicStroke = new BasicStroke(2.0F);
	private Vector xCoordinate;
	private Vector yCoordinate;

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: Stroke
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: Initializes the X & Y Coordinates.
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/    
	public Stroke()
	{        
		xCoordinate = new Vector();
		yCoordinate = new Vector();
	}

	public Stroke(int x, int y)
	{       
		xCoordinate = new Vector();
		yCoordinate = new Vector();
		xCoordinate.add(new Integer(x));        
		yCoordinate.add(new Integer(y));
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: addPoints
	 * DESCRIPTION	: Addspoints to a stroke
	 * ARGUMENTS	: x - specifies the X coordinate of the stroke 
	 *			      y - specifies the Y-coordinate of the stroke
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void addPoints(int x, int y)
	{
		xCoordinate.add(new Integer(x));
		yCoordinate.add(new Integer(y));
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: findStroke
	 * DESCRIPTION	: paints the stroke
	 * ARGUMENTS	: graphics 
	 * RETURNS		: None
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/    
	public void findStroke(Graphics graphics)
	{
		Graphics2D graphics2D = (Graphics2D)graphics;
		graphics2D.setStroke(basicStroke);
		for(int count = 1; count < xCoordinate.size(); count++)
		{
			graphics2D.drawLine(getXPoint(count - 1), getYPoint(count - 1), getXPoint(count), getYPoint(count));
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getPoints
	 * DESCRIPTION	: This method returns the points in a stroke 
	 * ARGUMENTS	: None 
	 * RETURNS		: pointArray - array of points in a stroke
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public Point[] getPoints()
	{
		int xPoint = xCoordinate.size();

		Point  pointArray [] = new Point[xPoint];
		for(int count = 0; count < xPoint; count++)
		{
			pointArray[count] = new Point();
			Integer xAxis = (Integer)xCoordinate.get(count);
			Integer yAxis = (Integer)yCoordinate.get(count);            
			pointArray[count].setXPoint(xAxis.intValue());
			pointArray[count].setYPoint(yAxis.intValue());
		}

		return pointArray;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getStrokeLength
	 * DESCRIPTION	: Returns the number of points in a stroke
	 * ARGUMENTS	: None 
	 * RETURNS		: number of points in the stroke
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public int getStrokeLength()
	{
		return xCoordinate.size();
	}    

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getXPoint
	 * DESCRIPTION	: Returns X-Coordinate of the point
	 * ARGUMENTS	: point - who's X-Coordinate has to be returned
	 * RETURNS		: X-Cordinate of the point
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private int getXPoint(int point)
	{
		return ((Integer)xCoordinate.get(point)).intValue();
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getYPoint
	 * DESCRIPTION	: Returns Y-Coordinate of the point
	 * ARGUMENTS	: point - who's Y-Coordinate has to be returned
	 * RETURNS		: Y-cordinate of the point
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	private int getYPoint(int point)
	{
		return ((Integer)yCoordinate.get(point)).intValue();
	}
}