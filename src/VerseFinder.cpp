#include "VerseFinder.hpp"


string VerseFinder::check2john(const vector<string>& keyWords)
{
 
    std::fstream file("text/2John.txt");

    if(file.is_open())
    {
        
    }
    else
    {
        std::cout<< "Error opening file!" << std::endl;
    }
    
}