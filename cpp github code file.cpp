#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <ctime>
#include <cctype>

using namespace std;


// ============================================================
//                    CONTACT CLASS
// ============================================================

class Contact
{
private:
    int contactID;
    string name;
    string phone;
    string email;
    string address;
    string category;
    string notes;
    bool favorite;
    string createdAt;

public:

    // Constructor
    Contact(int id = 0,
            string n = "",
            string p = "",
            string e = "",
            string a = "",
            string c = "",
            string nt = "",
            bool f = false,
            string date = "")
    {
        contactID = id;
        name = n;
        phone = p;
        email = e;
        address = a;
        category = c;
        notes = nt;
        favorite = f;

        if (date.empty())
            createdAt = getCurrentDateTime();
        else
            createdAt = date;
    }


    // ========================================================
    //                     GETTER FUNCTIONS
    // ========================================================

    int getID() const
    {
        return contactID;
    }

    string getName() const
    {
        return name;
    }

    string getPhone() const
    {
        return phone;
    }

    string getEmail() const
    {
        return email;
    }

    string getAddress() const
    {
        return address;
    }

    string getCategory() const
    {
        return category;
    }

    string getNotes() const
    {
        return notes;
    }

    bool isFavorite() const
    {
        return favorite;
    }

    string getCreatedAt() const
    {
        return createdAt;
    }


    // ========================================================
    //                     SETTER FUNCTIONS
    // ========================================================

    void setName(string n)
    {
        name = n;
    }

    void setPhone(string p)
    {
        phone = p;
    }

    void setEmail(string e)
    {
        email = e;
    }

    void setAddress(string a)
    {
        address = a;
    }

    void setCategory(string c)
    {
        category = c;
    }

    void setNotes(string n)
    {
        notes = n;
    }


    // ========================================================
    //                  DISPLAY CONTACT
    // ========================================================

    void display() const
    {
        cout << "\n";
        cout << "============================================\n";
        cout << "              CONTACT DETAILS\n";
        cout << "============================================\n";

        cout << "Contact ID : " << contactID << endl;
        cout << "Name       : " << name << endl;
        cout << "Phone      : " << phone << endl;
        cout << "Email      : " << email << endl;
        cout << "Address    : " << address << endl;
        cout << "Category   : " << category << endl;
        cout << "Notes      : " << notes << endl;
        cout << "Favorite   : " << (favorite ? "Yes" : "No") << endl;
        cout << "Created At : " << createdAt << endl;

        cout << "============================================\n";
    }


    // ========================================================
    //                  UPDATE CONTACT
    // ========================================================

    void update()
    {
        cout << "\n========== UPDATE CONTACT ==========\n";

        cout << "Enter new name: ";
        getline(cin >> ws, name);

        cout << "Enter new phone number: ";
        getline(cin, phone);

        cout << "Enter new email: ";
        getline(cin, email);

        cout << "Enter new address: ";
        getline(cin, address);

        cout << "Enter new category: ";
        getline(cin, category);

        cout << "Enter notes: ";
        getline(cin, notes);

        cout << "\nContact updated successfully!\n";
    }


    // ========================================================
    //                  FAVORITE TOGGLE
    // ========================================================

    void toggleFavorite()
    {
        favorite = !favorite;
    }


    // ========================================================
    //                CURRENT DATE & TIME
    // ========================================================

    static string getCurrentDateTime()
    {
        time_t now = time(nullptr);

        tm* localTime = localtime(&now);

        char buffer[80];

        strftime(buffer,
                 sizeof(buffer),
                 "%d-%m-%Y %H:%M:%S",
                 localTime);

        return string(buffer);
    }
};


// ============================================================
//                    UTILITY FUNCTIONS
// ============================================================

// Convert special file separator character
string sanitizeField(string value)
{
    for (char& ch : value)
    {
        if (ch == '|')
            ch = '/';
    }

    return value;
}


// Convert string to lowercase
string toLowerCase(string text)
{
    transform(text.begin(),
              text.end(),
              text.begin(),
              [](unsigned char c)
              {
                  return static_cast<char>(tolower(c));
              });

    return text;
}


