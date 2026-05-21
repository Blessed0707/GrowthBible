#include "VerseFinder.hpp"
#include <iostream>
#include <unordered_map>

// Look-up map matching human-readable book titles to unique database primary keys
const std::unordered_map<std::string, int> bookMap = {
    {"Genesis", 0}, {"Exodus", 1}, {"Leviticus", 2}, {"Numbers", 3}, {"Deuteronomy", 4}, {"Joshua", 5}, {"Judges", 6}, {"Ruth", 7}, {"1 Samuel", 8}, {"2 Samuel", 9},
    {"1 Kings", 10}, {"2 Kings", 11}, {"1 Chronicles", 12}, {"2 Chronicles", 13}, {"Ezra", 14},
    {"Nehemiah", 15}, {"Esther", 16}, {"Job", 17}, {"Psalms", 18}, {"Proverbs", 19},
    {"Ecclesiastes", 20}, {"Song of Solomon", 21}, {"Isaiah", 22}, {"Jeremiah", 23}, {"Lamentations", 24},
    {"Ezekiel", 25}, {"Daniel", 26}, {"Hosea", 27}, {"Joel", 28}, {"Amos", 29},
    {"Obadiah", 30}, {"Jonah", 31}, {"Micah", 32}, {"Nahum", 33}, {"Habakkuk", 34},
    {"Zephaniah", 35}, {"Haggai", 36}, {"Zechariah", 37}, {"Malachi", 38},

    // New Testament
    {"Matthew", 39}, {"Mark", 40}, {"Luke", 41}, {"John", 42}, {"Acts", 43},
    {"Romans", 44}, {"1 Corinthians", 45}, {"2 Corinthians", 46}, {"Galatians", 47}, {"Ephesians", 48},
    {"Philippians", 49}, {"Colossians", 50}, {"1 Thessalonians", 51}, {"2 Thessalonians", 52}, {"1 Timothy", 53},
    {"2 Timothy", 54}, {"Titus", 55}, {"Philemon", 56}, {"Hebrews", 57}, {"James", 58},
    {"1 Peter", 59}, {"2 Peter", 60}, {"1 John", 61}, {"2 John", 62}, {"3 John", 63},
    {"Jude", 64}, {"Revelation", 65}
};

VerseFinder::VerseFinder(SqliteDb& db) : mDbRef(db) {}

bool VerseFinder::fetchVerse(const std::string& bookName, int chapter, int verseNum, bool silent) {
    // Verify database connection state before attempting execution
    if (!mDbRef.isOpen()) {
        std::cerr << "[VerseFinder] Error: Database connection is inactive." << std::endl;
        return false;
    }

    // Resolve string title to internal integer tracking ID
    auto it = bookMap.find(bookName);
    if (it == bookMap.end()) {
        std::cerr << "[VerseFinder] Error: Book '" << bookName << "' not supported by lookup mapping." << std::endl;
        return false;
    }
    int bookId = it->second;

    // Parameterized SQL string targeting schema column identifiers
    std::string sql = "SELECT verse FROM bible WHERE Book = ? AND Chapter = ? AND Versecount = ?;";
    sqlite3_stmt* stmt = nullptr;

    // Compile textual SQL declaration into byte-code execution plan
    int prepareResult = sqlite3_prepare_v2(mDbRef.getNativeHandle(), sql.c_str(), -1, &stmt, nullptr);
    if (prepareResult != SQLITE_OK) {
        std::cerr << "[VerseFinder] SQL Error: Failed to compile statement handle." << std::endl;
        return false;
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
        if (!silent)
        {
            std::cout << "[VerseFinder] Target record address out of bounds (" 
                << bookName << " " << chapter << ":" << verseNum << ")." << std::endl;
        }
        return false;
    }

    // Deallocate instruction memory allocated during statement preparation
    sqlite3_finalize(stmt);
    return true;
}