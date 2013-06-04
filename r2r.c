#include "r2r.h"
#include "file_handles.h"
#include "windows.h"
int main (int argc, char *argv[]) 
{

        R2RDatabase *database;
        R2RRun *newrun;

        GtkWidget *main_window;

        gtk_init(&argc, &argv);

        database = load_data();
        main_window = create_windows(database, newrun);

        gtk_widget_show_all(main_window);
        gtk_main();

        save_database("data/save.r2r", database);
        free_database(database);
        return 0;

}
