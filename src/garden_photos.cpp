#include <iostream>
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include "image_panel.h"
#include "photos.h"
#include "menu.h"
#include <wx/menu.h>
#include <wx/dirdlg.h>
#include <wx/progdlg.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/sysopt.h>
#include "garden_photos.h"
#include "config.h"
#include "main_frame.h"
#include "sidebar_item.h"
#include "sidebar.h"
#include "utils.h"

//#define wxMAC_USE_NATIVE_TOOLBAR 0


using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

bool garden_photos::OnInit()
{
    
//    wxSystemOptions::SetOption("mac.toolbar.no-native", 1);
    
    //Init Configuration
    m_cfg = new config("GardenPhotos");
    m_cfg->init();
     
    //Database Connection
    Poco::Data::SQLite::Connector::registerConnector();
    session = new Session("SQLite",m_cfg->db_path());

    // make sure to call this first
    wxInitAllImageHandlers();
    
    frame = new main_frame(NULL, wxID_ANY, wxT(""), wxPoint(50,50), wxSize(1024,768), wxDEFAULT_FRAME_STYLE);
    frame->init(*m_cfg,*session);
    
    

    frame->Show();
    
    return true;
}

IMPLEMENT_APP(garden_photos)
