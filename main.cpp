#include <iostream>
#include "sqlite.hpp"
#include "VerseFinder.hpp"
#include "WordSearch.hpp"
#include "MainMenu.hpp"

using std::cout;
using std::endl;



int main() {
    // // Execution path resolution utilizing local working directory outputs
    // std::string dbPath = "kjvbible.db";

    // //Initialize database handle resources
    // SqliteDb db(dbPath);

    // // Instantiate execution abstraction layer
    // VerseFinder finder(db);

    // // Dispatch verification test routines
    // finder.fetchVerse("Genesis", 1, 1);
    // finder.fetchVerse("Exodus", 20, 3);
    // finder.fetchVerse("Psalms", 23, 1);
    // finder.fetchVerse("Psalms", 91, 1);
    // finder.fetchVerse("Proverbs", 3, 5);
    // finder.fetchVerse("Isaiah", 40, 31);
    // finder.fetchVerse("Jeremiah", 29, 11);
    // finder.fetchVerse("Daniel", 3, 17);
    // finder.fetchVerse("Jonah", 1, 1);
    // finder.fetchVerse("Malachi", 3, 10);

    // // New Testament
    // finder.fetchVerse("Matthew", 5, 3);
    // finder.fetchVerse("Matthew", 6, 9);
    // finder.fetchVerse("John", 3, 16);
    // finder.fetchVerse("John", 11, 35);
    // finder.fetchVerse("Romans", 8, 28);
    // finder.fetchVerse("Romans", 12, 2);
    // finder.fetchVerse("Philippians", 4, 13);
    // finder.fetchVerse("Ephesians", 6, 10);
    // finder.fetchVerse("Hebrews", 11, 1);
    // finder.fetchVerse("Revelation", 21, 4);

    
    MainMenu ui;
    while(!ui.diplayMenu())
    {

    }
    return 0;
}

