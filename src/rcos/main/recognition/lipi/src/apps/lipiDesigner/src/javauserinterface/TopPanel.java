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
 * FILE DESCR	: TopPanel  is a class which displays all the controls present in the Top Panel of UI
 * CONTENTS		:
 *			getDisplay
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.awt.Component;
import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JPanel;
import javax.swing.border.Border;

public class TopPanel extends JPanel
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: TopPanel
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: Creates all the necessary controls and set the borders properly
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public TopPanel()
	{
		try
		{
			JPanel jcontrols = new JPanel();

			StrokeDisplay drawP = new StrokeDisplay();
			dis = drawP;
			drawP.setPreferredSize(new Dimension(260,200));

			//Added code to resize the visualization window
			drawP.setMaximumSize(new Dimension(260,200));
			drawP.setAlignmentY(Component.CENTER_ALIGNMENT);

			//Till here
			ClassSamples delP = new ClassSamples(drawP);
			delP.setPreferredSize(new Dimension(350,300));
			delP.setMaximumSize(new Dimension(350,300));

			Border etched = BorderFactory.createEtchedBorder();
			drawP.setBorder(BorderFactory.createTitledBorder(etched, "Display Area"));

			jcontrols.setBorder(BorderFactory.createTitledBorder(etched, "Browse Training Data Samples"));
			Dimension dimension1 = new Dimension(700, 250);
			jcontrols.setPreferredSize(dimension1);
			jcontrols.setLayout(new BoxLayout(jcontrols, BoxLayout.X_AXIS));
			jcontrols.add(delP);
			jcontrols.add(drawP);
			add(jcontrols);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in TopPanel class from Constructor function"+ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getDisplay
	 * DESCRIPTION	: This function will return reference to StrokeDisplay object
	 * ARGUMENTS	: None
	 * RETURNS		: dis - reference to StrokeDisplay instance
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static StrokeDisplay getDisplay()
	{
		return dis;
	}

	private static StrokeDisplay dis;
}