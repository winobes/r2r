#include "callbacks.h"

void open_window(GtkWidget *widget, gpointer data)
{
        GtkWidget *window = data;
        gtk_widget_show_all(window);
        gtk_window_present(GTK_WINDOW(window));
}

gboolean hide_window(GtkWidget *widget, gpointer data)
{
        GtkWidget *window = data;
        gtk_widget_hide(GTK_WIDGET(window));
 
        return true;
}

static void populate_MRUs(NEW_DATA *new_data)
{

        /* Create the list of previously used workout types
         * and connect them to the combo box
         */

        R2RDatabase *database = new_data->database;
        GtkComboBoxText *workout_type_entry = new_data->workout_type_entry;
        GtkComboBoxText *route_entry = new_data->route_entry;

        int i, j;
        gboolean already_in;
        gchar **types = NULL;
        guint ntypes = 0;
        for (i = 0; i < database->nruns; i++) {
                already_in = false;
                for (j = 0; j < ntypes; j++) {
                        if (g_strcmp0(database->run[i]->type, types[j]) == 0 ||
                            database->run[i]->type[0] == ' ' ||
                            database->run[i]->type[0] == '\0')
                                already_in = true;
                }
                if (!already_in) {
                        ntypes++;
                        types = g_realloc(types, ntypes * sizeof(gchar *));
                        types[ntypes - 1]  = g_malloc(database->run[i]->type_len + 1);
                        g_strlcpy(types[ntypes - 1], database->run[i]->type,
                                database->run[i]->type_len + 1);
                }
        }

        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(workout_type_entry));
        for (i = 0; i < ntypes; i++) {
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(workout_type_entry),
                        NULL, types[i]);
                g_free(types[i]);
        }
        g_free(types);

        /* Create the list of previously used routes
         * and connect them to the combo box
         */
        gchar **routes = NULL;
        guint nroutes = 0;
        for (i = 0; i < database->nruns; i++) {
                already_in = false;
                for (j = 0; j < nroutes; j++) {
                        if (g_strcmp0(database->run[i]->route, routes[j]) == 0 ||
                            database->run[i]->route[0] == ' ' ||
                            database->run[i]->route[0] == '\0') 
                                already_in = true;
                }
                if (!already_in) {
                        nroutes++;
                        routes = g_realloc(routes, nroutes * sizeof(gchar *));
                        routes[nroutes - 1]  = g_malloc(database->run[i]->route_len + 1);
                        g_strlcpy(routes[nroutes - 1], database->run[i]->route,
                                database->run[i]->route_len + 1);
                }
        }

        gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(route_entry));
        for (i = 0; i < nroutes; i++) {
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(route_entry),
                        NULL, routes[i]);
                g_free(routes[i]);
        }
        g_free(routes);
}


void init_newrun_window_for_new_run(GtkWidget *widget, gpointer data)
{
        NEW_DATA *new_data = (NEW_DATA *) data;
        new_data->edit_index = -1;
        populate_MRUs(new_data);

        // keep deault date (today)
        set_date_calendar(GTK_CALENDAR(new_data->calendar), (gpointer) new_data);

        // setting the distance to 0
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(new_data->distance_chooser), 0);

        // setting the time fields to 0
        gtk_entry_buffer_set_text(new_data->seconds_buff, "00", 2);
        gtk_entry_buffer_set_text(new_data->minutes_buff, "00", 2);
        gtk_entry_buffer_set_text(new_data->hours_buff, "00", 2); 

        printf("setting combobox to nothing!!\n");
        GtkWidget *entry = gtk_bin_get_child(GTK_BIN(new_data->route_entry));
        gtk_entry_set_text(GTK_ENTRY(entry), "");
        entry = gtk_bin_get_child(GTK_BIN(new_data->workout_type_entry));
        gtk_entry_set_text(GTK_ENTRY(entry), "");

        // setting the feel3 button active 
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->feel3), true);

        // setting the morning button active
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->time1), true);
        
        //clearing the notes buffer
        gtk_text_buffer_set_text(new_data->notes_buff, "\0", -1);

        open_window(GTK_WIDGET(widget), (gpointer) new_data->newrun_window);

}

