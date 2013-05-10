#include "file_handles.h"
#include <string.h>
#include <stdio.h>
#include <csv.h>

int load_info_file(char *open_database)
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
        ch = getc(fp);

        /* Reading the database name */
        i = 0;
        while (ch != '\n') {
                temp[i] = ch;
                ch = getc(fp);
                i++;
                if (ch == EOF || i > MAX_NAME)
                        return 2;
        }

        temp[i] = '\0';
        strcpy(open_database, temp);
        fclose(fp);

        return 0;
}

int load_database(R2RDatabase *data, char *file)
{

        printf("inside load now\n\n");

        int retval = 0;

        size_t temp_len = 3;
        char *temp = malloc(temp_len);
        if (temp == NULL)
                return 1; //insufficient memory

        size_t path_len = 5;

        size_t full_len = path_len + MAX_NAME;
        char full_path[full_len + 1];
        strcpy(full_path, "data/");
        strcat(full_path, file);

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
        data->name = malloc(data->name_len + 1);
        csv_get_field(data->name, data->name_len, buffer, 0, 0);
        csv_get_field(temp, temp_len,  buffer, 0, 1);
        data->units = strtol(temp, NULL, 10);
        data->nruns = csv_get_height(buffer) - 1;

        /* Create body space */
        int i, j; 
        data->run = malloc(data->nruns * sizeof(R2RRun*));
        for (i = 0; i < data->nruns; i++) 
                data->run[i] = malloc(sizeof(R2RRun));

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
        if (temp_temp == NULL) 
                return 1; // insufficient memory
        else 
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
                csv_get_field(temp, temp_len, buffer, i+1, 6);
                data->run[i]->type = strtol(temp, NULL, 10);
                csv_get_field(temp, temp_len, buffer, i+1, 7);
                data->run[i]->feel = strtol(temp, NULL, 10);  
                csv_get_field(temp, temp_len, buffer, i+1, 8);
                data->run[i]->time = strtol(temp, NULL, 10);

                data->run[i]->route_len = csv_get_field_length(buffer, i+1, 9);
                data->run[i]->route = malloc(data->run[i]->route_len);
                csv_get_field(data->run[i]->route, data->run[i]->route_len, buffer, i+1, 9);

                data->run[i]->notes_len = csv_get_field_length(buffer, i+1, 10);
                data->run[i]->notes = malloc(data->run[i]->notes_len);
                csv_get_field(data->run[i]->notes, data->run[i]->notes_len, buffer, i+1, 10);
 
        } 
 
        end:
        csv_destroy_buffer(buffer);
        free(temp);
        return 0;
}
         


int main() {

        char *open_database = malloc(MAX_NAME);

        load_info_file(open_database);

        printf("database filename = %s\n", open_database);

        R2RDatabase *data = malloc(sizeof(R2RDatabase));

        load_database(data, open_database);

        printf(" units = %i name = %s\n", data->units, data->name);

        free(data->name);        
        free(data);
        free(open_database);

        return 0;
}
