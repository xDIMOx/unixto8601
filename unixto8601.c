/*
 * This is free and unencumbered software released into the public domain.
 * 
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 * 
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <https://unlicense.org/>
 */

#include <stdint.h>
#include <unistd.h>

enum Month {
	JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC
};

/*
 * This program gets a time stamp in unix epoch format as an argument or
 * read it from standard input, then translates it to ISO 8601, outputing
 * to stardard output.
 */

int
is_leap(int year)
{
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

/* days in month */
int
dim(int mon, int leap)
{
	if (leap && mon == FEB) {
		return 29;
	}

	switch (mon) {
	case JAN: return 31;
	case FEB: return 28;
	case MAR: return 31;
	case APR: return 30;
	case MAY: return 31;
	case JUN: return 30;
	case JUL: return 31;
	case AUG: return 31;
	case SEP: return 30;
	case OCT: return 31;
	case NOV: return 30;
	case DEC: return 31;
	}

	return 0;
}

int
main(int argc, char *argv[])
{
	int i, c;
	int total_days, rem_secs;
	int year, leap;
	int mon, nd, day;
	int hour, min;

	int64_t time; /* As of 2025, Unix time is 64 bits in most systems */

	ssize_t ret;

	char *str;

	char buf[22]; /* To fit a 64 bit int as digits, '\n' and '\0' */

	/*
	 * This program assumes ASCII
	 */

	if (argc < 2) {
		if ((ret = read(0, buf, sizeof(char) * 22)) <= 0) {
			return 1;
		}
		for (i = 0; buf[i] != '\n' && i < 21; ++i);
		buf[21] = buf[i] = '\0';
		str = &buf[0];
	} else {
		str = argv[1];
	}

	for (time = i = 0; str[i] != '\0'; ++i) {
		if ((c = str[i] - '0') < 0 || c > 9) {
			return 1;
		}
		time = time * 10 + c;
	}

	total_days = time / (60 * 60 * 24);
	rem_secs = time % (60 * 60 * 24);

	for (year = 1970; total_days >= ((leap = is_leap(year)) ? 366 : 365); ++year) {
		total_days -= leap ? 366 : 365;
	}

	for (mon = JAN; total_days >= (nd = dim(mon, leap)); ++mon) {
		total_days -= nd;
	}

	day = total_days;
	if (rem_secs > 0) {
		++day;
	}

	hour = rem_secs / (60 * 60);
	rem_secs = rem_secs % (60 * 60);

	min = rem_secs / 60;
	rem_secs = rem_secs % 60;

	return 0;
}
