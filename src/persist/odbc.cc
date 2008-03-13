/*
 * FUNCTION:
 * ODBC driver -- developed/tested with iODBC http://www.iodbc.org
 *
 * ODBC is basically brain-damaged, and so is this driver.
 * The problem is that ODBC forces you to guess how many columns there
 * are in your reply, and etc. which we don't know a-priori.  Also
 * makes VARCHAR difficult (impossible ??) to support correctly!
 * Blame it on SQLBindCol(), which is a terrible idea.  @#$%^ Microsoft.
 *
 * HISTORY:
 * Copyright (c) 2002 Linas Vepstas <linas@linas.org>
 * created by Linas Vepstas  March 2002
 */

#include <stack>
#include <string>

#include <sql.h>
#include <sqlext.h>
#include <stdio.h>

class ODBCRecordSet;

class ODBCConnection
{
	friend class ODBCRecordSet;
	private:
		std::string dbname;
		std::string username;
		SQLHENV sql_henv;
		SQLHDBC sql_hdbc;
		std::stack<ODBCRecordSet *> free_pool;

		ODBCRecordSet *get_record_set(void);

	public:
		ODBCConnection(const char * dbname,
		               const char * username,
		               const char * authentication);
		~ODBCConnection();

		ODBCRecordSet *exec(const char * buff);
};

class ODBCRecordSet
{
	friend class ODBCConnection;
	private:
		ODBCConnection *conn;
		SQLHSTMT sql_hstmt;
	
		int ncols;
		int arrsize;
		char **column_labels;
		int  *column_datatype;
		char **values;
		int  *vsizes;

		void alloc_and_bind_cols(int ncols);
		ODBCRecordSet(ODBCConnection *);
		~ODBCRecordSet();
	public:
		void release(void);
};


// cheesy hack for missing PERR
#define PERR printf

/* =========================================================== */

#define PRINT_SQLERR(HTYPE,HAN)                                \
{                                                              \
	char sql_stat[10];                                          \
	SQLSMALLINT msglen;                                         \
	SQLINTEGER err;                                             \
	char msg[200];                                              \
	                                                            \
	SQLGetDiagRec(HTYPE, HAN, 1, (SQLCHAR *) sql_stat,          \
	              &err, (SQLCHAR*) msg, sizeof(msg), &msglen);  \
	PERR("(%ld) %s\n", err, msg);                               \
}

/* =========================================================== */

ODBCConnection::ODBCConnection(const char * _dbname,
                               const char * _username,
                               const char * _authentication)
{
	SQLRETURN rc;

	if (NULL == _dbname)
	{
		PERR("No DB specified");
		return;
	}

	/* Allocate environment handle */
	rc = SQLAllocEnv(&sql_henv);
	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR("Can't SQLAllocEnv, rc=%d", rc);
		return;
	}

	/* set the ODBC version */
	rc = SQLSetEnvAttr(sql_henv, SQL_ATTR_ODBC_VERSION, 
			       (void*)SQL_OV_ODBC3, 0); 
	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR("Can't SQLSetEnv, rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_ENV, sql_henv);
		SQLFreeHandle(SQL_HANDLE_ENV, sql_henv);
		return;
	}

	/* allocate the connection handle */
	rc = SQLAllocConnect(sql_henv, &sql_hdbc); 
	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR ("Can't SQLAllocConnect handle rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_ENV, sql_henv);
		SQLFreeHandle(SQL_HANDLE_ENV, sql_henv);
		return;
	}

	/* set the timeout to 5 seconds ?? hack alert fixme */
	// SQLSetConnectAttr(sql_hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER *)5, 0);

	if (NULL == _authentication) _authentication = "";
	rc = SQLConnect(sql_hdbc, 
			 (SQLCHAR*) _dbname, SQL_NTS,
			 (SQLCHAR*) _username, SQL_NTS,
			 (SQLCHAR*) _authentication, SQL_NTS);

	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR ("Can't perform SQLConnect rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_DBC, sql_hdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, sql_hdbc);
		SQLFreeHandle(SQL_HANDLE_ENV, sql_henv);
		return;
	}

	dbname = _dbname;
	username = _username;
}

/* =========================================================== */

ODBCConnection::~ODBCConnection()
{
	SQLDisconnect(sql_hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, sql_hdbc);
	sql_hdbc = NULL;

	SQLFreeHandle(SQL_HANDLE_ENV, sql_henv);
	sql_henv = NULL;
	
	while (!free_pool.empty())
	{
		ODBCRecordSet *rs = free_pool.top();
		delete rs;
		free_pool.pop();
	}
}

/* =========================================================== */
#define DEFAULT_NUM_COLS 50

ODBCRecordSet * ODBCConnection::get_record_set(void)
{
	ODBCRecordSet *rs;
	if (!free_pool.empty())
	{
		rs = free_pool.top();
		free_pool.pop();
		rs->ncols = -1;
	}
	else
	{
		rs = new ODBCRecordSet(this);
	}

	rs->alloc_and_bind_cols(DEFAULT_NUM_COLS);

	return rs;
}

