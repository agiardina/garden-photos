#include <vector>
#include "sqlite3.h"
#include "photos.h"

std::vector<photo> Photos::get_all_photos()
{
  std::vector<photo> result;
  sqlite3_stmt* stmt = 0;
  int rc = sqlite3_prepare_v2( conn,"select id from photos order by date_time_original desc", -1, &stmt, 0 );

  rc = sqlite3_exec( conn, "BEGIN TRANSACTION", 0, 0, 0 );
  while ( sqlite3_step( stmt ) == SQLITE_ROW ) { // While query has result-rows.
    unsigned int id = sqlite3_column_int(stmt, 0);
    result.push_back({id});
  }
  rc = sqlite3_step( stmt );
  char *zErrMsg = 0;  //  Can perhaps display the error message if rc != SQLITE_OK.
  rc = sqlite3_exec( conn, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
  rc = sqlite3_finalize( stmt );
  return result;
};
