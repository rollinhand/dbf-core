/****************************************************************************
 * odbf.c
 ****************************************************************************
 * dbf Reader and Converter for dBASE files
 * Implementation
 *
 * Author: Bjoern Berg <clergyman@gmx.de>
 * Modifications: Uwe Steinmann <uwe@steinmann.cx>
 *
 ****************************************************************************
 * Functions to write dBASE files
 ****************************************************************************
 * $Id$
 ***************************************************************************/

#include <libdbf/libdbf.h>
#include "dbf.h"

static P_DBF *op_dbf;

/* writeDBFHeader() {{{
 * creates the DBF Header with the information provided by DB_FIELD
 */
int
writeDBFHeader (FILE *fp, P_DBF *p_dbf,
    const char *in /* __unused */, const char *out /* __unused */)
{
	int i, columns;
	char *fields, *fieldptr;

	columns = dbf_NumCols(p_dbf);
	if(NULL == (fields = malloc(columns * SIZE_OF_DB_FIELD))) {
		return -1;
	}

	fieldptr = fields;
	for (i = 0; i < columns; i++) {
		char field_type;
		const char *field_name;
		int field_length, field_decimals;
		field_type = dbf_ColumnType(p_dbf, i);
		field_name = dbf_ColumnName(p_dbf, i);
		field_length = dbf_ColumnSize(p_dbf, i);
		field_decimals = dbf_ColumnDecimals(p_dbf, i);
		dbf_SetField((DB_FIELD *)fieldptr, field_type, field_name, field_length, field_decimals);
		fieldptr += SIZE_OF_DB_FIELD;
	}
	op_dbf = dbf_CreateFH(fileno(fp), (DB_FIELD *)fields, columns);

	return 0;
}
/* }}} */

/* writeDBFFooter() {{{
 * creates the DBF Footer
 */
int writeDBFFooter (FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *export_filename)
{
	//dbf_Close(op_dbf);
}
/* }}} */

/* writeDBFLine {{{
 * creates a line in the DBF document for each data set
 */
int
writeDBFLine(FILE *fp, P_DBF *p_dbf,
    const unsigned char *value, int record_length,
    const char *in /* unused */, const char *out /* unused */)
{
	if(0 > dbf_WriteRecord(op_dbf, value, record_length))
		exit(1);
	return 0;
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: sw=4 ts=4 fdm=marker
 * vim<600: sw=4 ts=4
 */
