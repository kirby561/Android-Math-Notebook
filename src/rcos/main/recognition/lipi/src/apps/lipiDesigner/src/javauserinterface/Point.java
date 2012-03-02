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
 * FILE DESCR: BottomPanel  is a class which displays all the controls present in the Bottom Panel of UI
 *
 * CONTENTS:
 *			getXPoint
 *			setXPoint
 *			getYPoint
 *			setYPoint
 *
 * AUTHOR:     Ravi Kiran
 *
 * DATE:       August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
public class Point
{

	public Point()
	{
		xPoint = 0;
		yPoint = 0;
	}

	public Point(int m, int n)
	{
		xPoint = m;
		yPoint = n;
	}   

	public int getXPoint()
	{
		return xPoint;
	}

	public void setXPoint(int point)
	{
		xPoint = point;
	}

	public int getYPoint()
	{
		return yPoint;
	}

	public void setYPoint(int point)
	{
		yPoint = point;
	}

	private int xPoint;
	private int yPoint;	

}