//
//  init.cpp
//  GardenPhotos
//
//  Created by Andrea Giardina on 05/06/21.
//

#include <stdio.h>
#include <iostream>
#include <vector>
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
    Poco::File big_thumbs_path(cfg.thumbs_path() + "/960");
    Poco::File medium_thumbs_path(cfg.thumbs_path() + "/320");
    Poco::File small_thumbs_path(cfg.thumbs_path() + "/60");
    
    result<int> res;

    try {
        base_path.createDirectories();
        tmp_path.createDirectories();
        thumbs_path.createDirectories();
        big_thumbs_path.createDirectories();
        medium_thumbs_path.createDirectories();
        small_thumbs_path.createDirectories();
    } catch (Poco::Exception e) {
        std::cout << e.message();
    }
    
    if (!utils::is_dir_writable(base_path)) return res.error("Impossible to create folder " + cfg.base_path());
    if (!utils::is_dir_writable(tmp_path)) return res.error("Impossible to create folder " + cfg.tmp_path());
    if (!utils::is_dir_writable(thumbs_path)) return res.error("Impossible to create folder " + cfg.thumbs_path());
    if (!utils::is_dir_writable(big_thumbs_path)) return res.error("Impossible to create big thumbs folder ");
    if (!utils::is_dir_writable(medium_thumbs_path)) return res.error("Impossible to create medium thumbs folder");
    if (!utils::is_dir_writable(small_thumbs_path)) return res.error("Impossible to create small thumbs folder");
    return res.ok(1);
}

int max_version(Poco::Data::Session &session)
{
    bool version_exists = false;
    Poco::Data::Statement select_table(session);
    int max = -1;
    
    select_table << "SELECT 1 FROM sqlite_master WHERE type='table' AND name='version'", Poco::Data::Keywords::into(version_exists);
    select_table.execute();
    if (version_exists) {
        Poco::Data::Statement select_max(session);
        select_max << "SELECT max(id) FROM version", Poco::Data::Keywords::into(max);
        select_max.execute();
    }
    return max;
}

result<Poco::Data::Session*> init::init_db(config &cfg)
{
    Poco::Data::SQLite::Connector::registerConnector();
    result<Poco::Data::Session*> res;
    
    std::vector<std::string> migrations;
    migrations.push_back("CREATE TABLE version (id INTEGER PRIMARY KEY)");
    migrations.push_back("CREATE TABLE galleries ("
                            "id integer PRIMARY KEY AUTOINCREMENT NOT NULL,"
                            "path text"
                         ")");
    migrations.push_back("CREATE TABLE photos ("
                            "id integer PRIMARY KEY AUTOINCREMENT,"
                            "path text,"
                            "origin text,"
                            "width integer,"
                            "height integer,"
                            "size integer,"
                            "date_time_original text,"
                            "orientation integer,"
                            "exif text,"
                            "gallery_id integer,"
                            "is_favorite INTEGER(1)"
                         ")");
    
    Poco::Data::Session* session = new Poco::Data::Session("SQLite",cfg.db_path());
    int version_id = max_version(*session)+1;
    for (;version_id<migrations.size();version_id++) {
        Poco::Data::Statement stmt(*session);
        Poco::Data::Statement stmt_insert_version(*session);
        stmt << migrations[version_id], Poco::Data::Keywords::now;
        stmt_insert_version << "INSERT INTO version(id) VALUES (?)",Poco::Data::Keywords::use(version_id);
        stmt_insert_version.execute();
    }
    
    return res.ok(session);
}
