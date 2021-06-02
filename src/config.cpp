//
//  config.cpp
//  gardenphotos
//
//  Created by Andrea Giardina on 01/06/21.
//

#include <iostream>
#include <stdio.h>
#include <wx/config.h>
#include <wx/stdpaths.h>
#include "config.h"

void config::init()
{
    if (!Read("base_path", &m_base_path)) {
        m_base_path = wxStandardPaths::Get().GetDocumentsDir() + "/GardenPhotos";
        Write("base_path",m_base_path);
    }
    if (!Read("thumbs_path", &m_thumbs_path)) {
        m_thumbs_path = m_base_path + "/thumbs";
        Write("thumbs_path", m_thumbs_path);
    }
    if (!Read("tmp_path", &m_tmp_path)) {
        m_tmp_path = m_base_path + "/tmp";
        Write("tmp_path", m_tmp_path);
    }
    if (!Read("db_path", &m_db_path)) {
        m_db_path = m_base_path + "/db";
        Write("tmp_path", m_db_path);
    }
}

std::string config::base_path()
{
    return std::string(m_base_path.mb_str());
}

std::string config::thumbs_path()
{
    return std::string(m_thumbs_path.mb_str());
}

std::string config::tmp_path()
{
    return std::string(m_tmp_path.mb_str());
}

std::string config::db_path()
{
    return std::string(m_db_path.mb_str());
}
