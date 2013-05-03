#include "file_handles.h"
#include <string.h>
#include <stdio.h>
#include <csv.h>

int load_info_file(char open_database[MAX_NAME])
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

        return 0;
}

int load_database(R2RDatabase *data, char file[MAX_NAME])
{

        int retval = 0;

        int temp_len = 3;
        char temp[temp_len];

        CSV_BUFFER *buffer = csv_create_buffer();
        if (csv_load(buffer, file) != 0) {
                retval = 1;
                goto end;
        
        data->name_len = csv_get_field_length(buffer, 0, 0);
        data->name = malloc(name_len);

        if (csv_get_field(data->name, data->name_len, buffer, 0, 0) != 0) {
                retval = 2;
                goto end;
        }
                
        
        csv_get_field(temp, temp_len,  buffer, 0, 1)
        data->units = strtol(temp, NULL, 10);

        end:
        csv_destroy_buffer(buffer);
        return retval;
         


int main() {

        char open_database[256];

        int retval = load_info_file(open_database);

        printf("%s|\n", open_database);

        return 0;
}
