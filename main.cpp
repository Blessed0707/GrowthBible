#include <iostream>

#include "VerseFinder.hpp"
#include "DailyVerse.hpp"

using std::cout;
using std::endl;
#include "sqlite.hpp"
#include "VerseFinder.hpp"
#include <iostream>

int main() {
    // Execution path resolution utilizing local working directory outputs
    std::string dbPath = "holybible.db";

    // Initialize database handle resources
    SqliteDb db(dbPath);

    // Instantiate execution abstraction layer
    VerseFinder finder(db);

    //Instantiate daily verse handler
    DailyVerse dailyVerse(db);

    // Dispatch verification test routines
    finder.fetchVerse("Genesis", 1, 1);
    finder.fetchVerse("Revelation", 21, 4);
    dailyVerse.RandomVerse();
    return 0;
}

