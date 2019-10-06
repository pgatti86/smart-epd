#include <time.h>
#include "time_formatter.h"

static char const *ISO8601_FORMATTER = "%Y-%m-%dT%H:%M:%SZ";

static char const *TIME_FORMATTER = "%R";

static char const *DATE_FORMATTER = "%d/%m/%Y";

static char const *LITERAL_DATE_FORMATTER = "%a %d %b";

static void time_formatter_format_date(char *dst, time_info_t *timeinfo, char const *formatter, int dst_length) {
	
	strftime(dst, dst_length, formatter, timeinfo);
}

void time_formatter_format_current_date(time_info_t *timeinfo, char *dst) {

	time_formatter_format_date(dst, timeinfo, LITERAL_DATE_FORMATTER, 11);
}

void time_formatter_format_current_time(time_info_t *timeinfo, char *dst) {
	
	time_formatter_format_date(dst, timeinfo, TIME_FORMATTER, 9);
}