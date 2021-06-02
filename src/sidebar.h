//
//  sidebar.h
//  gardenphotos
//
//  Created by Andrea Giardina on 02/04/21.
//

#ifndef sidebar_h
#define sidebar_h

#include <iostream>
#include <wx/wx.h>
#include "sidebar_item.h"

class sidebar : public wxPanel
{
private:
    sidebar_item* m_photos_link;
    sidebar_item* m_favorites_link;
    
    void on_item_click(wxMouseEvent& event);
public:
    sidebar(wxFrame* parent): wxPanel(parent) {};
    void set_active_item(std::string uid);
    void init();
};

#endif /* sidebar_h */
