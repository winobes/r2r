#include "r2r.h"
#include "windows.c"


int main (int argc, char *argv[]) 
{

        int i;

        char **database_filename; // array of previously opened databases
        int open_database; 
        int data_init; // 0 if the database was initialized
        int n_databases = 0;

        R2RRun **data;
        char database_name[MAX_NAME];
        int n_runs = 0;

        GtkWidget *runlist_window;
        GtkWidget *newrun_window;

        load_info_file(&open_database, &database_filename, &n_databases);
        data_init = load_database(database_filename[open_database], 
                &database_name, &data, &n_runs);
        gtk_init(&argc, &argv);
        
        create_newrun_window(&newrun_window, data, &n_runs); 
        create_runlist_window(&runlist_window, 
                data, n_runs, (char *) database_name, &newrun_window);

        gtk_widget_show_all(runlist_window);

        gtk_main();


        if (data_init == 0) {
                for (i = 0; i < n_runs; i++) {
                        free(data[i]->route);
                        free(data[i]->notes);
                        free(data[i]);
                }
                free(data);
        } 


        return 0;

}

/*
        printf("\n%s\n", database_name);
        printf("%i\n", n_runs);
        printf("UNITS = %i\n", UNITS);

        for (i = 0; i < n_runs; i++) {
        printf("i = %i\n", i);
        printf("Year = %i\n", data[i]->year);
        printf("Month = %i\n", data[i]->month);
        printf("Day = %i\n", data[i]->day);
        printf("run_n = %i\n", data[i]->run_n);
        printf("Distance = %f\n", data[i]->distance);
        printf("Duration = %f\n", data[i]->duration);
        printf("workout type = %i\n", data[i]->workout_type);
        printf("feel = %i\n", data[i]->feel);
        printf("time_of_day = %i\n", data[i]->time_of_day);
        printf("Route: %s\n", data[i]->route);
        printf("Notes: %s\n", data[i]->notes);
        printf("\n");
        }
*/
