#include "../headers/DB.h"

// Connect to DB
bool Database::connect(const std::string& connectionString) {
  SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
  SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
  SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

  ret = SQLDriverConnect(
    hDbc,
    NULL,
    (SQLCHAR*)connectionString.c_str(),
    SQL_NTS,
    NULL,
    0,
    NULL,
    SQL_DRIVER_COMPLETE
  );

  if (SQL_SUCCEEDED(ret)) {
    // std::cout << "Connected successfully!\n";
    return true;
  }
  else {
    std::cout << "Connection failed!\n";
    return false;
  }
}

// Disconnect
void Database::disconnect() {
  if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
  if (hDbc) SQLDisconnect(hDbc);
  if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
  if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

// Execute any query (INSERT, UPDATE, DELETE, etc.)
bool Database::executeNonQuery(const std::string& query) {
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
  ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);
  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
  return SQL_SUCCEEDED(ret);
}

// Execute SELECT query and print results (for testing/debug)
bool Database::executeQuery(const std::string& query) {
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
  ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

  if (!SQL_SUCCEEDED(ret)) {
    std::cout << "Query failed!\n";
    return false;
  }

  SQLSMALLINT columns;
  SQLNumResultCols(hStmt, &columns);

  while (SQLFetch(hStmt) == SQL_SUCCESS) {
    for (SQLUSMALLINT i = 1; i <= columns; i++) {
      char buf[256];
      SQLGetData(hStmt, i, SQL_C_CHAR, buf, sizeof(buf), NULL);
      std::cout << buf << " | ";
    }
    std::cout << std::endl;
  }

  SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
  return true;
}

// Execute SELECT query and return statement handle for custom fetch
SQLHSTMT Database::executeQueryHandle(const std::string& query) {
  SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
  ret = SQLExecDirect(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

  if (!SQL_SUCCEEDED(ret)) {
    std::cout << "Query failed!\n";
    return NULL;
  }

  return hStmt;
}