static void set_time_strings(gint total,
                gchar *hours_str, gchar *minutes_str, gchar *seconds_str)
{
        //printf("%i becomes", total);

        int hours = total/(60 * 60);    
        total = total % (60 * 60);

        int minutes = total/60;
        total = total % 60;

        int seconds = total;
        
        g_snprintf(hours_str, 3, "%i", hours);
        g_snprintf(minutes_str, 3, "%i", minutes);
        g_snprintf(seconds_str, 3, "%i", seconds);

        //printf("%i hours, %i minutes, %i seconds.\n", hours, minutes, seconds);
}

        

void init_newrun_window_for_edit(GtkWidget *widget, gpointer data)
{

        NEW_DATA *new_data = (NEW_DATA *) data;
        populate_MRUs(new_data);

        GtkTreeSelection *selection;
        GtkTreeModel *model;
        GtkTreeIter iter;
        GValue value = G_VALUE_INIT;

        gchar buff[30];
        gchar seconds_buff[3];
        gchar minutes_buff[3];
        gchar hours_buff[3];

        selection = gtk_tree_view_get_selection(new_data->runlist);
        gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
        gtk_tree_selection_get_selected(selection, &model, &iter);
        gtk_tree_model_get_value(model, &iter, INDEX, &value);
        new_data->edit_index = g_value_get_int(&value);

        // set the date
        set_time_strings(new_data->database->run[new_data->edit_index]->duration,
                        hours_buff, minutes_buff, seconds_buff);

        gtk_spin_button_set_value(GTK_SPIN_BUTTON(new_data->day_chooser), 
                        new_data->database->run[new_data->edit_index]->day);
        g_snprintf(buff, 30, 
                "%i", new_data->database->run[new_data->edit_index]->month);
        gtk_combo_box_set_active_id(GTK_COMBO_BOX(new_data->month_chooser), buff);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(new_data->year_chooser), 
                        new_data->database->run[new_data->edit_index]->year);

        // setting the distance  
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(new_data->distance_chooser), 
                        new_data->database->run[new_data->edit_index]->distance);

        // setting the time fields
        gtk_entry_buffer_set_text(new_data->seconds_buff, seconds_buff, 2);
        gtk_entry_buffer_set_text(new_data->minutes_buff, minutes_buff, 2);
        gtk_entry_buffer_set_text(new_data->hours_buff, hours_buff, 2); 

        // setting the entries
        GtkWidget *entry = gtk_bin_get_child(GTK_BIN(new_data->route_entry));
        gtk_entry_set_text(GTK_ENTRY(entry), 
                new_data->database->run[new_data->edit_index]->route); 

        entry = gtk_bin_get_child(GTK_BIN(new_data->workout_type_entry));
        gtk_entry_set_text(GTK_ENTRY(entry), 
                new_data->database->run[new_data->edit_index]->type); 

        // setting the feel button active 
        switch (new_data->database->run[new_data->edit_index]->feel) {
        case 1:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->feel1), true);
                break;
        case 2:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->feel2), true);
                break;
        case 3:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->feel3), true);
                break;
        case 4:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->feel4), true);
                break;
        case 5:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->feel5), true);
                break;
        }

        // setting the time of day button active
        switch (new_data->database->run[new_data->edit_index]->time) {
        case 1:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->time1), true);
                break;
        case 2:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->time2), true);
                break;
        case 3:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->time3), true);
                break;
        case 4:
                gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(new_data->time4), true);
                break;
        }

        //clearing the notes buffer
        gtk_text_buffer_set_text(new_data->notes_buff,
                new_data->database->run[new_data->edit_index]->notes, 
                new_data->database->run[new_data->edit_index]->notes_len);


        open_window(GTK_WIDGET(widget), (gpointer) new_data->newrun_window);
}

