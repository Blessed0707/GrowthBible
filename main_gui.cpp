#include <wx/wx.h>
#include <wx/filefn.h>
#include <sstream>
#include "VerseFinder.hpp"
#include "DailyVerse.hpp"
#include "WordSearch.hpp"
#include "ChapterFinder.hpp"
#include "sqlite.hpp"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

class MainWindow : public wxFrame {
public:
    MainWindow(const wxString& title);

private:
    SqliteDb* mDb;
    VerseFinder* mFinder;
    DailyVerse* mDailyVerse;
    WordSearch* mWordSearch;
    ChapterFinder* mChapterFinder;

    wxTextCtrl* mBookInput;
    wxTextCtrl* mChapterInput;
    wxTextCtrl* mVerseInput;
    wxTextCtrl* mOutput;

    wxTextCtrl* mChapterBookInput;
    wxTextCtrl* mChapterNumInput;

    void OnFetchVerse(wxCommandEvent& event);
    void OnRandomVerse(wxCommandEvent& event);
    void OnSearchWord(wxCommandEvent& event);
    void OnFetchChapter(wxCommandEvent& event);
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    if (!wxFileExists("holybible.db")) {
        wxMessageBox("Database not found!", "Error", wxOK | wxICON_ERROR);
        return false;
    }
    MainWindow* window = new MainWindow("Growth Bible");
    window->Show(true);
    return true;
}

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600))
{
    mDb = new SqliteDb("holybible.db");
    mFinder = new VerseFinder(*mDb);
    mDailyVerse = new DailyVerse(*mDb);
    mWordSearch = new WordSearch(*mDb);
    mChapterFinder = new ChapterFinder(*mDb);

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Row 1: Book, Chapter, Verse inputs
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Book:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    mBookInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
    inputSizer->Add(mBookInput, 0, wxRIGHT, 10);

    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Chapter:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    mChapterInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(50, -1));
    inputSizer->Add(mChapterInput, 0, wxRIGHT, 10);

    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Verse:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    mVerseInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(50, -1));
    inputSizer->Add(mVerseInput, 0, wxRIGHT, 10);

    wxButton* fetchVerseBtn = new wxButton(panel, wxID_ANY, "Read Verse");
    inputSizer->Add(fetchVerseBtn, 0);

    mainSizer->Add(inputSizer, 0, wxALL, 10);

    // Row 2: Action buttons
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* randomBtn = new wxButton(panel, wxID_ANY, "Verse of the Day");
    wxButton* searchBtn = new wxButton(panel, wxID_ANY, "Word Search");
    wxButton* chapterBtn = new wxButton(panel, wxID_ANY, "Read Chapter");
    btnSizer->Add(randomBtn, 0, wxRIGHT, 10);
    btnSizer->Add(searchBtn, 0, wxRIGHT, 10);
    btnSizer->Add(chapterBtn, 0);

    mainSizer->Add(btnSizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Row 3: Chapter inputs
    wxBoxSizer* chapterInputSizer = new wxBoxSizer(wxHORIZONTAL);
    chapterInputSizer->Add(new wxStaticText(panel, wxID_ANY, "Chapter Book:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    mChapterBookInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(150, -1));
    chapterInputSizer->Add(mChapterBookInput, 0, wxRIGHT, 10);

    chapterInputSizer->Add(new wxStaticText(panel, wxID_ANY, "Chapter:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    mChapterNumInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(50, -1));
    chapterInputSizer->Add(mChapterNumInput, 0, wxRIGHT, 10);

    mainSizer->Add(chapterInputSizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Output text area
    mOutput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                              wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2);
    mainSizer->Add(mOutput, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(mainSizer);

    fetchVerseBtn->Bind(wxEVT_BUTTON, &MainWindow::OnFetchVerse, this);
    randomBtn->Bind(wxEVT_BUTTON, &MainWindow::OnRandomVerse, this);
    searchBtn->Bind(wxEVT_BUTTON, &MainWindow::OnSearchWord, this);
    chapterBtn->Bind(wxEVT_BUTTON, &MainWindow::OnFetchChapter, this);
}

void MainWindow::OnFetchVerse(wxCommandEvent& event) {
    std::string book = mBookInput->GetValue().ToStdString();
    int chapter = wxAtoi(mChapterInput->GetValue());
    int verse = wxAtoi(mVerseInput->GetValue());

    if (book.empty() || chapter == 0 || verse == 0) {
        mOutput->SetValue("Please enter a book, chapter, and verse.");
        return;
    }

    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    mFinder->fetchVerse(book, chapter, verse, true);
    std::cout.rdbuf(old);

    mOutput->SetValue(wxString::FromUTF8(buffer.str()));
}

void MainWindow::OnRandomVerse(wxCommandEvent& event) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    mDailyVerse->RandomVerse();
    std::cout.rdbuf(old);

    mOutput->SetValue(wxString::FromUTF8(buffer.str()));
}

void MainWindow::OnSearchWord(wxCommandEvent& event) {
    wxString word = wxGetTextFromUser("Enter a word to search:", "Word Search");
    if (word.IsEmpty()) return;

    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    mWordSearch->searchWord(word.ToStdString()); // NOT getSearchWord()
    std::cout.rdbuf(old);

    mOutput->SetValue(wxString::FromUTF8(buffer.str()));
}

void MainWindow::OnFetchChapter(wxCommandEvent& event) {
    std::string book = mChapterBookInput->GetValue().ToStdString();
    int chapter = wxAtoi(mChapterNumInput->GetValue());

    if (book.empty() || chapter == 0) {
        mOutput->SetValue("Please enter a book and chapter.");
        return;
    }

    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    mChapterFinder->fetchChapter(book, chapter);
    std::cout.rdbuf(old);

    mOutput->SetValue(wxString::FromUTF8(buffer.str()));
}