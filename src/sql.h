/*****************************************************************************
 * sql.h
 *****************************************************************************
 * conversion of dbf files to sql
 * 
 * Version 0.2, 2003-09-08
 * Author: Dr Georg Roesler, groesle@gwdg.de
 *
 * History:
 * 2003-09-08	teterin,berg	Fixing some errors in the produced SQL statements
 *								Support for MySQL and PostGres
 * 2003-02-24	jones			some minor changes
 * - Version 0.1 - February 2003
 *	 first implementation in dbf.c
 ******************************************************************************/

#ifndef _SQL_H_
#define _SQL_H_

#include "dbf.h"

int setNoDrop (FILE *fp, P_DBF *p_dbf,
    const char *input, const char *separator);
int setNoCreate (FILE *fp, P_DBF *p_dbf,
    const char *input, const char *separator);
int setSQLTrim(FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *mode);
int setSQLUsecopy(FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *mode);
int setSQLEmptyStrIsNULL(FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *mode);
int writeSQLHeader(FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *export_filename);
int writeSQLFooter(FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *export_filename);
int writeSQLLine(FILE *fp, P_DBF *p_dbf,
    const unsigned char *value, int header_length,
    const char *filename,  const char *export_filename);

#endif
