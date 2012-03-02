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
 * FILE DESCR	: ClassSamples  is a class used for creating ClassList & SampleList box
 * CONTENTS		:
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.awt.Component;
import java.awt.Dimension;

import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class ClassSamples extends JPanel
{
	String root = "";
	LetterListClass jclassID = null;
	LetterListSample jsampleID = null;
	StrokeDisplay draw = null;
	
	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: ClassSamples
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: StrokeDisplay
	 * RETURNS		: None
	 * NOTES		: Creates the controls required to operate on Classes and Samples
	 *			      Creates the List Boxes to display list of Classes and Samples
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public ClassSamples(StrokeDisplay graph)
	{
		LipiTkUI.DEBUG("Enter Into ClassSamples Class::Constructor Function");
		try
		{
			draw = graph;
			JPanel buttons = new JPanel();
			//added code
			String operatingSystem = System.getProperty("os.name").toLowerCase();
			if(operatingSystem.startsWith("windows"))
			{
				buttons.setPreferredSize(new Dimension(120, 50));
				buttons.setMaximumSize(new Dimension(120, 50));
			}
			else
			{
				buttons.setPreferredSize(new Dimension(130, 50));
				buttons.setMaximumSize(new Dimension(130, 50));
			}
			//till here
			buttons.setLayout(new BoxLayout(buttons, BoxLayout.Y_AXIS));

			JButton jremovesample = new JButton("Delete Sample");
			//Added code to fix the button size
			jremovesample.setSize(20, 20);
			jremovesample.setPreferredSize(new java.awt.Dimension(110, 20));
			if(operatingSystem.startsWith("windows"))
			{
				jremovesample.setMaximumSize(new java.awt.Dimension(110, 20));
			}
			else
			{
				jremovesample.setMaximumSize(new java.awt.Dimension(123, 20));
			}
			jremovesample.setMinimumSize(new java.awt.Dimension(110, 20));
			jremovesample.setAlignmentX(Component.CENTER_ALIGNMENT);
			jremovesample.setHorizontalAlignment(SwingConstants.LEFT);

			JButton jremoveclass = new JButton("Delete Class");
			//Added code to fix the button size
			jremoveclass.setSize(20, 20);
			jremoveclass.setPreferredSize(new java.awt.Dimension(110, 20));
			if(operatingSystem.startsWith("windows"))
			{
				jremoveclass.setMaximumSize(new java.awt.Dimension(110, 20));
			}
			else
			{
				jremoveclass.setMaximumSize(new java.awt.Dimension(123, 20));
			}
			jremoveclass.setMinimumSize(new java.awt.Dimension(110, 20));
			jremoveclass.setAlignmentX(Component.CENTER_ALIGNMENT);
			jremoveclass.setHorizontalAlignment(SwingConstants.LEFT);

			buttons.add(jremovesample);
			buttons.add(jremoveclass);

			JPanel jcp = new JPanel();
			jcp.setLayout(new BoxLayout(jcp, BoxLayout.X_AXIS));
			jcp.setPreferredSize(new Dimension(350, 300));
			jcp.setMaximumSize(new Dimension(350, 300));

			jsampleID = new LetterListSample("Samples");
			jclassID = new LetterListClass(jsampleID, root, "Classes");

			jcp.add(jclassID);
			jcp.add(jsampleID);
			jcp.add(buttons);

			add(jcp, "North");

			jremovesample.addActionListener(LipiTkUI.getLipiTkActionRef());

			jremoveclass.addActionListener(LipiTkUI.getLipiTkActionRef());
		}
		catch (Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in ClassSamples class from Constructor function");
		}
		
		LipiTkUI.DEBUG("Exit From ClassSamples Class::Constructor Function");
	}
}