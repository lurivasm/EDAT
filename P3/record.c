#include "record.h"

struct record_ {
    /* to be implemented */
};

record_t* record_create(void** values, int ncols, long next) {
    /* to be implemented */
}

void* record_get(record_t* record, int n) {
    /* to be implemented */
}

long record_next(record_t* record) {
    /* to be implemented */
}

void record_free(record_t* record) {
    /* to be implemented */
}
