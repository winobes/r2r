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

void refresh_list(GtkWidget *widget, gpointer data)
{
        int i;
        RUNLIST *runlist = data;
        R2RDatabase *database = runlist->database;
        GtkListStore *store = runlist->store;
        GtkTreeIter iter; 

        gtk_list_store_clear(store);

        for (i = 0; i < database->nruns; i++) {
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
