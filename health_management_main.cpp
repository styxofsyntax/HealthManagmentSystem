#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <ctime>

using namespace std;

enum colors
{
    black = 0,
    red = 4,
    grey = 8,
    light_red = 0xC,
    blue = 1,
    purple = 5,
    light_blue = 9,
    light_purple = 0xD,
    green = 2,
    yellow = 6,
    light_green = 0xA,
    light_yellow = 0xE,
    aqua = 3,
    white = 7,
    light_aqua = 0xB,
    bright_white = 0xF
};

struct patient
{
    string id, name, dateBirth, contact, city, gndr, cnic;

    void input()
    {
        cout << "Patient Name: ";
        getline(cin, name);
        cout << "CNIC: ";
        getline(cin, cnic);
        cout << "Gender: ";
        getline(cin, gndr);
        cout << "Date of Birth: ";
        getline(cin, dateBirth);
        cout << "Contact Number: ";
        getline(cin, contact);
        cout << "City: ";
        getline(cin, city);
    }
};

int login();
int adminPortal();
int staffPortal(int &);
int patientEntry();
int patientSearch(int &);
int accountSearch(int &);
int patientFileView();
int accountFileView();
int signUp();
void deleteFile(int);

void patientDataDelete(string &);
void patientDataEdit(string &);
void accountDataDelete(string &);
void accountDataEdit(string &);
void username_generate(string &, char &, string &);
void password_generate(string &);
string id_generator(char);

long long fetch_last_id();
char slctInput(char, char);
void textColor(int);
void buffer(string, int);

/*
function returning 0 means exit is chosen
0 in switch cases is used to indicate exit

adminPortal user = admin, password admin123
staffPortal user = staff, password staff123
*/

int main()
{
    int loginID = -1;
    int slct;
    string id;

    textColor(light_yellow);
    cout << "press 1 for help menu.\n";
    cout << "Press 2 to proceed.\n";

    if (slctInput('1', '2') == '1')
    {
        system("CLS");
        textColor(aqua);
        cout << "\tGuidelines: \n\n";

        textColor(light_green);
        cout << "Admin Credentials\n";
        textColor(white);

        cout << "username: admin\npasword: admin123\n";
        cout << "\nPressing the option button automatically selects the option, there is no need to press enter.";

        textColor(aqua);
        cout << "\n\tFunctionalities: \n\n";
        textColor(white);

        cout << "There are mainly 2 types of logins, admin and staff.\n"
             << "Admin has access to all functionalities while staff is limited to some.\n";

        textColor(light_green);
        cout << "\nPatient Related:\n";
        textColor(white);
        cout << "1. Entry\n2. Search\n\tView Data\n\tEdit Data\n\tDelete Data (Admin Only)\n";
        cout << "3. All Data View\n4. All Data Delete (Admin Only)\n";
        cout << "For searching patient id is to be entered which is generated upon patient entry.\n";

        textColor(light_green);
        cout << "\nStaff Account Related: (Admin Only)\n";
        textColor(white);
        cout << "1. Account Creation\n\tCreates a username based upon departement.\n\tCreates a random password of length 8.\n";
        cout << "2. Search\n\tView Data\n\tEdit Password\n\tDelete Data\n";
        cout << "3. All Data View\n4. All Data Delete\n";

        cout << "\npress any key to proceed: ";
        _getch();
    }

    while (true) // loop used to cycle through menus back and forth
    {
        do
        {
            if (loginID == -1)
            {
                loginID = login();
            }
            switch (loginID)
            {
            case 0:
                slct = adminPortal();
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                slct = staffPortal(loginID);
                break;
            case -1:
                textColor(light_red);
                cout << "\nIncorrect username or password, please try again!";
                Sleep(1000);
                system("CLS");
                break;
            }
        } while (loginID == -1);

        switch (slct)
        {
        case 1:
            patientEntry();
            break;
        case 2:
            patientSearch(loginID);
            break;
        case 3:
            patientFileView();
            break;
        case 4:
            deleteFile(1);
            break;
        case 5:
            signUp();
            break;
        case 6:
            accountSearch(loginID);
            break;
        case 7:
            accountFileView();
            break;
        case 8:
            deleteFile(2);
            break;
        case 0: // loginID is set to -1 so code can enter previous menu
            loginID = -1;
            break;
        }
    }
    return 0;
}

