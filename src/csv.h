/******************************************************************************
 * csv.h
 ******************************************************************************
 * dbf Reader and Converter for dBASE files
 * Author: Bjoern Berg <clergyman@gmx.de>
 *
 ******************************************************************************
 * This includes enable dbf to write CSV files
 ******************************************************************************
 * $Id$
 ******************************************************************************/

#ifndef _CSV_H_
#define _CSV_H_

#include "dbf.h"

int setCSVSep (FILE *fp, P_DBF * p_dbf,
    const char *input, const char *separator);
int writeCSVHeader (FILE *fp, P_DBF * p_dbf,
    const char *input, const char *output);
int writeCSVLine (FILE *fp, P_DBF * p_dbf, const unsigned char *value,
    int header_length, const char *input, const char *output);
#endif
