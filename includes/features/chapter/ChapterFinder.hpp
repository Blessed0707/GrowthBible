#pragma once
#include <string>
#include "sqlite.hpp"
#include "VerseFinder.hpp"
#include <iostream>



/**
 * @class ChapterFinder
 * @brief Handles querying and extracting chapter text from the database.
 */

 class ChapterFinder : public VerseFinder
 {
    public:
        ChapterFinder(SqliteDb& db);
        void getChapter();
    private:

 };
