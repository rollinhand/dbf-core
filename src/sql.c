/*****************************************************************************
 * sql.c
 *****************************************************************************
 * conversion of dbf files to sql
 * 
 * Author: 	Dr Georg Roesler, groesle@gwdg.de
 * 			Mikhail Teterin,
 *			Björn Berg, clergyman@gmx.de
 *
 *****************************************************************************
 * $Id$
 ****************************************************************************/

#include "dbf.h"
#include "sql.h"

/* Whether to trim SQL strings from either side: */
static int trimright = 0;
static int trimleft = 0;

/* Whether to use copy instead of insert statements */
static int usecopy = 0;

/* Whether to add a 'drop table' statement */
static unsigned int sql_drop_table = 1;

/* Whether to create the sql table */
static unsigned int sql_create_table = 1;

/* Whether to output NULL for empty strings */
static int empty_str_is_null = 0;

/* setSQLEmptyStrIsNULL() {{{
 * Handler for the '--empty-str-is-null' option.
 * Output NULL for empty strings.
 */
int
setSQLEmptyStrIsNULL(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	empty_str_is_null = 1;
	return 0;
}
/* }}} */

/* setNoDrop() {{{
 * Handler for the '--nodrop' option.
 * disable output of DROP TABLE statement
 */
int
setNoDrop(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	sql_drop_table = 0;
	return 0;
}
/* }}} */

/* setNoCreate() {{{
 * Handler for the '--nocreate' option.
 * disable output of CREATE TABLE statement
 */
int
setNoCreate(FILE *output, P_DBF *p_dbf,
    const char *filename, const char *level)
{
	sql_create_table = 0;
	return 0;
}
/* }}} */

/* setSQLTrim() {{{
 * Handler for the '--tim char' option.
 */
int setSQLTrim(FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *mode)
{
	if (mode[1] != '\0')
		goto invalid;
	switch (mode[0]) {
		case 'R':
		case 'r':
			trimright = 1;
			return 0;
		case 'L':
		case 'l':
			trimleft = 1;
			return 0;
		case 'B':
		case 'b':
			trimleft = trimright = 1;
			return 0;
		invalid:
		default:
			fprintf(stderr, _("Invalid trim mode ``%s''. Expecting ``r'', ``l'', or ``b'' for both."),
			    mode);
			return 1;
	}
}
/* }}} */

/* setSQLUsecopy() {{{
 * Handler for the '--usecopy' option.
 */
int setSQLUsecopy(FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *mode)
{
	usecopy = 1;
	return(0);
}
/* }}} */

/* writeSQLHeader() {{{
 * creates the SQL Header with the information provided by DB_FIELD
 */
int writeSQLHeader (FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *export_filename)
{
	fprintf(fp, "-- %s -- \n--\n"
	    "-- SQL code with the contents of dbf file %s\n\n", export_filename, filename);

	if ( sql_drop_table ) {
		fprintf(fp, "\nDROP TABLE %s;\n", tablename);
	}   
	if ( sql_create_table ) {
		int unsigned l1,l2;
		int i, columns;

		fprintf(fp, "\nCREATE TABLE %s (\n", tablename);

		columns = dbf_NumCols(p_dbf);
		for (i = 0; i < columns; i++) {
			char field_type;
			const char *field_name;
			int field_length, field_decimals;
			field_type = dbf_ColumnType(p_dbf, i);
			field_name = dbf_ColumnName(p_dbf, i);
			field_length = dbf_ColumnSize(p_dbf, i);
			field_decimals = dbf_ColumnDecimals(p_dbf, i);
			fprintf(fp, "  %-11s ", field_name);
			switch(field_type) {
				case 'C':
					/*
					 * SQL 2 requests "character varying" at this point,
					 * but oracle, informix, db2, MySQL and PGSQL
					 * support also "varchar". To be compatible to most
					 * SQL databases we should use varchar for the moment.
					 * - berg, 2003-09-08
					 */
					fprintf(fp, "varchar(%d)", field_type == 'M' ? 10 : field_length);
				break;
				case 'M':
					/*
					 * M stands for memo fields which are currently not
					 * supported by dbf.
					 * - berg, 2003-09-08
					 */
					fprintf(stderr, _("Invalid mode. Cannot convert this dBASE file. Memo fields are not supported."));
					return 1;
				break;
				case 'I':
					fputs("int", fp);
				break;
				case 'N':
					l1 = field_length;
					l2 = field_decimals;
					if((l1 < 10) && (l2 == 0))
						fputs("int", fp);
					else
						fprintf(fp, "numeric(%d,%d)",
								l1, l2);
				break;
				case 'F':
					l1 = field_length;
					l2 = field_decimals;
					fprintf(fp, "numeric(%d, %d)", l1, l2);
				break;
				case 'B': {
					/*
					 * In VisualFoxPro 'B' stands for double so it is an int value
					 */
					int dbversion = dbf_GetVersion(p_dbf);
					if ( dbversion == VisualFoxPro ) {
						l1 = field_length;
						l2 = field_decimals;
						fprintf(fp, "numeric(%d, %d)", l1, l2);
					} else if ( dbversion == dBase3 ) {
							fprintf(stderr, _("Invalid mode. Cannot convert this dBASE file. Memo fields are not supported."));
						return 1;
					}

				break;
				}
				case 'D':
					fputs("date", fp);
				break;
				case 'L':
					/*
					 * Type logical is not supported in SQL, you have to use number
					 * resp. numeric to keep to the standard
					 */
					 fprintf(fp, "boolean");
				break;
				default:
					fprintf(fp, "/* unsupported type ``%c'' */", field_type);
			}
			if (i < columns-1)
				fputc(',', fp);
			fputs("\n", fp);
		}
		fputs(");\n\n", fp);
	}

	if(usecopy) {
		int columns, i;

		fprintf(fp, "COPY %s (", tablename);
		columns = dbf_NumCols(p_dbf);
		for (i = 0; i < columns; i++) {
			const char *field_name;
			field_name = dbf_ColumnName(p_dbf, i);
			fprintf(fp, "%s", field_name);
			if (i < columns-1)
				fputc(',', fp);
		}
		fputs(") FROM stdin;\n", fp);
	}

	return 0;
}
/* }}} */

