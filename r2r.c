#include "r2r.h"
#include "file_handles.h"
#include "windows.h"
int main (int argc, char *argv[]) 
{

        path_len = 5;
        path = g_malloc(path_len + 1);
        g_snprintf(path, path_len + 1, "%s", "data/");

        R2RDatabase *database;
        R2RRun *newrun = malloc(sizeof(R2RRun));

        GtkWidget *main_window;

        gtk_init(&argc, &argv);

        database = load_data();
        main_window = create_windows(database, newrun);

        gtk_widget_show_all(main_window);
        gtk_main();

        save_database(database);
        free_database(database);
        g_free(path);

        return 0;

}
