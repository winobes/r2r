#include "r2r.h"
#include <string.h>

typedef struct NEW_DATA {

        R2RDatabase *database;
        R2RRun *newrun;
        gint edit_index;

        GtkWidget *runlist_window;
        GtkTreeView *runlist;
        GtkListStore *runlist_store;

        GtkWidget *newrun_window;
        GtkCalendar *calendar;
        GtkSpinButton *distance_chooser;
        GtkSpinButton *day_chooser;
        GtkComboBoxText *month_chooser;
        GtkSpinButton *year_chooser;
        GtkEntryBuffer *seconds_buff;
        GtkEntryBuffer *minutes_buff;
        GtkEntryBuffer *hours_buff;
        GtkRadioButton *feel1;
        GtkRadioButton *feel2;
        GtkRadioButton *feel3;
        GtkRadioButton *feel4;
        GtkRadioButton *feel5;
        GtkRadioButton *time1;
        GtkRadioButton *time2;
        GtkRadioButton *time3;
        GtkRadioButton *time4;
        GtkComboBoxText *workout_type_entry;
        GtkComboBoxText *route_entry;
        GtkTextBuffer *notes_buff;

} NEW_DATA;

void open_window(GtkWidget *widget, gpointer data);
void init_newrun_window_for_new_run(GtkWidget *widget, gpointer data);
void init_newrun_window_for_edit(GtkWidget *widget, gpointer data);

gboolean hide_window(GtkWidget *window, gpointer data);
 
void  set_date_calendar(GtkCalendar *calendar, gpointer data); 
void set_date_day(GtkSpinButton *button, gpointer data);
void set_date_month(GtkComboBox *combo_box, gpointer data);
void set_date_year(GtkSpinButton *button, gpointer data);
void set_time(GtkWidget *widget, gpointer data);
void set_distance(GtkSpinButton *button, gpointer data);
void set_type(GtkWidget *widget, gpointer data);
void set_feel_1(GtkWidget *widget, gpointer data);
void set_feel_2(GtkWidget *widget, gpointer data);
void set_feel_3(GtkWidget *widget, gpointer data);
void set_feel_4(GtkWidget *widget, gpointer data);
void set_feel_5(GtkWidget *widget, gpointer data);
void set_time_1(GtkWidget *widget, gpointer data);
void set_time_2(GtkWidget *widget, gpointer data);
void set_time_3(GtkWidget *widget, gpointer data);
void set_time_4(GtkWidget *widget, gpointer data);
void set_route(GtkWidget *widget, gpointer data);
void save(GtkWidget *widget, gpointer data);

void refresh_list(GtkWidget *widget, gpointer data);
void destroy_new_data(GtkWidget *widget, gpointer data);
