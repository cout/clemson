// Paul Brannan
// CPSC 428 Assignment 6
// April 19, 1999

import java.io.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;

// Note -- I did not use cucs for this project.  There are a few reasons as
// to why:
//   1) I want to know how to do AWT stuff so I won't be lost if/when I move
//      to Swing/JFC.
//   2) I'm to lazy to update my CLASSPATH (j/k).

// StudentDatabaseGUI is a GUI interface for the StudentDatabase class.
class StudentDatabaseGUI extends Frame implements ActionListener, WindowListener {

	// instance variables
	StudentDatabase db = new StudentDatabase();

	// panels
	Panel panel1a = new Panel(new GridLayout(1, 0, 5, 5));
	Panel panel1b = new Panel(new GridLayout(1, 0, 5, 5));
	Panel panel2 = new Panel(new GridLayout(0, 1));
	Panel panel3 = new Panel();
	Panel panel4 = new Panel();
	Panel panel5 = new Panel();
	Panel panel6 = new Panel();

	// input fields
	TextField firstName = new TextField("", 10);
	TextField lastName = new TextField("", 10);
	TextField major = new TextField("", 10);
	TextField className = new TextField("", 10);
	TextField homeTown = new TextField("", 10);

	// buttons
	Button changeClassButton = new Button("Change class");
	Button addButton = new Button("Add");
	Button showSelectionButton = new Button("Show Selection");
	Button showAllButton = new Button("Show All");
	Button quitButton = new Button("Quit");
	Button ok = new Button("OK");

	// dialogs
	Label dialogMessage = new Label("");
	Dialog dialog = new Dialog(this, true);

	// other widgets
	java.awt.List nameList = new java.awt.List(20, true);
	java.util.ArrayList names = new java.util.ArrayList();
	TextArea studentInfo = new TextArea("", 20, 25);

	// constructor -- set up the interface
	StudentDatabaseGUI() {
		// set up the window
		setTitle("Student Information Database");
		setBackground(Color.gray);
		setSize(425, 575);
		setLayout(new FlowLayout());
		addWindowListener(this);
		addNotify();

		// set up the panels
		add(panel1a);
		add(panel1b);
		add(panel2);
		panel2.add(panel3);
		panel2.add(panel4);
		panel2.add(panel5);
		panel2.add(panel6);

		// add the misc. widgets the the window
		panel1a.add(new Label("Names"));
		panel1a.add(new Label("Student Information"));
		panel1b.add(nameList);
		studentInfo.setEditable(false);
		panel1b.add(studentInfo);

		// add the input fields/buttons
		panel3.add(new Label("First Name:"));
		panel3.add(firstName);
		panel3.add(new Label("Last Name:"));
		panel3.add(lastName);
		panel4.add(new Label("Major:"));
		panel4.add(major);
		panel4.add(new Label("Class:"));
		panel4.add(className);
		panel4.add(changeClassButton);
		panel5.add(new Label("Hometown:"));
		panel5.add(homeTown);
		panel5.add(addButton);

		// add the show buttons
		panel6.add(showSelectionButton);
		panel6.add(showAllButton);
		panel6.add(quitButton);

		// initialize events
		nameList.addActionListener(this);
		changeClassButton.addActionListener(this);
		addButton.addActionListener(this);
		showSelectionButton.addActionListener(this);
		showAllButton.addActionListener(this);
		quitButton.addActionListener(this);

		// construct the message box dialog
		dialog.setLayout(new GridLayout(0, 1));
		dialog.setSize(300, 100);
		dialog.add(dialogMessage);
		dialog.add(ok);
		ok.addActionListener(this);

		// and show the window
		show();
	}

	// update the names list
	private void updateNames() {
		nameList.removeAll();

		// this could (and probably should) be generated as each 
		// record is added to the database, but this is simpler
		// for now.
		names = db.getNames();
		ListIterator it;
		for(it = names.listIterator(); it.hasNext(); ) {
			Name tmp = (Name)it.next();
			nameList.add(tmp.toString());
		}
	}

	// update the fields with the currently selected name
	private void updateFields() {
	}

	// display a message (error?)
	private void displayMessage(String s) {
		dialogMessage.setText(s);
		dialog.show();
		// studentInfo.setText(s);
	}

	// add a record to the database
	private void addRecord() {
		if(firstName.getText().equals("")) {
			displayMessage("Please enter a first name.");
			return;
		}
		if(lastName.getText().equals("")) {
			displayMessage("Please enter a last name.");
			return;
		}
		if(major.getText().equals("")) {
			displayMessage("Please enter a major code.");
			return;
		}
		Name n = new Name(firstName.getText(), lastName.getText());
		db.addRecordFor(n, Integer.parseInt(major.getText()),
			className.getText(), homeTown.getText());

		// update the names list
		updateNames();

		// clear the fields so new values can be entered
		firstName.setText("");
		lastName.setText("");
		major.setText("");
		className.setText("");
		homeTown.setText("");
	}

	// change class for a student
	private void changeClass() {
		int[] indexes = nameList.getSelectedIndexes();
		if(indexes.length == 0) {
			displayMessage("Please select one or more names.");
			return;
		}

		for(int i = 0; i < indexes.length; i++) {
			db.setClass((Name)names.get(i), className.getText());
		}
	}

	// show information for a student or students
	private void showInfo() {
		int[] indexes = nameList.getSelectedIndexes();
		if(indexes.length == 0) {
			displayMessage("Please select one or more names.");
			return;
		}

		String s = "";
		for(int i = 0; i < indexes.length; i++) {
			Name n = (Name)names.get(indexes[i]);
			s += db.recordFor(n).toString();
			s += "\n";
		}

		studentInfo.setText(s);
	}

	// show all students
	private void showAll() {
		studentInfo.setText(db.toString());
	}

	// handle an event
	public void actionPerformed(ActionEvent e) {
		// Get the source of the action
		Object source = e.getSource();

		// what happens if we get two consecutive events?
		// disableButtons and enableButtons aren't defined, though
		// they are used in the class notes.

		// this should probably be a hash table or something,
		// but no documentation on how to do this properly?
		if(source instanceof Button) {
			if(source == changeClassButton) {
				changeClass();
			} else if(source == addButton) {
				addRecord();
			} else if(source == showSelectionButton) {
				showInfo();
			} else if(source == showAllButton) {
				showAll();
			} else if(source == quitButton) {
				// quit
				dispose();
			} else if(source == ok) {
				// clear the dialog box
				dialog.setVisible(false);
			} else {
				// um...?
			}
		} else if(source instanceof java.awt.List) {
			// must be the name list, so update fields
			updateFields();
		} else {
			// um...?
		}
	}

	// these are from the WindowBase class -- repaint the screen when
	// something's changed
	public void windowOpened(WindowEvent e) { repaint(); }
	public void windowDeiconified(WindowEvent e) { repaint(); }
	public void windowActivated(WindowEvent e) { repaint(); }

	// more from Windowbase class -- handle exit events
	public void windowClosing(WindowEvent e) { dispose(); }
	public void windowClosed(WindowEvent e) { System.exit(0); }

	// And this is out of order, but I'm ordering by actions.
	// We need these because we are implementing WindowListener.
	public void windowDeactivated(WindowEvent e) {}
	public void windowIconified(WindowEvent e) {}

	// create an instance of studentDatabaseGUI and let it go
	public static void main(String[] args) {
		StudentDatabaseGUI gui = new StudentDatabaseGUI();
	}
}

