//
//  main_frame.cpp
//  GardenPhotos
//
//  Created by Andrea Giardina on 01/06/21.
//

#include <Poco/Data/Session.h>
#include <wx/event.h>
#include <wx/toolbar.h>
#include <wx/colour.h>
#include <wx/dirdlg.h>
#include <wx/progdlg.h>
#include <wx/bitmap.h>
#include "main_frame.h"
#include "utils.h"
#include "menu.h"
#include "config.h"
#include "photos.h"
#include "events.h"

void main_frame::init(config &cfg, Poco::Data::Session &session)
{
    m_cfg = &cfg;
    m_session = &session;
    build();
    bind_events();
}

void main_frame::build()
{
    SetBackgroundColour(wxColour(44,44,43));
    
    populate_menu();
    populate_toolbar();
    
    m_main_panel = new image_panel(this);
    m_main_panel->init(*m_cfg, *m_session);
    
    m_sidebar = new sidebar(this);
    m_sidebar->init();
    
    m_sizer = new wxBoxSizer(wxHORIZONTAL);
    m_sizer->Add(m_sidebar, 0, wxALIGN_LEFT);
    m_sizer->Add(m_main_panel, 2, wxEXPAND);
    SetSizer(m_sizer);
}

void main_frame::populate_menu()
{
    m_menu = new menu();
    m_menu->init();
    SetMenuBar(m_menu);
}

void main_frame::populate_toolbar()
{
    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddTool(GP_EVT_TOGGLE_FAVORITE,"Toggle Favorite",utils::toolbar_icon("favorite",GetContentScaleFactor()));
    toolbar->Realize();
}

void main_frame::bind_events()
{
    Bind(wxEVT_COMMAND_MENU_SELECTED, &main_frame::on_import, this, GP_EVT_IMPORT_FOLDER);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &main_frame::on_favorite_toggle, this, GP_EVT_TOGGLE_FAVORITE);
    Bind(GP_SIDEBAR_CLICK, &main_frame::on_sidebar_click,this);
    Bind(GP_PHOTO_CHANGED, &main_frame::on_photo_changed,this);
}

void main_frame::on_favorite_toggle( wxEvent& event)
{
    std::cout << event.GetId() << "\n";
    std::cout << GP_EVT_IMPORT_FOLDER << "\n";
    std::cout << GP_EVT_TOGGLE_FAVORITE << "\n";
    
    int photo_id = m_main_panel->displayed_photo();
    if (photo_id > 0) { //No current photo displayed
        photos::photo curr_photo = photos::toggle_favorite(photo_id, *m_session);
        show_photo_as_favorite(curr_photo.is_favorite);
    }
}

void main_frame::on_import( wxEvent& WXUNUSED(event) )
{
    wxDirDialog dir_dlg(NULL, "Choose folder to import", "",
                    wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
    
    if (dir_dlg.ShowModal() == wxID_OK)
    {
    
        photos::gallery gallery = photos::gallery_by_path(m_cfg->base_path(),*m_session);
        std::vector<std::string> photos_to_import = photos::photos_on_folder(utils::wx2str(dir_dlg.GetPath()));
        int n_photos = photos_to_import.size();
        wxProgressDialog dialog("Import","Importing photos",n_photos);
        for ( int i = 0; i < n_photos; ++i ) {
            if ( !dialog.Update(i)) {
                // Cancelled by user.
                break;
            }
            photos::add_photo_to_gallery(photos_to_import[i], gallery, *m_session, m_cfg->thumbs_path());
        }
    }
}

void main_frame::on_sidebar_click(wxCommandEvent& event)
{
    std::string uid = std::string(event.GetString().mb_str());
    m_sidebar->set_active_item(uid);
    
    if (uid=="photos") {
        m_main_panel->load_photos();
    } else if (uid=="favorites") {
        m_main_panel->load_favorites_photos();
    }
}

void main_frame::on_photo_changed(wxCommandEvent &event)
{
    int id_photo = event.GetInt();
    photos::photo curr_photo = photos::select_photo_from_id(id_photo, *m_session);
    show_photo_as_favorite(curr_photo.is_favorite);
}

void main_frame::show_photo_as_favorite(bool is_favorite)
{
    if (is_favorite) {
        GetToolBar()->SetToolNormalBitmap(GP_EVT_TOGGLE_FAVORITE,utils::toolbar_icon("favoritewhite",GetContentScaleFactor()));
    } else {
        GetToolBar()->SetToolNormalBitmap(GP_EVT_TOGGLE_FAVORITE,utils::toolbar_icon("favorite",GetContentScaleFactor()));
    }
}
