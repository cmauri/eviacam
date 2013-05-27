/////////////////////////////////////////////////////////////////////////////
// Name:        simplelog.h
// Purpose:     simple logger
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
#ifndef SIMPLELOG_H
#define SIMPLELOG_H

#include <stdio.h>

#if defined (__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief SLOG priority
 */
typedef enum {

	/** EMERGENCY level */
	SLOG_PRIO_EMERG=0,

	/** ALERT level */
	SLOG_PRIO_ALERT=1,

	/** CRITICAL level */
	SLOG_PRIO_CRIT=2,

	/** ERROR level */
	SLOG_PRIO_ERR=3,

	/** WARNING level */
	SLOG_PRIO_WARNING=4,

	/** NOTICE level */
	SLOG_PRIO_NOTICE=5,

	/** INFORMATION level */
	SLOG_PRIO_INFO=6,

	/** DEBUG level */
	SLOG_PRIO_DEBUG=7

} slog_priority_t;

/**
 * @brief Set log priority
 *
 * Default priority is SLOG_PRIO_ERR
 *
 * @param pio priority of the message to log.
 * @see slog_priority_t
 */
void slog_set_priority (slog_priority_t prio);

/**
 * @brief Get log priority
 *
 * @see slog_priority_t
 */
slog_priority_t slog_get_priority ();

/**
 * @brief Set log stream
 *
 * Default stream is stdout
 *
 * @param stream. If NULL defaults to stdout
 */
void slog_set_stream (FILE* stream);
 
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
void slog_write (slog_priority_t prio, const char* fmt, ...);

/*
#define SLOG_DEBUG(fmt,...)  slog_write (SLOG_PRIO_DEBUG, fmt, __VA_ARGS__);
*/

#if defined (__cplusplus)
}
#endif /* __cplusplus */

#endif /* SIMPLELOG_H */