/* writeSQLFooter() {{{
 * creates the SQL Footer
 */
int writeSQLFooter (FILE *fp, P_DBF *p_dbf,
    const char *filename, const char *export_filename)
{
	if(usecopy)
		fputs("\\.\n", fp);
}
/* }}} */

/* writeSQLLine() {{{
 * fills the SQL table
 */
int
writeSQLLine (FILE *fp, P_DBF *p_dbf, 
    const unsigned char *value, int record_length,
    const char *filename, const char *export_filename)
{
	int i, columns;

	columns = dbf_NumCols(p_dbf);

	if(!usecopy)
		fprintf(fp, "INSERT INTO %s VALUES (", tablename);

	for (i = 0; i < columns; i++) {
		const unsigned char *end, *begin;
		char field_type;
		int isstring;
		int isdate;
		int isbool;
		field_type = dbf_ColumnType(p_dbf, i);
		isstring = (field_type == 'M' || field_type == 'C');
		isdate = (field_type == 'D');
		isbool = (field_type == 'L');

		/*
		 * A string is only trimmed if trimright and/or trimleft is set
		 * Other datatypes are always "trimmed" to determine, if they
		 * are empty, in which case they are printed out as NULL -- to
		 * keep the SQL correctness.	-mi	Aug, 2003
		 */
		begin = value;
		value += dbf_ColumnSize(p_dbf, i); /* The next field */
		end = value;

		/* Remove NULL chars at end of field */
		while(--end != begin && *end == '\0')
			;

		if(begin == end && *end == '\0') {
			if(usecopy)
				fputs("\\N", fp);
			else
				fputs("NULL", fp);
			continue;
		}

		end++;

		/*
		 * Non-string data-fields are already right justified
		 * and actually don't need right-trimming, but if we right trim
		 * them as well, we will determine NULL values easily.
		 */
		if (!isstring || (isstring && trimright)) {
			while (--end != begin && *end == ' ')
				;
			if (end == begin && *end == ' ') {
				if(empty_str_is_null || !isstring) {
					if(usecopy)
						fputs("\\N", fp);
					else
						fputs("NULL", fp);
				} else {
					if(!usecopy) {
						putc('\'', fp);
						putc('\'', fp);
					}
				}
				/* Is this the last field? */
				if (i < columns-1) {
					if(usecopy)
						putc('\t', fp);
					else
						putc(',', fp);
				}
				continue;
			}
			end++;
		}

		if (trimleft || !isstring) {
			while (begin != end && *begin == ' ')
				begin++;
		}

		if (!usecopy && (isdate || isstring)) {
			putc('\'', fp);
		}

		/* Output the field data */
		if (isbool) {
			char sign = *begin++;
			if ( sign == 't' || sign == 'y' || sign == 'T' || sign == 'Y') {
				fprintf(fp, "true");
			} else {
				fprintf(fp, "false");
			}

		} else if (field_type == 'B' || field_type == 'F') {
			char fmt[30];

			sprintf(fmt, "%%%d.%df", dbf_ColumnSize(p_dbf, i), dbf_ColumnDecimals(p_dbf, i));
			fprintf(fp, fmt, *(double *)begin);
			begin += dbf_ColumnSize(p_dbf, i);		

		} else {

			do	{ /* Output the non-empty string:*/

				char sign = *begin++;	/* cast operations */
				switch (sign) {
					case '\'':
						putc('\\', fp);
						putc('\'', fp);
						break;
					case '\"':
						putc('\\', fp);
						putc('\"', fp);
						break;
					default:
						putc(sign, fp);
				}
			} while (begin < end);

		}

		if (!usecopy && (isdate || isstring))
			putc('\'', fp);

		/* Is this the last field? */
		if (i < columns-1) {
			if(usecopy)
				putc('\t', fp);
			else
				putc(',', fp);
		}

	}
	/* Terminate INSERT INTO or COPY line ; */
	if(!usecopy)
		fputs(");", fp);
	fputs("\n", fp);

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
