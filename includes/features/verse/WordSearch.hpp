#pragma once
#include <string>
#include <iostream>
#include "sqlite.hpp"
#include "VerseFinder.hpp"

class WordSearch : public VerseFinder {

    public:
        WordSearch(SqliteDb& db);
        void getSearchWord();

    private:
        std::string highlightWord(const std::string& verse, const std::string& word);
        bool searchWord(const std::string& word);
};
