#include "r2r.h"

#define MAX_NAME 256

/* Function: load_info_file
 * ------------------------
 * Loads the name of the current database from a list of previously
 * opened database filenames. The list is contained in the install
 * directory "info.dat".
 *
 * Returns:
 *  0: success
 *  1: cannot find "info.dat"
 *  2: "info.dat" is corrupted.
 */
int load_info_file(char open_database[MAX_NAME]);

/* Function: load_database
 * -----------------------
 * Copies data from the given database file.
 * 
 * Returns:
 *  0: success
 *  1: cannot find the given file
 *  2: database file is corrupted
 */
int load_database(R2RDatabase *data, char file[MAX_NAME]);