int login()
{
    string adminPass = "admin123", staffPass = "staff123";
    string user;
    char pass[17] = {0};
    int passLength = 0;
    bool correct_login = false;

    system("CLS");

    textColor(yellow);

    for (int i = 0; i < 43; i++)
        cout << '\xcd';
    cout << '\n';
    cout << "|---Welcome to Health Management System---|\n";
    for (int i = 0; i < 43; i++)
        cout << '\xcd';
    cout << '\n';

    textColor(aqua);
    cout << "\n"
         << "\tUser Login Section\n";
    textColor(white);

    cout << "Username: ";
    getline(cin, user);
    cout << "Password: ";

    while (true)
    {
        pass[passLength] = _getch(); // instantly fetches the character as it is typed

        if (passLength < 16 && pass[passLength] > 32 && pass[passLength] < 126) // asterik is printed and index is incremented only when valid character is added
        {
            cout << '*';
            passLength++;
        }

        else if (passLength > 0) // when index is zero 'backspace' and 'enter' do not work
                                 // so that anything other than the password cannot be erased
                                 // also prevents entering null password
        {
            if (pass[passLength] == 13) // when enter key pressed loop breaks
            {
                pass[passLength] = '\0';
                break;
            }

            if (pass[passLength] == 8) // when backspace key pressed asterik removed and array counter decremented
            {
                passLength--;
                cout << "\b \b"; // 'space' overwrites asterik
            }
        } // else if()
    }     // while()

    if (user == "admin" && pass == adminPass)
        return 0;

    // accounts from file are checked
    ifstream accData("user_data_file.txt");
    if (accData.is_open())
    {
        string user_saved, pass_saved;
        while (getline(accData, user_saved))
        {
            getline(accData, pass_saved, (char)3);
            if (user == user_saved && pass == pass_saved)
            {
                cout << "\n EQUAL\n";
                correct_login = true;
                break;
            }
        }
        accData.close();
    }
    else
    {
        textColor(light_red);
        cout << "\nError: \"user_data_file\" file not found!\n";
        textColor(white);
    }
    if (correct_login)
    {
        int i = 0;
        while (user[i] != '.')
        {
            i++;
        }
        char dept[2];
        dept[0] = user[i + 1];
        dept[1] = user[i + 2];

        if (strcmp(dept, "\\cd"))
            return 1;
        else if (strcmp(dept, "\\md"))
            return 2;
        else if (strcmp(dept, "\\su"))
            return 3;
        else if (strcmp(dept, "\\ne"))
            return 4;
        else if (strcmp(dept, "\\sk"))
            return 5;
    }

    return -1; // wrong credentials
}

int adminPortal()
{
    system("CLS");
    textColor(aqua);
    cout << "\tAdmin Portal\n\n";
    textColor(white);

    cout << "1. Patient Entry\n";
    cout << "2. Patient Search\n";
    cout << "3. Patient All Data View\n";
    cout << "4. Patient All Data Delete\n";
    cout << "5. Account Creation\n";
    cout << "6. Account Search\n";
    cout << "7. Account All Data View\n";
    cout << "8. Account All Data Delete\n";
    cout << "0. Exit\n";

    return slctInput('0', '8') - 48;
} // adminPortal()

int staffPortal(int &login_type)
{
    system("CLS");
    textColor(aqua);
    cout << "\tStaff Portal\n\n";
    textColor(white);

    cout << "1. Patient Entry\n";
    cout << "2. Search\n";
    cout << "3. Patient Data View\n";
    cout << "0. Exit\n";

    return slctInput('0', '3') - 48;
} // staffPortal()

