//
//  sidebar_item.h
//  gardenphotos
//
//  Created by Andrea Giardina on 07/03/21.
//

#ifndef sidebar_item_h
#define sidebar_item_h

#include <iostream>
#include <wx/wx.h>

wxDECLARE_EVENT(GP_SIDEBAR_CLICK, wxCommandEvent);

class sidebar_item: public wxPanel
{
private:
    void on_item_click(wxMouseEvent& event);
    bool m_active = false;
public:
    std::string m_uid = "";
    sidebar_item(wxWindow* parent, const std::string uid, const std::string label, const std::string icon);
    void active(bool active_flag);
};
#endif /* sidebar_item_h */
