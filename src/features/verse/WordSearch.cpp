#include "WordSearch.hpp"

WordSearch::WordSearch(SqliteDb& db) : VerseFinder(db) {}  // ADD THIS

bool WordSearch::searchWord(const std::string& word)
{
    if(!mDbRef.isOpen())
    {
        std::cerr << "[WordSearch] Error: Database connection is inactive." << std::endl;
        return false;
    }

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