// Validate phone number
bool isValidPhone(string phone)
{
    if (phone.length() != 10)
        return false;

    for (char c : phone)
    {
        if (!isdigit(c))
            return false;
    }

    return true;
}


// Validate email
bool isValidEmail(string email)
{
    regex pattern(R"(^[\w.%+-]+@[\w.-]+\.[A-Za-z]{2,}$)");

    return regex_match(email, pattern);
}


// Validate name
bool isValidName(string name)
{
    return !name.empty();
}


// ============================================================
//                   ADDRESS BOOK CLASS
// ============================================================

class AddressBook
{
private:

    vector<Contact> contacts;

    int nextID;

    const string dataFile = "address_book_data.txt";
    const string backupFile = "address_book_backup.txt";
    const string exportFile = "address_book_export.csv";


public:

    // ========================================================
    //                    CONSTRUCTOR
    // ========================================================

    AddressBook()
    {
        nextID = 1;

        loadFromFile();
    }


    // ========================================================
    //                    SAVE TO FILE
    // ========================================================

    void saveToFile()
    {
        ofstream outfile(dataFile);

        if (!outfile)
        {
            cout << "\nError: Could not save data.\n";
            return;
        }

        // Store next ID first
        outfile << nextID << endl;

        // Store all contacts
        for (const Contact& c : contacts)
        {
            outfile
                << c.getID() << "|"
                << sanitizeField(c.getName()) << "|"
                << sanitizeField(c.getPhone()) << "|"
                << sanitizeField(c.getEmail()) << "|"
                << sanitizeField(c.getAddress()) << "|"
                << sanitizeField(c.getCategory()) << "|"
                << sanitizeField(c.getNotes()) << "|"
                << c.isFavorite() << "|"
                << sanitizeField(c.getCreatedAt())
                << endl;
        }

        outfile.close();
    }


    // ========================================================
    //                    LOAD FROM FILE
    // ========================================================

    void loadFromFile()
    {
        ifstream infile(dataFile);

        // If file does not exist, start with empty address book
        if (!infile)
            return;

        string line;

        // Read next ID
        if (getline(infile, line))
        {
            try
            {
                nextID = stoi(line);
            }
            catch (...)
            {
                nextID = 1;
            }
        }

        // Read contacts
        while (getline(infile, line))
        {
            stringstream ss(line);

            string id;
            string name;
            string phone;
            string email;
            string address;
            string category;
            string notes;
            string fav;
            string createdAt;

            getline(ss, id, '|');
            getline(ss, name, '|');
            getline(ss, phone, '|');
            getline(ss, email, '|');
            getline(ss, address, '|');
            getline(ss, category, '|');
            getline(ss, notes, '|');
            getline(ss, fav, '|');
            getline(ss, createdAt, '|');

            if (id.empty())
                continue;

            try
            {
                Contact c(
                    stoi(id),
                    name,
                    phone,
                    email,
                    address,
                    category,
                    notes,
                    fav == "1",
                    createdAt
                );

                contacts.push_back(c);
            }
            catch (...)
            {
                // Ignore corrupted records
            }
        }

        infile.close();
    }


    // ========================================================
    //                    ADD CONTACT
    // ========================================================

