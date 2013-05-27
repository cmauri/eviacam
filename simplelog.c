/////////////////////////////////////////////////////////////////////////////
// Name:        simplelog.c
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Copyright:   (C) 2012-13 Cesar Mauri Loba - CREA Software Systems
// 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////
#include "simplelog.h"
#include <assert.h>

static slog_callback_t g_slog_callback= slog_stream_callback;
static slog_priority_t g_slog_priority= SLOG_PRIO_ERR;

SLOG_DECLSPEC const char* slog_get_priority_name (slog_priority_t prio)
{
	switch(prio) {
		case SLOG_PRIO_EMERG: return "EMERGENCY";
		case SLOG_PRIO_ALERT: return "ALERT";
		case SLOG_PRIO_CRIT: return "CRITICAL";
		case SLOG_PRIO_ERR: return "ERROR";
		case SLOG_PRIO_WARNING: return "WARNING";
		case SLOG_PRIO_NOTICE: return "NOTICE";
		case SLOG_PRIO_INFO: return "INFO";
		case SLOG_PRIO_DEBUG: return "DEBUG";
		default:
			assert (0);
	}
	return NULL;
}

void slog_set_priority (slog_priority_t prio)
{
	g_slog_priority= prio;
}

slog_priority_t slog_get_priority ()
{
	return g_slog_priority;
}

void SLOG_DECLSPEC slog_set_callback (slog_callback_t c)
{
	g_slog_callback= c;
}
 
void SLOG_DECLSPEC slog_write (slog_priority_t prio, const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);

	if (g_slog_callback) g_slog_callback(prio, fmt, ap);
}

static FILE* g_slog_stream_file= NULL;

void SLOG_DECLSPEC slog_stream_callback (slog_priority_t prio, const char* fmt, va_list ap)
{
	if (!g_slog_stream_file) g_slog_stream_file= stderr;

	if (prio<= g_slog_priority) {
		fprintf (g_slog_stream_file, "%s: ", slog_get_priority_name(prio));
		vfprintf (g_slog_stream_file, fmt, ap);
		fprintf (g_slog_stream_file, "\n");
	}
}

void SLOG_DECLSPEC slog_stream_set_stream (FILE *stream)
{
	g_slog_stream_file= stream;
}
