//
//  menu.cpp
//  gardenphotos
//
//  Created by Andrea Giardina on 14/02/21.
//

#include "menu.h"

void menu::init()
{
    file = new wxMenu();
    file->Append(IMPORT_FOLDER, wxT("&Import folder"));
    Append(file, wxT("&File"));
}
