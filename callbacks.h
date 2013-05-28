#include "r2r.h"

typedef struct RUNLIST {
        R2RDatabase *database;
        GtkListStore *store;
} RUNLIST;

typedef struct NEW_DATA {
        R2RRun *newrun;
        GtkCalendar *calendar;
        GtkSpinButton *day_chooser;
        GtkComboBoxText *month_chooser;
        GtkSpinButton *year_chooser;
} NEW_DATA;

void open_window(GtkWidget *widget, gpointer data);

gboolean hide_window(GtkWidget *window, gpointer data);
 
void  set_date_calendar(GtkCalendar *calendar, gpointer data); 
void set_date_day(GtkSpinButton *button, gpointer data);
void set_date_month(GtkComboBox *combo_box, gpointer data);
void set_date_year(GtkSpinButton *button, gpointer data);

void refresh_list(GtkWidget *widget, gpointer data);
