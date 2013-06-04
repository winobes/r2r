#include "callbacks.h"

void open_window(GtkWidget *widget, gpointer data)
{
        GtkWidget *window = data;
        gtk_widget_show_all(window);
}

gboolean hide_window(GtkWidget *widget, gpointer data)
{
        GtkWidget *window = data;
        gtk_widget_hide(GTK_WIDGET(window));
 
        return true;
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
                
        printf("%i/%i/%i\n", month, day, year);
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

        printf("time = %i seconds\n", newrun->duration);
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
printf("set newrun->type string\n");
        ((NEW_DATA*) data)->newrun->type_len = 
                g_utf8_strlen(((NEW_DATA*) data)->newrun->type, -1);
printf("set newrun->type_len\n");
        printf("%s(%i)\n", ((NEW_DATA*) data)->newrun->type, (int) ((NEW_DATA*) data)->newrun->type_len);
}

void set_feel_1(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 1;
        printf("%i\n", ((NEW_DATA*) data)->newrun->feel);
}

void set_feel_2(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 2;
        printf("%i\n", ((NEW_DATA*) data)->newrun->feel);
}

void set_feel_3(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 3;
        printf("%i\n", ((NEW_DATA*) data)->newrun->feel);
}

void set_feel_4(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 4;
        printf("%i\n", ((NEW_DATA*) data)->newrun->feel);
}

void set_feel_5(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->feel = 5;
        printf("%i\n", ((NEW_DATA*) data)->newrun->feel);
}

void set_time_1(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 1;
        printf("%i\n", ((NEW_DATA*) data)->newrun->time);
}

void set_time_2(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 2;
        printf("%i\n", ((NEW_DATA*) data)->newrun->time);
}

void set_time_3(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 3;
        printf("%i\n", ((NEW_DATA*) data)->newrun->time);
}

void set_time_4(GtkWidget *widget, gpointer data)
{
        if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget)))
                ((NEW_DATA*) data)->newrun->time = 4;
        printf("%i\n", ((NEW_DATA*) data)->newrun->time);
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
printf("set newrun->route string\n");
        ((NEW_DATA*) data)->newrun->route_len = 
                g_utf8_strlen(((NEW_DATA*) data)->newrun->route, -1);
printf("set newrun->route_len\n");
        printf("%s(%i)\n", ((NEW_DATA*) data)->newrun->route, (int) ((NEW_DATA*) data)->newrun->route_len);
}

void save_new(GtkWidget *widget, gpointer data)
{

        R2RDatabase *database = ((NEW_DATA*) data)->database;
        R2RRun *newrun = ((NEW_DATA*) data)->newrun;

        database->nruns++;
        printf("problem\n");
        database->run = g_realloc(database->run, database->nruns * sizeof(R2RRun*));
        printf("/problem\n");

        database->run[database->nruns-1] = newrun;

        ((NEW_DATA*) data)->newrun = g_malloc(sizeof(R2RRun));
       
        printf("newrun as malloced %p\n",((NEW_DATA*) data)->newrun);

        gtk_widget_hide(GTK_WIDGET(((NEW_DATA*) data)->newrun_window));  

        int i;
        for (i = 0; i < database->nruns; i++) {
                printf("database %i is at %p.\n", i, database->run[i]);
        }

}
        

void refresh_list(GtkWidget *widget, gpointer data)
{
        int i;
        RUNLIST *runlist = data;
        R2RDatabase *database = runlist->database;
        GtkListStore *store = runlist->store;
        GtkTreeIter iter; 

        gtk_list_store_clear(store);

        printf("there are %i runs.\n", database->nruns);

        printf("the firal run is distance %f.\n", database->run[database->nruns-1]->distance);

        for (i = 0; i < database->nruns; i++) {
        printf("appending run %i\n", i);
                gtk_list_store_append(store, &iter);
                gtk_list_store_set (store, &iter,
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
