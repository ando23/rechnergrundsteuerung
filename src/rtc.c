// Copyright 2022-2024; for authors see bottom
// Licence: MIT

#include "rtc.h"
#include "cpu.h"
#include "string.h"
#include "textmode.h"


#define RTC_CMD	0x70
#define RTC_DATA	0x71

#define RTC_FUNC_CUR_SEC	0x00
#define RTC_FUNC_ALARM_SEC	0x01
#define RTC_FUNC_CUR_MIN	0x02
#define RTC_FUNC_ALARM_MIN	0x03
#define RTC_FUNC_CUR_HOUR	0x04
#define RTC_FUNC_ALARM_HOUR	0x05
#define RTC_FUNC_CUR_DOW	0x06
#define RTC_FUNC_CUR_DAY	0x07
#define RTC_FUNC_CUR_MONTH	0x08
#define RTC_FUNC_CUR_YEAR	0x09
#define RTC_FUNC_STATEREG_A	0x0A
#define RTC_FUNC_STATEREG_B	0x0B
#define RTC_FUNC_STATEREG_C	0x0C
#define RTC_FUNC_STATEREG_D	0x0D
#define RTC_FUNC_CUR_EPOQUE	0x32

struct rtc_time_t {
	uint8_t year, month, day, hour, min, sec, dow;
} rtc_current_time;


void rtc_init() {
	rtc_current_time.year = 0;
	rtc_current_time.month = 0;
	rtc_current_time.day = 0;
	rtc_current_time.hour = 0;
	rtc_current_time.min = 0;
	rtc_current_time.sec = 0;
	rtc_current_time.dow = 0;
	
	cpu_out8( RTC_CMD, 0x8A );
	cpu_out8( RTC_DATA, 0x20 );

	cpu_out8( RTC_CMD, RTC_FUNC_STATEREG_B );
	uint8_t v = cpu_in8( RTC_DATA ) | 0b00000110;
	//CHECKME: muss hier noch: cpu_out8( RTC_CMD, RTC_FUNC_STATEREG_B );
	cpu_out8( RTC_DATA, v );
}

	
void rtc_update() {
	cpu_out8( RTC_CMD, RTC_FUNC_CUR_YEAR );
	rtc_current_time.year = cpu_in8( RTC_DATA );

	cpu_out8( RTC_CMD, RTC_FUNC_CUR_MONTH );
	rtc_current_time.month = cpu_in8( RTC_DATA );

	cpu_out8( RTC_CMD, RTC_FUNC_CUR_DAY );
	rtc_current_time.day = cpu_in8( RTC_DATA );

	cpu_out8( RTC_CMD, RTC_FUNC_CUR_HOUR );
	rtc_current_time.hour = cpu_in8( RTC_DATA );

	cpu_out8( RTC_CMD, RTC_FUNC_CUR_MIN );
	rtc_current_time.min = cpu_in8( RTC_DATA );

	cpu_out8( RTC_CMD, RTC_FUNC_CUR_SEC );
	rtc_current_time.sec = cpu_in8( RTC_DATA );
}

void rtc_print() {
	char buf[200];
	
	snprintf(buf, sizeof(buf), "\rAktuelle Zeit: 20%d-%d-%d %d:%d:%d",
		rtc_current_time.year,
		rtc_current_time.month,
		rtc_current_time.day,
		rtc_current_time.hour,
		rtc_current_time.min,
		rtc_current_time.sec
		);
	puts(buf);
}

/* ---- Authors (in alphabetical order) ----
 * Andreas Herzig
 */