/*****************************************************************************
 * dbf.h
 *****************************************************************************
 * Author: Bjoern Berg, June 2002
 * Email: clergyman@gmx.de
 * dbf Reader and Converter for dBASE files
 * 
 *****************************************************************************
 * $Id$
 *****************************************************************************/

#ifndef __DBF_H__
#define __DBF_H__

#include "config.h"
#include <libdbf/libdbf.h>

#ifdef ENABLE_NLS
#include <libintl.h>
#define _(a) dgettext(GETTEXT_PACKAGE, a)
#else
#define _(a) a
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#ifdef __unix__
#	include <sys/stat.h>
#	include <unistd.h>
#elif __MSDOS__
#	include <io.h>
#	include <sys\stat.h>
#elif _WIN32
#	include <io.h>
#	include <sys\stat.h>
#endif

/*
 * special anubisnet and dbf includes
 */
#include "codepages.h"

#define DBF_FILE_CHECK 1

/*
 * These defines are used to distinguish between types in the
 * dbf fields.
 */
#define IS_STRING 1
#define IS_NUMERIC 2

#ifndef O_BINARY
#define O_BINARY 0
#endif


/*
 * V A R I A B L E S
 */

extern unsigned int verbosity;
extern char *tablename;

/*
 *	FUNCTIONS
 */

typedef int	(*headerMethod)(FILE *output, P_DBF * p_dbf,
    const char *filename, const char *export_filename);

typedef int	(*footerMethod)(FILE *output, P_DBF * p_dbf,
    const char *filename, const char *export_filename);

typedef int	(*lineMethod)(FILE *output, P_DBF * p_dbf,
    const unsigned char *value, int header_length,
    const char *filename, const char *export_filename);

#endif
