//
//  utils.h
//  gardenphotos
//
//  Created by Andrea Giardina on 24/05/21.
//

#ifndef utils_h
#define utils_h

#include <iostream>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/window.h>
#include <Poco/File.h>

namespace utils
{
int str_to_int(const std::string s, const int default_value=0);
std::string resource_path(const std::string relative_path);
wxBitmap small_icon(const std::string str_icon);
wxBitmap toolbar_icon(const std::string str_icon,double dpi);
std::string wx2str(const wxString input);
bool is_dir_writable(Poco::File path);
}
#endif /* utils_h */
