//
//  menu.h
//  gardenphotos
//
//  Created by Andrea Giardina on 01/06/21.
//

#ifndef menu_h
#define menu_h

#include <wx/menu.h>

class menu: public wxMenuBar
{
private:
    wxMenu* file;
    
public:
    using wxMenuBar::wxMenuBar;
    void init();
};

#endif /* menu_h */
