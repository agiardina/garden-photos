#include <wx/wx.h>
#include <wx/sizer.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/config.h>
#include "sqlite3.h"
#include "image_panel.h"
#include "photos.h"



class MyApp: public wxApp
{
    
  wxFrame *frame;
  wxImagePanel * drawPane;
  wxPanel *sidebar;
  //wxPanel *main;

public:
  bool OnInit()
  {
    //Read Configuration
    wxConfig *config = new wxConfig("AGallery");
    
    wxString base_path;
    wxString thumbs_path;
    wxString tmp_path;
    wxString db_path;
     

    if (!config->Read("BasePath", &base_path)) {
      base_path = wxStandardPaths::Get().GetDocumentsDir() + "/AGallery";
      config->Write("BasePath", base_path);
    }

    if (!config->Read("ThumbsPath", &thumbs_path)) {
      thumbs_path = base_path + "/thumbs";
      config->Write("ThumbsPath",thumbs_path);
    }

    if (!config->Read("TmpPath", &tmp_path)) {
      tmp_path = base_path + "/tmp";
      config->Write("TmpPath",tmp_path);
    }

    if (!config->Read("DBPath", &db_path)) {
      db_path = base_path + "/db";
      config->Write("DBPath",db_path);
    }	    
    delete config;


    sqlite3 *db;
    //sqlite3_stmt *res;
    
    int rc = sqlite3_open(db_path, &db);
    
    if (rc != SQLITE_OK) {
      std::cout << db_path << "\n";
      fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
      sqlite3_close(db);
      return 1;
    } 
    Photos photos = Photos(db);
    
    
    
    // make sure to call this first
    wxInitAllImageHandlers();

    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame(NULL, wxID_ANY, wxT("Garden Photos"), wxPoint(50,50), wxSize(1024,768));

    sidebar = new wxPanel(frame);

    wxBitmap exit(wxT("add.png"), wxBITMAP_TYPE_PNG);
    wxToolBar* toolbar = new wxToolBar(frame, -1);
    frame->SetToolBar(toolbar);
    toolbar->AddTool(wxID_EXIT, wxT("Exit application"), exit);
    toolbar->Realize();	

    drawPane = new wxImagePanel(frame);
    drawPane->setPhotos(photos.get_all_photos());
    sizer->Add(drawPane, 1, wxEXPAND);	
        
    frame->SetSizer(sizer);
        
    frame->Show();
    return true;
  } 
    
};

IMPLEMENT_APP(MyApp)