int patientEntry()
{
    patient p;

    system("CLS");
    textColor(aqua);
    cout << "\tPatient Data Entry\n\n";
    textColor(white);

    p.input();

    system("CLS");
    textColor(aqua);
    cout << "\tDepartement Referral\n\n";
    textColor(white);

    cout << "1. Department of Cardialogy.\n";
    cout << "2. Department of Medicine.\n";
    cout << "3. Department of Surguery.\n";
    cout << "4. Department of Neurology.\n";
    cout << "5. Department of Skin.\n";
    cout << "0. Exit.\n";

    char slct = slctInput('0', '5');

    p.id = id_generator(slct); // generating id for patient

    // saving patient data into file
    ofstream dataFile_write;
    dataFile_write.open("patient_data_file.txt", ios::app);

    dataFile_write << p.id << '\n';
    dataFile_write << p.name << '\n';
    dataFile_write << p.cnic << '\n';
    dataFile_write << p.gndr << '\n';
    dataFile_write << p.dateBirth << '\n';
    dataFile_write << p.contact << '\n';
    dataFile_write << p.city << '$';
    dataFile_write.close();

    system("CLS");

    switch (slct)
    {
    case '1':
        cout << "Referred to Department of Cardiology.";
        break;
    case '2':
        cout << "Referred to Department of Medicine.";
        break;
    case '3':
        cout << "Referred to Department of Surgery.";
        break;
    case '4':
        cout << "Referred to Department of Neurology.";
        break;
    case '5':
        cout << "Referred to Department of Skin.";
        break;
    case '0':
        return 0;
    }

    textColor(light_yellow);
    cout << "\nData Saved!\n";
    textColor(white);
    cout << "Press any key to exit: ";
    _getch();
    textColor(white);

    cout << "\n";

} // patientEntry()

int patientSearch(int &login_type)
{
    patient p;

    system("CLS");
    textColor(aqua);
    cout << "\tSearch Menu\n\n";
    textColor(white);

    string search_id;
    cout << "Enter the id of patient: ";
    getline(cin, search_id);

    system("CLS");

    buffer("Searching", 3);

    char high = '2';
    cout << "\33[2K\r"; // clears one line
    bool found = false;
    fstream dataFile;
    dataFile.open("patient_data_file.txt", ios::in);

    if (dataFile.is_open())
    {
        while (getline(dataFile, p.id))
        {
            getline(dataFile, p.name);
            getline(dataFile, p.cnic);
            getline(dataFile, p.gndr);
            getline(dataFile, p.dateBirth);
            getline(dataFile, p.contact);
            getline(dataFile, p.city, '$');
            if (p.id == search_id)
            {
                found = true;
                break;
            }
        }
    }
    else
    {
        textColor(light_red);
        cout << "\nError: \"patient_data_file\" file not found!\n";
        textColor(white);
    }
    dataFile.close();

    if (found)
        cout << "User Found!\n\n";
    else
    {
        cout << "User Not Found!\n\n";
        cout << "Press any key to exit: ";
        _getch();
        return 0;
    }

    Sleep(800);

    cout << "1. View Data\n2. Edit Data\n";
    if (login_type == 0) // Delete option is only accessible to admin
    {
        high = '3';
        cout << "3. Delete Data\n";
    }
    cout << "0. Exit\n";
    switch (slctInput('0', high))
    {
    case '1':
        system("CLS");
        textColor(aqua);
        cout << "\tPatient Data\n\n";
        textColor(white);
        cout << "Name: " << p.name << "\n";
        cout << "CNIC: " << p.cnic << "\n";
        cout << "Gender: " << p.gndr << "\n";
        cout << "Date of birth: " << p.dateBirth << "\n";
        cout << "Contact: " << p.contact << "\n";
        cout << "City: " << p.city << "\n\n";
        break;
    case '2':
        patientDataEdit(search_id);
        break;
    case '3':
        patientDataDelete(search_id);
        break;
    case '0':
        textColor(white);
        return 0;
    }
    textColor(white);
    cout << "Press any key to exit: ";
    _getch();
} // patientSearch()

int accountSearch(int &login_type)
{
    system("CLS");
    textColor(aqua);
    cout << "\tSearch Menu\n\n";
    textColor(white);

    string search_username, saved_username, saved_pass;
    cout << "Enter the username of account: ";
    getline(cin, search_username);

    system("CLS");

    buffer("Searching", 3);

    char high = '2';
    cout << "\33[2K\r"; // clears one line
    bool found = false;
    fstream dataFile;
    dataFile.open("user_data_file.txt", ios::in);

    if (dataFile.is_open())
    {
        while (getline(dataFile, saved_username))
        {
            getline(dataFile, saved_pass, (char)3);
            if (saved_username == search_username)
            {
                found = true;
                break;
            }
        }
    }
    else
    {
        textColor(light_red);
        cout << "\nError: \"user_data_file\" file not found!\n";
        textColor(white);
    }
    dataFile.close();

    if (found)
        cout << "Account Found!\n\n";
    else
    {
        cout << "Account Not Found!\n\n";
        cout << "Press any key to exit: ";
        _getch();
        return 0;
    }

    Sleep(800);

    cout << "1. View Data\n2. Edit Password\n";
    if (login_type == 0) // Delete option is only accessible to admin
    {
        high = '3';
        cout << "3. Delete Data\n";
    }
    cout << "0. Exit\n";
    switch (slctInput('0', high))
    {
    case '1':
        system("CLS");
        textColor(aqua);
        cout << "\tAccount Data\n\n";
        textColor(white);
        cout << "Username: " << search_username << "\n";
        cout << "password: " << saved_pass << "\n\n";
        break;
    case '2':
        accountDataEdit(search_username);
        break;
    case '3':
        accountDataDelete(search_username);
        break;
    case '0':
        textColor(white);
        return 0;
    }
    textColor(white);
    cout << "Press any key to exit: ";
    _getch();
}

