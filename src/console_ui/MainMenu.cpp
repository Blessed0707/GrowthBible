#include "MainMenu.hpp"

// A helper function to capitalize the start of every word
void makeTitleCase(std::string& str) {
    if (str.empty()) return;
    
    // First, make everything lowercase
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    
    // Capitalize the first character
    str[0] = std::toupper(str[0]);
    
    // Capitalize any character that comes immediately after a space
    for (size_t i = 1; i < str.length(); ++i) {
        if (str[i - 1] == ' ') {
            str[i] = std::toupper(str[i]);
        }
    }
}

bool MainMenu::diplayMenu()
{
    // Main Menu console prompts
    std::cout << "Welcome to the Growth Bible App!" << std::endl;
    std::cout << "Please Select an Option: " << std::endl;
    std::cout << "1) Daily Verse" << std::endl;
    std::cout << "2) Verse Finder" << std::endl;
    std::cout << "3) Word Search" << std::endl;
    std::cout << "0) Exit" << std::endl;

    std::cout << "->"; 
    
    this->getInput();
    if(mUserInput == 0)
    {
        std::cout<<"Goodbye"<<std::endl;
        return true;
    }
    else if(mUserInput == 1)
    {
        return true;
    }
    else if (mUserInput == 2)
    {
        if(!this->verseFinder())
        {
            this->clearTerminal();
        }
    }
}

void MainMenu::getInput()
{
    bool success = false;
    while (!success) 
    {
        int temp = -1;
        std::cin >> temp; 

        // Check if the user typed something that wasn't an integer
        if (std::cin.fail()) 
        {
            std::cin.clear(); // Clear the error flags
            std::cin.ignore(10000, '\n'); // Discard the bad input text up to the newline
            
            std::cout << "\nInvalid input! Please enter a number." << std::endl;
            std::cout << "->";
            continue;
        }

        // Check if the number is out of range
        if (temp < 0 || temp > 3)
        {
            std::cout << "\nPlease enter an appropriate value (1-3)!" << std::endl;
            std::cout << "->";
            continue;
        }
        else
        {
            success = true;
            mUserInput = temp;
        }
    }
}

void MainMenu::clearTerminal() 
{
    // \033[2J clears the entire screen
    // \033[1;1H moves the cursor to row 1, column 1
    std::cout << "\033[2J\033[1;1H";
}

bool MainMenu::verseFinder()
{
    std::string book;
    int chapter;
    int verseNum;
    bool success = false;

   
    this->clearTerminal();
    std::cout<<"[VerseFinder] Enter a BOOK, CHAPTER and VERSE NUMBER: "<<std::endl;

    //loop for valid input for the three requests
    std::cout<<"BOOK: ";
   // Clear any lingering newline characters in the buffer before using getline
    std::cin >> std::ws; 

    while (true) // book loop
    {
        std::getline(std::cin, book); // Use getline to support books like "1 Kings"
        
        if (std::cin.fail() || book.empty())
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Please enter a valid BOOK.\nBOOK: ";
        }
        else
        {
            makeTitleCase(book);
            break;
        }
    }

        std::cout<<"CHAPTER: ";

        while(!success)//chapter loop
    {
        std::cin >> chapter;
        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000,'\n');

            std::cout<<"Please enter a valid CHAPTER."<<std::endl;
            std::cout<<"CHAPTER: ";
        }
        else
        {
            break;
        }
    }

        std::cout<<"VERSE: ";

        while(!success)//verse loop
    {
        std::cin >> verseNum;
        if(std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(10000,'\n');

            std::cout<<"Please enter a valid VERSE."<<std::endl;
            std::cout<<"VERSE: ";
        }
        else
        {
            break;
        }
    }

    //create a db and input it into verse finder
    SqliteDb bibleDb("kjvbible.db");
    VerseFinder find(bibleDb);
    if(!find.fetchVerse(book,chapter,verseNum))
    {
        std::cout<<"[VerseFinder] Failed" << std::endl;
        return false;
    }
    else
    {
        std::cout<<"[VerseFinder] Success" << std::endl;
        return true;
    }

    


}



