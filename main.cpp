#include <iostream>
#include "sqlite.hpp"
#include "VerseFinder.hpp"
#include "ChapterFinder.hpp"
#include "DailyVerse.hpp"

int main() {
    // Execution path resolution utilizing local working directory outputs
    std::string dbPath = "holybible.db";

    // Initialize database handle resources
    SqliteDb db(dbPath);

    // Instantiate execution abstraction layer
    VerseFinder finder(db);

    //Instantiate daily verse handler
    DailyVerse dailyVerse(db);

    //
    ChapterFinder chapterFinder(db);

    // Dispatch verification test routines
    finder.fetchVerse("Genesis", 1, 1);
    finder.fetchVerse("Revelation", 21, 4);
    dailyVerse.RandomVerse();
    chapterFinder.fetchSpecificChapter();
    return 0;
}