    void addContact()
    {
        string name;
        string phone;
        string email;
        string address;
        string category;
        string notes;

        cout << "\n";
        cout << "============================================\n";
        cout << "                ADD CONTACT\n";
        cout << "============================================\n";

        // Name
        cout << "Enter name: ";
        getline(cin >> ws, name);

        if (!isValidName(name))
        {
            cout << "\nInvalid name!\n";
            return;
        }

        // Phone
        cout << "Enter phone number (10 digits): ";
        getline(cin, phone);

        if (!isValidPhone(phone))
        {
            cout << "\nInvalid phone number!\n";
            cout << "Phone number must contain exactly 10 digits.\n";
            return;
        }

        // Duplicate phone check
        for (const Contact& c : contacts)
        {
            if (c.getPhone() == phone)
            {
                cout << "\nA contact with this phone number already exists!\n";
                return;
            }
        }

        // Email
        cout << "Enter email: ";
        getline(cin, email);

        if (!email.empty() && !isValidEmail(email))
        {
            cout << "\nInvalid email address!\n";
            return;
        }

        // Duplicate email check
        if (!email.empty())
        {
            for (const Contact& c : contacts)
            {
                if (toLowerCase(c.getEmail()) ==
                    toLowerCase(email))
                {
                    cout << "\nA contact with this email already exists!\n";
                    return;
                }
            }
        }

        // Address
        cout << "Enter address: ";
        getline(cin, address);

        // Category
        cout << "Enter category "
             << "(Family/Friends/Work/College/Emergency): ";

        getline(cin, category);

        // Notes
        cout << "Enter notes: ";
        getline(cin, notes);

        // Create object
        Contact newContact(
            nextID,
            name,
            phone,
            email,
            address,
            category,
            notes,
            false
        );

        // Store object
        contacts.push_back(newContact);

        cout << "\nContact added successfully!\n";
        cout << "Contact ID: " << nextID << endl;

        nextID++;

        // Save automatically
        saveToFile();
    }


    // ========================================================
    //                  VIEW ALL CONTACTS
    // ========================================================

    void viewContacts()
    {
        cout << "\n";
        cout << "============================================\n";
        cout << "               ALL CONTACTS\n";
        cout << "============================================\n";

        if (contacts.empty())
        {
            cout << "No contacts available.\n";
            return;
        }

        for (const Contact& c : contacts)
        {
            c.display();
        }
    }


    // ========================================================
    //                  SEARCH CONTACT
    // ========================================================

    void searchContact()
    {
        string keyword;

        cout << "\n";
        cout << "============================================\n";
        cout << "               SEARCH CONTACT\n";
        cout << "============================================\n";

        cout << "Enter name, phone or email: ";

        getline(cin >> ws, keyword);

        string searchKey = toLowerCase(keyword);

        bool found = false;

        for (const Contact& c : contacts)
        {
            string name = toLowerCase(c.getName());
            string phone = toLowerCase(c.getPhone());
            string email = toLowerCase(c.getEmail());

            if (name.find(searchKey) != string::npos ||
                phone.find(searchKey) != string::npos ||
                email.find(searchKey) != string::npos)
            {
                c.display();

                found = true;
            }
        }

        if (!found)
        {
            cout << "\nNo matching contact found.\n";
        }
    }


    // ========================================================
    //                  UPDATE CONTACT
    // ========================================================

    void updateContact()
    {
        int id;

        cout << "\n";
        cout << "============================================\n";
        cout << "               UPDATE CONTACT\n";
        cout << "============================================\n";

        cout << "Enter Contact ID: ";

        if (!(cin >> id))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid ID!\n";
            return;
        }

        for (Contact& c : contacts)
        {
            if (c.getID() == id)
            {
                c.update();

                saveToFile();

                return;
            }
        }

