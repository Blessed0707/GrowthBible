#pragma once
#include <string>
#include "sqlite.hpp"
#include "bookmap.hpp"



/**
 * @class VerseFinder
 * @brief Handles querying and extracting scripture text from the database.
 */


class VerseFinder : public BookIdMap {
protected:
    SqliteDb& mDbRef; ///< Reference to the active database connection wrapper
    bool fetchVerse(const std::string& bookName, int chapter, int verseNum,bool silent = false);
    bool fetchChapter(const std::string& bookName, int chapter);
public:
    /**
     * @brief Constructs a VerseFinder instance bound to an open database wrapper.
     * @param db Reference to an initialized SqliteDb object.
     */
    VerseFinder(SqliteDb& db);

    /**
     * @brief Executes a compiled SQL query to retrieve and display a specific scripture verse.
     * @param bookName The localized string name of the book (e.g., "Genesis").
     * @param chapter The target chapter index.
     * @param verseNum The target verse index.
     */
    void getVerse();
};

