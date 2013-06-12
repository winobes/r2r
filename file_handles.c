#include "file_handles.h"

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
static int load_info_file(R2RDatabase *database)
{

        int ch = '\0';
        int i = 0;
        int line;
        char temp[MAX_NAME];

        FILE *fp = fopen("data/info.dat", "r");
        if (fp == NULL)
                return 1;

        /* Reading the line number of last open database */
        while (ch != '\n') {
                ch = getc(fp);
                temp[i] = ch;
                i++;
                if (ch == EOF || i > MAX_NAME) {
                        return 2;
                }
        }
        temp[i] = '\0';
        line = strtol(temp, NULL, 10); 
        if (line == 0)
                return 2;

        /* Moving to the line */
        i = 1;
        while (i < line) { 
                ch = getc(fp);
                if (ch == '\n')
                        i++;
                if (ch == EOF) 
                        return 2;
        }
        fpos_t pos;
        fgetpos(fp, &pos);
        ch = getc(fp);

        /* Counting the charecters in database name */
        i = 0;
        char cch;
        while (cch != '\n') {
                printf("%c\n",cch);
                cch = getc(fp);
                i++;
                if (cch == EOF)
                        return 2;
        }

        /* Allocating filename space */
        database->filename_len = i + 1;
        database->filename = g_malloc(database->filename_len + 1);

        /* Reading the database name */
        fsetpos(fp, &pos);
        i = 0;
        while (ch != '\n') {
                printf("ch = %c\n",ch);
                ch = getc(fp);
                database->filename[i] = ch;
                i++;
                if (ch == EOF)
                        return 2;
        }

        database->filename[i-1] = '\0';
        fclose(fp);

        return 0;
}

/* Function: load_database
 * -----------------------
 * Copies data from the given database file.
 * 
 * Returns:
 *  0: success
 *  1: cannot find the given file
 *  2: database file is corrupted
 */
static int load_database(R2RDatabase *data)
{

        int retval = 0;

        size_t temp_len = 3;
        char *temp = g_malloc(temp_len + 1);
        if (temp == NULL)
                return 1; //insufficient memory


        char full_path[data->filename_len + path_len + 1];
        strcpy(full_path, path);
        strcat(full_path, data->filename);

        CSV_BUFFER *buffer = csv_create_buffer();
        if (csv_load(buffer, full_path) != 0) {
                retval = 1;
                printf("filenotfound\n");
                goto end;
        }

        /* Load header info:
         * Database name
         * Units preference
         * Number of entries (runs)
         */ 
        data->name_len = csv_get_field_length(buffer, 0, 0);
        data->name = g_malloc(data->name_len + 1);
        csv_get_field(data->name, data->name_len, buffer, 0, 0);
        csv_get_field(temp, temp_len,  buffer, 0, 1);
        data->units = strtol(temp, NULL, 10);
        data->nruns = csv_get_height(buffer) - 1;

        /* Create body space */
        int i, j; 
        data->run = g_malloc(data->nruns * sizeof(R2RRun*));
        for (i = 0; i < data->nruns; i++) 
                data->run[i] = g_malloc(sizeof(R2RRun));

        /* Load body */
        for (i = 1; i < data->nruns + 1; i++) {
        for (j = 0; j < NFIELDS; j++) {
                if (csv_get_field_length(buffer, i, j) > temp_len)
                        temp_len = csv_get_field_length(buffer, i, j);
                if (csv_get_field_length(buffer, i, j) == 0 )
                        printf("%i,%i\n", i,j);
        }
        }
        char *temp_temp = realloc(temp, temp_len + 1);
        if (temp_temp == NULL) {
                g_free(temp); 
                return 1; // insufficient memory
        } else 
                temp = temp_temp;

        for (i = 0; i < data->nruns; i++) {
                csv_get_field(temp, temp_len, buffer, i+1, 0);
                data->run[i]->year = strtol(temp, NULL, 10);

                csv_get_field(temp, temp_len, buffer, i+1, 1);
                data->run[i]->month = strtol(temp, NULL, 10);

                csv_get_field(temp, temp_len, buffer, i+1, 2);
                data->run[i]->day = strtol(temp, NULL, 10);

                csv_get_field(temp, temp_len, buffer, i+1, 3);
                data->run[i]->run_n = strtol(temp, NULL, 10);

                csv_get_field(temp, temp_len, buffer, i+1, 4);
                data->run[i]->distance = strtof(temp, NULL);

                csv_get_field(temp, temp_len, buffer, i+1, 5);
                data->run[i]->duration= strtof(temp, NULL);

                 data->run[i]->type_len = csv_get_field_length(buffer, i+1, 6);
                data->run[i]->type = g_malloc(data->run[i]->type_len + 1);
                csv_get_field(data->run[i]->type, data->run[i]->type_len, buffer, i+1, 6);
       
                csv_get_field(temp, temp_len, buffer, i+1, 7);
                data->run[i]->feel = strtol(temp, NULL, 10);  

                csv_get_field(temp, temp_len, buffer, i+1, 8);
                data->run[i]->time = strtol(temp, NULL, 10);

                data->run[i]->route_len = csv_get_field_length(buffer, i+1, 9);
                data->run[i]->route = g_malloc(data->run[i]->route_len + 1);
                csv_get_field(data->run[i]->route, data->run[i]->route_len, buffer, i+1, 9);

                data->run[i]->notes_len = csv_get_field_length(buffer, i+1, 10);
                data->run[i]->notes = g_malloc(data->run[i]->notes_len + 1);
                csv_get_field(data->run[i]->notes, data->run[i]->notes_len, buffer, i+1, 10);
 
        } 
 
        end:
        csv_destroy_buffer(buffer);
        g_free(temp);
        return 0;
}

