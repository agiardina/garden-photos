//
//  utils.cpp
//  GardenPhotos
//
//  Created by Andrea Giardina on 20/05/21.
//

#include <stdio.h>
#include <wx/stdpaths.h>
#include <wx/window.h>
#include <wx/image.h>
#include "utils.h"

using namespace utils;


int utils::str_to_int(const std::string s, const int default_value)
{
    if (s=="") {
        return default_value;
    } else {
        try {
            return std::stoi(s);
        } catch(...) {
            return default_value;
        }
    }
};

std::string utils::wx2str(const wxString input)
{
    return std::string(input.mb_str());
}

std::string utils::resource_path(const std::string relative_path)
{
    return utils::wx2str(wxStandardPaths::Get().GetResourcesDir()) + "/" + relative_path;
}

wxBitmap utils::small_icon(const std::string str_icon)
{
    return wxBitmap(utils::resource_path(str_icon+"-24.png"), wxBITMAP_TYPE_PNG);
}

wxBitmap utils::toolbar_icon(const std::string str_icon,double scale)
{
    if (scale < 2.0) {
        return wxBitmap(utils::resource_path(str_icon +"-24.png"), wxBITMAP_TYPE_PNG);
    } else {
        wxImage img(utils::resource_path(str_icon +"-48.png"), wxBITMAP_TYPE_PNG);
        return wxBitmap(img,-1,scale);
    }
    
}
