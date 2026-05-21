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

    // Parameterized SQL string targeting schema column identifiers
    
    // The '%' || ? || '%' syntax concatenates the wildcard characters with the search term, enabling substring matching
    std::string sql = "SELECT Book, Chapter, Versecount, verse FROM bible WHERE verse LIKE '%' || ? || '%';";
    sqlite3_stmt* stmt = nullptr;

    //prepares the sql statement for parsing and execution
    int prepareResult = sqlite3_prepare_v2(mDbRef.getNativeHandle(), sql.c_str(), -1, &stmt, nullptr);
    if(prepareResult != SQLITE_OK)
    {
        std::cerr << "[WordSearch] SQL ERROR: Failed to compile statement handle." << std::endl;
        return false;
    }
    //binds the user variable word to the first placeholder in the statement (the '?')
    sqlite3_bind_text(stmt,1,word.c_str(),-1,nullptr);

    int matchCount = 0;
    //runs the statement and checks if it returns any rows matching the search criteria
    //stops when there are no more rows to process (i.e., sqlite3_step returns something other than SQLITE_ROW)
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        int bookId = sqlite3_column_int(stmt,0);
        int chapter = sqlite3_column_int(stmt,1);
        int verseCount = sqlite3_column_int(stmt,2);
        const unsigned char* verseText = sqlite3_column_text(stmt,3);

        auto bookNameOpt = map.getBookName(bookId);
        std::string bookName = bookNameOpt.has_value() ? bookNameOpt.value() : "Unknown Book";
        
        std::cout << "\n========================================" << std::endl;
        std::cout << bookName << " " << chapter << ":" << verseCount << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << verseText << std::endl;
        std::cout << "========================================\n" << std::endl;
        matchCount++;
    }
    std::cout << "[WordSearch] Total matches found for '" << word << "': " << matchCount << std::endl;
    // Deallocate instruction memory allocated during statement preparation
    sqlite3_finalize(stmt);
    return true;

}

