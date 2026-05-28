
#include "VerseFinder.hpp"
#include <iostream>
#include "bookmap.hpp"




VerseFinder::VerseFinder(SqliteDb& db) : mDbRef(db) {}

<<<<<<< HEAD
bool VerseFinder::fetchVerse(const std::string& bookName, int chapter, int verseNum) {

    BookIdMap map; //create a map
=======
bool VerseFinder::fetchVerse(const std::string& bookName, int chapter, int verseNum, bool silent) {
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6

    // Verify database connection state before attempting execution
    if (!mDbRef.isOpen()) {
        std::cerr << "[VerseFinder] Error: Database connection is inactive." << std::endl;
        return false;
    }

<<<<<<< HEAD
    auto bookIdOpt = map.getBookID(bookName);
    if(!bookIdOpt.has_value())
    {
        std::cerr << "[VerseFinder] Error: Book '" << bookName<< "' not found"<<std::endl;
=======
        // Resolve string title to internal integer tracking ID
    auto bookIdOpt = getBookID(bookName);  // capture the optional
    if (!bookIdOpt.has_value()) {
        std::cerr << "[VerseFinder] Error: Book '" << bookName 
                << "' not supported by lookup mapping." << std::endl;
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6
        return false;
    }
    int bookId = bookIdOpt.value();

    //sql data base query step 1: Preparation
    //this involves creating an sql striing with parameter placeholders ('?') and compiling 
    //it into a statement handle that the sqlite engine can execute   
    // Parameterized SQL string targeting schema column identifiers
    std::string sql = "SELECT verse FROM bible WHERE Book = ? AND Chapter = ? AND Versecount = ?;";
    sqlite3_stmt* stmt = nullptr;

    // Compile textual SQL declaration into byte-code execution plan
    int prepareResult = sqlite3_prepare_v2(mDbRef.getNativeHandle(), sql.c_str(), -1, &stmt, nullptr);
    if (prepareResult != SQLITE_OK) {
        std::cerr << "[VerseFinder] SQL Error: Failed to compile statement handle." << std::endl;
        return false;
    }

    //query execution step 2: Parameter Binding
    //this involves inputting variables into the prepared statements placeholders
    // Bind execution parameters to 1-indexed placeholder positions
    sqlite3_bind_int(stmt, 1, bookId);
    sqlite3_bind_int(stmt, 2, chapter);
    sqlite3_bind_int(stmt, 3, verseNum);

    //query step 3: step execution
    //this involves executing the the prepared and binded statements command.
    //once a row of data is found, the corrosponding verse is extracted and outputted
    //the "SELECT verse" part shows only one column (verse) of data being queried
    //so only column index 0 is used to extract the data.

    //extracted and printed to the console
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

        // After printing the verse, ask about full chapter
        if(silent)
        {
            std::cout << "Read full chapter? (y/n): ";
            char choice;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                fetchChapter(bookName, chapter);
            }
        }
    } 
    else {
        if (!silent)
        {
            std::cout << "[VerseFinder] Target record address out of bounds (" 
                << bookName << " " << chapter << ":" << verseNum << ")." << std::endl;
        }
        return false;
    }

    //query step 4: finalization
    //Removes the under the hood pointers to the database and compiled statement and frees memory.
    // Deallocate instruction memory allocated during statement preparation
    sqlite3_finalize(stmt);
    return true;
<<<<<<< HEAD
=======
}

bool VerseFinder::fetchChapter(const std::string& bookName, int chapter) {
    if (!mDbRef.isOpen()) {
        std::cerr << "[ChapterFinder] Error: Database connection is inactive." << std::endl;
        return false;
    }

    auto bookIdOpt = getBookID(bookName);
    if (!bookIdOpt.has_value()) {
        std::cerr << "[ChapterFinder] Error: Book '" << bookName 
                << "' not supported by lookup mapping." << std::endl;
        return false;
    }
    int bookId = bookIdOpt.value();

    std::string sql = "SELECT Versecount, verse FROM bible WHERE Book = ? AND Chapter = ? ORDER BY Versecount;";
    sqlite3_stmt* stmt = nullptr;

    int prepareResult = sqlite3_prepare_v2(mDbRef.getNativeHandle(), sql.c_str(), -1, &stmt, nullptr);
    if (prepareResult != SQLITE_OK) {
        std::cerr << "[ChapterFinder] SQL Error: Failed to compile statement handle." << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, bookId);
    sqlite3_bind_int(stmt, 2, chapter);

    std::cout << "\n========================================" << std::endl;
    std::cout << bookName << " - Chapter " << chapter << std::endl;
    std::cout << "========================================" << std::endl;

    bool anyResults = false;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int verseNum = sqlite3_column_int(stmt, 0);
        const unsigned char* rawText = sqlite3_column_text(stmt, 1);
        std::cout << verseNum << ". " << rawText << std::endl;
        anyResults = true;
    }

    if (!anyResults) {
        std::cout << "[ChapterFinder] Error: " << bookName << " chapter " 
                  << chapter << " does not exist." << std::endl;
    }

    std::cout << "========================================\n" << std::endl;
    sqlite3_finalize(stmt);
    return anyResults;
}


void VerseFinder::getVerse()
{
    std::string book;
    int chapter = 0;
    int verse = 0;
    std::cout << "Book: ";
    std::cin >> book;
    book[0] = toupper(book[0]); // capitalize first letter
    std::cout << "Chapter: ";
    std::cin >> chapter;
    std::cout << "Verse: ";
    std::cin >> verse;
    fetchVerse(book, chapter, verse);
>>>>>>> d6f8b7530a3e1420d7316962cf45f95eeaf01bc6
}