int signUp()
{
    system("CLS");
    string name, username, pass;
    char slct;

    textColor(aqua);
    cout << "\tSignUp Menu\n\n";
    textColor(white);
    cout << "Enter name: ";
    getline(cin, name);

    int i = 0;
    while (name[i] != '\0')
    {
        i++;

        if (!((name[i - 1] == ' ') || (name[i - 1] >= 'A' && name[i - 1] <= 'Z') || (name[i - 1] >= 'a' && name[i - 1] <= 'z')))
        {
            // invalid username if any character is found other than alphabets (capital and small) and space
            system("CLS");
            textColor(aqua);
            cout << "\tSignUp Menu\n\n";
            textColor(light_red);
            cout << "\nError: invalid characters in name!\n";
            textColor(white);
            cout << "Enter name: ";
            getline(cin, name);
            i = 0;
        }
    }

    cout << "\nChoose departement: \n\n";

    cout << "1. Department of Cardialogy.\n";
    cout << "2. Department of Medicine.\n";
    cout << "3. Department of Surguery.\n";
    cout << "4. Department of Neurology.\n";
    cout << "5. Department of Skin.\n";

    slct = slctInput('1', '5');
    username_generate(name, slct, username);
    password_generate(pass);
    cout << "\ngenerated username is: " << username << '\n';
    cout << "\ngenerated password is: " << pass << '\n';

    ofstream userFile;
    userFile.open("user_data_file.txt", ios::app);
    userFile << username << '\n';
    userFile << pass << (char)3;
    userFile.close();

    system("pause");
    return 0;
}

int accountFileView()
{
    system("CLS");
    textColor(aqua);
    cout << "\tAccount Data View\n\n";
    textColor(white);

    ifstream dataFile_read;
    dataFile_read.open("user_data_file.txt");
    string saved_username, saved_pass;

    if (dataFile_read.is_open())
    {
        while (getline(dataFile_read, saved_username))
        {
            getline(dataFile_read, saved_pass, (char)3);
            cout << "username: " << saved_username << "\n";
            cout << "password: " << saved_pass << "\n";
            for (int i = 0; i < 43; i++)
                cout << '\xcd';
            cout << "\n\n";
        }
    }
    else
    {
        textColor(light_red);
        cout << "\nError: \"user_login_data\" file not found!\n";
        textColor(white);
    }
    dataFile_read.close();
    cout << "Press any key to exit: ";
    _getch();
    return 0;
}

int patientFileView()
{
    system("CLS");
    textColor(aqua);
    cout << "\tPatient Data View\n\n";
    textColor(white);

    ifstream dataFile_read;
    dataFile_read.open("patient_data_file.txt");
    patient p;

    if (dataFile_read.is_open())
    {
        while (getline(dataFile_read, p.id))
        {
            getline(dataFile_read, p.name);
            getline(dataFile_read, p.cnic);
            getline(dataFile_read, p.gndr);
            getline(dataFile_read, p.dateBirth);
            getline(dataFile_read, p.contact);
            getline(dataFile_read, p.city, '$');
            cout << "Name: " << p.name << "\n";
            cout << "CNIC: " << p.cnic << "\n";
            cout << "Gender: " << p.gndr << "\n";
            cout << "Date of birth: " << p.dateBirth << "\n";
            cout << "Contact: " << p.contact << "\n";
            cout << "City :" << p.city << "\n";
            for (int i = 0; i < 43; i++)
                cout << '\xcd';
            cout << "\n\n";
        }
    }
    else
    {
        textColor(light_red);
        cout << "\nError: \"patient_data_file\" file not found!\n";
        textColor(white);
    }
    dataFile_read.close();

    cout << "Press any key to exit: ";
    _getch();

    return 0;
}

