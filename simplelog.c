/////////////////////////////////////////////////////////////////////////////
// Name:        simplelog.c
// Author:      Cesar Mauri Loba (cesar at crea-si dot com)
// Copyright:   (C) 2012 Cesar Mauri Loba - CREA Software Systems
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

#include <stdarg.h>

static slog_priority_t g_slog_priority= SLOG_PRIO_ERR;
static FILE* g_slog_stream= NULL;

void slog_set_priority (slog_priority_t prio)
{
	g_slog_priority= prio;
}

slog_priority_t slog_get_priority ()
{
	return g_slog_priority;
}

void slog_set_stream (FILE* stream)
{
	g_slog_stream= stream;
}
 
/**
 * @brief Log a message to output or file
 *
 * The message fmt must be used like printf().
 * The message will be logged only if (prio<= current log level)
 *
 * @param pio priority of the message to log.
 * @param fmt message to log.
 * @param ... Variables to include into the message.
 */
void slog_write (slog_priority_t prio, const char* fmt, ...)
{
	va_list ap;
	va_start (ap, fmt);
	
	if (!g_slog_stream) g_slog_stream= stdout;

	if (prio<= g_slog_priority) {
		switch(prio) {
			case SLOG_PRIO_EMERG: 
				fprintf (g_slog_stream, "EMERGENCY: "); break;
			case SLOG_PRIO_ALERT:
				fprintf (g_slog_stream, "ALERT: "); break;
			case SLOG_PRIO_CRIT: 
				fprintf (g_slog_stream, "CRITICAL: "); break;
			case SLOG_PRIO_ERR: 
				fprintf (g_slog_stream, "ERROR: "); break;
			case SLOG_PRIO_WARNING: 
				fprintf (g_slog_stream, "WARNING: "); break;
			case SLOG_PRIO_NOTICE: 
				fprintf (g_slog_stream, "NOTICE: "); break;
			case SLOG_PRIO_INFO: 
				fprintf (g_slog_stream, "INFO: "); break;
			case SLOG_PRIO_DEBUG: 
				fprintf (g_slog_stream, "DEBUG: "); break;
		}
		vfprintf (g_slog_stream, fmt, ap);
	}
}