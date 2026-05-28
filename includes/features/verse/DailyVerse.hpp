#include <string>
#include "sqlite.hpp"
#include "VerseFinder.hpp"  // ← must include the parent

class DailyVerse : public VerseFinder  // ← inherits from VerseFinder
{
public:
    /**
     * @brief Constructs a DailyVerse instance bound to an open database wrapper.
     * @param db Reference to an initialized SqliteDb object.
     */
    DailyVerse(SqliteDb& db);

    /**
     * @brief Fetches a random verse from the database.
     */
    void RandomVerse();

private:
    
};
