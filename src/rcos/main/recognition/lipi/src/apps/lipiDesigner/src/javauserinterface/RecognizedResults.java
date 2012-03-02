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
 * FILE DESCR: RecognizedResults  is a class which creates the RecoziedResult list box.
 *
 * CONTENTS:
 *			valueChanged
 *			getResultListID
 *
 * AUTHOR:     Ravi Kiran
 *
 * DATE:       August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.awt.Dimension;

import javax.swing.BorderFactory;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.Border;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

public class RecognizedResults extends JPanel implements ListSelectionListener
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: RecognizedResults
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: label - Specifies the name of the ListBox
	 * RETURNS		: None
	 * NOTES		: Constructs the RecognizedResultsList box class object
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public RecognizedResults(String label)
	{
		try
		{
			String [] results = {"Top1 Choice","Top2 Choice","Top3 Choice","Top4 Choice","Top5 Choice"};

			jletterList = new JList(results);
			resultIDs = jletterList;
			Border etched = BorderFactory.createEtchedBorder();
			jletterList.setBorder(BorderFactory.createTitledBorder(etched,label));

			jletterList.setSelectionMode(0);
			jletterList.setSelectedIndex(0);
			jletterList.addListSelectionListener(this);
			jletterList.setVisibleRowCount(6);
			JScrollPane jscrollpane = new JScrollPane(jletterList);
			//Increased the width of Jlist
			jscrollpane.setPreferredSize(new Dimension(175,200));
			add(jscrollpane);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in RecognizedResults class from Constructor function" + ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: valueChanged
	 * DESCRIPTION	: Invoked when selected item n JList is changed
	 * ARGUMENTS	: listselectionevent -  contains the reference to the ListSelectionEvent generated
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void valueChanged(ListSelectionEvent listselectionevent)
	{
		if(!listselectionevent.getValueIsAdjusting() && jletterList.getSelectedIndex() != -1)
		{
			jletterList.getSelectedIndex();
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getResultListID
	 * DESCRIPTION	: This Function will return the reference to the RecognizedResult List Box
	 * ARGUMENTS	: None
	 * RETURNS		: resultIDs - reference to the ListBox
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public static JList getResultListID()
	{
		return resultIDs;
	}

	private JList jletterList;
	private static JList resultIDs;
}