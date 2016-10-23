/**
 * \file      version.h
 * \package   libwbmqtt-dev
 * \brief     Utils for printing software version and Git commit
 * \author    Nikita webconn Maslov <n.maslov@contactless.ru>
 * \copyright 2016
 */

#pragma once

/* WBMQTT_VERSION and WBMQTT_COMMIT comes from Makefile on build.
 * If not, replace 'em with dummy
 */
#ifndef WBMQTT_VERSION
#define WBMQTT_VERSION "<version is not set>"
#endif

#ifndef WBMQTT_COMMIT
#define WBMQTT_COMMIT "<git commit is not set>"
#endif

#ifdef WBMQTT_DIRTYTREE
#define WBMQTT_DIRTYTREE_MSG "\033[31m\033[5mGIT TREE WAS DIRTY DURING THE BUILD!\033[0m\n"
#else
#define WBMQTT_DIRTYTREE_MSG ""
#endif

/* WBMQTT_NAME, WBMQTT_COPYRIGHT and WBMQTT_WRITERS are passed from
 * upper file before including this.
 * If one of this isn't passed, warning will be thrown.
 */
#ifndef WBMQTT_NAME
#warning "WBMQTT_NAME is not defined, passing dummy string"
#define WBMQTT_NAME ""
#endif

#ifndef WBMQTT_COPYRIGHT
#warning "WBMQTT_COPYRIGHT is not defined, passing dummy string"
#define WBMQTT_COPYRIGHT ""
#endif

#ifndef WBMQTT_WRITERS
#warning "WBMQTT_WRITERS is not defined, passing dummy string"
#define WBMQTT_WRITERS "WirenBoard team"
#endif

#ifndef WBMQTT_LICENSE
#define WBMQTT_LICENSE "MIT license"
#endif

#define STR(x) #x
#define XSTR(x) STR(x)

#define VERSION_MESSAGE() WBMQTT_NAME " version " XSTR(WBMQTT_VERSION) "\n" \
                    "Git revision " XSTR(WBMQTT_COMMIT) "\n" WBMQTT_DIRTYTREE_MSG \
                    "Build date: " __DATE__ " " __TIME__ "\n\n" \
                    "Copyright (c) " WBMQTT_COPYRIGHT "\n" \
                    "Distributed under the " WBMQTT_LICENSE "\n" \
                    "Written by " WBMQTT_WRITERS ".\n"