void patientDataEdit(string &edit_id)
{
    fstream tempFile("temp_edit.txt", ios::out);
    fstream dataFile("patient_data_file.txt", ios::in);
    patient p;

    system("CLS");
    textColor(aqua);
    cout << "\tEdit Patient Data\n\n";
    textColor(white);

    while (getline(dataFile, p.id))
    {
        if (p.id != edit_id)
        {
            getline(dataFile, p.name);
            getline(dataFile, p.cnic);
            getline(dataFile, p.gndr);
            getline(dataFile, p.dateBirth);
            getline(dataFile, p.contact);
            getline(dataFile, p.city, '$');
        }
        else
            p.input();

        tempFile << p.id << '\n';
        tempFile << p.name << '\n';
        tempFile << p.cnic << '\n';
        tempFile << p.gndr << '\n';
        tempFile << p.dateBirth << '\n';
        tempFile << p.contact << '\n';
        tempFile << p.city << '$';
    }

    tempFile.close();
    dataFile.close();

    remove("patient_data_file.txt");
    rename("temp_edit.txt", "patient_data_file.txt");
}

void patientDataDelete(string &delete_id)
{
    fstream tempFile("temp_del.txt", ios::out);
    fstream dataFile("patient_data_file.txt", ios::in);
    patient p;

    system("CLS");
    textColor(aqua);
    cout << "\tDelete Patient Data\n\n";
    textColor(white);

    while (getline(dataFile, p.id))
    {
        getline(dataFile, p.name);
        getline(dataFile, p.cnic);
        getline(dataFile, p.gndr);
        getline(dataFile, p.dateBirth);
        getline(dataFile, p.contact);
        getline(dataFile, p.city, '$');

        if (p.id != delete_id)
        {
            tempFile << p.id << '\n';
            tempFile << p.name << '\n';
            tempFile << p.cnic << '\n';
            tempFile << p.gndr << '\n';
            tempFile << p.dateBirth << '\n';
            tempFile << p.contact << '\n';
            tempFile << p.city << '$';
        }
    }

    tempFile.close();
    dataFile.close();

    remove("patient_data_file.txt");
    rename("temp_del.txt", "patient_data_file.txt");

    cout << "\33[2K\r";
    cout << "Patient Data Deleted!\n\n";
}

void accountDataEdit(string &edit_id)
{
    fstream tempFile("temp_edit.txt", ios::out);
    fstream dataFile("user_data_file.txt", ios::in);
    string saved_username, saved_password;
    char temp[16];

    system("CLS");
    textColor(aqua);
    cout << "\tEdit Account Password\n\n";
    textColor(white);

    while (getline(dataFile, saved_username))
    {
        getline(dataFile, saved_password, (char)3);
        if (saved_username == edit_id)
        {
            cout << "Enter New Password: ";
            cin.getline(temp, 16);
            saved_password = temp;
        }

        tempFile << saved_username << '\n';
        tempFile << saved_password << (char)3;
    }

    tempFile.close();
    dataFile.close();

    remove("user_data_file.txt");
    rename("temp_edit.txt", "user_data_file.txt");
}

void accountDataDelete(string &delete_id)
{
    system("CLS");
    textColor(aqua);
    cout << "\tDelete Account Data\n\n";

    textColor(light_red);
    buffer("Deleting Patient Data", 1);
    textColor(white);

    fstream tempFile("temp_del.txt", ios::out);
    fstream dataFile("user_data_file.txt", ios::in);
    string saved_username, saved_password;

    while (getline(dataFile, saved_username))
    {
        getline(dataFile, saved_password, (char)3);

        if (saved_username != delete_id)
        {
            tempFile << saved_username << '\n';
            tempFile << saved_password << (char)3;
        }
    }

    tempFile.close();
    dataFile.close();

    remove("user_data_file.txt");
    rename("temp_del.txt", "user_data_file.txt");

    cout << "\33[2K\r";
    cout << "Account Data Deleted!\n\n";
}

