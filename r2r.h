#ifndef R2R_H
#define R2R_H

#include "stddef.h"

typedef struct R2RRun {
        int year;
        int month;
        int day; 
        int run_n; // cardinality wrt day
        float distance; //miles
        float duration; //minutes
        int workout_type;
        int feel; // how did it feel? 1-5 scale
        int time_of_day; // MORNING, MIDDAY, AFTERNOON, EVENING
        char *route; //name of the route
        size_t route_len;
        char *notes; //additional notes
        size_t notes_len;
} R2RRun;

typedef struct R2RDatabase {
        char *name;
        size_t name_len;
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

int UNITS = MILES;

#endif /* R2R_H */
