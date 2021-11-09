// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

/**
 * This is the class that handles everything.  It's the core of the
 * application.  When starting an app.  The first thing in wxWidgets
 * that we have access to is the \p OnInit method.  After that method
 * returns, the event loop start.
 */
class MyApp : public wxApp {
public:
  virtual bool OnInit();
};

/**
 * This class is a frame (a.k.a. a window).  This frame emits a lot of
 * different events.  On this case we only implement the methods which
 * handles the button presses.
 *
 * In the class constructor we define a lot of properties of the
 * frame, or create and assing children.
 */
class MyFrame : public wxFrame {
public:
  MyFrame();

private:
  void OnHello(wxCommandEvent &event);
  void OnExit(wxCommandEvent &event);
  void OnAbout(wxCommandEvent &event);
};

/**
 * With this enum we only define custom ids for haddling of events.
 */
enum {
  ID_Hello = 1
};

// Here wxWidgets implements the main function for us.  We must
// remember that any C/C++ program must have a main function.
wxIMPLEMENT_APP(MyApp);

/**
 * This method is the first access we have to the app inside
 * wxWidgets.
 *
 * In this case, we create a new frame, of type \c MyFrame and then
 * show it on the screen.
 *
 * This method returns true if the init was successful, in other case,
 * the app will crash (Or it believe that crash).
 */
bool MyApp::OnInit() {
  MyFrame *frame = new MyFrame();
  frame->Show(true);
  return true;
}

/**
 * This is the frame constructor, in here we define a lot of
 * properties of the frame.  Or in this case, we append some children as
 * a menu bar, a status bar and finally bind the events.
 */
MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, "Hello World") {
  wxMenu *menuFile = new wxMenu;
  menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                   "Help string shown in status bar for this menu item");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  wxMenu *menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText("Welcome to wxWidgets!");

  Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
  Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

/**
 * This method handles the quit or exit button action.  For example as
 * we click on the frame close button, or when we click on file ->
 * quit.
 */
void MyFrame::OnExit(wxCommandEvent &WXUNUSED(event)) {
  Close(true);
}

/**
 * This method handles the about button click.  Showing up a box
 * (a.k.a. pop-up dialog) with the information about the program (None
 * of it).
 */
void MyFrame::OnAbout(wxCommandEvent &WXUNUSED(event)) {
  wxMessageBox("This is a wxWidgets Hello World example", "About Hello World",
               wxOK | wxICON_INFORMATION);
}

/**
 * This method handles the hello button click.  Showing up a box with
 * a Hello world message.
 */
void MyFrame::OnHello(wxCommandEvent &WXUNUSED(event)) {
  wxLogMessage("Hello world from wxWidgets!");
}
