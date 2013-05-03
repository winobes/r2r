#include "callbacks.c"

static void create_runlist_window(GtkWidget **window,
        R2RRun **data, int n_runs, char *database_name, 
        GtkWidget **newrun_window)
{
        int i; 

        GtkWidget *grid;
        GtkWidget *menubar;
        GtkWidget *file;
        GtkWidget *filemenu;
        GtkWidget *quit;
        GtkWidget *tree;
        GtkWidget *newrun;
        GtkCellRenderer *renderer;
        GtkTreeViewColumn *year_column;
        GtkTreeViewColumn *month_column;
        GtkTreeViewColumn *day_column;
        GtkTreeViewColumn *run_n_column;
        GtkTreeViewColumn *distance_column;
        GtkTreeViewColumn *duration_column;
        GtkTreeViewColumn *workout_type_column;
        GtkTreeViewColumn *feel_column;
        GtkTreeViewColumn *time_of_day_column;
        GtkTreeViewColumn *route_column;
        GtkTreeIter iter;
        GtkListStore *store;

        /* Create the Window and top-level box */
        *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(*window), GTK_WIN_POS_CENTER);
        gtk_window_set_title(GTK_WINDOW(*window), database_name);
        grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(*window), grid);

        /* Create Buttons */
        newrun = gtk_button_new_with_label("New Run"); 
        
        /* Create the menu */
        menubar = gtk_menu_bar_new();
        filemenu = gtk_menu_new();
        file = gtk_menu_item_new_with_label("File");
        quit = gtk_menu_item_new_with_label("Quit");
        tree = gtk_tree_view_new();
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
        gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
        gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

        /* Create the tree view */
        store = gtk_list_store_new (N_COLUMNS,
                G_TYPE_INT,
                G_TYPE_INT,
                G_TYPE_INT,
                G_TYPE_INT,
                G_TYPE_FLOAT,
                G_TYPE_FLOAT,
                G_TYPE_INT,
                G_TYPE_INT,
                G_TYPE_INT,
                G_TYPE_STRING);
        for (i = 0; i < n_runs; i++) {
                gtk_list_store_append (store, &iter);
                gtk_list_store_set (store, &iter,
                        YEAR, data[i]->year,
                        MONTH, data[i]->month,
                        DAY, data[i]->day,
                        RUN_N, data[i]->run_n,
                        DISTANCE, data[i]->distance,
                        DURATION, data[i]->duration,
                        WORKOUT_TYPE, data[i]->workout_type,
                        FEEL, data[i]->feel,
                        TIME_OF_DAY, data[i]->time_of_day,
                        ROUTE, data[i]->route,
                        -1);
        }

        tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (store));
        renderer = gtk_cell_renderer_text_new();
        year_column = gtk_tree_view_column_new_with_attributes ("Year", renderer, "text", YEAR, NULL);
        month_column = gtk_tree_view_column_new_with_attributes ("Month", renderer, "text", MONTH, NULL);
        day_column = gtk_tree_view_column_new_with_attributes ("Day", renderer, "text", DAY, NULL);
        run_n_column = gtk_tree_view_column_new_with_attributes ("#", renderer, "text", RUN_N, NULL);
        distance_column = gtk_tree_view_column_new_with_attributes ("Miles", renderer, "text", DISTANCE, NULL);
        duration_column = gtk_tree_view_column_new_with_attributes ("Time", renderer, "text", DURATION, NULL);
        workout_type_column = gtk_tree_view_column_new_with_attributes ("Workout Type", renderer, "text", WORKOUT_TYPE, NULL);
        feel_column = gtk_tree_view_column_new_with_attributes ("Feel", renderer, "text", FEEL, NULL);
        time_of_day_column = gtk_tree_view_column_new_with_attributes ("Time of Day", renderer, "text", TIME_OF_DAY, NULL);
        route_column = gtk_tree_view_column_new_with_attributes ("Route", renderer, "text", ROUTE, NULL);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), year_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), month_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), day_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), run_n_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), distance_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), duration_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), workout_type_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), feel_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), time_of_day_column);
        gtk_tree_view_append_column (GTK_TREE_VIEW(tree), route_column);
       
        /* Pack everything into the box */
        gtk_grid_attach (GTK_GRID (grid), menubar, 0, 0, 1, 1); 
        //gtk_grid_attach_next_to (GTK_GRID(grid), tree, menubar, GTK_POS_BOTTOM, 1, 4);
        gtk_grid_attach(GTK_GRID(grid), tree, 0 , 1, 4, 1);
        gtk_grid_attach (GTK_GRID(grid), newrun, 3, 2, 1, 1);

        /* Connect all our callbacks */
        g_signal_connect_swapped(G_OBJECT(*window), "destroy",
                G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(G_OBJECT(quit), "activate",
                G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(G_OBJECT(newrun), "clicked",
                G_CALLBACK(open_window), (gpointer) *newrun_window);

}

static void create_newrun_window(GtkWidget **window, R2RRun **data, int *n_runs)
{
        GtkWidget *grid;
        GtkWidget *year_entry;
        GtkWidget *month_entry;
        GtkWidget *day_entry;
        GtkEntryBuffer **date_buf;
        GtkWidget *calendar;
        GtkWidget *calendar_window;
        GtkWidget *calendar_grid;
        GtkWidget *calendar_button;

        guint date[3];
        
        /* Create the window and top grid */ 
        *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(*window), 
                GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(*window),200, 600);
        gtk_window_set_title(GTK_WINDOW(*window), "New Run");
        grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(*window), grid);

        /* Create input widgets */
        date_buf = gtk_entry_buffer_new(NULL, -1);
        date_entry = gtk_entry_new_with_buffer(date_buf);
        calendar = gtk_calendar_new();
        calendar_button = gtk_button_new_with_label("Select Date");

        /* Create & pack the calendar window */
        calendar_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(calendar_window), 
                GTK_WIN_POS_CENTER);
        gtk_window_set_decorated(GTK_WINDOW(calendar_window),
                false);
        gtk_window_set_transient_for(GTK_WINDOW(calendar_window),
                GTK_WINDOW(*window));
        gtk_window_set_position(GTK_WINDOW(calendar_window), 
                GTK_WIN_POS_MOUSE);
        calendar_grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(calendar_window), calendar_grid);
        gtk_grid_attach(GTK_GRID(calendar_grid), calendar, 0, 0, 1, 1);
       
        /* Pack everything into the grid */
        gtk_grid_attach(GTK_GRID(grid), calendar_button, 1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), date_entry, 0, 0, 1, 1);
        

       
        g_signal_connect (*window, "delete-event",
                G_CALLBACK(gtk_widget_hide_on_delete), NULL); 
        g_signal_connect(G_OBJECT(calendar_button), "clicked",
                G_CALLBACK(open_window), (gpointer) calendar_window);
        g_signal_connect(calendar, "day-selected-double-click",
                G_CALLBACK(hide_window), (gpointer) calendar_window); 
        g_signal_connect(calendar, "day-selected-double-click",
                G_CALLBACK(set_date_ints), (gpointer) date);
}
