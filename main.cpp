/*
 * Smart Campus Lost & Found Management System
 * Lab Sheet-10 | BCA I Semester (Session 2026-2027) | COER University, Roorkee
 *
 * Concepts used: Arrays, Structures, File Handling, Functions,
 *                Loops, Searching, Sorting, Strings
 *
 * Data files (created automatically in the program's folder):
 *   items.txt    - all lost/found records
 *   students.txt - registered students
 *   report.txt   - exported report
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

// ============================================================
// Structure: one lost/found record
// ============================================================
struct Item {
    int id;               // unique record number
    string type;          // "LOST" or "FOUND"
    string name;          // e.g. "Black Wallet"
    string category;      // e.g. "Wallet"
    string description;   // colour, brand, marks
    string location;      // where it was lost / found
    string date;          // yyyy-mm-dd
    string reportedBy;    // student ID of the reporter
    string status;        // "OPEN" or "MATCHED"
    int matchedId;        // id of the matching record (0 = none)
};

// ============================================================
// Global data
// ============================================================
const int MAX_ITEMS = 500;
const int CATEGORY_COUNT = 6;
const string CATEGORIES[CATEGORY_COUNT] = {"ID Card", "Wallet", "Keys", "Laptop", "Mobile Phone", "Other"};

const string ITEMS_FILE = "items.txt";
const string STUDENTS_FILE = "students.txt";
const string REPORT_FILE = "report.txt";

Item items[MAX_ITEMS];    // array of records
int itemCount = 0;        // how many records are stored

// ============================================================
// String helper functions
// ============================================================

// Convert a string to lower case
string toLower(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] >= 'A' && text[i] <= 'Z') {
            text[i] = text[i] + ('a' - 'A');
        }
    }
    return text;
}

// Convert a string to upper case
string toUpper(string text) {
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            text[i] = text[i] - ('a' - 'A');
        }
    }
    return text;
}

// Remove spaces from the start and end of a string
string trim(const string &text) {
    size_t start = text.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

// Split a line into parts at every '|' character
vector<string> splitByPipe(const string &line) {
    vector<string> parts;
    string current = "";
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == '|') {
            parts.push_back(current);
            current = "";
        } else {
            current += line[i];
        }
    }
    parts.push_back(current);
    return parts;
}

// Returns true if the text contains only digits (and is not too long)
bool isNumber(const string &text) {
    if (text.empty() || text.size() > 9) {
        return false;
    }
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] < '0' || text[i] > '9') {
            return false;
        }
    }
    return true;
}

// ============================================================
// Input helper functions
// ============================================================

// Reads one full line. Exits the program if the input has ended.
string readLine() {
    string line;
    if (!getline(cin, line)) {
        cout << "\nInput ended. Exiting program.\n";
        exit(0);
    }
    return trim(line);
}

// Reads a non-empty line. '|' is replaced because it is our file separator.
string readText(const string &prompt) {
    string text;
    do {
        cout << prompt;
        text = readLine();
        if (text.empty()) {
            cout << "  Input cannot be empty. Try again.\n";
        }
    } while (text.empty());
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '|') {
            text[i] = '/';
        }
    }
    return text;
}

// Reads an integer between minValue and maxValue (inclusive)
int readInt(const string &prompt, int minValue, int maxValue) {
    while (true) {
        cout << prompt;
        string line = readLine();
        if (isNumber(line)) {
            int value = atoi(line.c_str());
            if (value >= minValue && value <= maxValue) {
                return value;
            }
        }
        cout << "  Please enter a number between " << minValue << " and " << maxValue << ".\n";
    }
}

// Reads a date in yyyy-mm-dd format
string readDate(const string &prompt) {
    while (true) {
        string date = readText(prompt);
        bool valid = (date.size() == 10 && date[4] == '-' && date[7] == '-');
        for (int i = 0; i < 10 && valid; i++) {
            if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')) {
                valid = false;
            }
        }
        if (valid) {
            return date;
        }
        cout << "  Invalid format. Use yyyy-mm-dd (example: 2026-09-15).\n";
    }
}

// Shows the category list and returns the chosen category
string chooseCategory() {
    cout << "Choose category:\n";
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        cout << "  " << (i + 1) << ". " << CATEGORIES[i] << "\n";
    }
    int choice = readInt("Enter choice (1-6): ", 1, CATEGORY_COUNT);
    return CATEGORIES[choice - 1];
}

// ============================================================
// File handling functions
// ============================================================

// Reads items.txt into the array
void loadItems() {
    itemCount = 0;
    ifstream file(ITEMS_FILE.c_str());
    if (!file) {
        return;     // first run: no file yet
    }
    string line;
    while (getline(file, line) && itemCount < MAX_ITEMS) {
        if (trim(line).empty()) {
            continue;
        }
        vector<string> p = splitByPipe(line);
        if (p.size() != 10 || !isNumber(p[0]) || !isNumber(p[9])) {
            continue;   // skip damaged lines
        }
        Item it;
        it.id = atoi(p[0].c_str());
        it.type = p[1];
        it.name = p[2];
        it.category = p[3];
        it.description = p[4];
        it.location = p[5];
        it.date = p[6];
        it.reportedBy = p[7];
        it.status = p[8];
        it.matchedId = atoi(p[9].c_str());
        items[itemCount++] = it;
    }
    file.close();
}

// Writes all records of the array to items.txt
void saveItems() {
    ofstream file(ITEMS_FILE.c_str());
    if (!file) {
        cout << "Error: could not save to " << ITEMS_FILE << "\n";
        return;
    }
    for (int i = 0; i < itemCount; i++) {
        file << items[i].id << "|" << items[i].type << "|" << items[i].name << "|"
             << items[i].category << "|" << items[i].description << "|"
             << items[i].location << "|" << items[i].date << "|"
             << items[i].reportedBy << "|" << items[i].status << "|"
             << items[i].matchedId << "\n";
    }
    file.close();
}

// Looks for a student ID in students.txt. Returns true if found.
bool findStudent(const string &studentId, string &name, string &password) {
    ifstream file(STUDENTS_FILE.c_str());
    if (!file) {
        return false;
    }
    string line;
    while (getline(file, line)) {
        vector<string> p = splitByPipe(line);
        if (p.size() == 3 && toUpper(p[0]) == toUpper(studentId)) {
            name = p[1];
            password = trim(p[2]);
            return true;
        }
    }
    return false;
}

// Adds a new student to students.txt
void registerStudent(const string &studentId, const string &name, const string &password) {
    ofstream file(STUDENTS_FILE.c_str(), ios::app);   // ios::app = add at the end
    file << studentId << "|" << name << "|" << password << "\n";
    file.close();
}

// ============================================================
// Display helper
// ============================================================
void displayItem(const Item &it) {
    cout << "----------------------------------------\n";
    cout << "ID          : " << it.id << "\n";
    cout << "Type        : " << it.type << "\n";
    cout << "Item Name   : " << it.name << "\n";
    cout << "Category    : " << it.category << "\n";
    cout << "Description : " << it.description << "\n";
    cout << "Location    : " << it.location << "\n";
    cout << "Date        : " << it.date << "\n";
    cout << "Reported By : " << it.reportedBy << "\n";
    cout << "Status      : " << it.status;
    if (it.matchedId != 0) {
        cout << " (matched with ID " << it.matchedId << ")";
    }
    cout << "\n";
}

// Next free record ID = highest existing ID + 1
int nextId() {
    int maxId = 0;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id > maxId) {
            maxId = items[i].id;
        }
    }
    return maxId + 1;
}

// ============================================================
// Module 1: Student Login
// Returns the student ID, or "" if login failed
// ============================================================
string studentLogin() {
    cout << "\n===== STUDENT LOGIN =====\n";
    string studentId = toUpper(readText("Enter Student ID: "));
    string name, savedPassword;

    if (!findStudent(studentId, name, savedPassword)) {
        cout << "New student. Let's register you.\n";
        name = readText("Enter your name: ");
        string password = readText("Create a password: ");
        registerStudent(studentId, name, password);
        cout << "Registered successfully! Welcome, " << name << ".\n";
        return studentId;
    }

    for (int attempt = 1; attempt <= 3; attempt++) {
        string password = readText("Enter password: ");
        if (password == savedPassword) {
            cout << "Login successful! Welcome back, " << name << ".\n";
            return studentId;
        }
        cout << "Wrong password. Attempts left: " << (3 - attempt) << "\n";
    }
    cout << "Too many wrong attempts.\n";
    return "";
}

// ============================================================
// Module 2 & 3: Report Lost / Found item
// ============================================================
void reportItem(const string &type, const string &studentId) {
    if (itemCount >= MAX_ITEMS) {
        cout << "Storage is full. Delete some records first.\n";
        return;
    }
    cout << "\n===== REPORT " << type << " ITEM =====\n";

    Item it;
    it.id = nextId();
    it.type = type;
    it.name = readText("Item name (e.g. Black Wallet): ");
    it.category = chooseCategory();
    it.description = readText("Description (colour, brand, marks): ");
    it.location = readText("Location where it was " + toLower(type) + ": ");
    it.date = readDate("Date (yyyy-mm-dd): ");
    it.reportedBy = studentId;
    it.status = "OPEN";
    it.matchedId = 0;

    items[itemCount++] = it;
    saveItems();
    cout << "\n" << type << " item reported successfully! Record ID = " << it.id << "\n";
}

// ============================================================
// Module 4: Search Item (linear search)
// ============================================================
void searchItem() {
    cout << "\n===== SEARCH ITEM =====\n";
    string keyword = toLower(readText("Enter keyword (name, category, description or location): "));

    int found = 0;
    for (int i = 0; i < itemCount; i++) {
        string allText = toLower(items[i].name + " " + items[i].category + " "
                                 + items[i].description + " " + items[i].location);
        if (allText.find(keyword) != string::npos) {
            displayItem(items[i]);
            found++;
        }
    }
    if (found == 0) {
        cout << "No records match \"" << keyword << "\".\n";
    } else {
        cout << "----------------------------------------\n";
        cout << found << " record(s) found.\n";
    }
}

// ============================================================
// Module 5: Auto Match Lost & Found
// ============================================================

// Names are similar if one contains the other, or they share a word longer than 2 letters
bool namesAreSimilar(string a, string b) {
    a = toLower(a);
    b = toLower(b);
    if (a.find(b) != string::npos || b.find(a) != string::npos) {
        return true;
    }
    istringstream streamA(a);
    string wordA;
    while (streamA >> wordA) {
        istringstream streamB(b);
        string wordB;
        while (streamB >> wordB) {
            if (wordA.size() > 2 && wordA == wordB) {
                return true;
            }
        }
    }
    return false;
}

void autoMatch() {
    cout << "\n===== AUTO MATCH LOST & FOUND =====\n";
    int matches = 0;

    for (int i = 0; i < itemCount; i++) {
        if (items[i].type != "LOST" || items[i].status != "OPEN") {
            continue;
        }
        for (int j = 0; j < itemCount; j++) {
            if (items[j].type != "FOUND" || items[j].status != "OPEN") {
                continue;
            }
            // Rule: same category AND similar item name
            if (toLower(items[i].category) == toLower(items[j].category)
                && namesAreSimilar(items[i].name, items[j].name)) {

                items[i].status = "MATCHED";
                items[i].matchedId = items[j].id;
                items[j].status = "MATCHED";
                items[j].matchedId = items[i].id;
                matches++;

                cout << "MATCH #" << matches << "\n";
                cout << "  LOST  (ID " << items[i].id << "): " << items[i].name
                     << " | reported by " << items[i].reportedBy << "\n";
                cout << "  FOUND (ID " << items[j].id << "): " << items[j].name
                     << " | at " << items[j].location
                     << " | reported by " << items[j].reportedBy << "\n";
                cout << "  -> Owner " << items[i].reportedBy << " should collect the item.\n";
                break;   // this lost item is matched; go to the next one
            }
        }
    }

    if (matches == 0) {
        cout << "No new matches found.\n";
    } else {
        saveItems();
        cout << "\n" << matches << " new match(es) saved.\n";
    }
}

// ============================================================
// Module 6: View All Records (with bubble sort)
// ============================================================
void viewAll() {
    cout << "\n===== VIEW ALL RECORDS =====\n";
    if (itemCount == 0) {
        cout << "No records yet.\n";
        return;
    }
    cout << "Sort by:  1. ID   2. Item Name (A-Z)   3. Date (oldest first)\n";
    int choice = readInt("Enter choice (1-3): ", 1, 3);

    // Copy the array so the original order stays unchanged
    Item sorted[MAX_ITEMS];
    for (int i = 0; i < itemCount; i++) {
        sorted[i] = items[i];
    }

    // Bubble sort
    for (int i = 0; i < itemCount - 1; i++) {
        for (int j = 0; j < itemCount - 1 - i; j++) {
            bool swapNeeded = false;
            if (choice == 1) {
                swapNeeded = sorted[j].id > sorted[j + 1].id;
            } else if (choice == 2) {
                swapNeeded = toLower(sorted[j].name) > toLower(sorted[j + 1].name);
            } else {
                swapNeeded = sorted[j].date > sorted[j + 1].date;
            }
            if (swapNeeded) {
                Item temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < itemCount; i++) {
        displayItem(sorted[i]);
    }
    cout << "----------------------------------------\n";
    cout << "Total records: " << itemCount << "\n";
}

// ============================================================
// Module 7: Delete Record
// ============================================================
void deleteRecord(const string &studentId) {
    cout << "\n===== DELETE RECORD =====\n";
    if (itemCount == 0) {
        cout << "No records to delete.\n";
        return;
    }
    int id = readInt("Enter record ID to delete: ", 1, 999999999);

    // Linear search for the record
    int position = -1;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == id) {
            position = i;
            break;
        }
    }
    if (position == -1) {
        cout << "No record with ID " << id << ".\n";
        return;
    }
    if (toUpper(items[position].reportedBy) != toUpper(studentId)) {
        cout << "You can delete only the records reported by you.\n";
        return;
    }

    displayItem(items[position]);
    cout << "Delete this record? (y/n): ";
    string answer = readLine();
    if (answer != "y" && answer != "Y") {
        cout << "Deletion cancelled.\n";
        return;
    }

    // Shift the remaining records one place to the left
    for (int i = position; i < itemCount - 1; i++) {
        items[i] = items[i + 1];
    }
    itemCount--;

    // If another record was matched with the deleted one, reopen it
    for (int i = 0; i < itemCount; i++) {
        if (items[i].matchedId == id) {
            items[i].matchedId = 0;
            items[i].status = "OPEN";
        }
    }
    saveItems();
    cout << "Record deleted.\n";
}

// ============================================================
// Module 8: Item Statistics
// ============================================================

// Builds the statistics as text (used by both Statistics and Export)
string buildStatistics() {
    int lost = 0, found = 0, matched = 0, open = 0;
    int perCategory[CATEGORY_COUNT] = {0};

    for (int i = 0; i < itemCount; i++) {
        if (items[i].type == "LOST") {
            lost++;
        } else {
            found++;
        }
        if (items[i].status == "MATCHED") {
            matched++;
        } else {
            open++;
        }
        for (int c = 0; c < CATEGORY_COUNT; c++) {
            if (toLower(CATEGORIES[c]) == toLower(items[i].category)) {
                perCategory[c]++;
            }
        }
    }

    ostringstream out;
    out << "Total records   : " << itemCount << "\n";
    out << "Lost items      : " << lost << "\n";
    out << "Found items     : " << found << "\n";
    out << "Matched records : " << matched << "\n";
    out << "Open records    : " << open << "\n";
    out << "\nItems by category:\n";
    for (int c = 0; c < CATEGORY_COUNT; c++) {
        out << "  " << CATEGORIES[c];
        for (size_t pad = CATEGORIES[c].size(); pad < 14; pad++) {
            out << " ";
        }
        out << ": " << perCategory[c] << "\n";
    }
    return out.str();
}

void showStatistics() {
    cout << "\n===== ITEM STATISTICS =====\n";
    cout << buildStatistics();
}

// ============================================================
// Module 9: Export Report
// ============================================================
void exportReport() {
    cout << "\n===== EXPORT REPORT =====\n";
    ofstream file(REPORT_FILE.c_str());
    if (!file) {
        cout << "Error: could not create " << REPORT_FILE << "\n";
        return;
    }
    file << "SMART CAMPUS LOST & FOUND - REPORT\n";
    file << "========================================\n";
    file << buildStatistics();
    file << "\nALL RECORDS\n";
    for (int i = 0; i < itemCount; i++) {
        file << "----------------------------------------\n";
        file << "ID          : " << items[i].id << "\n";
        file << "Type        : " << items[i].type << "\n";
        file << "Item Name   : " << items[i].name << "\n";
        file << "Category    : " << items[i].category << "\n";
        file << "Description : " << items[i].description << "\n";
        file << "Location    : " << items[i].location << "\n";
        file << "Date        : " << items[i].date << "\n";
        file << "Reported By : " << items[i].reportedBy << "\n";
        file << "Status      : " << items[i].status;
        if (items[i].matchedId != 0) {
            file << " (matched with ID " << items[i].matchedId << ")";
        }
        file << "\n";
    }
    file.close();
    cout << "Report exported to: " << REPORT_FILE << "\n";
}

// ============================================================
// Menu
// ============================================================
void printMenu(const string &studentId) {
    cout << "\n========== MAIN MENU (Logged in: " << studentId << ") ==========\n";
    cout << " 1. Student Login (switch user)\n";
    cout << " 2. Report Lost Item\n";
    cout << " 3. Report Found Item\n";
    cout << " 4. Search Item\n";
    cout << " 5. Auto Match Lost & Found\n";
    cout << " 6. View All Records\n";
    cout << " 7. Delete Record\n";
    cout << " 8. Item Statistics\n";
    cout << " 9. Export Report\n";
    cout << "10. Exit\n";
}

// ============================================================
// main
// ============================================================
int main() {
    loadItems();

    cout << "========================================\n";
    cout << "  SMART CAMPUS LOST & FOUND SYSTEM\n";
    cout << "  COER University, Roorkee\n";
    cout << "========================================\n";

    // A student must log in before using the system
    string currentStudent = "";
    while (currentStudent == "") {
        currentStudent = studentLogin();
    }

    int choice = 0;
    while (choice != 10) {
        printMenu(currentStudent);
        choice = readInt("Enter your choice (1-10): ", 1, 10);

        switch (choice) {
            case 1: {
                string newStudent = studentLogin();
                if (newStudent != "") {
                    currentStudent = newStudent;
                }
                break;
            }
            case 2:
                reportItem("LOST", currentStudent);
                break;
            case 3:
                reportItem("FOUND", currentStudent);
                break;
            case 4:
                searchItem();
                break;
            case 5:
                autoMatch();
                break;
            case 6:
                viewAll();
                break;
            case 7:
                deleteRecord(currentStudent);
                break;
            case 8:
                showStatistics();
                break;
            case 9:
                exportReport();
                break;
            case 10:
                cout << "Thank you for using the system. Goodbye!\n";
                break;
        }
    }
    return 0;
}
