#include "VerseFinder.hpp"
#include <iostream>
#include "BookMap.hpp"



VerseFinder::VerseFinder(SqliteDb& db) : mDbRef(db) {}

bool VerseFinder::fetchVerse(const std::string& bookName, int chapter, int verseNum) {

    BookIdMap map; //create a map

    // Verify database connection state before attempting execution
    if (!mDbRef.isOpen()) {
        std::cerr << "[VerseFinder] Error: Database connection is inactive." << std::endl;
        return false;
    }

    auto bookIdOpt = map.getBookID(bookName);
    if(!bookIdOpt.has_value())
    {
        std::cerr << "[VerseFinder] Error: Book '" << bookName<< "' not found"<<std::endl;
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
    } else {
        std::cout << "[VerseFinder] Target record address out of bounds (" 
                  << bookName << " " << chapter << ":" << verseNum << ")." << std::endl;
    }

    //query step 4: finalization
    //Removes the under the hood pointers to the database and compiled statement and frees memory.
    // Deallocate instruction memory allocated during statement preparation
    sqlite3_finalize(stmt);
    return true;
}