/*****************************************************************************
 * statistic.c
 * inherits the statistic functions for dBASE files
 * Author: Bjoern Berg, September 2002
 * Email: clergyman@gmx.de
 * dbf Reader and Converter for dBase III, IV, 5.0
 *
 *
 *****************************************************************************
 * $Id$
 ****************************************************************************/

#include "dbf.h"
#include "statistic.h"


static const char *get_db_version (int version) {
	static char name[31];

	switch (version) {
		case 0x02:
			// without memo fields
			return "FoxBase";
		case 0x03:
			// without memo fields
			return "FoxBase+/dBASE III+";
		case 0x04:
			// without memo fields
			return "dBASE IV";
		case 0x05:
			// without memo fields
			return "dBASE 5.0";
		case 0x83:
			return "FoxBase+/dBASE III+";
		case 0x8B:
			return "dBASE IV";
		case 0x30:
			// without memo fields
			return "Visual FoxPro";
		case 0xF5:
			// with memo fields
			return "FoxPro 2.0";
		default:
			sprintf(name, _("Unknown (code 0x%.2X)"), version);
			return name;
	}
}


/* output for header statistic */
void
dbf_file_info (P_DBF *p_dbf)
{
	int version, memo;

	version	= dbf_GetVersion(p_dbf);
	memo = (version  & 128)==128 ? 1 : 0;
	printf("\n");
	printf(_("File statistics:"));
	printf("\n");
	printf(_("dBase version.........:"));
	printf(" \t %s (%s)\n",
			dbf_GetStringVersion(p_dbf), memo?"with memo":"without memo");
	printf(_("Date of last update...:"));
	printf(" \t %s\n", dbf_GetDate(p_dbf));
	printf(_("Number of records.....:"));
	printf(" \t %d (0x%08x)\n", dbf_NumRows(p_dbf), dbf_NumRows(p_dbf));
	printf(_("Length of header......:"));
	printf(" \t %d (0x%04x)\n", dbf_HeaderSize(p_dbf), dbf_HeaderSize(p_dbf));
	printf(_("Record length.........:"));
	printf(" \t %d (0x%04x)\n", dbf_RecordLength(p_dbf), dbf_RecordLength(p_dbf));
	printf(_("Columns in file.......:"));
	printf(" \t %d \n", dbf_NumCols(p_dbf));
}

/* output for field statistic */
#define linelength	73

void
dbf_field_stat (P_DBF *p_dbf)
{
	const struct DB_FIELD *dbf;
	int cross[] = {1,17,25,41,57,73};
	int i, columns;
	columns = dbf_NumCols(p_dbf);

	drawline(linelength, cross, (sizeof(cross)/sizeof(int)));
	printf("| ");
	printf(_("field name"));
	printf("\t| ");
	printf(_("type"));
	printf("\t| ");
	printf(_("field address"));
	printf("\t| ");
	printf(_("length"));
	printf("\t| ");
	printf(_("field dec."));
	printf("\t|\n");
	drawline(linelength, cross, sizeof(cross)/sizeof(int));
	for (i = 0; i < columns; i++) {
		char field_type;
		const char *field_name;
		int field_length, field_decimals, field_address;
		field_type = dbf_ColumnType(p_dbf, i);
		field_name = dbf_ColumnName(p_dbf, i);
		field_length = dbf_ColumnSize(p_dbf, i);
		field_decimals = dbf_ColumnDecimals(p_dbf, i);
		field_address = dbf_ColumnAddress(p_dbf, i);

		printf("|%13.11s\t| %3c\t| %8x\t| %3d\t\t| %3d\t\t|\n",
			   field_name, field_type, field_address, field_length, field_decimals);
	}
	drawline(linelength, cross, sizeof(cross)/sizeof(int));
}
