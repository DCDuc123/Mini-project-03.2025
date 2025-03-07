#include "bitmask_utils.h"

void set_status_bit(uint8_t *status, uint8_t flag) {
    *status |= flag;
}

void clear_status_bit(uint8_t *status, uint8_t flag) {
    *status &= ~flag;
}

int is_status_set(uint8_t status, uint8_t flag) {
    return (status & flag) != 0;
}
