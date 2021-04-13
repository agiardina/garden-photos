//
//  garden_photos.h
//  gardenphotos
//
//  Created by Andrea Giardina on 27/02/21.
//

#ifndef garden_photos_h
#define garden_photos_h

#include <iostream>
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/sizer.h>
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
#include "garden_photos.h"
#include "sidebar.h"

class garden_photos: public wxApp
{
private:
    wxFrame *frame;
    image_panel *drawPane;
    image_panel_controller *m_image_panel_controller;
    sidebar *sid;
    Menu *menu;
    Poco::Data::Session *session;
    std::string thumbs_path;
    
    //wxPanel *main;
    
public:
    bool OnInit();
    void onImport( wxEvent& WXUNUSED(event) );
    void on_sidebar_click(wxCommandEvent& event);
    void show_photo(int id);
};

#endif /* garden_photos_h */
