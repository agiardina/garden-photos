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
#include "image_panel.h"
#include "photos.h"
#include "menu.h"
#include "config.h"
#include "garden_photos.h"
#include "main_frame.h"
#include "sidebar.h"

class garden_photos: public wxApp
{
private:
    config* m_cfg;
    main_frame *frame;
    Poco::Data::Session *session;

public:
    bool OnInit();
    void on_sidebar_click(wxCommandEvent& event);
    void show_photo(int id);
};

#endif /* garden_photos_h */