/* =========================================================== */

ODBCRecordSet *
ODBCConnection::exec(const char * buff)
{
	ODBCRecordSet *rs;
	SQLRETURN rc;

	rs = get_record_set();
	if (!rs) return NULL;

	rc = SQLExecDirect(rs->sql_hstmt, (SQLCHAR *)buff, SQL_NTS);

	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR ("Can't perform query rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_STMT, rs->sql_hstmt);
		rs->release();
		return NULL;
	}

	/* Use numbr of columns to indicate that the query hasn't 
	 * given results yet. */
	rs->ncols = -1;
	return rs;
}

/* =========================================================== */

#define DEFAULT_VARCHAR_SIZE 4040

void
ODBCRecordSet::alloc_and_bind_cols(int ncols)
{
	SQLINTEGER err;
	SQLRETURN rc;
	int i;

	if (ncols > arrsize)
	{
		if (column_labels)
		{
			for (i=0; i<arrsize; i++)
			{
				if (column_labels[i]) delete column_labels[i];
			}
			delete column_labels;
		}
		if (column_datatype) delete column_datatype;

		if (values)
		{
			for (i=0; i<arrsize; i++)
			{
				if (NULL == values[i])
				{
					delete values[i];
				}
			}
			delete values;
		}
		if (vsizes) delete vsizes;

		column_labels = new char*[ncols];
		column_datatype = new int[ncols];
		values = new char*[ncols];
		vsizes = new int[ncols];

		/* intialize */
		for (i = 0; i<ncols; i++)
		{
			column_labels[i] = NULL;
			column_datatype[i] = 0;
			values[i] = NULL;
			vsizes[i] = 0;
		}
		arrsize = ncols; 
	}

	/* Initialize the newly realloc'ed entries */
	for (i=0; i<ncols; i++)
	{
		column_labels[i] = NULL;
		column_datatype[i] = 0;

		if (NULL == values[i])
		{
			values[i] = new char[DEFAULT_VARCHAR_SIZE];
			vsizes[i] = DEFAULT_VARCHAR_SIZE;
			values[i][0] = 0;
		}
		rc = SQLBindCol(sql_hstmt, i+1, SQL_C_CHAR, 
			values[i], vsizes[i], &err);
		if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
		{
			PERR ("Can't bind col=%d rc=%d", i, rc);
			PRINT_SQLERR (SQL_HANDLE_STMT, sql_hstmt);
			return;
		}
	}
}

/* =========================================================== */
/* pseudo-private routine */


ODBCRecordSet::ODBCRecordSet(ODBCConnection *_conn)
{
	SQLRETURN rc;

	if (!_conn) return;
	conn = _conn;
	
	ncols = -1;
	arrsize = 0;
	column_labels = NULL;
	column_datatype = NULL;
	values = NULL;
	vsizes = NULL;

	rc = SQLAllocStmt (conn->sql_hdbc, &sql_hstmt);
	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR("Can't allocate statment handle, rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_STMT, sql_hstmt);
		/* oops memory leak */
		return;
	}
}

/* =========================================================== */

void
ODBCRecordSet::release(void)
{
	if (!this) return;

	SQLFreeHandle(SQL_HANDLE_STMT, sql_hstmt);
	sql_hstmt = NULL;

	conn->free_pool.push(this);
}

/* =========================================================== */

ODBCRecordSet::~ODBCRecordSet()
{
	int i;

	conn = NULL;

	for (i=0; i<arrsize; i++)
	{
		delete column_labels[i];
		delete values[i];
	}
	delete column_labels;
	column_labels = NULL;

	delete column_datatype;
	column_datatype = NULL;

	delete values;
	values = NULL;

	delete vsizes;
	vsizes = NULL;
}

/* =========================================================== */

int main ()
{
	ODBCConnection *conn;
	conn = new ODBCConnection("opencog", "linas", NULL);

	ODBCRecordSet *rs = conn->exec("SELECT * FROM Atoms;");
	rs->release();

	return 0;
}


/* =========================================================== */

#if OLD_CODE


/* =========================================================== */

DuiDBRecordSet *
dui_odbc_connection_tables (DuiDBConnection *dbc)
{
	DuiODBCConnection *conn = (DuiODBCConnection *) dbc;
	DuiODBCRecordSet *rs;
	SQLRETURN rc;

	ENTER ("(conn=%p)", conn);
	if (!conn) return NULL;

	rs = dui_odbc_recordset_new (conn);
	if (!rs) return NULL;

	rc = SQLTables (rs->sql_hstmt,NULL, 0, NULL, 0, NULL,0, NULL, 0);

	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR ("Can't perform query rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_STMT, rs->sql_hstmt);
		dui_odbc_recordset_release (&rs->recset);
		return NULL;
	}

	LEAVE ("(conn=%p)", conn);
	/* Use numbr of columns to indicate that the query hasn't 
	 * given results yet. */
	rs->ncols = -1;
	return &rs->recset;
}


/* =========================================================== */

