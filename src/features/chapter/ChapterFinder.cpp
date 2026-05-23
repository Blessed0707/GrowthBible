#include "ChapterFinder.hpp"
#include "bookmap.hpp"

ChapterFinder::ChapterFinder(SqliteDb& db) : VerseFinder(db) {}
void ChapterFinder::fetchSpecificChapter()
{
    std::string book;
    int chapter = 0;
    std::cout << "Book: ";
    std::cin >> book;
    book[0] = toupper(book[0]); // capitalize first letter
    std::cout << "Chapter: ";
    std::cin >> chapter;
    fetchChapter(book, chapter);
}

