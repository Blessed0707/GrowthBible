#include "sqlite.hpp"
#include <iostream>

// ============================================================================
// CONSTRUCTOR: This runs the exact moment a SqliteDatabase object is created
// ============================================================================
SqliteDb::SqliteDb(const std::string& dbPath) : mpDB(nullptr), mIsOpen(false) {
    
    // sqlite3_open takes two arguments:
    // 1. The file path string (converted to a C-style string using .c_str())
    // 2. The address of our db pointer (&db) where it will store the open connection
    int connectionResult = sqlite3_open(dbPath.c_str(), &mpDB);

    // SQLITE_OK is a built-in constant equal to 0, meaning the file opened flawlessly
    if (connectionResult == SQLITE_OK) {
        mIsOpen = true;
        std::cout << "[DATABASE] Success: Connected to " << dbPath << std::endl;
    } else {
        // If it fails (e.g., file missing), sqlite3_errmsg prints the exact reason why
        std::cerr << "[DATABASE] Critical Error: Could not open database!" << std::endl;
        std::cerr << "[DATABASE] SQLite Reason: " << sqlite3_errmsg(mpDB) << std::endl;
        
        // Safety: If it failed but allocated memory anyway, clean it up immediately
        if (mpDB != nullptr) {
            sqlite3_close(mpDB);
            mpDB = nullptr;
        }
    }
}

// ============================================================================
// DESTRUCTOR: This runs automatically when the object dies or the program exits
// ============================================================================
SqliteDb::~SqliteDb() {
    // We only need to disconnect if we actually have an active connection pointer
    if (mpDB != nullptr) {
        // sqlite3_close hangs up the "phone line" and frees all RAM used by the DB engine
        sqlite3_close(mpDB);
        mpDB = nullptr;
        mIsOpen = false;
        std::cout << "[DATABASE] Connection closed cleanly." << std::endl;
    }
}