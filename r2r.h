#ifndef R2R_H
#define R2R_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdlib.h>

#define NFIELDS 11
#define MAX_NAME 256

typedef struct R2RRun {
        int year;
        int month;
        int day; 
        int run_n; // cardinality wrt day
        float distance; //miles
        int duration; //seconds
        char *type;
        size_t type_len; //not including null. Actual length of the array is len+1
        int feel; // how did it feel? 1-5 scale
        int time; // MORNING, MIDDAY, AFTERNOON, EVENING
        char *route; //name of the route
        size_t route_len; // not including null!
        char *notes; //additional notes
        size_t notes_len; //not including null!
} R2RRun;

typedef struct R2RDatabase {
        char *name;
        size_t name_len; //not including null!
        int units;
        R2RRun **run;
        size_t nruns;
} R2RDatabase;
        

enum {
        MILES,
        KILOMETERS,
} units;

enum {
        YEAR,
        MONTH,
        DAY,
        RUN_N,
        DISTANCE,
        DURATION,
        WORKOUT_TYPE,
        FEEL,
        TIME_OF_DAY,
        ROUTE,
        N_COLUMNS,
} columns;

enum {
        MORNING,
        MIDDAY,
        AFTERNOON,
        EVENING,
} time_of_day;

#endif /* R2R_H */
