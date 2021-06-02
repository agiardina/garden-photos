//
//  sidebar_item.cpp
//  gardenphotos
//
//  Created by Andrea Giardina on 07/03/21.
//

#include <stdio.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include "sidebar_item.h"
#include "utils.h"

wxDEFINE_EVENT(GP_SIDEBAR_CLICK, wxCommandEvent);

sidebar_item::sidebar_item(wxWindow* parent, const std::string uid, const std::string str_label, const std::string str_icon) : wxPanel(parent)
{
    m_uid = uid;
    
    wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->SetMinSize(250, 35);
    this->SetSizer(sizer);
    
    wxStaticText* label = new wxStaticText(this, wxID_ANY, str_label);
    label->SetForegroundColour(wxColor(255,255,255));
    wxStaticBitmap* icon = new wxStaticBitmap(this,wxID_ANY,wxBitmap(utils::resource_path(str_icon+"-18.png"), wxBITMAP_TYPE_PNG));
    
    icon->Bind(wxEVT_LEFT_DOWN, &sidebar_item::on_item_click,this);
    label->Bind(wxEVT_LEFT_DOWN, &sidebar_item::on_item_click,this);
    Bind(wxEVT_LEFT_DOWN, &sidebar_item::on_item_click,this);
    
    sizer->Add(icon,wxSizerFlags().Expand().Border(wxTOP | wxLEFT, 10));
    sizer->Add(label,wxSizerFlags().Expand().Border(wxTOP | wxLEFT, 10));
    this->SetSizer(sizer);
    
}

void sidebar_item::on_item_click(wxMouseEvent& event)
{
    wxCommandEvent cmd_event(GP_SIDEBAR_CLICK);
    cmd_event.SetString(m_uid);
    wxPostEvent(this, cmd_event);
}

void sidebar_item::active(bool active_flag)
{
    m_active = active_flag;
    if (m_active) {
        this->SetBackgroundColour(wxColour(75,70,67));
    } else {
        this->SetBackgroundColour(wxColour(44,44,43));
    }
}
