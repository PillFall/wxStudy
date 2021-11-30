/**
 * On this application we study in depth all the configuration classes
 * for wxWidgets.
 */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifndef  WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/log.h"
#include "wx/config.h"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "sample.xpm"
#endif

/**
 * Basic app class, needed as wxWidgets entry point.
 */
class MyApp : public wxApp {
public:
  virtual bool OnInit();
  virtual int OnExit();
};

class MyFrame : public wxFrame {
public:
  MyFrame();
  virtual ~MyFrame();

  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnDelete(wxCommandEvent& event);

private:
  wxTextCtrl *m_text;
  wxCheckBox *m_check;

  wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(wxID_DELETE, MyFrame::OnDelete)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

/**
 * Principal method for accessing wxWidgets API.  This is the first
 * method we have access to and from all application is deployed.
 */
bool MyApp::OnInit() {

  // we're using wxConfig's "create-on-demand" feature: it will create the
  // config object when it's used for the first time. It has a number of
  // advantages compared with explicitly creating our wxConfig:
  //  1) we don't pay for it if we don't use it
  //  2) there is no danger to create it twice

  // application and vendor name are used by wxConfig to construct the name
  // of the config file/registry key and must be set before the first call
  // to Get() if you want to override the default values (the application
  // name is the name of the executable and the vendor name is the same)
  SetVendorName(wxT("wxWidgets"));
  SetAppName(wxT("wxConfig")); // not needed, it's the default value

  wxFileConfig *config = new wxFileConfig();
  wxConfig::Set(config);

  // or you could also write something like this:
  //  wxConfigBase *config = wxConfigBase::Get();
  // This should be the standard way to create and modify the config
  // file/registry key but I don't wanna create a registry key on
  // Windows, I explicitly tell wxWidgets to use a file.

  // uncomment this to force writing back of the defaults for all values
  // if they're not present in the config - this can give the user an idea
  // of all possible settings for this program
  config->SetRecordDefaults();

  // create the main program window
  MyFrame *frame = new MyFrame;
  frame->Show(true);

  // use our config object...
  if ( config->Read(wxT("/Controls/Check"), 1l) != 0 ) {
    wxMessageBox(wxT("You can disable this message box by unchecking\n")
                 wxT("the checkbox in the main window (of course, a real\n")
                 wxT("program would have a checkbox right here but we\n")
                 wxT("keep it simple)"),
                 wxT("Welcome to wxConfig"),
                 wxICON_INFORMATION | wxOK);
  }

  return true;
}

/**
 * Handles exit event.
 */
int MyApp::OnExit() {
  // clean up: Set() returns the active config object as Get() does, but unlike
  // Get() it doesn't try to create one if there is none (definitely not what
  // we want here!)
  delete wxConfigBase::Set((wxConfigBase *) NULL);

  return 0;
}

/**
 * Main frame constructor
 */
MyFrame::MyFrame()
  : wxFrame((wxFrame *) NULL, wxID_ANY, wxT("wxConfig")) {
  SetIcon(wxICON(sample));

  wxMenu *file_menu = new wxMenu;

  file_menu->Append(wxID_DELETE, wxT("&Delete"), wxT("Delete config file"));
  file_menu->AppendSeparator();
  file_menu->Append(wxID_ABOUT, wxT("&About\tF1"), wxT("About this sample"));
  file_menu->AppendSeparator();
  file_menu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Exit the program"));
  wxMenuBar *menu_bar = new wxMenuBar;
  menu_bar->Append(file_menu, wxT("&File"));
  SetMenuBar(menu_bar);

#if wxUSE_STATUSBAR
  CreateStatusBar();
#endif

  wxPanel *panel = new wxPanel(this);
  new wxStaticText(panel, wxID_ANY, wxT("These controls remember their values!"),
                   wxPoint(10, 10), wxSize(300, 20));
  m_text = new wxTextCtrl(panel, wxID_ANY, wxT(""), wxPoint(10, 40), wxSize(300, 20));
  m_check = new wxCheckBox(panel, wxID_ANY, wxT("show welcome message box at startup"),
                           wxPoint(10, 70), wxSize(300, 20));

  // restore the control's values from the config

  // NB: in this program, the config object is already created at this moment
  // because we had called Get() from MyApp::OnInit(). However, if you later
  // change the code and don't create it before this line, it won't break
  // anything - unlike if you manually create wxConfig object with Create()
  // or in any other way (then you must be sure to create it before using it!).
  wxConfigBase *config = wxConfigBase::Get();

  // we could write Read("/Controls/Text") as well, it's just to show SetPath()
  config->SetPath(wxT("/Controls"));

  m_text->SetValue(config->Read(wxT("Text"), wxT("")));
  m_check->SetValue(config->Read(wxT("Check"), 1l) != 0);

  // SetPath() understands ".."
  config->SetPath(wxT("../MainFrame"));

  // restore frame position and size
  int x = config->Read(wxT("x"), 50),
    y = config->Read(wxT("y"), 50),
    w = config->Read(wxT("w"), 350),
    h = config->Read(wxT("h"), 200);

  Move(x, y);
  SetClientSize(w, h);

  config->SetPath(wxT("/"));
  wxString s;
  if (config->Read(wxT("TestValue"), &s)) {
    wxLogStatus(this, wxT("TestValue from config is '%s'"), s.c_str());
  } else {
    wxLogStatus(this, wxT("TestValue not found in the config"));
  }
}

/**
 * Basic quit handler.
 */
void MyFrame::OnQuit(wxCommandEvent&) {
  Close(true);
}

/**
 * Basic about handler.
 */
void MyFrame::OnAbout(wxCommandEvent&) {
  wxMessageBox(wxT("wxConfig"),
               wxT("About"),
               wxICON_INFORMATION | wxOK);
}

/**
 * Handles deletion of configuration.
 */
void MyFrame::OnDelete(wxCommandEvent&) {
  wxConfigBase *config = wxConfigBase::Get();
  if ( config == NULL ) {
    wxLogError(wxT("No config to delete!"));
    return;
  }

  if (config->DeleteAll()) {
    wxLogMessage(wxT("Config file/registry key successfully deleted."));

    delete wxConfigBase::Set(NULL);
    wxConfigBase::DontCreateOnDemand();
  } else {
    wxLogError(wxT("Deleting config file/registry key failed."));
  }
}

/**
 * Hanldes main frame destruction.
 */
MyFrame::~MyFrame() {
  wxConfigBase *config = wxConfigBase::Get();
  if ( config == NULL )
    return;

  // save the control's values to the config
  config->Write(wxT("/Controls/Text"), m_text->GetValue());
  config->Write(wxT("/Controls/Check"), m_check->GetValue());

  // save the frame position
  int x, y, w, h;
  GetClientSize(&w, &h);
  GetPosition(&x, &y);
  config->Write(wxT("/MainFrame/x"), (long) x);
  config->Write(wxT("/MainFrame/y"), (long) y);
  config->Write(wxT("/MainFrame/w"), (long) w);
  config->Write(wxT("/MainFrame/h"), (long) h);

  config->Write(wxT("/TestValue"), wxT("A test value"));
}
