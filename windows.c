#include "r2r.h"
#include "windows.h"
#include "callbacks.h"

static GtkWidget* create_menubar()
{
        GtkWidget *menubar;
        GtkWidget *file;
        GtkWidget *filemenu;
        GtkWidget *quit;

        /* Create the menu */
        menubar = gtk_menu_bar_new();
        filemenu = gtk_menu_new();
        file = gtk_menu_item_new_with_label("File");
        quit = gtk_menu_item_new_with_label("Quit");
        gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
        gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
        gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);

        /* Connect callbacks */
        g_signal_connect(G_OBJECT(quit), "activate",
                G_CALLBACK(gtk_main_quit), NULL);

        return menubar;
}
 

static GtkWidget* create_runlist_window(GtkWidget *newrun_window, 
                                        R2RDatabase *database)
{
        int i; 

        /* Gtk Widgets */
        GtkWidget *window;
        GtkWidget *grid;
        GtkWidget *menubar;
        GtkWidget *tree;
        GtkWidget *newrun;
        GtkWidget *refresh;
        GtkCellRenderer *renderer;
        GtkListStore *store;
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

        /* Global settings */
        GtkSettings *default_settings = gtk_settings_get_default();
        g_object_set(default_settings, "gtk-button-images", TRUE, NULL);  
      
        /* Create the Window and top-level box */
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_title(GTK_WINDOW(window), database->name);
        grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), grid);

        /* Create menubar */
        menubar = create_menubar();

        /* Create Buttons */
        newrun = gtk_button_new_with_label("New Run"); 
        refresh = gtk_button_new_with_label("Refresh List");

        /* Create the tree view */
        tree = gtk_tree_view_new();
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
        gtk_grid_attach(GTK_GRID(grid), tree, 0 , 1, 4, 1);
        gtk_grid_attach(GTK_GRID(grid), newrun, 3, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), refresh, 1, 2, 1, 1); 
 
        /* structed up data for callbacks */ 
        static RUNLIST runlist;
        runlist.database = database;
        runlist.store = store;

        /* Connect all our callbacks */
        g_signal_connect_swapped(G_OBJECT(window), "destroy",
                G_CALLBACK(gtk_main_quit), NULL);
        g_signal_connect(G_OBJECT(newrun), "clicked",
                G_CALLBACK(open_window), (gpointer) newrun_window);
        g_signal_connect(G_OBJECT(refresh), "clicked",
                G_CALLBACK(refresh_list), (gpointer) &runlist);
        
        /* Initial refresh to populate the list */
        refresh_list(NULL, &runlist);

        return window;
}

static GtkWidget* create_month_chooser()
{

        GtkWidget *month_chooser;
        month_chooser = gtk_combo_box_text_new();
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser), "11", "Dec");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser), "10", "Nov");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "9", "Oct");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "8", "Sep");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "7", "Aug");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "6", "Jul");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "5", "Jun");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "4", "May");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "3", "Apr");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "2", "Mar");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "1", "Feb");
        gtk_combo_box_text_prepend(GTK_COMBO_BOX_TEXT(month_chooser),  "0", "Jan");

        return month_chooser;
}

static GtkWidget* create_newrun_window()
{
        GtkWidget *window;
        GtkWidget *menubar;
        GtkWidget *grid;
        GtkWidget *calendar;
        GtkWidget *calendar_window;
        GtkWidget *calendar_grid;
        GtkWidget *calendar_button;
        static GtkWidget *day_chooser;
        GtkAdjustment *day_adjustment;
        GtkWidget *month_chooser;
        GtkWidget *year_chooser;
        GtkAdjustment *year_adjustment;
        
        R2RRun *newrun = malloc(sizeof(R2RRun));
        static NEW_DATA new_data;

        /* Create the window and top grid */ 
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(window), 
                GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window),200, 600);
        gtk_window_set_title(GTK_WINDOW(window), "New Run");
        grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), grid);

        /* Create the menubar */
        menubar = create_menubar();
       
        /* Create input widgets */
        calendar = gtk_calendar_new();
        calendar_button = gtk_button_new_with_label("calendar");

        /* Create & pack the calendar window */
        calendar_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(calendar_window), 
                GTK_WIN_POS_CENTER);
        gtk_window_set_decorated(GTK_WINDOW(calendar_window),
                false);
        gtk_window_set_transient_for(GTK_WINDOW(calendar_window),
                GTK_WINDOW(window));
        gtk_window_set_position(GTK_WINDOW(calendar_window), 
                GTK_WIN_POS_MOUSE);
        calendar_grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(calendar_window), calendar_grid);
        gtk_grid_attach(GTK_GRID(calendar_grid), calendar, 0, 0, 1, 1);

        /* Create the date choosers */
        day_adjustment = gtk_adjustment_new(0, 1, 31, 1, 0, 0);
        day_chooser = gtk_spin_button_new(day_adjustment, 1, 0);
        gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(day_chooser), true);
        gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(day_chooser), true);

        year_adjustment = gtk_adjustment_new(0, 1900, 9999, 1, 0, 0);
        year_chooser = gtk_spin_button_new(year_adjustment, 1, 0);

        month_chooser = create_month_chooser();

        /* Populate the data struct */ 
        new_data.newrun = newrun;
        new_data.calendar = GTK_CALENDAR(calendar);
        new_data.day_chooser = GTK_SPIN_BUTTON(day_chooser);
        new_data.month_chooser = GTK_COMBO_BOX_TEXT(month_chooser);
        new_data.year_chooser = GTK_SPIN_BUTTON(year_chooser);

        /* Pack everything into the grid */
        gtk_grid_attach (GTK_GRID (grid), menubar, 0, 0, 1, 1); 
        gtk_grid_attach(GTK_GRID(grid), day_chooser, 0, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), month_chooser, 1, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), year_chooser, 2, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), calendar_button, 3, 1, 1, 1);


        /* Initialize the widgets and new_data */        
        set_date_calendar(GTK_CALENDAR(calendar), (gpointer) &new_data);

        /* Connect callbacks */
        g_signal_connect(window, "delete-event",
                G_CALLBACK(gtk_widget_hide_on_delete), NULL); 
        g_signal_connect(G_OBJECT(calendar_button), "clicked",
                G_CALLBACK(open_window), (gpointer) calendar_window);
        g_signal_connect(calendar, "day-selected-double-click",
                G_CALLBACK(hide_window), (gpointer) calendar_window); 
        g_signal_connect(calendar, "day-selected-double-click",
                G_CALLBACK(set_date_calendar), (gpointer) &new_data);
        g_signal_connect(month_chooser, "changed",
                G_CALLBACK(set_date_month), (gpointer) &new_data);
        g_signal_connect(year_chooser, "changed",
                G_CALLBACK(set_date_year), (gpointer) &new_data);
        g_signal_connect(day_chooser, "changed",
                G_CALLBACK(set_date_day), (gpointer) &new_data);

        return window;
}

/* Top-Level and shared widgets */
GtkWidget* create_windows(R2RDatabase *database)
{

        GtkWidget *runlist_window;
        GtkWidget *newrun_window;

        newrun_window = create_newrun_window();
        runlist_window = create_runlist_window(newrun_window, database);

        /* Return the window we want open at start */
        return runlist_window;
       
} 
