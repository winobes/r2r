static void open_window(GtkWidget *widget, gpointer data)
{

        GtkWidget *window = data;

        gtk_widget_show_all(window);

}

static gboolean hide_window(GtkWidget *caller, gpointer closing)
{
        GtkWidget *window = closing;
        
        gtk_widget_hide(GTK_WIDGET(window));
 
return true;

}

static gboolean set_date_ints(GtkCalendar *calendar, guint* date)
{

       gtk_calendar_get_date(GTK_CALENDAR(calendar), &date[YEAR], 
                &date[MONTH], 
                &date[DAY]);
      
}
