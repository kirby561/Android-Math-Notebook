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
 * FILE DESCR	: LetterListSample  is a class which handles the events generated in the SampleList box
 * CONTENTS		:
 *          setRoot
 *			valueChanged
 *			updateList
 *			getSampleList
 *
 * AUTHOR		: Ravi Kiran
 * DATE			: August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/
import java.awt.Dimension;
import java.io.File;
import java.io.FilenameFilter;

import javax.swing.BorderFactory;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.border.Border;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

public class LetterListSample extends JPanel implements ListSelectionListener
{
	private static String rootdir= "";

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: setRoot
	 * DESCRIPTION	: Sets the root of list samples 
	 * ARGUMENTS	: root - contains the string to the path of root directory
	 * RETURNS		: void
	 * NOTES		: None 
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static void setRoot(String str)
	{
		rootdir = str;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: LetterListSample
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: label String - variable which contains the name of the list box
	 * RETURNS		: None
	 * NOTES		: Constructs the SampleListBox object with the name specified in the label parameter
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public LetterListSample(String label)
	{
		LipiTkUI.DEBUG("Enter Into LetterListSample Class::Constructor Function");
		try
		{
			String [] empty_list = {};
			jletterList = new JList(empty_list);
			sampleIDs = jletterList;
			Border etched = BorderFactory.createEtchedBorder();
			jletterList.setBorder(BorderFactory.createTitledBorder(etched,label));
			jletterList.addListSelectionListener(this);
			jletterList.setVisibleRowCount(6);
			JScrollPane jscrollpane = new JScrollPane(jletterList);
			jscrollpane.setPreferredSize(new Dimension(120,200));
			add(jscrollpane);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LetterListSample clas From Constructor function");
		}
		LipiTkUI.DEBUG("Exit From LetterListSample Class::Constructor Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: valueChanged
	 * DESCRIPTION	: This method will get called when ever the contents of SampleListBox get changed
	 * ARGUMENTS	: listselectionevent - contains the reference to the ListSelectionEvent
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void valueChanged(ListSelectionEvent listselectionevent)
	{
		LipiTkUI.DEBUG("Enter Into LetterListSample Class::valueChanged Function");

		try
		{
			if(!listselectionevent.getValueIsAdjusting() && (jletterList.getModel().getSize ()) != 0)
			{
				if( jletterList.getSelectedIndex() == - 1)
				{
					jletterList.setSelectedIndex(0);
				}
				int i=jletterList.getSelectedIndex();
				String str = (String)jletterList.getModel().getElementAt(i);
				if(str.compareTo("") != 0)
				{
					ParseFile parseFile =  new ParseFile(rootdir+str);
					repaint();
				}
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LetterListSample class from Value Changed event");
		}
		LipiTkUI.DEBUG("Exit From LetterListSample Class::valueChanged Function");
	}


	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: updateList
	 * DESCRIPTION	: This method will set the new list of samples into the ListBox
	 * ARGUMENTS	: dir String - variable which contains the path to the directory where the files are present
	 * RETURNS		: None
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public void updateList(String dir)
	{
		LipiTkUI.DEBUG("Enter Into LetterListSample Class::updateList Function");
		try
		{
			rootdir = dir;
			File f2 = new File(dir);

			LipiTkUI.DEBUG("******* File extension seperator" + f2.pathSeparatorChar+"**************");
			FilenameFilter filter = new LipiTKUtil();
			String [] samples = f2.list(filter);

			int length = samples.length;
			LipiTkUI.DEBUG("Length:"+length);
			for(int i=0;i<length;i++)
			{
				LipiTkUI.DEBUG(samples[i]);
				samples[i] = samples[i].substring(0,(samples[i].indexOf(".")));

				LipiTkUI.DEBUG("*******Seperated File:"+ samples[i]);
			}

			if((jletterList !=null) && (length != 0))
				jletterList.setListData(samples);
			else
				LipiTkUI.DEBUG("LETTER LIST NULL");
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in LetterList Sample from updateList function");
		}
		LipiTkUI.DEBUG("Exit From LetterListSample Class::updateList Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: getSampleList
	 * DESCRIPTION	: This method will return the reference to the created SampleListBox
	 * ARGUMENTS	: None
	 * RETURNS		: sampleIDs - conatins the reference to SampleList box
	 * NOTES		: None
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static JList getSampleList()
	{
		return sampleIDs;
	}

	private JList jletterList;
	private static JList sampleIDs ;
}