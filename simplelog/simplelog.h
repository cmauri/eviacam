/////////////////////////////////////////////////////////////////////////////
// Name:        simplelog.h
// Purpose:     simple logger
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
#ifndef SIMPLELOG_H
#define SIMPLELOG_H

#include <stdio.h>
#include <stdarg.h>

#if defined (__cplusplus)
extern "C" {
#endif /* __cplusplus */

/** 
 *  @def SLOG_DECLSPEC
 *  Some compilers use a special export keyword
 */
#ifndef SLOG_DECLSPEC
# if defined(WIN32)
#  define SLOG_DECLSPEC	
# else
#  define SLOG_DECLSPEC
# endif
#endif

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
 * @brief Log callback function type
**/
typedef void (*slog_callback_t)(slog_priority_t, const char*, va_list);

/**
 * @brief Get priority name
 */
SLOG_DECLSPEC const char* slog_get_priority_name (slog_priority_t prio);

/**
 * @brief Set log callback
 *
 * If not set, slog_stream_callback is used
 *
 * @param c. Pointer the callback function. If NULL, all logging is supressed.
 */
void SLOG_DECLSPEC slog_set_callback (slog_callback_t c);

/**
 * @brief Set log priority
 *
 * Default priority is SLOG_PRIO_ERR
 *
 * @param pio priority of the message to log.
 * @see slog_priority_t
 */
void SLOG_DECLSPEC slog_set_priority (slog_priority_t prio);

/**
 * @brief Get log priority
 *
 * @see slog_priority_t
 */
slog_priority_t SLOG_DECLSPEC slog_get_priority ();
 
/**
 * @brief Log a message to output or file
 *
 * The message fmt must be used like printf().
 *
 * @param pio priority of the message to log.
 * @param fmt message to log.
 * @param ... Variables to include into the message.
 */
void SLOG_DECLSPEC slog_write (slog_priority_t prio, const char* fmt, ...)
#ifdef  __GNUC__
	 __attribute__ ((format (printf, 2, 3)))	// Compile time format check
#endif
;

/** @name log macros
 */
/*@{*/
#define SLOG_EMERG(fmt,...) slog_write(SLOG_PRIO_EMERG, fmt, ## __VA_ARGS__)
#define SLOG_ALERT(fmt,...) slog_write(SLOG_PRIO_ALERT, fmt, ## __VA_ARGS__)
#define SLOG_CRIT(fmt,...) slog_write(SLOG_PRIO_CRIT, fmt, ## __VA_ARGS__)
#define SLOG_ERR(fmt,...) slog_write(SLOG_PRIO_ERR, fmt, ## __VA_ARGS__)
#define SLOG_WARNING(fmt,...) slog_write(SLOG_PRIO_WARNING, fmt, ## __VA_ARGS__)
#define SLOG_NOTICE(fmt,...) slog_write(SLOG_PRIO_NOTICE, fmt, ## __VA_ARGS__)
#define SLOG_INFO(fmt,...) slog_write(SLOG_PRIO_INFO, fmt, ## __VA_ARGS__)
#define SLOG_DEBUG(fmt,...) slog_write(SLOG_PRIO_DEBUG, fmt, ## __VA_ARGS__)
/*@}*/

/**
 * @name stream based log callback funtions
 */
/*@{*/
/**
 * @brief Stream based log callback function.
 *
 * Send the message to a stream (stderr by default)
 * The message will be logged only if (prio<= current log level)
 * see slog_stream_callback function
 *
 * To be used as parameter for slog_set_callback and NOT to be
 * called directly.
 */ 
void SLOG_DECLSPEC slog_stream_callback (slog_priority_t, const char*, va_list);

/**
 * @brief Set the logging priority for the stream based log callback function.
 *
 * @param stream. If NULL defaults to stderr
 */
void SLOG_DECLSPEC slog_stream_set_stream (FILE *stream);
/*@}*/

#if defined (__cplusplus)
}
#endif /* __cplusplus */

#endif /* SIMPLELOG_H */
