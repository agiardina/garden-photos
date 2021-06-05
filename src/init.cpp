//
//  init.cpp
//  GardenPhotos
//
//  Created by Andrea Giardina on 05/06/21.
//

#include <stdio.h>
#include <iostream>
#include <Poco/Data/Session.h>
#include <Poco/Data/SQLite/Connector.h>
#include "Poco/File.h"
#include "Poco/Exception.h"
#include "init.h"
#include "utils.h"
#include "config.h"
#include "result.h"


result<int> init::init_fs(config &cfg)
{
    Poco::File base_path(cfg.base_path());
    Poco::File tmp_path(cfg.tmp_path());
    Poco::File thumbs_path(cfg.thumbs_path());
    
    result<int> res;

    try {
        base_path.createDirectories();
        tmp_path.createDirectories();
        thumbs_path.createDirectories();
    } catch (Poco::Exception e) {
        std::cout << e.message();
    }
    
    if (!utils::is_dir_writable(base_path)) return res.error("Impossible to create folder " + cfg.base_path());
    if (!utils::is_dir_writable(tmp_path)) return res.error("Impossible to create folder " + cfg.tmp_path());
    if (!utils::is_dir_writable(thumbs_path)) return res.error("Impossible to create folder " + cfg.thumbs_path());
    return res.ok(1);
}

//result<Poco::Data::Session*> init_db(config &cfg)
//{
//    Poco::Data::SQLite::Connector::registerConnector();
//    result<Poco::Data::Session*> res;
//    Poco::Data::Session* sess = new Poco::Data::Session("SQLite",cfg.db_path());
//    return res.ok(sess);
//}

result<Poco::Data::Session*> init::init_db(config &cfg)
{
    Poco::Data::SQLite::Connector::registerConnector();
    result<Poco::Data::Session*> res;
    return res.ok(new Poco::Data::Session("SQLite",cfg.db_path()));
}
