#include "WordSearch.hpp"

WordSearch::WordSearch(SqliteDb& db) : VerseFinder(db) {}  // ADD THIS

bool WordSearch::searchWord(const std::string& word)
{
    if(!mDbRef.isOpen())
    {
        std::cerr << "[WordSearch] Error: Database connection is inactive." << std::endl;
        return false;
    }

    std::string searchTerm1 = word + "% %";
    std::string searchTerm2 = "% " + word + "% %";
    std::string searchTerm3 = "% " + word + "%";

    std::string sql = "SELECT Book, Chapter, Versecount, verse FROM bible WHERE verse LIKE ? OR verse LIKE ? OR verse LIKE ?;";
    sqlite3_stmt* stmt = nullptr;

    int prepareResult = sqlite3_prepare_v2(mDbRef.getNativeHandle(), sql.c_str(), -1, &stmt, nullptr);
    if (prepareResult != SQLITE_OK) {
        std::cerr << "[WordSearch] SQL Error: Failed to compile statement." << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, searchTerm1.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, searchTerm2.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, searchTerm3.c_str(), -1, SQLITE_STATIC);

    std::cout << "\n========================================" << std::endl;
    std::cout << "Results for: \"" << word << "\"" << std::endl;
    std::cout << "========================================" << std::endl;

    bool anyResults = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int bookId   = sqlite3_column_int(stmt, 0);
        int chapter  = sqlite3_column_int(stmt, 1);
        int verseNum = sqlite3_column_int(stmt, 2);
        const unsigned char* rawText = sqlite3_column_text(stmt, 3);

        auto bookNameOpt = getBookName(bookId);
        std::string bookName = bookNameOpt.has_value() ? bookNameOpt.value() : "Unknown";

        std::string verseText = reinterpret_cast<const char*>(rawText);
        std::cout << bookName << " " << chapter << ":" << verseNum << " - "
                  << verseText << std::endl;
        anyResults = true;
    }

    if (!anyResults) {
        std::cout << "No verses found containing \"" << word << "\"." << std::endl;
    }

    std::cout << "========================================\n" << std::endl;
    sqlite3_finalize(stmt);
    return anyResults;
}

void WordSearch::getSearchWord()
{
    std::string word;
    std::cin.ignore(); // ADD THIS
    std::cout << "Enter a word to search for in the Bible: ";
    std::getline(std::cin, word);
    searchWord(word);
}