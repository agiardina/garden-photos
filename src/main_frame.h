//
//  main_frame.h
//  gardenphotos
//
//  Created by Andrea Giardina on 01/06/21.
//

#ifndef main_frame_h
#define main_frame_h

#include <Poco/Data/Session.h>
#include <wx/frame.h>
#include <wx/event.h>
#include <wx/sizer.h>
#include "menu.h"
#include "config.h"
#include "image_panel.h"
#include "sidebar.h"

#define GP_TOGGLE_FAVORITE (wxID_HIGHEST + 1)

class main_frame: public wxFrame
{
private:
    //Widgets
    menu* m_menu;
    image_panel* m_main_panel;
    sidebar* m_sidebar;
    wxBoxSizer* m_sizer;
    
    
    config* m_cfg;
    Poco::Data::Session* m_session;
    
    
    //Private methods
    void populate_toolbar();
    void populate_menu();
    void build();
    void bind_events();
public:
    using wxFrame::wxFrame;
    void init(config &cfg, Poco::Data::Session &session);
    void on_import(wxEvent& WXUNUSED(event));
    void on_sidebar_click(wxCommandEvent& event);
    void on_photo_changed(wxCommandEvent& event);
    void on_favorite_toggle(wxEvent& event);
    void show_photo_as_favorite(bool is_favorite);
};

#endif /* main_frame_h */
