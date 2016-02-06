/*
 * Copyright © 2016 Romeo Calota
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Romeo Calota
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define MAX_LOG_LEN     2048

#define LOG(...) \
    do { \
    char log[MAX_LOG_LEN]; \
    snprintf(log, MAX_LOG_LEN - 1, __VA_ARGS__); \
    fprintf(stderr, \
            "Kinesix: INFO: %s: %s: %d: %s\n", \
            __FILE__, \
            __PRETTY_FUNCTION__, \
            __LINE__, \
            log); \
    } while (0)

#define LOG_INFO(message) LOG(message)

#define LOG_WARN(...) \
    do { \
    char log[MAX_LOG_LEN]; \
    snprintf(log, MAX_LOG_LEN - 1, __VA_ARGS__); \
    fprintf(stderr, \
            "Kinesix: WARNING: %s: %s: %d: %s\n", \
            __FILE__, \
            __PRETTY_FUNCTION__, \
            __LINE__, \
            log); \
    } while (0)

#define LOG_ERROR(...) \
    do { \
    char log[MAX_LOG_LEN]; \
    snprintf(log, MAX_LOG_LEN - 1, __VA_ARGS__); \
    fprintf(stderr, \
            "Kinesix: ERROR: %s: %s: %d: %s\n", \
            __FILE__, \
            __PRETTY_FUNCTION__, \
            __LINE__, \
            log); \
    } while (0)

#define LOG_FATAL(...) \
    do { \
    char log[MAX_LOG_LEN]; \
    snprintf(log, MAX_LOG_LEN - 1, __VA_ARGS__); \
    fprintf(stderr, \
            "Kinesix: FATAL: %s: %s: %d: %s\n", \
            __FILE__, \
            __PRETTY_FUNCTION__, \
            __LINE__, \
            log); \
    exit(EXIT_FAILURE); \
    } while (0)

#ifdef DEBUG_BUILD
#define LOG_DEBUG(...) \
    do { \
    char log[MAX_LOG_LEN]; \
    snprintf(log, MAX_LOG_LEN - 1, __VA_ARGS__); \
    fprintf(stderr, \
            "Kinesix: DEBUG: %s: %s: %d: %s\n", \
            __FILE__, \
            __PRETTY_FUNCTION__, \
            __LINE__, \
            log); \
    } while (0)
#else
#define LOG_DEBUG(...)
#endif

#endif // LOGGER_H