static update_dates(gpointer data)
{
        guint year = ((NEW_DATA *) data)->newrun->year;
        guint month = ((NEW_DATA *) data)->newrun->month;
        gchar month_str[3]; 
        g_snprintf(month_str, 3, "%i", month);
        guint day = ((NEW_DATA *) data)->newrun->day;

        GtkCalendar *calendar = ((NEW_DATA*) data)->calendar;
        GtkSpinButton *day_chooser = ((NEW_DATA*) data)->day_chooser;
        GtkComboBoxText *month_chooser = ((NEW_DATA*) data)->month_chooser;
        GtkSpinButton *year_chooser = ((NEW_DATA*) data)->year_chooser;


        gtk_spin_button_set_value(GTK_SPIN_BUTTON(day_chooser), day);
        gtk_spin_button_update(day_chooser);

        gtk_combo_box_set_active_id(GTK_COMBO_BOX(month_chooser), month_str);

        gtk_spin_button_set_value(GTK_SPIN_BUTTON(year_chooser), year);
        gtk_spin_button_update(year_chooser);

        gtk_calendar_select_day(GTK_CALENDAR(calendar), day);
        gtk_calendar_select_month(GTK_CALENDAR(calendar), month, year);
                
        //printf("%i/%i/%i\n", month, day, year);
}

void set_date_calendar(GtkCalendar *calendar, gpointer data)
{
        guint year;
        guint month;
        guint day;

        R2RRun *newrun = ((NEW_DATA *) data)->newrun;

        gtk_calendar_get_date(GTK_CALENDAR(calendar), 
                &year, 
                &month, 
                &day);

        newrun->year = year;
        newrun->month = month;
        newrun->day = day;

        update_dates(data);
        
}

void set_date_month(GtkComboBox *button, gpointer data)
{
        ((NEW_DATA*) data)->newrun->month = 
                strtol(gtk_combo_box_get_active_id(GTK_COMBO_BOX(button)), NULL, 10);
        update_dates(data);
}

void set_date_year(GtkSpinButton *button, gpointer data)
{
        ((NEW_DATA*) data)->newrun->year = 
                gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(button));
        update_dates(data);
}

void set_date_day(GtkSpinButton *button, gpointer data)
{
        ((NEW_DATA*) data)->newrun->day = 
                gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(button));
        update_dates(data);
}

static void set_time_entry(GtkEntry *entry) 
{
        gchar text[3];
        guint time = strtol(gtk_entry_get_text(entry), NULL, 10);
        g_snprintf(text, 3, "%i", time);
        if (time < 10) {
                text[1] = text[0];
                text[0] = '0';
                text[2] = '\n';
        }
        gtk_entry_set_text(entry, text);
}

void set_time(GtkWidget *widget, gpointer data)
{
        R2RRun *newrun = ((NEW_DATA*) data)->newrun;
        GtkEntryBuffer *hours = ((NEW_DATA*) data)->hours_buff;
        GtkEntryBuffer *minutes = ((NEW_DATA*) data)->minutes_buff;
        GtkEntryBuffer *seconds = ((NEW_DATA*) data)->seconds_buff;

        //set_time_entry(GTK_ENTRY(widget));
        newrun->duration = 
                 + strtol(gtk_entry_buffer_get_text(seconds), NULL, 10)
                 + 60 * strtol(gtk_entry_buffer_get_text(minutes), NULL, 10)
                 + 60 * 60 * strtol(gtk_entry_buffer_get_text(hours), NULL, 10);
}

void set_distance(GtkSpinButton *button, gpointer data)
{
        ((NEW_DATA*) data)->newrun->distance = 
                gtk_spin_button_get_value(GTK_SPIN_BUTTON(button));
}


void set_type(GtkWidget *widget, gpointer data)
{

        GtkComboBoxText *entry = ((NEW_DATA*) data)->workout_type_entry;

        /* Remember, this must be freed with g_free. If the same pointer is
         * used when it is appeneded to database, it can be freed with the
         * rest of database.
         */
        gchar *test;
        ((NEW_DATA*) data)->newrun->type = NULL;
        ((NEW_DATA*) data)->newrun->type =
                gtk_combo_box_text_get_active_text(entry);
        ((NEW_DATA*) data)->newrun->type_len = 
                g_utf8_strlen(((NEW_DATA*) data)->newrun->type, -1);
}