        cout << "\nContact not found!\n";
    }


    // ========================================================
    //                  DELETE CONTACT
    // ========================================================

    void deleteContact()
    {
        int id;

        cout << "\n";
        cout << "============================================\n";
        cout << "               DELETE CONTACT\n";
        cout << "============================================\n";

        cout << "Enter Contact ID: ";

        if (!(cin >> id))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid ID!\n";
            return;
        }

        for (auto it = contacts.begin();
             it != contacts.end();
             ++it)
        {
            if (it->getID() == id)
            {
                cout << "\nContact found:\n";

                it->display();

                string confirm;

                cout << "\nAre you sure you want to delete? (yes/no): ";

                cin >> confirm;

                if (confirm == "yes" ||
                    confirm == "YES")
                {
                    contacts.erase(it);

                    saveToFile();

                    cout << "\nContact deleted successfully!\n";
                }
                else
                {
                    cout << "\nDeletion cancelled.\n";
                }

                return;
            }
        }

        cout << "\nContact not found!\n";
    }


    // ========================================================
    //                  SORT CONTACTS
    // ========================================================

    void sortContacts()
    {
        if (contacts.empty())
        {
            cout << "\nNo contacts available to sort.\n";
            return;
        }

        sort(
            contacts.begin(),
            contacts.end(),
            [](const Contact& a, const Contact& b)
            {
                return toLowerCase(a.getName()) <
                       toLowerCase(b.getName());
            }
        );

        saveToFile();

        cout << "\nContacts sorted alphabetically by name!\n";
    }


    // ========================================================
    //                VIEW BY CATEGORY
    // ========================================================

    void viewByCategory()
    {
        string category;

        cout << "\n";
        cout << "============================================\n";
        cout << "              VIEW BY CATEGORY\n";
        cout << "============================================\n";

        cout << "Enter category: ";

        getline(cin >> ws, category);

        bool found = false;

        for (const Contact& c : contacts)
        {
            if (toLowerCase(c.getCategory()) ==
                toLowerCase(category))
            {
                c.display();

                found = true;
            }
        }

        if (!found)
        {
            cout << "\nNo contacts found in this category.\n";
        }
    }


    // ========================================================
    //                MARK / UNMARK FAVORITE
    // ========================================================

    void toggleFavorite()
    {
        int id;

        cout << "\n";
        cout << "============================================\n";
        cout << "          MARK / UNMARK FAVORITE\n";
        cout << "============================================\n";

        cout << "Enter Contact ID: ";

        if (!(cin >> id))
        {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << "\nInvalid ID!\n";
            return;
        }

        for (Contact& c : contacts)
        {
            if (c.getID() == id)
            {
                c.toggleFavorite();

                saveToFile();

                if (c.isFavorite())
                {
                    cout << "\nContact added to favorites!\n";
                }
                else
                {
                    cout << "\nContact removed from favorites!\n";
                }

                return;
            }
        }

        cout << "\nContact not found!\n";
    }


    // ========================================================
    //                  VIEW FAVORITES
    // ========================================================

    void viewFavorites()
    {
        cout << "\n";
        cout << "============================================\n";
        cout << "             FAVORITE CONTACTS\n";
        cout << "============================================\n";

        bool found = false;

        for (const Contact& c : contacts)
        {
            if (c.isFavorite())
            {
                c.display();

                found = true;
            }
        }

        if (!found)
        {
            cout << "No favorite contacts available.\n";
        }
    }


    // ========================================================
    //                    STATISTICS
    // ========================================================

    void showStatistics()
    {
        cout << "\n";
        cout << "============================================\n";
        cout << "             ADDRESS BOOK STATS\n";
        cout << "============================================\n";

        cout << "Total Contacts : "
             << contacts.size() << endl;

        int favorites = 0;

        for (const Contact& c : contacts)
        {
            if (c.isFavorite())
                favorites++;
        }

        cout << "Favorites      : "
             << favorites << endl;


        int family = 0;
        int friends = 0;
        int work = 0;
        int college = 0;
        int emergency = 0;


        for (const Contact& c : contacts)
        {
            string category =
                toLowerCase(c.getCategory());

            if (category == "family")
                family++;

            else if (category == "friends")
                friends++;

            else if (category == "work")
                work++;

            else if (category == "college")
                college++;

            else if (category == "emergency")
                emergency++;
        }


        cout << "\nCategory-wise Count:\n";

        cout << "Family     : "
             << family << endl;

        cout << "Friends    : "
             << friends << endl;

        cout << "Work       : "
             << work << endl;

        cout << "College     : "
             << college << endl;

        cout << "Emergency  : "
             << emergency << endl;
    }


    // ========================================================
    //                    BACKUP CONTACTS
    // ========================================================

    void backupContacts()
    {
        // Save latest data first
        saveToFile();

        ifstream source(dataFile, ios::binary);

        ofstream destination(
            backupFile,
            ios::binary
        );

        if (!source || !destination)
        {
            cout << "\nBackup failed!\n";
            return;
        }

        destination << source.rdbuf();

        source.close();
        destination.close();

        cout << "\nBackup created successfully!\n";

        cout << "Backup file: "
             << backupFile << endl;
    }


    // ========================================================
    //                    CSV ESCAPE
    // ========================================================

    string csvEscape(string value)
    {
        bool needsQuotes = false;

        if (value.find(',') != string::npos ||
            value.find('"') != string::npos)
        {
            needsQuotes = true;
        }

        string result;

        for (char c : value)
        {
            if (c == '"')
                result += "\"\"";
            else
                result += c;
        }

        if (needsQuotes)
        {
            result = "\"" + result + "\"";
        }

        return result;
    }


    // ========================================================
    //                    EXPORT TO CSV
    // ========================================================

    void exportToCSV()
    {
        ofstream outfile(exportFile);

        if (!outfile)
        {
            cout << "\nUnable to create CSV file.\n";
            return;
        }

        outfile
            << "ID,Name,Phone,Email,Address,"
            << "Category,Notes,Favorite,Created At\n";


        for (const Contact& c : contacts)
        {
            outfile
                << c.getID() << ","
                << csvEscape(c.getName()) << ","
                << csvEscape(c.getPhone()) << ","
                << csvEscape(c.getEmail()) << ","
                << csvEscape(c.getAddress()) << ","
                << csvEscape(c.getCategory()) << ","
                << csvEscape(c.getNotes()) << ","
                << (c.isFavorite() ? "Yes" : "No") << ","
                << csvEscape(c.getCreatedAt())
                << "\n";
        }

        outfile.close();

        cout << "\nContacts exported successfully!\n";

        cout << "CSV file: "
             << exportFile << endl;
    }


    // ========================================================
    //                      MAIN MENU
    // ========================================================

    void run()
    {
        int choice;

        do
        {
            cout << "\n\n";

            cout << "============================================\n";
            cout << "          SMART ADDRESS BOOK SYSTEM\n";
            cout << "============================================\n";

            cout << "1.  Add Contact\n";
            cout << "2.  View All Contacts\n";
            cout << "3.  Search Contact\n";
            cout << "4.  Update Contact\n";
            cout << "5.  Delete Contact\n";
            cout << "6.  Sort Contacts by Name\n";
            cout << "7.  View Contacts by Category\n";
            cout << "8.  Mark / Unmark Favorite\n";
            cout << "9.  View Favorite Contacts\n";
            cout << "10. View Statistics\n";
            cout << "11. Backup Contacts\n";
            cout << "12. Export Contacts to CSV\n";
            cout << "13. Exit\n";

            cout << "============================================\n";

            cout << "Enter your choice: ";


            // Validate menu input
            if (!(cin >> choice))
            {
                cin.clear();

                cin.ignore(10000, '\n');

                cout << "\nInvalid input!";
                cout << " Please enter a number.\n";

                continue;
            }


            switch (choice)
            {
                case 1:
                    addContact();
                    break;


                case 2:
                    viewContacts();
                    break;


                case 3:
                    searchContact();
                    break;


                case 4:
                    updateContact();
                    break;


                case 5:
                    deleteContact();
                    break;


                case 6:
                    sortContacts();
                    break;


                case 7:
                    viewByCategory();
                    break;


                case 8:
                    toggleFavorite();
                    break;


                case 9:
                    viewFavorites();
                    break;


                case 10:
                    showStatistics();
                    break;


                case 11:
                    backupContacts();
                    break;


                case 12:
                    exportToCSV();
                    break;


                case 13:

                    // Save before exiting
                    saveToFile();

                    cout << "\n";
                    cout << "============================================\n";
                    cout << " Thank you for using Smart Address Book!\n";
                    cout << "============================================\n";

                    break;


                default:

                    cout << "\nInvalid choice!";

                    cout << " Please enter a number between 1 and 13.\n";
            }

        }
        while (choice != 13);
    }
};


// ============================================================
//                         MAIN FUNCTION
// ============================================================

int main()
{
    AddressBook addressBook;

    addressBook.run();

    return 0;
}