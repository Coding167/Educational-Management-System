#ifndef DB_H_INCLUDED
#define DB_H_INCLUDED

#include <windows.h>
#include <sqlext.h>
#include <iostream>
#include <string>

class Database {
private:
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLHSTMT hStmt;
    SQLRETURN ret;

public:
    Database() : hEnv(NULL), hDbc(NULL), hStmt(NULL), ret(0) {}

    // Connect to DB
    bool connect(const std::string& connectionString);

    // Disconnect
    void disconnect();

    // Execute any query (INSERT, UPDATE, DELETE, etc.)
    bool executeNonQuery(const std::string& query);

    // Execute SELECT query and print results (for testing/debug)
    bool executeQuery(const std::string& query);

    // Execute SELECT query and return statement handle for custom fetch
    SQLHSTMT executeQueryHandle(const std::string& query);
};

#endif // DB_H_INCLUDED