# Smart Campus Lost & Found Management System (C++)

**Lab Sheet-10 | BCA I Semester (Session 2026-2027) | COER University, Roorkee, Uttarakhand**

A console-based C++ application where students can report lost items, submit found items, search for belongings, and automatically match lost and found records.

- **Name:** YOUR NAME HERE
- **Roll No / Enrollment No:** YOUR ROLL NUMBER HERE
- **Course:** BCA I Semester
- **Subject Code:** BCA111P

---

## 1. Objective of the Program

To develop a system where students can report lost items, submit found items, search for belongings, and match records automatically.

**Problem statement:** Every college faces the problem of lost items such as ID cards, wallets, keys, laptops and mobile phones. This system makes finding lost items faster and more organized.

---

## 2. Requirements

- C++ programming environment (g++ compiler, C++11 or newer)
- Text editor or IDE (VS Code)

## 3. Modules

| # | Module | What it does |
|---|--------|--------------|
| 1 | Student Login | Registers a new student or logs in an existing one (password, 3 attempts) |
| 2 | Report Lost Item | Saves a new LOST record |
| 3 | Report Found Item | Saves a new FOUND record |
| 4 | Search Item | Keyword search in name, category, description and location |
| 5 | Auto Match Lost & Found | Pairs LOST and FOUND records automatically |
| 6 | View All Records | Shows all records, sorted by ID, name or date |
| 7 | Delete Record | Deletes a record (only the student who reported it) |
| 8 | Item Statistics | Counts of lost / found / matched / open and per category |
| 9 | Export Report | Writes a full report to `report.txt` |
| 10 | Exit | Closes the program |

## 4. Concepts Used

| Concept | Where it is used |
|---------|------------------|
| Arrays | `Item items[500]` stores all records; `perCategory[]` in statistics |
| Structures | `struct Item` holds one record |
| File handling | `ifstream` / `ofstream` for `items.txt`, `students.txt`, `report.txt` |
| Functions | One function per module (`reportItem`, `searchItem`, `autoMatch`, ...) |
| Loops | Menu loop, searching, sorting, matching |
| Searching | Linear search (keyword search and delete-by-ID) |
| Sorting | Bubble sort in *View All Records* |
| Strings | `string`, `find()`, `substr()`, `getline()`, `istringstream` |

---

## 5. Algorithm / Logic Used

**Main program**
1. Load all saved records from `items.txt` into the array.
2. Ask the student to log in (or register).
3. Show the menu, read the choice, call the matching function.
4. Repeat until the user chooses 10 (Exit).

**Student Login**
1. Read the student ID and search `students.txt`.
2. If not found, register the student (name + password) and save.
3. If found, ask for the password (maximum 3 attempts).

**Report Lost / Found item**
1. Read item name, category, description, location and date (validated as `yyyy-mm-dd`).
2. Create a new record with ID = highest existing ID + 1 and status `OPEN`.
3. Add it to the array and save all records to the file.

**Search**
1. Convert the keyword to lower case.
2. Loop through every record; if the keyword is found inside name, category, description or location, display it.

**Auto Match**
1. For every `OPEN` LOST item, loop through every `OPEN` FOUND item.
2. If both have the **same category** and **similar names** (one name contains the other, or they share a word longer than 2 letters), they match.
3. Mark both as `MATCHED`, store each other's ID, and save.

**View All (Sorting)**
1. Copy the array, then apply bubble sort on the chosen field (ID, name or date).
2. Display the sorted list.

**Delete**
1. Find the record by ID (linear search) and check that the logged-in student reported it.
2. Shift the later records one place left, reduce the count, reopen any record that was matched with it, and save.

---

## 6. How to Run

```bash
g++ -std=c++11 main.cpp -o lostfound
./lostfound          # on Windows: lostfound.exe
```

The data files (`items.txt`, `students.txt`, `report.txt`) are created automatically in the same folder.

---

## 7. Source Code

The complete source code is in [`main.cpp`](main.cpp).

---

## 8. Sample Input and Output

**Student 1 logs in and reports a lost wallet**

```
===== STUDENT LOGIN =====
Enter Student ID: BCA2026001
New student. Let's register you.
Enter your name: Aman Kumar
Create a password: pass123
Registered successfully! Welcome, Aman Kumar.

Enter your choice (1-10): 2
===== REPORT LOST ITEM =====
Item name (e.g. Black Wallet): Black Wallet
Choose category:
  1. ID Card
  2. Wallet
  3. Keys
  4. Laptop
  5. Mobile Phone
  6. Other
Enter choice (1-6): 2
Description (colour, brand, marks): Black leather, COER ID inside
Location where it was lost: Library
Date (yyyy-mm-dd): 2026-09-15

LOST item reported successfully! Record ID = 1
```

**Student 2 reports a found wallet, then runs Auto Match**

```
FOUND item reported successfully! Record ID = 2

Enter your choice (1-10): 5
===== AUTO MATCH LOST & FOUND =====
MATCH #1
  LOST  (ID 1): Black Wallet | reported by BCA2026001
  FOUND (ID 2): Wallet black | at Canteen | reported by BCA2026002
  -> Owner BCA2026001 should collect the item.

1 new match(es) saved.
```

**Item Statistics**

```
Total records   : 2
Lost items      : 1
Found items     : 1
Matched records : 2
Open records    : 0

Items by category:
  ID Card       : 0
  Wallet        : 2
  Keys          : 0
  Laptop        : 0
  Mobile Phone  : 0
  Other         : 0
```

*(Add your own screenshots of the program running here.)*

---

## 9. Future Enhancements

- QR Code Verification
- Email Notification
- Face Recognition
- Mobile Application

## 10. Learning Outcomes

- Record management
- Searching algorithms
- File handling
- Logical programming
- Real-world application development

## 11. Conclusion

The Smart Campus Lost & Found Management System was developed in C++ using arrays, structures, file handling, functions, loops, searching, sorting and strings. Students can report lost and found items, search records, and let the system match them automatically. All data is saved in text files, so records are not lost when the program closes. The project shows how a real-world campus problem can be solved with simple, modular programming, and it can be extended later with QR verification, email alerts, face recognition and a mobile app.
