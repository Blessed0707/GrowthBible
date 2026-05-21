#pragma once
#include <string>
#include <iostream>
#include "sqlite.hpp"

class WordSearch {

    public:
        WordSearch(SqliteDb& db);
        bool searchWord(const std::string& word);

    private:
        SqliteDb& mDbRef; ///< Reference to the active database connection wrapper

};
