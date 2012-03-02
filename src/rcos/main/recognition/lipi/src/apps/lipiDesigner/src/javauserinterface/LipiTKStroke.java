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
 * FILE DESCR	: LipiTKStroke is a data structure defined to capture strokes 
 * CONTENTS		:
 *			addPoints
 *			getStroke
 *			findLipiTKStroke
 *			getStrokeSize
 *			isEmpty
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.awt.Graphics;
import java.util.ArrayList;

public class LipiTKStroke
{
	private ArrayList strokeList = null;
	private Stroke stroke = null;

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: LipiTKStroke
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: Initializes the class variables
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public LipiTKStroke()
	{
		strokeList =  new ArrayList();
		stroke =  new Stroke();
	}   

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: addPoints
	 * DESCRIPTION	: Adds points to a stroke
	 * ARGUMENTS	: x - x variable of the point in writing area
	 *			      y  - x variable of the point in writing area
	 *			      isListEmpty - true if stroke size is zero, else false
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/	
	public void addPoints(int x, int y, boolean isListEmpty)
	{
		if(strokeList.size() == 0)
		{
			isListEmpty = true;
		}
		if(! isListEmpty)
		{
			stroke = (Stroke)strokeList.get(strokeList.size() - 1);
			stroke.addPoints(x, y);
			strokeList.set(strokeList.size() - 1, stroke);        	
		} 
		else
		{
			strokeList.add(new Stroke(x, y));
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getStroke
	 * DESCRIPTION	: Returns the ink sample
	 * ARGUMENTS	: None
	 * RETURNS		: Stroke[] array of ink strokes(a sample)
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public Stroke[] getStroke()
	{
		int strokeSize = getStrokeSize();
		Stroke strokeArray[] = new Stroke[strokeSize];
		for(int count = 0; count < strokeSize; count++)
		{
			strokeArray[count] = (Stroke)strokeList.get(count);
		}

		return strokeArray;
	}


	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: findLipiTKStroke
	 * DESCRIPTION	: 
	 * ARGUMENTS	: graphics
	 * RETURNS		: None
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void findLipiTKStroke(Graphics graphics)
	{
		int strokeSize = getStrokeSize();
		Stroke strokeArray[] = getStroke();
		for(int storkeCount = 0; storkeCount < strokeSize; storkeCount++)
		{
			strokeArray[storkeCount].findStroke(graphics);
			Point coordinates [] = strokeArray[storkeCount].getPoints();
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getStrokeSize
	 * DESCRIPTION	: Returns the number of the strokes in the sample
	 * ARGUMENTS	: None
	 * RETURNS		: StrokeListSize - number of strokes in the sample
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/	
	public int getStrokeSize()
	{
		int strokeListSize = strokeList.size(); 
		return strokeListSize;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: isEmpty
	 * DESCRIPTION	: Checks the size of sample
	 * ARGUMENTS	: None
	 * RETURNS		: isStrokeEmpty - true if number of strokes in the sample is zero, else false
	 * NOTES		: None  
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/	
	public boolean isEmpty()
	{
		boolean isStrokeEmpty = (getStrokeSize() == 0); 
		return isStrokeEmpty;
	}
}