#ifndef BITMASK_UTILS_H
#define BITMASK_UTILS_H

#include <stdint.h>

#define STATUS_ON        (1 << 0)  // Module đang bật
#define STATUS_ERROR     (1 << 1)  // Module gặp lỗi
#define STATUS_WARNING   (1 << 2)  // cảnh báo lỗi

void set_status_bit(uint8_t *status, uint8_t flag);
void clear_status_bit(uint8_t *status, uint8_t flag);
int is_status_set(uint8_t status, uint8_t flag);

#endif