void set_feel_1(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 1;
}

void set_feel_2(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 2;
}

void set_feel_3(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 3;
}

void set_feel_4(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 4;
}

void set_feel_5(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 5;
}

void set_time_1(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 1;
}

void set_time_2(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 2;
}

void set_time_3(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 3;
}

void set_time_4(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 4;
}


void set_route(GtkWidget *widget, gpointer data)
{

        GtkComboBoxText *entry = ((NEW_DATA*) data)->route_entry;

        /* Remember, this must be freed with g_free. If the same pointer is
         * used when it is appeneded to database, it can be freed with the
         * rest of database.
         */
        gchar *test;
        ((NEW_DATA*) data)->newrun->route = NULL;
        ((NEW_DATA*) data)->newrun->route =
                gtk_combo_box_text_get_active_text(entry);
        ((NEW_DATA*) data)->newrun->route_len = 
                g_utf8_strlen(((NEW_DATA*) data)->newrun->route, -1);
}

static void set_notes(R2RRun *run, GtkTextBuffer *notes_buff)
{

        GtkTextIter start;
        GtkTextIter end;

        gtk_text_buffer_get_bounds(notes_buff, &start, &end);

         run->notes_len = gtk_text_iter_get_visible_line_offset(&end) - 
                gtk_text_iter_get_visible_line_offset(&start);
        
        run->notes = g_realloc(run->notes, run->notes_len + 1);

        g_snprintf(run->notes, run->notes_len + 1, "%s", gtk_text_buffer_get_text(notes_buff, &start, &end, false));
}
        
void save(GtkWidget *widget, gpointer data)
{

        NEW_DATA *new_data = (NEW_DATA*) data;
        R2RDatabase *database = new_data->database;
        R2RRun *new_run = new_data->newrun;
        gint edit = new_data->edit_index;

        /* setting notes here because we don't have a separate save button */
        set_notes(new_run, new_data->notes_buff);

        if (edit < 0) { //new run
        /* Append the new run to the database */
                database->nruns++;
                database->run = g_realloc(database->run, 
                        database->nruns * sizeof(R2RRun*)); 
                database->run[database->nruns - 1] = g_malloc(sizeof(R2RRun)); 
                memcpy(database->run[database->nruns - 1], new_run, sizeof(R2RRun));
 
        printf("new_run = %p\n", new_run);
        int i;
        for (i = 0; i < database->nruns; i++) {
                printf("run[%i] = %p\n", i, database->run[i]);
        }       } else { //edit existing run
        /* Replace the indexed run with the "new run" */
                memcpy(database->run[edit], new_run, sizeof(R2RRun));
        }

        /* Either way, we need to allocate a new new run */

        gtk_widget_hide(GTK_WIDGET(new_data->newrun_window));


}
        
/* Runlist window callbacks */

void refresh_list(GtkWidget *widget, gpointer data)
{
        int i;
        NEW_DATA *new_data = ((NEW_DATA*) data);
        R2RDatabase *database = new_data->database;
        GtkListStore *store = new_data->runlist_store;
        GtkTreeIter iter; 

        printf("(refresh) new_run = %p\n", new_data->newrun);
        gtk_list_store_clear(store);

        //printf("there are %i runs.\n", (int) database->nruns);

        for (i = 0; i < database->nruns; i++) {
        //printf("appending run %i\n", i);
                gtk_list_store_append(store, &iter);
                gtk_list_store_set (store, &iter,
                        INDEX, i,
                        YEAR, database->run[i]->year,
                        MONTH, database->run[i]->month,
                        DAY, database->run[i]->day,
                        RUN_N, database->run[i]->run_n,
                        DISTANCE, database->run[i]->distance,
                        DURATION, database->run[i]->duration,
                        WORKOUT_TYPE, database->run[i]->type,
                        FEEL, database->run[i]->feel,
                        TIME_OF_DAY, database->run[i]->time,
                        ROUTE, database->run[i]->route,
                        -1);
        }
}

void destroy_new_data(GtkWidget *widget, gpointer data)
{
        g_free(data);
}