R2RDatabase* load_data()
{
        R2RDatabase *database = g_malloc(sizeof(R2RDatabase));

        load_info_file(database);
        load_database(database);

        return database;
}

void free_run(R2RRun *run)
{
        puts("freeing route");
        g_free(run->route);
        puts("freeing type");
        g_free(run->type);
        puts("freeing notes");
        g_free(run->notes);
        puts("freeing run");
        g_free(run);
}

void free_database(R2RDatabase *database)
{
        int i;
        
        for (i = 0; i < database->nruns; i++) {
                free_run(database->run[i]);
        }
        g_free(database);
}

void save_database(R2RDatabase *database)
{

        puts("creating csv file");
        CSV_BUFFER *csv_file = csv_create_buffer();
        csv_set_text_delim(csv_file, '"');
        csv_set_field_delim(csv_file, ',');

        char buff[50];

        puts("writing header stuff");
        csv_set_field(csv_file, 0, 0, database->name);

        g_snprintf(buff, 50, "%i", database->units);
        csv_set_field(csv_file, 0, 1, buff); 
 
        puts("writing body");
        int i;
        for (i = 0; i < database->nruns; i++) {
        
                printf("writing date %i\n", i);
                g_snprintf(buff, 50, "%i", database->run[i]->year);
                csv_set_field(csv_file, i + 1, 0, buff);

                g_snprintf(buff, 50, "%i", database->run[i]->month);
                csv_set_field(csv_file, i + 1, 1, buff);

                g_snprintf(buff, 50, "%i", database->run[i]->day);
                csv_set_field(csv_file, i + 1, 2, buff);

                g_snprintf(buff, 50, "%i", database->run[i]->run_n);
                csv_set_field(csv_file, i + 1, 3, buff);

                printf("writing distance %i\n", i);
                g_snprintf(buff, 50, "%f", database->run[i]->distance);
                csv_set_field(csv_file, i + 1, 4, buff);

                printf("writing duration %i\n", i);
                g_snprintf(buff, 50, "%i", database->run[i]->duration);
                csv_set_field(csv_file, i + 1, 5, buff);

                printf("writing type %i\n", i);
                csv_set_field(csv_file, i + 1, 6, database->run[i]->type);

                printf("writing feel %i\n", i);
                g_snprintf(buff, 50, "%i", database->run[i]->feel);
                csv_set_field(csv_file, i + 1, 7, buff);

                printf("writing time %i\n", i);
                g_snprintf(buff, 50, "%i", database->run[i]->time);
                csv_set_field(csv_file, i + 1, 8, buff);

                printf("writing route %i\n", i);
                csv_set_field(csv_file, i + 1, 9, database->run[i]->route);

                printf("writing notes %i\n", i);
                printf("notes pointer = %p\n", database->run[i]->notes);
                printf("notes = %s\n", database->run[i]->notes);
                csv_set_field(csv_file, i + 1, 10, database->run[i]->notes);

        }

        char full_path[database->filename_len + path_len + 1];
        strcpy(full_path, path);
        strcat(full_path, database->filename);

        csv_save(full_path, csv_file);

}
               
//unit tester
//gcc `pkg-config --cflags gtk+-3.0` -o a file_handles.c `pkg-config --libs gtk+-3.0` -lcsv
/*
int main() {

        char *open_database = g_malloc(MAX_NAME + 1);

        load_info_file(open_database);

        printf("database filename = %s\n", open_database);

        R2RDatabase *data = g_malloc(sizeof(R2RDatabase));

        load_database(data, open_database);

        printf(" units = %i name = %s\n", data->units, data->name);

        int i;
        for (i = 0; i < data->nruns; i++) {
                g_free(data->run[i]->route);
                g_free(data->run[i]->notes);
                g_free(data->run[i]);
        }
        g_free(data->run);
        g_free(data->name);        
        g_free(data);
        g_free(open_database);

        return 0;
}
*/
