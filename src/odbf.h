/******************************************************************************
 * odbf.h
 ******************************************************************************
 * dbf Reader and Converter for dBASE files
 * Author: Bjoern Berg <clergyman@gmx.de>
 *
 ******************************************************************************
 * This includes enable dbf to write dBASE files
 ******************************************************************************
 * $Id$
 ******************************************************************************/

#ifndef _ODBASE_H_
#define _ODBASE_H_

#include "dbf.h"

int writeDBFHeader (FILE *fp, P_DBF * p_dbf,
    const char *input, const char *output);
int writeDBFFooter (FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *export_filename);
int writeDBFLine (FILE *fp, P_DBF * p_dbf, const unsigned char *value,
    int header_length, const char *input, const char *output);
#endif
