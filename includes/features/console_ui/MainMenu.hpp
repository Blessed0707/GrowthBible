#pragma once
#include <iostream>
#include <string>
#include "VerseFinder.hpp"
#include "sqlite.hpp"
#include <algorithm> // Required for std::transform
#include <cctype>    // Required for std::tolower


class MainMenu {

    public:
    MainMenu(){mUserInput = -1;}
    bool diplayMenu();
    void getInput();
    void clearTerminal(); 
    bool verseFinder();
   


    private:
    int mUserInput;


};

