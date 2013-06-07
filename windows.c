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
                        R2RDatabase *database, NEW_DATA *new_data)
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
                G_TYPE_INT,
                G_TYPE_STRING,
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
        g_signal_connect(G_OBJECT(window), "destroy",
                G_CALLBACK(destroy_new_data), (gpointer) new_data);
        g_signal_connect(G_OBJECT(newrun), "clicked",
                G_CALLBACK(init_newrun_window_for_new_run), (gpointer) new_data);
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

static GtkWidget* create_newrun_window(GtkWidget *runlist_window, 
                R2RDatabase *database, NEW_DATA *new_data)
{
        GtkWidget *window;
        GtkWidget *menubar;
        GtkWidget *grid;

        GtkWidget *date_label;
        GtkWidget *calendar;
        GtkWidget *calendar_window;
        GtkWidget *calendar_grid;
        GtkWidget *calendar_button;
        GtkWidget *day_chooser;
        GtkAdjustment *day_adjustment;
        GtkWidget *month_chooser;
        GtkWidget *year_chooser;
        GtkAdjustment *year_adjustment;

        GtkWidget *distance_label;
        GtkWidget *distance_chooser;
        GtkAdjustment *distance_adjustment;
        GtkWidget *distance_units_label;

        GtkWidget *duration_grid;
        GtkWidget *duration_label;
        GtkWidget *duration_delim_label1;
        GtkWidget *duration_delim_label2;
        GtkWidget *hours_entry; 
        GtkWidget *minutes_entry;
        GtkWidget *seconds_entry;
        GtkEntryBuffer *hours_buff;
        GtkEntryBuffer *minutes_buff;
        GtkEntryBuffer *seconds_buff;

        GtkWidget *workout_type_label;
        GtkWidget *workout_type_entry;

        GtkWidget *feel_label;
        GtkWidget *feel_grid;
        GtkWidget *feel1;
        GtkWidget *feel2;
        GtkWidget *feel3;
        GtkWidget *feel4;
        GtkWidget *feel5;
 
        GtkWidget *time_label;
        GtkWidget *time_grid;
        GtkWidget *time1;
        GtkWidget *time2;
        GtkWidget *time3;
        GtkWidget *time4;

        GtkWidget *route_label;
        GtkWidget *route_entry;
        
        GtkWidget *notes_button;
        GtkWidget *notes_window;
        GtkWidget *notes_grid;
        GtkWidget *notes_entry;
        GtkTextBuffer *notes_buff;
        
        GtkWidget *save_button;

        GtkWidget *separator[7];


        /* Create the window and top grid */ 
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(window), 
                GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(window),200, 200);
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
        date_label = gtk_label_new("Date");

        day_adjustment = gtk_adjustment_new(0, 1, 31, 1, 0, 0);
        day_chooser = gtk_spin_button_new(day_adjustment, 1, 0);
        gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(day_chooser), true);
        gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(day_chooser), true);

        year_adjustment = gtk_adjustment_new(0, 1900, 9999, 1, 0, 0);
        year_chooser = gtk_spin_button_new(year_adjustment, 1, 0);

        month_chooser = create_month_chooser();

        /* Create the distance widgets */
        distance_label = gtk_label_new("Distance");
        if (database->units == KILOMETERS) {
                distance_units_label = gtk_label_new(" miles");
        } else if (database->units == MILES) {
                distance_units_label = gtk_label_new(" kilometers");
        }

        distance_adjustment = gtk_adjustment_new(0, 0, 999, .1, 1, 1);
        distance_chooser = gtk_spin_button_new(distance_adjustment, .1, 1);

        /* Create the duration widgets */
        duration_grid = gtk_grid_new();
        duration_label = gtk_label_new("Time");
        duration_delim_label1 = gtk_label_new(":");
        duration_delim_label2 = gtk_label_new(":");
        seconds_buff = gtk_entry_buffer_new("00", 2);
        minutes_buff = gtk_entry_buffer_new("00", 2);
        hours_buff = gtk_entry_buffer_new("00", 2);
        seconds_entry = gtk_entry_new_with_buffer(seconds_buff);
        minutes_entry = gtk_entry_new_with_buffer(minutes_buff);
        hours_entry = gtk_entry_new_with_buffer(hours_buff);
        gtk_entry_set_max_length(GTK_ENTRY(seconds_entry), 2);
        gtk_entry_set_max_length(GTK_ENTRY(minutes_entry), 2);
        gtk_entry_set_max_length(GTK_ENTRY(hours_entry), 2);
        gtk_entry_set_width_chars(GTK_ENTRY(seconds_entry), 2);
        gtk_entry_set_width_chars(GTK_ENTRY(minutes_entry), 2);
        gtk_entry_set_width_chars(GTK_ENTRY(hours_entry), 2);
        gtk_entry_set_icon_tooltip_text(GTK_ENTRY(seconds_entry), 
                GTK_ENTRY_ICON_PRIMARY, "seconds");
        gtk_entry_set_icon_tooltip_text(GTK_ENTRY(minutes_entry), 
                GTK_ENTRY_ICON_PRIMARY, "minutes");
        gtk_entry_set_icon_tooltip_text(GTK_ENTRY(hours_entry), 
                GTK_ENTRY_ICON_PRIMARY, "hours");

        /* Create the workout type widgets */
        workout_type_label = gtk_label_new("Workout Type");
        gtk_label_set_justify(GTK_LABEL(workout_type_label), GTK_JUSTIFY_LEFT);
        workout_type_entry = gtk_combo_box_text_new_with_entry();

        /* Create the "How did it feel?" widgets */
        feel_label = gtk_label_new("How did it feel?");
        feel_grid = gtk_grid_new();
        feel1 = gtk_radio_button_new_with_label(NULL, "1");
        feel2 = gtk_radio_button_new_with_label(
                gtk_radio_button_get_group(GTK_RADIO_BUTTON(feel1)), "2");
        feel3 = gtk_radio_button_new_with_label(
                gtk_radio_button_get_group(GTK_RADIO_BUTTON(feel2)), "3");
        feel4 = gtk_radio_button_new_with_label(
                gtk_radio_button_get_group(GTK_RADIO_BUTTON(feel3)), "4");
        feel5 = gtk_radio_button_new_with_label(
                gtk_radio_button_get_group(GTK_RADIO_BUTTON(feel4)), "5");

        /* Create the Time of Day widgets */
        time_label = gtk_label_new("What time was it?");
        time_grid = gtk_grid_new();
        time1 = gtk_radio_button_new_with_label(NULL, "Morning");
        time2 = gtk_radio_button_new_with_label(
                gtk_radio_button_get_group(GTK_RADIO_BUTTON(time1)), "Mid-Day");
        time3 = gtk_radio_button_new_with_label(
                gtk_radio_button_get_group(GTK_RADIO_BUTTON(time2)), "Afternoon");
        time4 = gtk_radio_button_new_with_label(
                gtk_radio_button_get_group(GTK_RADIO_BUTTON(time3)), "Evening");


        /* Create the route  widgets */
        route_label = gtk_label_new("Route");
        gtk_label_set_justify(GTK_LABEL(route_label), GTK_JUSTIFY_LEFT);
        route_entry = gtk_combo_box_text_new_with_entry();

        /* Create the notes window */
        notes_button = gtk_button_new_with_label("Notes");
        notes_window = gtk_scrolled_window_new(NULL, NULL);
        gtk_window_set_position(GTK_WINDOW(notes_window), GTK_WIN_POS_CENTER);
        gtk_window_set_default_size(GTK_WINDOW(notes_window), 400, 100);
        notes_grid = gtk_grid_new();
        notes_buff = gtk_text_buffer_new(NULL);
        notes_entry = gtk_text_view_new_with_buffer(notes_buff);
        gtk_text_view_set_right_margin(GTK_TEXT_VIEW(notes_entry), 400);
        gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(notes_entry), GTK_WRAP_WORD_CHAR);
        gtk_container_add(GTK_CONTAINER(notes_window), notes_grid);
        gtk_grid_attach(GTK_GRID(notes_grid), notes_entry, 0, 0, 1, 1);


        /* Create the separatons */
        int i;
        for (i = 0; i < 6; i++)
                separator[i] = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
        separator[6] = gtk_separator_new(GTK_ORIENTATION_VERTICAL);

        /* Create the save button */
        save_button = gtk_button_new_with_label("Save");

        /* Populate the data struct */ 
        new_data->database = database; 
        new_data->calendar = GTK_CALENDAR(calendar);
        new_data->distance_chooser = GTK_SPIN_BUTTON(distance_chooser);
        new_data->day_chooser = GTK_SPIN_BUTTON(day_chooser);
        new_data->month_chooser = GTK_COMBO_BOX_TEXT(month_chooser);
        new_data->year_chooser = GTK_SPIN_BUTTON(year_chooser);
        new_data->seconds_buff = seconds_buff;
        new_data->minutes_buff = minutes_buff;
        new_data->hours_buff = hours_buff;
        new_data->feel3 = GTK_RADIO_BUTTON(feel3);
        new_data->time1 = GTK_RADIO_BUTTON(time1);
        new_data->workout_type_entry = GTK_COMBO_BOX_TEXT(workout_type_entry);
        new_data->route_entry = GTK_COMBO_BOX_TEXT(route_entry);
        new_data->newrun_window = window;
        new_data->runlist_window = runlist_window;


        /* Pack everything into the grid */
        gtk_grid_attach (GTK_GRID (grid), menubar, 0, 0, 1, 1); 

        gtk_grid_attach(GTK_GRID(grid), date_label, 0, 1, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), day_chooser, 0, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), month_chooser, 1, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), year_chooser, 2, 2, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), calendar_button, 3, 2, 1, 1);

        gtk_grid_attach(GTK_GRID(grid), distance_label, 0, 5, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), distance_chooser, 0, 6, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), distance_units_label, 1, 6, 1, 1);

        gtk_grid_attach(GTK_GRID(duration_grid), hours_entry, 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(duration_grid), duration_delim_label1, 1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(duration_grid), minutes_entry, 2, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(duration_grid), duration_delim_label2, 3, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(duration_grid), seconds_entry, 4, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), duration_grid, 3, 6, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), duration_label, 3, 5, 1, 1);

        gtk_grid_attach(GTK_GRID(grid), workout_type_label, 0, 8, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), workout_type_entry, 0, 9, 3, 1);

        gtk_grid_attach(GTK_GRID(grid), feel_label, 0, 11, 1, 1);
        gtk_grid_attach(GTK_GRID(feel_grid), feel1, 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(feel_grid), feel2, 1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(feel_grid), feel3, 2, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(feel_grid), feel4, 3, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(feel_grid), feel5, 4, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), feel_grid, 0, 12, 4, 1);
 
        gtk_grid_attach(GTK_GRID(grid), time_label, 0, 14, 1, 1);
        gtk_grid_attach(GTK_GRID(time_grid), time1, 0, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(time_grid), time2, 1, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(time_grid), time3, 2, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(time_grid), time4, 3, 0, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), time_grid, 0, 15, 4, 1);
 
        gtk_grid_attach(GTK_GRID(grid), route_label, 0, 17, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), route_entry, 0, 18, 4, 1);

        gtk_grid_attach(GTK_GRID(grid), separator[0], 0, 4, 4, 1);
        gtk_grid_attach(GTK_GRID(grid), separator[1], 0, 7, 4, 1);
        gtk_grid_attach(GTK_GRID(grid), separator[2], 0, 10, 4, 1);
        gtk_grid_attach(GTK_GRID(grid), separator[3], 0, 13, 4, 1);
        gtk_grid_attach(GTK_GRID(grid), separator[4], 0, 16, 4, 1);
        gtk_grid_attach(GTK_GRID(grid), separator[5], 0, 19, 4, 1);
        gtk_grid_attach(GTK_GRID(grid), separator[6], 2, 6, 1, 1);

        gtk_grid_attach(GTK_GRID(grid), save_button, 3, 21, 1, 1);
        gtk_grid_attach(GTK_GRID(grid), notes_button, 0, 21, 1, 1);

        /* Initialize the widgets and new_data */        

        /* Connect callbacks */
        g_signal_connect(window, "delete-event",
                G_CALLBACK(gtk_widget_hide_on_delete), NULL); 
        g_signal_connect(G_OBJECT(calendar_button), "clicked",
                G_CALLBACK(open_window), (gpointer) calendar_window);
        g_signal_connect(calendar, "day-selected-double-click",
                G_CALLBACK(hide_window), (gpointer) calendar_window); 
        g_signal_connect(calendar, "day-selected-double-click",
                G_CALLBACK(set_date_calendar), (gpointer) new_data);
        g_signal_connect(month_chooser, "changed",
                G_CALLBACK(set_date_month), (gpointer) new_data);
        g_signal_connect(year_chooser, "changed",
                G_CALLBACK(set_date_year), (gpointer) new_data);
        g_signal_connect(day_chooser, "changed",
                G_CALLBACK(set_date_day), (gpointer) new_data);
        g_signal_connect(save_button, "clicked",
                G_CALLBACK(set_time), (gpointer) new_data);
        g_signal_connect(hours_entry, "activate",
                G_CALLBACK(set_time), (gpointer) new_data);
        g_signal_connect(minutes_entry, "activate",
                G_CALLBACK(set_time), (gpointer) new_data);
        g_signal_connect(seconds_entry, "activate",
                G_CALLBACK(set_time), (gpointer) new_data);
        g_signal_connect(save_button, "clicked",
                G_CALLBACK(set_type), (gpointer) new_data);
        g_signal_connect(feel1, "toggled",
                G_CALLBACK(set_feel_1), (gpointer) new_data);
        g_signal_connect(feel2, "toggled",
                G_CALLBACK(set_feel_2), (gpointer) new_data);
        g_signal_connect(feel3, "toggled",
                G_CALLBACK(set_feel_3), (gpointer) new_data);
        g_signal_connect(feel4, "toggled",
                G_CALLBACK(set_feel_4), (gpointer) new_data);
        g_signal_connect(feel5, "toggled",
                G_CALLBACK(set_feel_5), (gpointer) new_data);
        g_signal_connect(time1, "toggled",
                G_CALLBACK(set_time_1), (gpointer) new_data);
        g_signal_connect(time2, "toggled",
                G_CALLBACK(set_time_2), (gpointer) new_data);
        g_signal_connect(time3, "toggled",
                G_CALLBACK(set_time_3), (gpointer) new_data);
        g_signal_connect(time4, "toggled",
                G_CALLBACK(set_time_4), (gpointer) new_data);
        g_signal_connect(save_button, "clicked",
                G_CALLBACK(set_route), (gpointer) new_data);
        g_signal_connect(save_button, "clicked",
                G_CALLBACK(save), (gpointer) new_data);
        g_signal_connect(G_OBJECT(notes_button), "clicked",
                G_CALLBACK(open_window), (gpointer) notes_window);

        return window;
}

/* Top-Level and shared widgets */
GtkWidget* create_windows(R2RDatabase *database, R2RRun *newrun)
{

        GtkWidget *runlist_window;
        GtkWidget *newrun_window;

        NEW_DATA *new_data = g_malloc(sizeof(NEW_DATA));
        new_data->newrun = newrun;


        newrun_window = create_newrun_window(runlist_window, 
                        database, new_data);
        runlist_window = create_runlist_window(newrun_window, database, new_data);

        /* Return the window we want open at start */
        return runlist_window;
       
} 
