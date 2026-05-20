#include "VerseFinder.hpp"
#include <iostream>
#include <unordered_map>

// Look-up map matching human-readable book titles to unique database primary keys
const std::unordered_map<std::string, int> bookMap = {
    {"Genesis", 0}, {"Exodus", 1}, {"Leviticus", 2}, {"Numbers", 3}, {"Deuteronomy", 4}
};

VerseFinder::VerseFinder(SqliteDb& db) : mDbRef(db) {}

void VerseFinder::fetchVerse(const std::string& bookName, int chapter, int verseNum) {
    // Verify database connection state before attempting execution
    if (!mDbRef.isOpen()) {
        std::cerr << "[VerseFinder] Error: Database connection is inactive." << std::endl;
        return;
    }

    // Resolve string title to internal integer tracking ID
    auto it = bookMap.find(bookName);
    if (it == bookMap.end()) {
        std::cerr << "[VerseFinder] Error: Book '" << bookName << "' not supported by lookup mapping." << std::endl;
        return;
    }
    int bookId = it->second;

    // Parameterized SQL string targeting schema column identifiers
    std::string sql = "SELECT verse FROM bible WHERE Book = ? AND Chapter = ? AND Versecount = ?;";
    sqlite3_stmt* stmt = nullptr;

    // Compile textual SQL declaration into byte-code execution plan
    int prepareResult = sqlite3_prepare_v2(mDbRef.getNativeHandle(), sql.c_str(), -1, &stmt, nullptr);
    if (prepareResult != SQLITE_OK) {
        std::cerr << "[VerseFinder] SQL Error: Failed to compile statement handle." << std::endl;
        return;
    }

    // Bind execution parameters to 1-indexed placeholder positions
    sqlite3_bind_int(stmt, 1, bookId);
    sqlite3_bind_int(stmt, 2, chapter);
    sqlite3_bind_int(stmt, 3, verseNum);

    // Evaluate statement virtual machine context
    int stepResult = sqlite3_step(stmt);
    if (stepResult == SQLITE_ROW) {
        // Extract UTF-8 text pointer from column result sequence index 0
        const unsigned char* rawText = sqlite3_column_text(stmt, 0);
        
        std::cout << "\n========================================" << std::endl;
        std::cout << bookName << " " << chapter << ":" << verseNum << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << rawText << std::endl;
        std::cout << "========================================\n" << std::endl;
    } else {
        std::cout << "[VerseFinder] Target record address out of bounds (" 
                  << bookName << " " << chapter << ":" << verseNum << ")." << std::endl;
    }

    // Deallocate instruction memory allocated during statement preparation
    sqlite3_finalize(stmt);
}