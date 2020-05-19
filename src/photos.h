#include "sqlite3.h"
#include <vector>

#ifndef _PHOTOS_H_
#define _PHOTOS_H_

struct photo {
  unsigned int id;  
};
  
class Photos {
 private:
  sqlite3 *conn;
 public:
 Photos(sqlite3 *db)
   :conn{db}
  {
  };
  std::vector<photo> get_all_photos();
};

#endif
