#include "WordSearch.hpp"
#include "BookMap.hpp"

WordSearch::WordSearch(SqliteDb& db) : mDbRef(db) {} //initialze the database reference in the constructor


bool WordSearch::searchWord(const std::string& word)
{
    BookIdMap map; //create a map
    // Verify database connection state before attempting execution
    if(!mDbRef.isOpen())
    {
        std::cerr << "[WordSearch] Error: Database connection is inactive." << std::endl;
        return false;
    }
}

