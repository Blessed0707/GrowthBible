#include "WordSearch.hpp"

WordSearch::WordSearch(SqliteDb& db) : VerseFinder(db) {}  // ADD THIS

bool WordSearch::searchWord(const std::string& word)
{
<<<<<<< HEAD
    BookIdMap map; //create a map

    // Verify database connection state before attempting execution
=======
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6
    if(!mDbRef.isOpen())
    {
        std::cerr << "[WordSearch] Error: Database connection is inactive." << std::endl;
        return false;
    }

<<<<<<< HEAD
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

=======
    // % means "anything before or after the word"
    std::string sql = "SELECT Book, Chapter, Versecount, verse FROM bible WHERE verse LIKE ?;";
    sqlite3_stmt* stmt = nullptr;

    int prepareResult = sqlite3_prepare_v2(mDbRef.getNativeHandle(), sql.c_str(), -1, &stmt, nullptr);
    if (prepareResult != SQLITE_OK) {
        std::cerr << "[WordSearch] SQL Error: Failed to compile statement." << std::endl;
        return false;
    }

    // Wrap word in % wildcards so it matches anywhere in the verse
    std::string searchTerm = "%" + word + "%";
    sqlite3_bind_text(stmt, 1, searchTerm.c_str(), -1, SQLITE_STATIC);

    std::cout << "\n========================================" << std::endl;
    std::cout << "Results for: \"" << word << "\"" << std::endl;
    std::cout << "========================================" << std::endl;

    bool anyResults = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int bookId    = sqlite3_column_int(stmt, 0);
        int chapter   = sqlite3_column_int(stmt, 1);
        int verseNum  = sqlite3_column_int(stmt, 2);
        const unsigned char* rawText = sqlite3_column_text(stmt, 3);

        auto bookNameOpt = getBookName(bookId);
        std::string bookName = bookNameOpt.has_value() ? bookNameOpt.value() : "Unknown";
        std::cout << "========================================\n" << std::endl;
        std::string verseText = reinterpret_cast<const char*>(rawText);
        std::cout << bookName << " " << chapter << ":" << verseNum << " - " 
          << highlightWord(verseText, word) << std::endl;
        anyResults = true;
    }

    if (!anyResults) {
        std::cout << "No verses found containing \"" << word << "\"." << std::endl;
    }

    std::cout << "========================================\n" << std::endl;
    sqlite3_finalize(stmt);
    return anyResults;
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6
}

std::string WordSearch::highlightWord(const std::string& verse, const std::string& word) {
    std::string result = verse;
    std::string redColor   = "\033[1;31m"; // bold red
    std::string resetColor = "\033[0m";    // reset

    size_t pos = 0;
    while ((pos = result.find(word, pos)) != std::string::npos) {
        result.insert(pos, redColor);
        result.insert(pos + redColor.length() + word.length(), resetColor);
        pos += redColor.length() + word.length() + resetColor.length();
    }
    return result;
}

void WordSearch::getSearchWord()
{
    std::string word;
    std::cin.ignore(); // ADD THIS
    std::cout << "Enter a word to search for in the Bible: ";
    std::getline(std::cin, word);
    searchWord(word);
}