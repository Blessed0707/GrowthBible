#include "WordSearch.hpp"

WordSearch::WordSearch(SqliteDb& db) : VerseFinder(db) {}

bool WordSearch::searchWord(const std::string& word)
{
    if(!mDbRef.isOpen())
    {
        std::cerr << "[WordSearch] Error: Database connection is inactive." << std::endl;
        return false;
    }

    // getBookID() and getBookName() are available here via inheritance
    // continue implementation...
    return true;
}