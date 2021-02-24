#include <vector>
#include <iostream>
#include "sqlite3.h"
#include "Poco/Exception.h"
#include "Poco/Glob.h"
#include "Poco/File.h"
#include "Poco/RecursiveDirectoryIterator.h"
#include "Poco/Environment.h"
#include "photos.h"
#include <exiv2/exiv2.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>


using namespace Poco::Data::Keywords;
using namespace Poco;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Glob;

namespace utils {
    int str_to_int(const std::string s, const int default_value=0) {
        if (s=="") {
            return default_value;
        } else {
            try {
                return std::stoi(s);
            } catch(...) {
                return default_value;
            }
        }
    }
};

namespace photos
{

int EXIF_INVALID_GPS = 1000;
    
// NAMESPSACE START
// ------------------------------------------------------------
std::vector<photo> all_photos(Poco::Data::Session& session)
{
    std::vector<photo> result;
    Statement select(session);
    photo curr_photo =
    {
        0,
        1000,
        1000
    };
    
    select << "SELECT id FROM photos ORDER BY date_time_original DESC", into(curr_photo.id), range(0, 1);
    
    while (!select.done())
    {
        try {
            select.execute();
            photo p;
            p.id  = curr_photo.id;
            result.push_back(p);
        }
        catch (Poco::Data::DataException &ex)
        {
            std::cout << ex.message();
//            throw Storage::StorageErrorException(ex.message());
        }
    }
    
    return result;
};


photos::gallery select_gallery_by_path(const std::string& path,Poco::Data::Session& session)
{
    photos:gallery g;
    Poco::Data::Statement select(session);
    select << "SELECT id, path FROM galleries",
            into(g.id),
            into(g.path),
            range(0, 1); //  iterate over result set one row at a time

    if (!select.done())
    {
        select.execute();
    }
    return g;
};

photos::gallery insert_gallery(gallery g,Poco::Data::Session& session)
{
    Poco::Data::Statement insert(session);
    insert << "INSERT INTO galleries (path) VALUES(?)",
        Poco::Data::Keywords::use(g.path);
    insert.execute();
    return select_gallery_by_path(g.path, session);
};

photos::gallery gallery_by_path(const std::string& path,Poco::Data::Session& session)
{
    photos:gallery g = select_gallery_by_path(path,session);
    if (g.id==0)
    {
        g.path = path;
        g = insert_gallery(g, session);
    }
    return g;
}

int select_photo_id_from_path(std::string& photo_path,Poco::Data::Session& session)
{
    Poco::Data::Statement select(session);
    photo curr_photo = {};

    select << "SELECT id FROM photos WHERE path = ? LIMIT 1",
                into(curr_photo.id),
                Poco::Data::Keywords::use(photo_path),
                range(0, 1);

    if (!select.done())
    {
        try
        {
            select.execute();
        }
        catch (Poco::Data::DataException &ex)
        {
            std::cout << ex.message();
        }
    }
    return curr_photo.id;
};

int insert_photo(photos::photo& photo, Poco::Data::Session& session)
{
    Poco::Data::Statement insert(session);
    insert << "INSERT INTO photos (path,width,height,size,date_time_original,orientation,gallery_id) VALUES (?,?,?,?,?,?,?)",
        Poco::Data::Keywords::use(photo.path),
        Poco::Data::Keywords::use(photo.width),
        Poco::Data::Keywords::use(photo.height),
        Poco::Data::Keywords::use(photo.size),
        Poco::Data::Keywords::use(photo.date_time_original),
        Poco::Data::Keywords::use(photo.orientation),
        Poco::Data::Keywords::use(photo.gallery_id);
    
    insert.execute();
    return select_photo_id_from_path(photo.path,session);
};


inline std::pair<int, int> calc_width_height(int width,int height,int max_size)
{
    int new_img_width, new_img_height;
    double ratio = (double)width / (double)height;
    
    if(width > height)
    {
        new_img_width = max_size;
        new_img_height = (int)(new_img_width / ratio);
    } else {
        new_img_height = max_size;
        new_img_width = (int)(ratio * new_img_height);
    }
    return std::make_pair(new_img_width, new_img_height);
};

void create_thumb(const cv::Mat &input,int photo_id, int width, int height, const std::string &thumbs_path,int max_size)
{
    int new_width,new_height;
    std::tie(new_width,new_height) = calc_width_height(width,height,max_size);
    std::string thumb_path = thumbs_path + "/"+std::to_string(max_size)+"/" + std::to_string(photo_id) + ".jpg";
    
    cv::Mat resized;
    cv::resize(input, resized, cv::Size(new_width,new_height));
    cv::imwrite(thumb_path, resized);
    std::cout << thumb_path << " saved\n";
};

void create_thumbs(const std::string &photo_path,int photo_id, int original_width, int original_height, const std::string &thumbs_path)
{
    cv::Mat input = cv::imread(photo_path, cv::IMREAD_COLOR);
    create_thumb(input,photo_id,original_width,original_height,thumbs_path,960);
    create_thumb(input,photo_id,original_width,original_height,thumbs_path,320);
    create_thumb(input,photo_id,original_width,original_height,thumbs_path,60);
};

bool add_photo_to_gallery(photos::photo photo, const photos::gallery& gallery,Poco::Data::Session& session,const std::string& thumbs_path)
{
    if (photo.width && photo.height) {
        photo.gallery_id = gallery.id;
        int photo_id = insert_photo(photo,session);
        create_thumbs(photo.path, photo_id, photo.width, photo.height,thumbs_path);
        return true;
    } else {
        return false;
    }
};

bool add_photo_to_gallery(const std::string& photo_path, const photos::gallery& gallery, Poco::Data::Session& session, const std::string& thumbs_path)
{
    photo p = path_to_photo(photo_path);
    return add_photo_to_gallery(p,gallery,session,thumbs_path);
};

std::string exif_key(Exiv2::ExifData &exif, const std::string &key_str)
{
    try {
        Exiv2::ExifKey key(key_str);
        Exiv2::ExifData::iterator it = exif.findKey(key);
        if (it != exif.end()) {
            return it->toString();
        } else {
            return "";
        }
    } catch (...) {
        return "";
    }
};

std::string exif_date(Exiv2::ExifData &exif)
{
    return exif_key(exif,"Exif.Photo.DateTimeOriginal");
};

int exif_orientation(Exiv2::ExifData &exif)
{
    int orientation = utils::str_to_int(exif_key(exif,"Exif.Image.Orientation"),1);
    if (orientation < 1 || orientation > 8) {
        return 1;
    } else {
        return orientation;
    }
};

int exif_width(Exiv2::ExifData &exif)
{
    return utils::str_to_int(exif_key(exif,"Exif.Image.ImageWidth"));
};

int exif_height(Exiv2::ExifData &exif)
{
    return utils::str_to_int(exif_key(exif,"Exif.Image.ImageLength"));
};

cv::Size photo_width_height(std::string photo_path)
{
    cv::Mat img = cv::imread(photo_path.c_str());
    return img.size();
};

int photo_size(std::string photo_path)
{
    Poco::File photo_file(photo_path);
    return photo_file.getSize();
};

double exif_gps(Exiv2::ExifData &exif, const std::string &coord_key, const std::string &ref_key)
{
    double degrees = 0.0;
    
    Exiv2::ExifKey key(coord_key);
    Exiv2::ExifData::iterator it = exif.findKey(key);
    if (it != exif.end() && it->count() == 3)
    {
        double deg_num, deg_den, min_num, min_den, sec_num, sec_den;
        
        deg_num = (double)(it->toRational(0).first);
        deg_den = (double)(it->toRational(0).second);
        min_num = (double)(it->toRational(1).first);
        min_den = (double)(it->toRational(1).second);
        sec_num = (double)(it->toRational(2).first);
        sec_den = (double)(it->toRational(2).second);
        
        if ((deg_den != 0) &&
            (min_den != 0) &&
            (sec_den != 0 || sec_num == 0)) {
            
            if (sec_num == 0) { // 0/0 is valid for seconds
                sec_num = 1;
            }
            
            degrees = deg_num/deg_den;
            if ((min_num / min_den) != 1) {
                degrees += (min_num / min_den / 60.0);
            }
            if ((sec_num / sec_den) != 1) {
                degrees += (sec_num / sec_den / 3600.0);
            }
        }
        
        std::string dir =  exif_key(exif,ref_key);
        if (dir=="S" || dir == "W") {
            degrees *= -1.0;
        }
        return degrees;
    }
};

double exif_latitude(Exiv2::ExifData &exif)
{
    return exif_gps(exif,"Exif.GPSInfo.GPSLatitude","Exif.GPSInfo.GPSLatitudeRef");
};

double exif_longitude(Exiv2::ExifData &exif)
{
    return exif_gps(exif,"Exif.GPSInfo.GPSLongitude","Exif.GPSInfo.GPSLongitudeRef");
};

photo path_to_photo(std::string photo_path)
{
    photo p;
    try {
        auto image = Exiv2::ImageFactory::open(photo_path);
        assert(image.get() != 0);
        image->readMetadata();
        Exiv2::ExifData &exif = image->exifData();
        
        cv::Size wh;
        
        p.orientation = exif_orientation(exif);
        p.width = exif_width(exif);
        p.height = exif_width(exif);
        if (p.width == 0|| p.height == 0) {
            wh = photo_width_height(photo_path);
            p.width = wh.width;
            p.height = wh.height;
        }
        p.lat = exif_latitude(exif);
        p.lng = exif_longitude(exif);
        p.date_time_original = exif_date(exif);
        p.path = photo_path;
        p.size = photo_size(photo_path);
        
    } catch (...) {
//        std::cout << "Impossible to import " + photo_path + "\n";
        std::exception_ptr eptr = std::current_exception();
    }
    return p;
};

std::vector<std::string> photos_on_folder(std::string base_path)
{
    std::vector<std::string> files;
    Path path(base_path);
    SiblingsFirstRecursiveDirectoryIterator end;
    
    for (SiblingsFirstRecursiveDirectoryIterator it(path); it != end; ++it)
    {
        std::string filename = it.path().toString();
        if (it.path().getExtension()=="jpg") {
            files.push_back(it.path().toString(Poco::Path::PATH_NATIVE));
        }
    }
    return files;
};


// NAMESPSACE END
// ------------------------------------------------------------
}