DuiDBRecordSet *
dui_odbc_connection_table_columns (DuiDBConnection *dbc, 
                                   const char * tablename)
{
	DuiODBCConnection *conn = (DuiODBCConnection *) dbc;
	DuiODBCRecordSet *rs;
	SQLRETURN rc;

	ENTER ("(conn=%p, table=%s)", conn, tablename);
	if (!conn || !tablename) return NULL;

	rs = dui_odbc_recordset_new (conn);
	if (!rs) return NULL;

   rc=SQLColumns (rs->sql_hstmt,NULL, 0, NULL, 0, 
                  (char *) tablename, SQL_NTS, NULL, 0);

	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR ("Can't perform query rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_STMT, rs->sql_hstmt);
		dui_odbc_recordset_release (&rs->recset);
		return NULL;
	}

	LEAVE ("(conn=%p, table=%s)", conn, tablename);
	/* Use numbr of columns to indicate that the query hasn't 
	 * given results yet. */
	rs->ncols = -1;
	return &rs->recset;
}


/* =========================================================== */

static void
dui_odbc_recordset_get_column_labels (DuiODBCRecordSet *rs)
{
	SQLSMALLINT ncols;
	SQLRETURN rc;
	int i;

	if (0 <= rs->ncols) return;

	/* If number of columns is negative, then we haven't 
	 * gotten any results back yet.  Start by getting the 
	 * column labels. 
	 */

	rc = SQLNumResultCols (rs->sql_hstmt, &ncols);
	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR ("Can't get num columns rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_STMT, rs->sql_hstmt);
		return;
	}

	if (ncols > rs->arrsize)
	{
		PERR( "screwed not enough columns !! ");
		ncols =  rs->arrsize;
	}

	for (i=0; i<ncols; i++)
	{
		char namebuff[300];
		SQLSMALLINT namelen;
		SQLUINTEGER column_size;
		SQLSMALLINT datatype;
		SQLSMALLINT decimal_digits;
		SQLSMALLINT nullable;

		rc = SQLDescribeCol (rs->sql_hstmt, i+1, namebuff, 299, &namelen,
   			 &datatype, &column_size, &decimal_digits, &nullable);
		if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
		{
			PERR ("Can't describe col rc=%d", rc);
			PRINT_SQLERR (SQL_HANDLE_STMT, rs->sql_hstmt);
			return;
		}

		namebuff[namelen] = 0x0;
		PINFO ("column %d has name\'%s\'", i, namebuff);

		g_free (rs->column_labels[i]);
		rs->column_labels[i] = g_strdup (namebuff);
		rs->column_datatype[i] = datatype;
	}

	rs->ncols = ncols;
}

/* =========================================================== */

static int
get_col_by_name (DuiODBCRecordSet *rs, const char * fieldname)
{
	int i;
	const char * fp;

	/* lookup the column number based on the column name */
	for (i=0; i<rs->ncols; i++)
	{
		if (!strcasecmp (fieldname, rs->column_labels[i])) return i;
	}

	/* oops. Try removing the table name if possible */
	fp = strrchr (fieldname, '.');
	if (!fp) return -1;
	fp ++;

	for (i=0; i<rs->ncols; i++)
	{
		if (!strcasecmp (fp, rs->column_labels[i])) return i;
	}

	return -1;
}

/* =========================================================== */

int
dui_odbc_recordset_rewind (DuiDBRecordSet *recset)
{
	/* XXX implement me */
	/* DuiODBCRecordSet *rs = (DuiODBCRecordSet *) recset; */
	
	return dui_odbc_recordset_fetch_row (recset);
}

/* =========================================================== */

int
dui_odbc_recordset_fetch_row (DuiDBRecordSet *recset)
{
	DuiODBCRecordSet *rs = (DuiODBCRecordSet *) recset;
	SQLRETURN rc;

	if (!rs) return 0;

	rc = SQLFetch(rs->sql_hstmt);  

	/* no more data */
	if (SQL_NO_DATA == rc) return 0;
	if (SQL_NULL_DATA == rc) return 0;

	if ((SQL_SUCCESS != rc) && (SQL_SUCCESS_WITH_INFO != rc))
	{
		PERR ("Can't fetch row rc=%d", rc);
		PRINT_SQLERR (SQL_HANDLE_STMT, rs->sql_hstmt);
		return 0;
	}

	return 1;
}

/* =========================================================== */

const char *
dui_odbc_recordset_get_value (DuiDBRecordSet *recset, const char * fieldname)
{
	DuiODBCRecordSet *rs = (DuiODBCRecordSet *) recset;
	int column;

	if (!rs) return NULL;

	/* If number of columns is negative, then we haven't 
	 * gotten any results back yet.  Start by getting the 
	 * column labels. 
	 */
	if (0 > rs->ncols)
	{
		dui_odbc_recordset_get_column_labels (rs);
	}

	column = get_col_by_name (rs, fieldname);
	if (0 > column) return NULL;

	LEAVE ("(rs=%p, fieldname=%s) {val=\'%s\'}", rs, fieldname,  rs->values[column]);
	return rs->values[column];
}

#endif /* USE_ODBC */
/* ============================= END OF FILE ================= */
 
