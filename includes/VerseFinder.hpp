#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using std::string;
using std::vector;

class VerseFinder
{
    public:
    VerseFinder(string verse , vector <string> keyWords): keyVerse(verse), KeyWords(keyWords)
    {

    }
   
    string check2john(const vector<string>& keyWords);

    private:
    string keyVerse;
    vector <string> KeyWords;

};