#include <iostream>
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/config.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include "sqlite3.h"
#include "image_panel.h"
#include "image_panel_controller.h"
#include "photos.h"
#include "menu.hpp"
#include <wx/menu.h>
#include <wx/dirdlg.h>
#include <wx/progdlg.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include "garden_photos.h"
#include "sidebar_item.h"
#include "sidebar.h"

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

bool garden_photos::OnInit()
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
    this->thumbs_path = std::string(thumbs_path.mb_str());
    
    //Database Connection
    Poco::Data::SQLite::Connector::registerConnector();
    session = new Session("SQLite",std::string(db_path.mb_str()));
    
    // make sure to call this first
    wxInitAllImageHandlers();
    
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    frame = new wxFrame(NULL, wxID_ANY, wxT(""), wxPoint(50,50), wxSize(1024,768), wxDEFAULT_FRAME_STYLE);
    
    menu = new Menu();
    frame->SetMenuBar(menu->menubar);
    frame->Bind(wxEVT_COMMAND_MENU_SELECTED, &garden_photos::onImport, this, IMPORT_FOLDER);
    
    wxToolBar* toolbar = new wxToolBar(frame, -1);
    toolbar->SetWindowStyle(wxNO_BORDER);
    frame->SetBackgroundColour(wxColour(44,44,43));
    frame->SetToolBar(toolbar);
    toolbar->Realize();

    drawPane = new image_panel(frame);
    m_image_panel_controller = new image_panel_controller();
    m_image_panel_controller->init(*drawPane, *session, std::string(thumbs_path.mb_str()));
    
    sid = new sidebar(frame);
    sid->init();
    sid->Bind(GP_SIDEBAR_CLICK, &garden_photos::on_sidebar_click,this);
    
    sizer->Add(sid, 0, wxALIGN_LEFT);
    sizer->Add(drawPane, 2, wxEXPAND);
    frame->SetSizer(sizer);
    frame->Show();
    
    return true;
}

void garden_photos::on_sidebar_click(wxCommandEvent& event)
{
    std::string uid = std::string(event.GetString().mb_str());
    sid->set_active_item(uid);
    
    if (uid=="photos") {
        m_image_panel_controller->load_photos();
    } else if (uid=="favorites") {
        m_image_panel_controller->load_favorites_photos();
    }
}
    
void garden_photos::onImport( wxEvent& WXUNUSED(event) )
{
    wxDirDialog dir_dlg(NULL, "Choose folder to import", "",
                    wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    
    if (dir_dlg.ShowModal() == wxID_OK)
    {
        wxConfig config("AGallery");
        wxString thumbs_path;
        config.Read("ThumbsPath",&thumbs_path);
        
        std::string base_path = std::string(dir_dlg.GetPath().mb_str());
        photos::gallery gallery = photos::gallery_by_path(base_path,*session);
        std::vector<std::string> photos_to_import = photos::photos_on_folder(base_path);
        int n_photos = photos_to_import.size();
        wxProgressDialog dialog("Import","Importing photos",n_photos);
        for ( int i = 0; i < n_photos; ++i ) {
            if ( !dialog.Update(i)) {
                // Cancelled by user.
                break;
            }
            photos::add_photo_to_gallery(photos_to_import[i], gallery, *session, this->thumbs_path);
        }
    }
}

IMPLEMENT_APP(garden_photos)
