#include "Poco/Data/Session.h"
#include "Poco/Glob.h"
#include "Poco/Path.h"
#include <iostream>
#include <vector>

#ifndef _PHOTOS_H_
#define _PHOTOS_H_

namespace photos
{

// NAMESPSACE START
// ------------------------------------------------------------

struct photo {
    unsigned int id = 0;
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int size = 0;
    unsigned int orientation = 1;
    double lat = 0.0;
    double lng = 0.0;
    std::string date_time_original = "";
    unsigned int gallery_id = 0;
    std::string path = "";
    unsigned int local_gallery_id = 0;
    std::string local_path = "";
};

struct gallery {
    unsigned int id = 0;
    std::string path;
};

std::vector<photo> all_photos(Poco::Data::Session& session);
std::vector<std::string> photos_on_folder(std::string base_path);
photo path_to_photo(std::string path);
gallery gallery_by_path(const std::string& path,Poco::Data::Session& session);
bool add_photo_to_gallery(photos::photo photo, const photos::gallery& gallery,Poco::Data::Session& session,const std::string& thumbs_path);
bool add_photo_to_gallery(const std::string& photo_path, const photos::gallery& gallery, Poco::Data::Session& session, const std::string& thumbs_path);


// NAMESPSACE START
// ------------------------------------------------------------
}
#endif