void deleteFile(int file)
{
    system("CLS");
    textColor(light_red);
    buffer("Deleting File", 1);
    textColor(white);
    ofstream dataFile;
    switch (file)
    {
    case 1:
        remove("patient_data_file.txt");
        dataFile.open("patient_data_file.txt");
        dataFile.close();
        break;
    case 2:
        remove("user_data_file.txt");
        dataFile.open("user_data_file.txt");
        dataFile.close();
        break;
    }

    cout << "\33[2K\r";
    cout << "File Deleted!\n\n";
    cout << "\npress any key to proceed: ";
    _getch();
}

void username_generate(string &name, char &slct, string &username)
{
    int spaces = 0;

    for (int i = 0; name[i] != '\0'; i++)
    {
        if (name[i] == ' ') // counts the total number of spaces in name.
            spaces++;
    }

    int i = 0;
    if (spaces != 0)
    {
        while (true)
        {
            if (spaces == 0)
                break;

            for (int j = 0; (j < 2 && name[i] != ' '); j++) // copies at max 2 characters of the non-last name
            {
                username += name[i];
                i++;
            }

            while (name[i] != ' ') // finds the index of space character in name array
                i++;

            spaces--; // decrementing space counter
            i++;      // moving to next character after space
        }
    }
    while (name[i] != '\0') // copying the last name fully
    {
        username += name[i];
        i++;
    }

    username += '.';

    switch (slct)
    {
    case '1':
        username += "cd";
        break;
    case '2':
        username += "md";
        break;
    case '3':
        username += "su";
        break;
    case '4':
        username += "ne";
        break;
    case '5':
        username += "sk";
    }
}

void password_generate(string &pass)
{
    const int size = 9;
    pass = "        "; // automatically adds NULL at the end
    char ascii;
    int i = 0;
    bool symbol = false;

    srand((unsigned)time(0));

    while (i < size - 1)
    {
        ascii = 33 + (rand() % 93);

        if (ascii >= '#' && ascii <= '&' && symbol == false)
        {
            symbol = true;
            pass[i] = ascii;
            i++;
        }
        else if (ascii >= '0' && ascii <= '9' || ascii >= 'A' && ascii <= 'Z' || ascii >= 'a' && ascii <= 'z')
        {
            pass[i] = ascii;
            i++;
        }
    }
}

string id_generator(char dept)
{
    time_t currTime;
    tm *currTm;
    char date[9];
    int j = 0;
    time(&currTime);
    currTm = localtime(&currTime);
    strftime(date, 9, "%d%m%Y", currTm);
    while (j < 2)
    {
        for (int i = 4; i < 8; i++)
        {
            date[i] = date[i + 1];
        }
        j++;
    }

    long long last_id;
    long long new_id;
    int serial_id = 0; // default value of serial is set to 0;
                       // if last id was not made on current date serial number starts from 0
                       // otherwise we increment it based on previous serial number

    last_id = fetch_last_id();
    new_id = stoll(date);

    if ((last_id / 10000) == stoll(date)) // if last id date is equal to current date
        serial_id = (last_id % 1000) + 1; // the serial number of last id is incremented by 1

    new_id = (new_id * 10) + (dept - 48); // departement number is stored after current date (takes 1 digit)

    new_id = (new_id * 1000) + serial_id; // serial id is saved after departement number (takes 3 digits)

    return to_string(new_id);
}

long long fetch_last_id()
{
    long long id, new_id;
    int chk;
    string temp;
    fstream dataFile;
    dataFile.open("patient_data_file.txt", ios::in);

    if (dataFile.is_open())
    {
        while (getline(dataFile, temp))
        {
            id = stoll(temp);
            for (int i = 0; i < 5; i++)
                getline(dataFile, temp);

            getline(dataFile, temp, '$');
        }
    }
    else
        cout << "\nError: \"patient_data_file\" file not found!\n";
    dataFile.close();
    return id;
}

char slctInput(char low, char high)
{
    char slct;
    slct = _getch();
    if (slct < low || slct > high)
    {
        textColor(light_red);
        cout << "Enter a valid option: ";
        textColor(white);

        while (slct < low || slct > high) // loop used to prompt user to enter valid char if input is invalid
        {
            slct = _getch();
        }
    }
    return slct;
} // slctInput

void textColor(int x)
{
    HANDLE console_color;
    console_color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_color, x);
}

void buffer(string str, int n)
{
    cout << str;
    for (int i = 0; i < n; i++)
    {
        cout << ".";
        Sleep(800);
    }
}