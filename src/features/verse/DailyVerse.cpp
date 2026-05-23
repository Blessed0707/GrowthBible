#include "DailyVerse.hpp"
#include "VerseFinder.hpp"
#include <iostream>
#include <unordered_map>
#include <random>

DailyVerse::DailyVerse(SqliteDb& db) : VerseFinder(db) {}

void DailyVerse::RandomVerse()
{
    std::cout << "\n========================================" << std::endl;
    std::cout << "VERSE OF THE DAY";
    // Verify database connection state before attempting execution
    if (!mDbRef.isOpen()) {
        std::cerr << "[VerseFinder] Error: Database connection is inactive." << std::endl;
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    int found = 0;
    while(!found)
    {
        std::uniform_int_distribution<int> bookDist(0, 65);    // random book
        std::uniform_int_distribution<int> chapterDist(1, 150); // random chapter
        std::uniform_int_distribution<int> verseDist(1, 176);   // random verse

        int randomBook = bookDist(gen);
        auto bookNameOpt = getBookName(randomBook);
        if (!bookNameOpt.has_value()) continue; // skip if not found
        std::string randomBookName = bookNameOpt.value();


        int randomChapter = chapterDist(gen);
        int randomVerse   = verseDist(gen);
        found = fetchVerse(randomBookName, randomChapter, randomVerse, true);
    }
}