#include <chrono>
#include "DailyVerse.hpp"
#include "VerseFinder.hpp"
#include <iostream>
#include <unordered_map>
#include <random>

DailyVerse::DailyVerse(SqliteDb& db) : VerseFinder(db) {}

void DailyVerse::RandomVerse()
{
    if (!mDbRef.isOpen()) {
        std::cerr << "[VerseFinder] Error: Database connection is inactive." << std::endl;
        return;
    }

    // Use current date as seed so same verse shows all day
    auto now = std::chrono::system_clock::now();
    auto seed = std::chrono::duration_cast<std::chrono::hours>(
    now.time_since_epoch()).count() / 24;

    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> bookDist(0, 65);
    std::uniform_int_distribution<int> chapterDist(1, 150);
    std::uniform_int_distribution<int> verseDist(1, 176);

    bool found = false;
    while (!found)
    {
        int randomBook = bookDist(gen);
        auto bookNameOpt = getBookName(randomBook);
        if (!bookNameOpt.has_value()) continue;
        std::string randomBookName = bookNameOpt.value();

        int randomChapter = chapterDist(gen);
        int randomVerse   = verseDist(gen);
        found = fetchVerse(randomBookName, randomChapter, randomVerse, true);
    }
}