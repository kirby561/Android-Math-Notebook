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
 * CONTENTS  :
 *				actionPerformed
 *				getTrainMode
 *				getDrawEntry
 *				setButtonStatus
 *
 * AUTHOR    : Ravi Kiran
 * DATE		 : August 18, 2006
 * CHANGE HISTORY:
 * Author       Date            Description of change
 ************************************************************************/

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;
import javax.swing.border.*;
import java.awt.*;

public class BottomPanel extends JPanel implements ActionListener
{

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE		    : 18-08-06
	 * NAME		    : BottomPanel
	 * DESCRIPTION	: constructor
	 * ARGUMENTS	: None
	 * RETURNS		: None
	 * NOTES		: This method performs the following operation
	 *				  1. Constructs the BottomPanel class object
	 *  			  2. Creates the necessary controls required in UI
	 *  			  3. Adds the corresponding actionListener classes to each control
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/

	public BottomPanel()
	{

		LipiTkUI.DEBUG("Enter Into BottomPanel Class::Constructor Function");

		try
		{
			Border etched = BorderFactory.createEtchedBorder();

			train_mode = true;

			JPanel buttons   = new JPanel();
			buttons.setLayout(new BoxLayout(buttons, BoxLayout.Y_AXIS));

			JPanel radio = new JPanel();
			radio.setLayout(new BoxLayout(radio, BoxLayout.X_AXIS));
			radio.setBorder(BorderFactory.createTitledBorder(etched, "Training Mode"));
			radio.setPreferredSize(new Dimension(135,50));

			jaddSample = new JButton("Add Sample");
			//Added code to fix the button size
			jaddSample.setSize(new java.awt.Dimension(10,10));
			jaddSample.setPreferredSize(new Dimension(110,20));
			jaddSample.setMaximumSize(new Dimension(110,20));
			jaddSample.setMinimumSize(new Dimension(110,20));
			jaddSample.setAlignmentX(Component.CENTER_ALIGNMENT);
			jaddSample.setHorizontalAlignment(SwingConstants.LEFT);

			jaddClass = new JButton("Add Class");
			//Added code to fix the button size
			jaddClass.setSize(20,20);
			jaddClass.setPreferredSize(new Dimension(110,20));
			jaddClass.setMaximumSize(new Dimension(110,20));
			jaddClass.setMinimumSize(new Dimension(110,20));
			jaddClass.setAlignmentX(Component.CENTER_ALIGNMENT);
			jaddClass.setHorizontalAlignment(SwingConstants.LEFT);

			jrecognize = new JButton("Recognize");
			//Added code to fix the button size
			jrecognize.setSize(20,20);
			jrecognize.setPreferredSize(new Dimension(110,20));
			jrecognize.setMaximumSize(new Dimension(110,20));
			jrecognize.setMinimumSize(new Dimension(110,20));
			jrecognize.setAlignmentX(Component.CENTER_ALIGNMENT);
			jrecognize.setHorizontalAlignment(SwingConstants.LEFT);

			jtrain = new JButton("Train");
			//Added code to fix the button size
			jtrain.setSize(20,20);
			jtrain.setPreferredSize(new Dimension(110,20));
			jtrain.setMaximumSize(new Dimension(110,20));
			jtrain.setMinimumSize(new Dimension(110,20));
			jtrain.setAlignmentX(Component.CENTER_ALIGNMENT);
			jtrain.setHorizontalAlignment(SwingConstants.LEFT);
			jtrain.setEnabled(false);

			jclear = new JButton("Clear");
			//Added code to fix the button size
			jclear.setSize(20,20);
			jclear.setPreferredSize(new Dimension(110,20));
			jclear.setMaximumSize(new Dimension(110,20));
			jclear.setMinimumSize(new Dimension(110,20));
			jclear.setAlignmentX(Component.CENTER_ALIGNMENT);
			jclear.setHorizontalAlignment(SwingConstants.LEFT);

			JRadioButton auto = new JRadioButton("Auto",true);
			JRadioButton manual = new JRadioButton("Manual");

			auto.addActionListener(this);
			manual.addActionListener(this);

			ButtonGroup TrainMode = new ButtonGroup();
			TrainMode.add(auto);
			TrainMode.add(manual);

			buttons.add(jaddClass);
			buttons.add(jaddSample);
			buttons.add(jrecognize);
			buttons.add(jtrain);
			buttons.add(jclear);
			buttons.add(radio);
			radio.add(auto);
			radio.add(manual);

			RecognizedResults resultL = new RecognizedResults("Recognized Classes");
			resultL.setPreferredSize(new Dimension(375, 200));

			DataEntry drawP = new DataEntry();
			drawP.setPreferredSize(new Dimension(260,200));
			drawP.setMaximumSize(new Dimension(260,200));
			drawP.setBorder(BorderFactory.createTitledBorder(etched, "Writing Area "));

			stroke = drawP;

			JPanel jcontrols = new JPanel();
			jcontrols.setLayout(new BoxLayout(jcontrols, BoxLayout.X_AXIS));
			jcontrols.setBorder(BorderFactory.createTitledBorder(etched, "Add New Data Samples"));
			jcontrols.setPreferredSize(new Dimension(700,250));

			jcontrols.add(drawP);
			jcontrols.add(buttons);

			jcontrols.add(resultL);
			add(jcontrols);

			jaddSample.addActionListener(LipiTkUI.getLipiTkActionRef());
			jaddClass.addActionListener(LipiTkUI.getLipiTkActionRef());
			jrecognize.addActionListener(LipiTkUI.getLipiTkActionRef());
			jtrain.addActionListener(LipiTkUI.getLipiTkActionRef());
			jclear.addActionListener(LipiTkUI.getLipiTkActionRef());
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in Bottom Panel class from Constructor function" + ex.toString());
		}

		LipiTkUI.DEBUG("Exit From BottomPanel Class::Constructor Function");
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE		    : 18-08-06
	 * NAME		    : actionPerformed
	 * DESCRIPTION	: 
	 * ARGUMENTS	: ActionEvent
	 * RETURNS		: None
	 * NOTES		: ActionEvent contains the reference to the actionevent generated
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description 
	 *************************************************************************************/
	public void actionPerformed(ActionEvent e)   
	{
		try
		{
			if(((String)e.getActionCommand()).equals("Auto"))
			{
				train_mode = true;
				if(LipiTkActionClass.train_happened)
					jtrain.setEnabled(false);
				else
					jtrain.setEnabled(true);

				LipiTkUI.DEBUG("Auto Selected");
			}
			else
			{
				train_mode = false;
				jtrain.setEnabled(true);
				LipiTkUI.DEBUG("Manual Selected");
			}
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in Bottom Panel class from actionPerformed function" + ex.toString());
		}
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE		    : 18-08-06
	 * NAME		    : getTrainMode
	 * DESCRIPTION	: Returns the train_mode
	 * ARGUMENTS	: None
	 * RETURNS		: boolean
	 * NOTES		: return true indicates Auto
	 * 			      return false indicates Manual
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static boolean getTrainMode()
	{
		return train_mode;
	}

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE		    : 18-08-06
	 * NAME		    : getDrawEntry
	 * DESCRIPTION	: This method will return the Stroke which contains the necessary pen-points the following operation
	 * ARGUMENTS	: None
	 * RETURNS		: DataEntry
	 * NOTES		: return stroke contains the strokes drawn on the writing area
	 *
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	 public static DataEntry getDrawEntry()
	 {
		 return stroke;
	 }

	/**********************************************************************************
	 * AUTHOR		: Ravi Kiran
	 * DATE			: 18-08-06
	 * NAME			: SetButtonStatus
	 * DESCRIPTION	: Set the status for 'Train' and 'Recognize' buttons
	 * ARGUMENTS	: train_butn_status - indicates the status for 'Train' button in UI
	 *			      recognize_butn_status - indicates the status for 'Recognize' button in UI 
	 * RETURNS		: None
	 * NOTES		: None
	 * 			  
	 * CHANGE HISTROY
	 * Author			Date				Description
	 *************************************************************************************/
	public static void setButtonStatus(boolean train_butn_status, boolean recognize_butn_status)
	{
		try
		{
			if(train_butn_status)
			{
				if(!LipiTkActionClass.train_happened && !StrokeFileManager.checkModalDataExists(LipiTkActionClass.project_name))
					jtrain.setEnabled(true);
				else
				{
					if(train_mode)
						jtrain.setEnabled(false);
					else
						jtrain.setEnabled(true);
				}
			}
			else
			{
				if(train_mode)
					jtrain.setEnabled(false);
				else
					jtrain.setEnabled(true);
			}

			jrecognize.setEnabled(recognize_butn_status);
		}
		catch(Exception ex)
		{
			System.out.println("JAVA UI Interface: Exception in Bottom Panel class from SetButtonStatus function" + ex.toString());
		}
	}

	public static DataEntry stroke = null;
	private static boolean train_mode;
	private static JButton jaddSample;
	private static JButton jaddClass;
	private static JButton jrecognize;
	private static JButton jtrain;
	private static JButton jclear;
};