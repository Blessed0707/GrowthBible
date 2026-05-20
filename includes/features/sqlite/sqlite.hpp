#pragma once
#include <string>
#include "sqlite3.h"


class SqliteDb {
public:
    SqliteDb(const std::string& dbPath); 
    ~SqliteDb();

    sqlite3* getNativeHandle() const { return mpDB; }
    bool isOpen() const { return mIsOpen; }

private:
    sqlite3* mpDB;
    bool mIsOpen;
};