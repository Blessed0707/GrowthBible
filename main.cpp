#include <iostream>
#include "sqlite.hpp"
#include "VerseFinder.hpp"
#include "ChapterFinder.hpp"
#include "DailyVerse.hpp"
#include "WordSearch.hpp"

int main() {
    // Execution path resolution utilizing local working directory outputs
    std::string dbPath = "holybible.db";

    // Initialize database handle resources
    SqliteDb db(dbPath);

    // Instantiate execution abstraction layer
    VerseFinder finder(db);

    //Instantiate daily verse handler
    DailyVerse dailyVerse(db);

    //Instantiate chapter finder handler
    ChapterFinder chapterFinder(db);

    //
    WordSearch wordSearch(db);


    // Dispatch verification test routines
    finder.getVerse();
    dailyVerse.RandomVerse();
    chapterFinder.getChapter();
    wordSearch.getSearchWord();
    return 0;
}

