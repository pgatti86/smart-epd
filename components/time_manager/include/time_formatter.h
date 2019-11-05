#ifndef __TIME_FORMATTER_H__
#define __TIME_FORMATTER_H__

#include "time_info.h"

#ifdef __cplusplus

extern "C" {
#endif
    void time_formatter_format_current_date(time_info_t *timeinfo, char *dst);

    void time_formatter_format_current_time(time_info_t *timeinfo, char *dst);

    void time_formatter_format_current_seconds(time_info_t *timeinfo, char *dst);

#ifdef __cplusplus
}
#endif


#endif