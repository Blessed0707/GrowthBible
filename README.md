## Growth Bible

Growth Bible is a C++ desktop application for reading, exploring, and searching scripture. It is backed by a local SQLite database and ships with both a command-line interface and a graphical desktop interface built with wxWidgets.

## Features

- **Verse Lookup** — Look up any verse by book, chapter, and verse number.
- **Chapter Reader** — Read an entire chapter at once, formatted verse by verse.
- **Verse of the Day** — Get a randomly selected verse, perfect for daily devotionals.
- **Word Search** — Search the full text of scripture for any word and see every verse it appears in, with the matched word highlighted in the results.
- **GUI and CLI** — Use the polished desktop app for everyday use, or the lightweight command-line tool for quick lookups and testing.

## Tech Stack

- **Language:** C++17
- **Database:** SQLite (via the SQLite amalgamation, embedded directly in the project)
- **GUI Framework:** wxWidgets
- **Build System:** CMake

## Project Structure

```
GrowthBible/
├── main.cpp                  # CLI entry point
├── main_gui.cpp               # GUI entry point (wxWidgets)
├── CMakeLists.txt
├── sqlite_engine/             # SQLite amalgamation (sqlite3.c / sqlite3.h)
├── includes/
│   └── features/
│       ├── sqlite/            # Database wrapper
│       ├── navigation/        # Book name <-> ID mapping
│       ├── verse/             # Verse lookup & word search
│       └── chapter/           # Chapter lookup
└── src/
    └── features/               # Implementation files matching the includes structure
```

## Architecture

The application is built around a small inheritance chain so shared functionality (like resolving a book name to its internal ID) only needs to be written once:

```
BookIdMap
   └── VerseFinder   (verse lookup, word search via WordSearch)
            └── ChapterFinder   (full chapter lookup)
```

- **`BookIdMap`** resolves human-readable book names (e.g. "Genesis") to the internal integer IDs used in the database, and back again.
- **`VerseFinder`** queries the database for a specific verse.
- **`WordSearch`** (inherits `VerseFinder`) searches across all verses for a given word.
- **`ChapterFinder`** (inherits `VerseFinder`) retrieves every verse in a given chapter.
- **`DailyVerse`** (inherits `VerseFinder`) selects a random verse for the "Verse of the Day" feature.
- **`SqliteDb`** wraps the raw SQLite3 C API in a small, safe-to-use class.

## Getting Started

### Prerequisites

- A C++17-compatible compiler (MinGW recommended on Windows)
- [CMake](https://cmake.org/download/) (3.15+)
- [wxWidgets](https://www.wxwidgets.org/downloads/) (only required for the GUI build) — built and located at `C:/wxWidgets`

### Building

```bash
git clone https://github.com/Blessed0707/GrowthBible.git
cd GrowthBible
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

This produces three targets:

| Target | Description |
|---|---|
| `MyExecutable` | Command-line version of the app |
| `GrowthBibleGUI` | Desktop GUI version (requires wxWidgets) |
| `TestExecutable` | Test suite |

### Running

**GUI:**
```bash
./GrowthBibleGUI.exe
```

**CLI:**
```bash
./MyExecutable.exe
```

### Installer

A Windows installer is available under [Releases](../../releases) — download `GrowthBibleSetup.exe` to install Growth Bible without building from source.

## Database

Growth Bible reads from a local `holybible.db` SQLite file containing one table:

```sql
CREATE TABLE bible (
    Book        INT,   -- internal book ID (0 = Genesis ... 65 = Revelation)
    Chapter     INT,
    Versecount  INT,   -- verse number within the chapter
    verse       VARCHAR(528)
);
```

## Roadmap

- [ ] Inline highlighting refinements for word search results
- [ ] Bookmarking / favorites
- [ ] Reading plans
- [ ] Cross-platform builds (macOS / Linux)

## Authors

- Temi Maku
- Joshua Riley

## License

This project is for personal/educational use.
