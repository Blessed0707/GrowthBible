#pragma once
#include <unordered_map>
#include <string>
#include <optional>

class BookIdMap
{
    public:
    /**
     * @brief Resolves a text string to its corresponding database primary key.
     * @param bookName The string name of the book (e.g., "Genesis").
     * @return std::optional<int> The integer ID if found; std::nullopt if unknown.
     */
    std::optional<int> getBookID(const std::string bookName);

    std::optional<std::string> getBookName(const int bookId);
 
    private:
    // The master cross-reference translation table
    static const std::unordered_map<std::string, int> mBookMap;


};



