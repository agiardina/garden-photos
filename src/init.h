//
//  init.h
//  GardenPhotos
//
//  Created by Andrea Giardina on 05/06/21.
//

#ifndef init_h
#define init_h

#include <Poco/Data/Session.h>
#include "config.h"
#include "result.h"

namespace init
{
result<int> init_fs(config &cfg);
result<Poco::Data::Session*> init_db(config &cfg);
}

#endif /* init_h */
