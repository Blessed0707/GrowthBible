#include <iostream>

#include "VerseFinder.hpp"

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

    // Dispatch verification test routines
    finder.fetchVerse("Genesis", 1, 1);
    finder.fetchVerse("Genesis", 1, 3);

    return 0;
}