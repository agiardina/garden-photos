//
//  sidebar.cpp
//  GardenPhotos
//
//  Created by Andrea Giardina on 02/04/21.
//

#include <stdio.h>
#include <stdio.h>
#include "sidebar.h"
#include "sidebar_item.h"
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/sizer.h>


void sidebar::init()
{
    this->SetWindowStyle(wxNO_BORDER);
    wxSizer* sidebar_sizer = new wxBoxSizer(wxVERTICAL);
    
    m_photos_link = new sidebar_item(this,"photos", "All photos", "photos");
    m_favorites_link = new sidebar_item(this,"favorites", "Favorites", "favorite");
    m_photos_link->active(true);
    
    this->SetSizer(sidebar_sizer);
    sidebar_sizer->Add(m_photos_link,wxSizerFlags(0).Border(wxTOP, 35));
    sidebar_sizer->Add(m_favorites_link,wxSizerFlags(0));
}

void sidebar::set_active_item(std::string uid)
{
    if (uid == "photos") {
        m_photos_link->active(true);
        m_favorites_link->active(false);
    } else if (uid == "favorites") {
        m_photos_link->active(false);
        m_favorites_link->active(true);
    }
    this->Refresh();
}
