#include <wx/statline.h>
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

    // Colors
    wxColour mBgColor      = wxColour(245, 247, 250);
    wxColour mAccentColor  = wxColour(67, 97, 238);
    wxColour mWhite        = wxColour(255, 255, 255);
    wxColour mTextColor    = wxColour(30, 30, 30);
    wxColour mBorderColor  = wxColour(200, 200, 210);

    wxButton* MakeButton(wxPanel* panel, const wxString& label);
    wxTextCtrl* MakeInput(wxPanel* panel, const wxString& hint, int width);

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

wxButton* MainWindow::MakeButton(wxPanel* panel, const wxString& label) {
    wxButton* btn = new wxButton(panel, wxID_ANY, label, wxDefaultPosition, wxSize(140, 36));
    btn->SetBackgroundColour(mAccentColor);
    btn->SetForegroundColour(mWhite);
    btn->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    return btn;
}

wxTextCtrl* MainWindow::MakeInput(wxPanel* panel, const wxString& hint, int width) {
    wxTextCtrl* input = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(width, 32));
    input->SetHint(hint);
    input->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    input->SetBackgroundColour(mWhite);
    input->SetForegroundColour(mTextColor);
    return input;
}

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(900, 650))
{
    mDb = new SqliteDb("holybible.db");
    mFinder = new VerseFinder(*mDb);
    mDailyVerse = new DailyVerse(*mDb);
    mWordSearch = new WordSearch(*mDb);
    mChapterFinder = new ChapterFinder(*mDb);

    SetBackgroundColour(mBgColor);

    wxPanel* panel = new wxPanel(this);
    panel->SetBackgroundColour(mBgColor);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Title
    wxStaticText* title_label = new wxStaticText(panel, wxID_ANY, "Growth Bible");
    title_label->SetFont(wxFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    title_label->SetForegroundColour(mAccentColor);
    mainSizer->Add(title_label, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 15);

    // Divider
    wxStaticLine* line1 = new wxStaticLine(panel);
    mainSizer->Add(line1, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
    mainSizer->AddSpacer(10);

    // Row 1: Verse lookup
    wxStaticText* verseLabel = new wxStaticText(panel, wxID_ANY, "Verse Lookup");
    verseLabel->SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    verseLabel->SetForegroundColour(mTextColor);
    mainSizer->Add(verseLabel, 0, wxLEFT, 25);
    mainSizer->AddSpacer(5);

    wxBoxSizer* verseSizer = new wxBoxSizer(wxHORIZONTAL);
    mBookInput = MakeInput(panel, "Book", 160);
    mChapterInput = MakeInput(panel, "Chapter", 70);
    mVerseInput = MakeInput(panel, "Verse", 70);
    wxButton* fetchVerseBtn = MakeButton(panel, "Read Verse");

    verseSizer->Add(mBookInput, 0, wxRIGHT, 8);
    verseSizer->Add(mChapterInput, 0, wxRIGHT, 8);
    verseSizer->Add(mVerseInput, 0, wxRIGHT, 8);
    verseSizer->Add(fetchVerseBtn, 0);
    mainSizer->Add(verseSizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 25);

    // Row 2: Chapter lookup
    wxStaticText* chapterLabel = new wxStaticText(panel, wxID_ANY, "Chapter Lookup");
    chapterLabel->SetFont(wxFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    chapterLabel->SetForegroundColour(mTextColor);
    mainSizer->Add(chapterLabel, 0, wxLEFT, 25);
    mainSizer->AddSpacer(5);

    wxBoxSizer* chapterSizer = new wxBoxSizer(wxHORIZONTAL);
    mChapterBookInput = MakeInput(panel, "Book", 160);
    mChapterNumInput = MakeInput(panel, "Chapter", 70);
    wxButton* chapterBtn = MakeButton(panel, "Read Chapter");

    chapterSizer->Add(mChapterBookInput, 0, wxRIGHT, 8);
    chapterSizer->Add(mChapterNumInput, 0, wxRIGHT, 8);
    chapterSizer->Add(chapterBtn, 0);
    mainSizer->Add(chapterSizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 25);

    // Row 3: Action buttons
    wxStaticLine* line2 = new wxStaticLine(panel);
    mainSizer->Add(line2, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
    mainSizer->AddSpacer(10);

    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* randomBtn = MakeButton(panel, "Verse of the Day");
    wxButton* searchBtn = MakeButton(panel, "Word Search");
    btnSizer->Add(randomBtn, 0, wxRIGHT, 10);
    btnSizer->Add(searchBtn, 0);
    mainSizer->Add(btnSizer, 0, wxLEFT | wxRIGHT | wxBOTTOM, 25);

    // Output area
    mOutput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                              wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH2);
    mOutput->SetFont(wxFont(11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    mOutput->SetBackgroundColour(mWhite);
    mOutput->SetForegroundColour(mTextColor);
    mainSizer->Add(mOutput, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 25);

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
    mWordSearch->searchWord(word.ToStdString());
    std::cout.rdbuf(old);

    std::string result = buffer.str();
    wxString wxResult = wxString::FromUTF8(result);
    mOutput->SetValue(wxResult);

    // Highlight searched word in red bold
    wxString wxWord = word.Lower();
    wxString lowerResult = wxResult.Lower();
    int start = 0;
    while ((start = lowerResult.find(wxWord, start)) != (int)wxString::npos) {
        mOutput->SetStyle(start, start + wxWord.length(),
            wxTextAttr(*wxRED, wxNullColour,
                wxFont(11, wxFONTFAMILY_ROMAN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD)));
        start += wxWord.length();
    }
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