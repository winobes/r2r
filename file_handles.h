#include "r2r.h"
#include <string.h>
#include <stdio.h>
#include <csv.h>

/* Function: load_data
 * -------------------
 * Returns a pointer to the data loaded by load_database. If something goes
 * wrong or if load_info_file does not show an open database or is corrupted
 * we should return a new database.
 */
R2RDatabase* load_data();

/* Function: free_database
 * ----------------------
 * Frees the memory allocated by load_data and load_database
 */
void free_run(R2RRun *run);
void free_database(R2RDatabase *database);
void save_database(char *filename, R2RDatabase *database);
