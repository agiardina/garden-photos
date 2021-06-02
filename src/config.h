//
//  config.h
//  gardenphotos
//
//  Created by Andrea Giardina on 01/06/21.
//

#ifndef config_h
#define config_h

#include <wx/config.h>
#include <wx/string.h>

class config: public wxConfig
{
private:
    wxString m_base_path;
    wxString m_thumbs_path;
    wxString m_tmp_path;
    wxString m_db_path;
    
public:
    using wxConfig::wxConfig;
    void init();
    std::string base_path();
    std::string thumbs_path();
    std::string db_path();
    std::string tmp_path();
};

#endif /* config_h */
