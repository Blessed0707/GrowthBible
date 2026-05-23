#pragma once
#include <string>
#include <iostream>
#include "sqlite.hpp"
#include "VerseFinder.hpp"

class WordSearch : public VerseFinder {

    public:
        WordSearch(SqliteDb& db);
        bool searchWord(const std::string& word);

    private:

};
