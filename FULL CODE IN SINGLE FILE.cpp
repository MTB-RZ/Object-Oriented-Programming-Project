#include <iostream>
#include <fstream>
using namespace std;

class Violation;

void generateFine();
void generateTicketFileName(int ticketId, char* fileName);
void appendArray(char* base, char* add);
void intToChar(int num, char* result);
void copyArray(char* source, char*& destination);
void displayArray(char* array);
void inputString(char*& buffer, int size);
int inputIntRange(int min, int max);
int loadTicketCounter();
void saveTicketCounter(int id);
int loadViolations(const char* fileName, Violation*& list);
void showDriverHistory();
void showTicketById();
void checkViolationHistory();
void normalizeString(char* str);
bool sameString(char* a, char* b);
bool driverExistsAndValid(long int id, char* name, char*& ticketsOut);
void updateDriverFile(long int id, char* name, int newTicketId);
bool officerExists(long int id, char*& nameOut);
void updateOfficerFile(long int id, char* name);
void addViolation();
void removeViolation();

class Person {
protected:
    char* name;
    long int id;
public:
    Person() {
        name = nullptr;
        id = 0;
    }

    Person(char* n, long int i) {
        id = i;
        name = nullptr;
        copyArray(n, name);
    }

    Person(const Person& other) {
        id = other.id;
        name = nullptr;
        if (other.name) {
            copyArray(other.name, name);
        }
    }

    Person& operator=(const Person& other) {
        if (this != &other) {
            delete[] name;
            name = nullptr;
            id = other.id;
            if (other.name) {
                copyArray(other.name, name);
            }
        }
        return *this;
    }

    ~Person() {
        delete[] name;
    }

    char* getName()const {
        return name;
    }

    long int getId()const {
        return id;
    }
};

class Driver : public Person {
public:
    Driver() : Person() {}
    Driver(char* n, long int i) : Person(n, i) {}
};

class Officer : public Person {
public:
    Officer() : Person() {}
    Officer(char* n, long int i) : Person(n, i) {}
};

class Violation {
protected:
    char* name;
    int fine;
public:
    Violation() {
        name = nullptr;
        fine = 0;
    }

    Violation(char* n, int f) {
        fine = f;
        name = nullptr;
        copyArray(n, name);
    }

    Violation(const Violation& other) {
        fine = other.fine;
        name = nullptr;
        if (other.name) {
            copyArray(other.name, name);
        }
    }

    Violation& operator=(const Violation& other) {
        if (this != &other) {
            delete[] name;
            name = nullptr;
            fine = other.fine;
            if (other.name) {
                copyArray(other.name, name);
            }
        }
        return *this;
    }

    ~Violation() {
        delete[] name;
    }

    char* getName()const {
        return name;
    }

    int getFine()const {
        return fine;
    }
};

class Vehicle {
protected:
    long int vehicleId;
    char* modelName;
public:
    Vehicle(long int id, char* model) {
        vehicleId = id;
        modelName = nullptr;
        copyArray(model, modelName);
    }

    virtual ~Vehicle() {
        delete[] modelName;
    }

    long int getVehicleId() {
        return vehicleId;
    }

    char* getModelName() {
        return modelName;
    }

    virtual char* getType() = 0;
};

class Car : public Vehicle {
public:
    Car(long int id, char* model) : Vehicle(id, model) {}
    char* getType() {
        return (char*)"Car";
    }
};

class Bike : public Vehicle {
public:
    Bike(long int id, char* model) : Vehicle(id, model) {}
    char* getType() {
        return (char*)"Bike";
    }
};

class Ticket {
    int ticketId;
    int paid;

public:
    Ticket(int id, int status = 0) {
        ticketId = id;
        paid = status;
    }

    int getId()const {
        return ticketId;
    }

    int isPaid()const {
        return paid;
    }

    void markPaid() {
        if (paid) {
            cout << "Ticket " << ticketId << " is already PAID." << endl;
            return;
        }
        else {
            int totalTickets = loadTicketCounter();
            int** ticketData = new int* [totalTickets];
            for (int i = 0; i < totalTickets; i++) {
                ticketData[i] = new int[2];
            }

            ifstream in("all_tickets.txt");
            if (in) {
                for (int i = 0; i < totalTickets; i++) {
                    in >> ticketData[i][0] >> ticketData[i][1];
                }
                in.close();
            }

            ticketData[ticketId - 1][1] = 1;
            paid = 1;

            ofstream out("all_tickets.txt");
            for (int i = 0; i < totalTickets; i++) {
                out << ticketData[i][0] << "\t" << ticketData[i][1] << endl;
            }
            out.close();

            for (int i = 0; i < totalTickets; i++) {
                delete[] ticketData[i];
            }
            delete[] ticketData;

            cout << "Ticket " << ticketId << " marked as PAID." << endl;
            updateTicketFileStatus();
        }
    }

    void saveToAllTicketsFile() {
        ofstream out("all_tickets.txt", ios::app);
        out << ticketId << "\t" << paid << endl;
        out.close();
    }

private:
    void updateTicketFileStatus() {
        char fileName[30];
        generateTicketFileName(ticketId, fileName);

        ifstream in(fileName);
        if (!in) {
            cout << "Ticket file not found." << endl;
            return;
        }
        else {
            char** lines = nullptr;
            int lineCount = 0;
            char buffer[200];

            while (in.getline(buffer, 200)) {
                char* temp = new char[200];
                int i = 0;
                while (buffer[i] != '\0') {
                    temp[i] = buffer[i];
                    i++;
                }
                temp[i] = '\0';

                char** newLines = new char* [lineCount + 1];
                for (int j = 0; j < lineCount; j++) {
                    newLines[j] = lines[j];
                }
                newLines[lineCount] = temp;
                if (lines) {
                    delete[] lines;
                }
                lines = newLines;

                lineCount++;
            }
            in.close();

            if (lineCount > 0) {
                delete[] lines[lineCount - 1];
                char* statusLine = new char[20];
                char statusText[] = "Status: PAID";
                int i = 0;
                while (statusText[i] != '\0') {
                    statusLine[i] = statusText[i];
                    i++;
                }
                statusLine[i] = '\0';
                lines[lineCount - 1] = statusLine;
            }

            ofstream out(fileName);
            for (int i = 0; i < lineCount; i++) {
                out << lines[i] << endl;
                delete[] lines[i];
            }
            delete[] lines;
            out.close();
        }
    }
};

class Payment {
public:
    void markTicketPaid(int ticketId) {
        Ticket t(ticketId);
        t.markPaid();
    }
};

class TrafficDepartment {
protected:
    Payment payment;

    bool checkPassword() {
        char* input = nullptr;
        cout << "Enter Admin Password: ";
        inputString(input, 20);

        ifstream file("password.txt");
        char* stored = nullptr;
        if (!file) {
            cout << "Password file missing. Please create password.txt with admin password." << endl;
            delete[] input;
            return false;
        }
        else {
            char buffer[20];
            file >> buffer;
            copyArray(buffer, stored);
            file.close();

            bool valid = sameString(input, stored);
            if (!valid) {
                cout << "Incorrect password!" << endl;
            }

            delete[] input;
            delete[] stored;
            return valid;
        }
    }

    void changePassword() {
        char* oldPass = nullptr;
        char* newPass = nullptr;

        cout << "Enter old password: ";
        inputString(oldPass, 20);

        ifstream file("password.txt");
        char buffer[20];
        file >> buffer;
        file.close();

        char* stored = nullptr;
        copyArray(buffer, stored);

        if (!sameString(oldPass, stored)) {
            cout << "Old password incorrect!" << endl;
            delete[] oldPass;
            delete[] stored;
            return;
        }
        else {
            cout << "Enter new password (max 20 chars): ";
            newPass = new char[20];
            cin.getline(newPass, 20, '\n');

            ofstream out("password.txt");
            out << newPass;
            out.close();

            cout << "Password changed successfully." << endl;

            delete[] oldPass;
            delete[] newPass;
            delete[] stored;
        }
    }

    void payFineOption() {
        int ticketId;
        cout << "Enter Ticket ID to mark as PAID: ";
        ticketId = inputIntRange(1, loadTicketCounter());
        payment.markTicketPaid(ticketId);
    }

    void addDriver() {
        char* name = nullptr;
        cout << "Driver Name: ";
        inputString(name, 100);

        cout << "Driver ID: ";
        long int id = inputIntRange(100, 9999);

        char* tickets = nullptr;
        driverExistsAndValid(id, name, tickets);

        updateDriverFile(id, name, 0);
        cout << "Driver added successfully." << endl;

        delete[] name;
        delete[] tickets;
    }

    void addOfficer() {
        char* name = nullptr;
        cout << "Officer Name: ";
        inputString(name, 100);

        cout << "Officer ID: ";
        long int id = inputIntRange(100, 9999);

        char* existing = nullptr;
        if (officerExists(id, existing)) {
            cout << "Officer updated." << endl;
            delete[] existing;
        }
        else {
            cout << "New officer added." << endl;
        }

        updateOfficerFile(id, name);
        delete[] name;
    }
    void deleteDriverOrOfficer() {
        int choice;
        cout << "1. Delete Driver" << endl;
        cout << "2. Delete Officer" << endl;
        cout << "Select option: ";
        choice = inputIntRange(1, 2);

        if (choice == 1) {
            long int id;
            cout << "Enter Driver ID to delete: ";
            id = inputIntRange(100, 9999);

            ifstream in("drivers.txt");
            char** names = nullptr;
            char** tickets = nullptr;
            long int* ids = nullptr;
            int count = 0;

            char bufferName[100], bufferTickets[200];
            long int bufferId;

            while (in.getline(bufferName, 100, '\t')) {
                in >> bufferId;
                in.ignore();
                in.getline(bufferTickets, 200);

                char** newNames = new char* [count + 1];
                char** newTickets = new char* [count + 1];
                long int* newIds = new long int[count + 1];

                for (int i = 0; i < count; i++) {
                    newIds[i] = ids[i];
                    newNames[i] = names[i];
                    newTickets[i] = tickets[i];
                }

                newIds[count] = bufferId;
                newNames[count] = new char[100];
                copyArray(bufferName, newNames[count]);
                newTickets[count] = new char[200];
                copyArray(bufferTickets, newTickets[count]);

                if (names) delete[] names;
                if (tickets) delete[] tickets;
                if (ids) delete[] ids;

                names = newNames;
                tickets = newTickets;
                ids = newIds;
                count++;
            }
            in.close();

            ofstream out("drivers.txt");
            bool found = false;
            for (int i = 0; i < count; i++) {
                if (ids[i] != id) {
                    out << names[i] << "\t" << ids[i] << "\t" << tickets[i] << endl;
                }
                else {
                    found = true;
                }
                delete[] names[i];
                delete[] tickets[i];
            }
            delete[] names;
            delete[] tickets;
            delete[] ids;
            out.close();

            if (found) {
                cout << "Driver deleted." << endl;
            }
            else {
                cout << "Driver not found." << endl;
            }

        }
        else {
            long int id;
            cout << "Enter Officer ID to delete: ";
            id = inputIntRange(100, 9999);

            ifstream in("officer.txt");
            char** names = nullptr;
            long int* ids = nullptr;
            int count = 0;

            char bufferName[100];
            long int bufferId;

            while (in.getline(bufferName, 100, '\t')) {
                in >> bufferId;
                in.ignore();

                char** newNames = new char* [count + 1];
                long int* newIds = new long int[count + 1];

                for (int i = 0; i < count; i++) {
                    newNames[i] = names[i];
                    newIds[i] = ids[i];
                }

                newNames[count] = new char[100];
                copyArray(bufferName, newNames[count]);
                newIds[count] = bufferId;

                if (names) delete[] names;
                if (ids) delete[] ids;

                names = newNames;
                ids = newIds;
                count++;
            }
            in.close();

            ofstream out("officer.txt");
            bool found = false;
            for (int i = 0; i < count; i++) {
                if (ids[i] != id) {
                    out << names[i] << "\t" << ids[i] << endl;
                }
                else {
                    found = true;
                }
                delete[] names[i];
            }
            delete[] names;
            delete[] ids;
            out.close();

            if (found) {
                cout << "Officer deleted." << endl;
            }
            else {
                cout << "Officer not found." << endl;
            }
        }
    }

    void generateReports() {
        cout << "====== Reports ======" << endl;

        int totalTickets = loadTicketCounter();
        int paidCount = 0;
        int unpaidCount = 0;

        ifstream in("all_tickets.txt");
        for (int i = 0; i < totalTickets; i++) {
            int tid, status;
            in >> tid >> status;
            if (status == 1) {
                paidCount++;
            }
            else {
                unpaidCount++;
            }
        }
        in.close();

        cout << "Total Tickets: " << totalTickets << endl;
        cout << "Paid Tickets: " << paidCount << endl;
        cout << "Unpaid Tickets: " << unpaidCount << endl;
    }

    void viewAllDriversOrOfficers() {
        cout << endl << "View All:" << endl;
        cout << "1. Drivers" << endl;
        cout << "2. Officers" << endl;
        cout << "Select option: ";
        int choice = inputIntRange(1, 2);

        if (choice == 1) {
            ifstream in("drivers.txt");
            if (!in) {
                cout << "No drivers found." << endl;
                return;
            }
            else {
                cout << endl << "===== All Drivers =====" << endl;
                char name[100], tickets[200];
                long int id;
                while (in.getline(name, 100, '\t')) {
                    in >> id;
                    in.ignore();
                    in.getline(tickets, 200);
                    cout << "Name: " << name << endl;
                    cout << "ID: " << id << endl;
                    cout << "Tickets: " << tickets << endl;
                    cout << "-------------------------" << endl;
                }
                in.close();
            }
        }
        else {
            ifstream in("officer.txt");
            if (!in) {
                cout << "No officers found." << endl;
                return;
            }
            else {
                cout << endl << "===== All Officers =====" << endl;
                char name[100];
                long int id;
                while (in.getline(name, 100, '\t')) {
                    in >> id;
                    in.ignore();
                    cout << "Name: " << name << endl;
                    cout << "ID: " << id << endl;
                    cout << "-------------------------" << endl;
                }
                in.close();
            }
        }
    }

public:
    void adminMenu() {
        if (!checkPassword()) {
            return;
        }

        int choice;
        do {
            cout << endl << "====== ADMIN PANEL ======" << endl;
            cout << "1. Pay Fine" << endl;
            cout << "2. Add Driver" << endl;
            cout << "3. Add Officer" << endl;
            cout << "4. Delete Driver/Officer" << endl;
            cout << "5. View all Drivers/Officers" << endl;
            cout << "6. All Ticket Report" << endl;
            cout << "7. Add Violation" << endl;
            cout << "8. Remove Violation" << endl;
            cout << "9. Change Password" << endl;
            cout << "0. Back to Main Menu" << endl;
            cout << endl << "Please Enter Your choice: ";
            choice = inputIntRange(0, 9);

            switch (choice) {
            case 1: {
                payFineOption();
                break;
            }
            case 2: {
                addDriver();
                break;
            }
            case 3: {
                addOfficer();
                break;
            }
            case 4: {
                deleteDriverOrOfficer();
                break;
            }
            case 5: {
                viewAllDriversOrOfficers();
                break;
            }
            case 6: {
                generateReports();
                break;
            }
            case 7: {
                addViolation();
                break;
            }
            case 8: {
                removeViolation();
                break;
            }
            case 9: {
                changePassword();
                break;
            }
            }
        } while (choice != 0);
    }
};

int main() {
    int choice;
    do {
        cout << endl << "====== TRAFFIC FINE SYSTEM ======" << endl;
        cout << "1. Generate Fine" << endl;
        cout << "2. Check Violation History" << endl;
        cout << "3. Admin Panel" << endl;
        cout << "4. Exit" << endl;
        cout << "Select Option: ";
        choice = inputIntRange(1, 4);

        switch (choice) {
        case 1: {
            generateFine();
            break;
        }
        case 2: {
            checkViolationHistory();
            break;
        }
        case 3: {
            TrafficDepartment td;
            td.adminMenu();
            break;
        }
        }
    } while (choice != 4);

    return 0;
}

int loadTicketCounter() {
    ifstream file("ticket_counter.txt");
    int last = 0;
    if (file) {
        file >> last;
    }
    file.close();
    return last;
}

void saveTicketCounter(int id) {
    ofstream file("ticket_counter.txt");
    file << id;
    file.close();
}

int inputIntRange(int min, int max) {
    char buffer[20];
    int num;
    bool valid;

    do {
        cin >> buffer;
        valid = true;
        num = 0;

        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] < '0' || buffer[i] > '9') {
                valid = false;
                break;
            }
            num = num * 10 + (buffer[i] - '0');
        }

        if (!valid || num < min || num > max) {
            cout << "Invalid input. Enter between " << min << " and " << max << ": ";
            valid = false;
        }
    } while (!valid);

    return num;
}

void copyArray(char* source, char*& destination) {
    if (destination) {
        delete[] destination;
    }
    int len = 0;
    while (source[len] != '\0') {
        len++;
    }
    destination = new char[len + 1];
    for (int i = 0; i < len; i++) {
        destination[i] = source[i];
    }
    destination[len] = '\0';
}

void displayArray(char* array) {
    for (int i = 0; array[i] != '\0'; i++) {
        cout << array[i];
    }
}

void intToChar(int num, char* result) {
    int i = 0;
    if (num == 0) {
        result[i++] = '0';
    }
    while (num > 0) {
        result[i++] = (num % 10) + '0';
        num /= 10;
    }
    result[i] = '\0';
    for (int s = 0, e = i - 1; s < e; s++, e--) {
        char t = result[s];
        result[s] = result[e];
        result[e] = t;
    }
}

void appendArray(char* base, char* add) {
    int i = 0, j = 0;
    while (base[i] != '\0') {
        i++;
    }
    while (add[j] != '\0') {
        base[i++] = add[j++];
    }
    base[i] = '\0';
}

void normalizeString(char* str) {
    if (!str || str[0] == '\0') {
        return;
    }

    if (str[0] >= 'a' && str[0] <= 'z') {
        str[0] -= 32;
    }

    for (int i = 1; str[i] != '\0'; i++) {
        if (str[i - 1] == ' ' && str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
        else if (str[i - 1] != ' ' && str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 32;
        }
    }
}

bool sameString(char* a, char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return true;
}

void inputString(char*& newArr, int size) {
    char* newInput = new char[size];
    cin.ignore(100, '\n');
    cin.getline(newInput, size, '\n');
    int len = 0;
    while (newInput[len] != '\0') {
        len++;
    }
    delete[] newArr;
    newArr = new char[len + 1];
    for (int i = 0; i <= len; i++) {
        newArr[i] = newInput[i];
    }
    delete[] newInput;
    newInput = nullptr;
}

int loadViolations(const char* fileName, Violation*& list) {
    ifstream file(fileName);
    if (!file) {
        return 0;
    }

    int count = 0;
    char line[200];

    while (file.getline(line, 200)) {
        count++;
    }
    file.close();

    list = new Violation[count];

    ifstream file2(fileName);
    for (int i = 0; i < count; i++) {
        char name[100];
        int fine;

        file2.getline(name, 100, '\t');
        file2 >> fine;
        file2.ignore(1000, '\n');

        list[i] = Violation(name, fine);
    }
    file2.close();

    return count;
}

bool driverExistsAndValid(long int id, char* name, char*& ticketsOut) {
    ticketsOut = nullptr;
    char** names = nullptr;
    long int* ids = nullptr;
    char** tickets = nullptr;
    int count = 0;

    ifstream in("drivers.txt");
    if (in) {
        char tempName[100], tempTickets[200];
        long int tempId;

        while (in.getline(tempName, 100, '\t')) {
            in >> tempId;
            in.ignore();
            in.getline(tempTickets, 200);

            char** newNames = new char* [count + 1];
            long int* newIds = new long int[count + 1];
            char** newTickets = new char* [count + 1];

            for (int i = 0; i < count; i++) {
                newIds[i] = ids[i];

                newNames[i] = nullptr;
                copyArray(names[i], newNames[i]);

                newTickets[i] = nullptr;
                copyArray(tickets[i], newTickets[i]);
            }

            newIds[count] = tempId;

            newNames[count] = nullptr;
            copyArray(tempName, newNames[count]);

            newTickets[count] = nullptr;
            copyArray(tempTickets, newTickets[count]);

            for (int i = 0; i < count; i++) {
                delete[] names[i];
                delete[] tickets[i];
            }
            delete[] names;
            delete[] ids;
            delete[] tickets;

            names = newNames;
            ids = newIds;
            tickets = newTickets;
            count++;
        }
        in.close();
    }

    for (int i = 0; i < count; i++) {
        if (ids[i] == id) {
            normalizeString(names[i]);
            if (!sameString(names[i], name)) {
                cout << "Driver ID exists with a different name." << endl;
                for (int j = 0; j < count; j++) {
                    delete[] names[j];
                    delete[] tickets[j];
                }
                delete[] names;
                delete[] ids;
                delete[] tickets;
                return false;
            }

            copyArray(tickets[i], ticketsOut);

            for (int j = 0; j < count; j++) {
                delete[] names[j];
                delete[] tickets[j];
            }
            delete[] names;
            delete[] ids;
            delete[] tickets;
            return true;
        }
    }

    ticketsOut = new char[1];
    ticketsOut[0] = '\0';

    for (int j = 0; j < count; j++) {
        delete[] names[j];
        delete[] tickets[j];
    }
    delete[] names;
    delete[] ids;
    delete[] tickets;

    return true;
}

void updateDriverFile(long int id, char* name, int newTicketId) {
    char** names = nullptr;
    long int* ids = nullptr;
    char** tickets = nullptr;
    int count = 0;

    ifstream in("drivers.txt");
    if (in) {
        char tempName[100], tempTickets[200];
        long int tempId;

        while (in.getline(tempName, 100, '\t')) {
            in >> tempId;
            in.ignore();
            in.getline(tempTickets, 200);

            char** newNames = new char* [count + 1];
            long int* newIds = new long int[count + 1];
            char** newTickets = new char* [count + 1];

            for (int i = 0; i < count; i++) {
                newIds[i] = ids[i];

                newNames[i] = nullptr;
                copyArray(names[i], newNames[i]);

                newTickets[i] = nullptr;
                copyArray(tickets[i], newTickets[i]);
            }

            newIds[count] = tempId;

            newNames[count] = nullptr;
            copyArray(tempName, newNames[count]);

            newTickets[count] = nullptr;
            copyArray(tempTickets, newTickets[count]);

            for (int i = 0; i < count; i++) {
                delete[] names[i];
                delete[] tickets[i];
            }
            delete[] names;
            delete[] ids;
            delete[] tickets;

            names = newNames;
            ids = newIds;
            tickets = newTickets;
            count++;
        }
        in.close();
    }

    bool found = false;
    for (int i = 0; i < count; i++) {
        if (ids[i] == id) {
            if (newTicketId > 0) {
                int len = 0;
                while (tickets[i][len] != '\0') {
                    len++;
                }

                char* newTicketsStr = new char[len + 12];
                int j = 0;
                for (; j < len; j++) {
                    newTicketsStr[j] = tickets[i][j];
                }
                if (len > 0) {
                    newTicketsStr[j++] = ',';
                }
                char t[10];
                intToChar(newTicketId, t);
                int k = 0;
                while (t[k] != '\0') {
                    newTicketsStr[j++] = t[k++];
                }
                newTicketsStr[j] = '\0';

                delete[] tickets[i];
                tickets[i] = newTicketsStr;
            }
            found = true;
            break;
        }
    }

    if (!found) {
        char** newNames = new char* [count + 1];
        long int* newIds = new long int[count + 1];
        char** newTickets = new char* [count + 1];

        for (int i = 0; i < count; i++) {
            newIds[i] = ids[i];

            newNames[i] = nullptr;
            copyArray(names[i], newNames[i]);

            newTickets[i] = nullptr;
            copyArray(tickets[i], newTickets[i]);
        }

        newIds[count] = id;

        newNames[count] = nullptr;
        copyArray(name, newNames[count]);

        newTickets[count] = new char[10];
        intToChar(newTicketId, newTickets[count]);

        for (int i = 0; i < count; i++) {
            delete[] names[i];
            delete[] tickets[i];
        }
        delete[] names;
        delete[] ids;
        delete[] tickets;

        names = newNames;
        ids = newIds;
        tickets = newTickets;
        count++;
    }

    ofstream out("drivers.txt");
    for (int i = 0; i < count; i++) {
        out << names[i] << "\t" << ids[i] << "\t" << tickets[i] << endl;
        delete[] names[i];
        delete[] tickets[i];
    }
    delete[] names;
    delete[] ids;
    delete[] tickets;
}

bool officerExists(long int id, char*& nameOut) {
    nameOut = nullptr;
    ifstream in("officer.txt");
    if (!in) {
        return false;
    }

    char tempName[100];
    long int tempId;

    while (in.getline(tempName, 100, '\t')) {
        in >> tempId;
        in.ignore();
        if (tempId == id) {
            nameOut = nullptr;
            copyArray(tempName, nameOut);
            in.close();
            return true;
        }
    }
    in.close();
    return false;
}

void updateOfficerFile(long int id, char* name) {
    ofstream out("officer.txt", ios::app);
    out << name << "\t" << id << endl;
    out.close();
}

void generateTicketFileName(int ticketId, char* fileName) {
    char base[] = "TICKET_";
    char ext[] = ".txt";
    char idStr[10];

    int i = 0;
    while (base[i] != '\0') {
        fileName[i] = base[i];
        i++;
    }
    fileName[i] = '\0';

    intToChar(ticketId, idStr);
    appendArray(fileName, idStr);
    appendArray(fileName, ext);
}
void generateFine() {
    char* dName = nullptr;
    cout << "Driver Name: ";
    inputString(dName, 100);
    normalizeString(dName);

    cout << "Driver ID: ";
    long int dId = inputIntRange(100, 9999);

    char* oldTickets = nullptr;
    if (!driverExistsAndValid(dId, dName, oldTickets)) {
        delete[] dName;
        delete[] oldTickets;
        return;
    }

    cout << "Officer ID: ";
    long int oId = inputIntRange(100, 9999);

    char* oName = nullptr;
    if (officerExists(oId, oName)) {
        cout << "Officer \"";
        displayArray(oName);
        cout << "\" found." << endl;
    }
    else {
        cout << "Officer Name: ";
        inputString(oName, 100);
        normalizeString(oName);
        updateOfficerFile(oId, oName);
    }

    Officer* officer = new Officer(oName, oId);

    cout << "Vehicle Type (1.Car 2.Bike): ";
    int vt = inputIntRange(1, 2);

    cout << "Vehicle ID: ";
    long int vId = inputIntRange(0, 9999);

    char* vModel = nullptr;
    cout << "Vehicle Model: ";
    inputString(vModel, 100);
    normalizeString(vModel);

    Vehicle* v;
    if (vt == 1) {
        v = (Vehicle*)new Car(vId, vModel);
    }
    else {
        v = (Vehicle*)new Bike(vId, vModel);
    }

    Violation* violations = nullptr;
    int count;
    if (vt == 1) {
        count = loadViolations("violations_car.txt", violations);
    }
    else {
        count = loadViolations("violations_bike.txt", violations);
    }

    cout << endl << "Select Violation:" << endl;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". ";
        displayArray(violations[i].getName());
        cout << " (Rs " << violations[i].getFine() << ")" << endl;
    }

    int choice = inputIntRange(1, count);

    int ticketId = loadTicketCounter() + 1;
    saveTicketCounter(ticketId);

    updateDriverFile(dId, dName, ticketId);

    char fileName[30];
    generateTicketFileName(ticketId, fileName);

    ofstream all("all_tickets.txt", ios::app);
    all << ticketId << "\t0" << endl;
    all.close();

    ofstream ticket(fileName);
    ticket << "Ticket ID: " << ticketId << endl;
    ticket << "Driver: " << dName << " (" << dId << ")" << endl;
    ticket << "Vehicle: " << v->getType() << " | ID: " << v->getVehicleId() << " | Model: " << v->getModelName() << endl;
    ticket << "Violation: " << violations[choice - 1].getName() << endl;
    ticket << "Fine: Rs " << violations[choice - 1].getFine() << endl;
    ticket << "Officer: " << officer->getName() << " (" << officer->getId() << ")" << endl;
    ticket << "Status: UNPAID" << endl;
    ticket.close();

    cout << "Ticket Generated: ";
    displayArray(fileName);
    cout << endl;

    delete[] oldTickets;
    delete[] dName;
    delete[] vModel;
    delete[] violations;
    delete[] oName;
    delete officer;
    delete v;
}

void showTicketById() {
    cout << "Enter Ticket ID: ";
    int tId = inputIntRange(1, loadTicketCounter());

    char fileName[30];
    generateTicketFileName(tId, fileName);

    ifstream in(fileName);
    if (!in) {
        cout << "Ticket with ID " << tId << " not found." << endl;
        return;
    }

    char ch;
    cout << endl << "===== Ticket Details =====" << endl;
    while (in.get(ch)) {
        cout << ch;
    }
    cout << "===========================" << endl;
    in.close();
}

void showDriverHistory() {
    char* dName = nullptr;
    cout << "Driver Name: ";
    inputString(dName, 100);
    normalizeString(dName);

    cout << "Driver ID: ";
    long int dId = inputIntRange(100, 9999);

    char* tickets = nullptr;
    if (!driverExistsAndValid(dId, dName, tickets)) {
        delete[] dName;
        delete[] tickets;
        return;
    }

    if (tickets[0] == '\0') {
        cout << "No tickets found for this driver." << endl;
    }
    else {
        cout << "Tickets for " << dName << " (" << dId << "): " << tickets << endl;

        char tStr[20];
        int i = 0, j = 0;
        while (true) {
            if (tickets[i] == ',' || tickets[i] == '\0') {
                tStr[j] = '\0';
                i++;
                j = 0;

                int tId = 0;
                for (int k = 0; tStr[k] != '\0'; k++) {
                    tId = tId * 10 + (tStr[k] - '0');
                }

                char fileName[30];
                generateTicketFileName(tId, fileName);

                ifstream in(fileName);
                if (in) {
                    char ch;
                    cout << endl << "Ticket ID: " << tId << endl;
                    cout << "--------------------------" << endl;
                    while (in.get(ch)) {
                        cout << ch;
                    }
                    cout << "--------------------------" << endl;
                    in.close();
                }
                else {
                    cout << "Ticket file " << fileName << " missing." << endl;
                }

                if (tickets[i - 1] == '\0') {
                    break;
                }
            }
            else {
                tStr[j++] = tickets[i++];
            }
        }
    }

    delete[] dName;
    delete[] tickets;
}

void checkViolationHistory() {
    cout << endl << "Search violation history by:" << endl;
    cout << "1. Driver Name & ID" << endl;
    cout << "2. Ticket ID" << endl;
    cout << "Select Option: ";
    int choice = inputIntRange(1, 2);

    if (choice == 1) {
        showDriverHistory();
    }
    else {
        showTicketById();
    }
}

void addViolation() {
    cout << "Add Violation For:" << endl;
    cout << "1. Car" << endl << "2. Bike" << endl;
    int type = inputIntRange(1, 2);

    const char* fileName;
    if (type == 1) {
        fileName = "violations_car.txt";
    }
    else {
        fileName = "violations_bike.txt";
    }

    char* name = nullptr;
    cout << "Violation detail: ";
    inputString(name, 100);
    normalizeString(name);

    cout << "Fine Amount (100 to 10000 PKR): ";
    int fine = inputIntRange(100, 10000);

    ofstream out(fileName, ios::app);
    out << name << "\t" << fine << endl;
    out.close();

    cout << "Violation added successfully." << endl;
    delete[] name;
}

void removeViolation() {
    cout << "Remove Violation From:" << endl;
    cout << "1. Car" << endl << "2. Bike" << endl;
    cout << endl << "Please Enter your choice: ";
    int type = inputIntRange(1, 2);

    const char* fileName;
    if (type == 1) {
        fileName = "violations_car.txt";
    }
    else {
        fileName = "violations_bike.txt";
    }

    Violation* list = nullptr;
    int count = loadViolations(fileName, list);

    if (count == 0) {
        cout << "No violations found." << endl;
        return;
    }

    cout << endl << "Violations List:" << endl;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". ";
        displayArray(list[i].getName());
        cout << " (Rs " << list[i].getFine() << ")" << endl;
    }

    cout << endl << "Select violation to remove: ";
    int choice = inputIntRange(1, count);

    ofstream out(fileName);
    for (int i = 0; i < count; i++) {
        if (i != choice - 1) {
            out << list[i].getName() << "\t" << list[i].getFine() << endl;
        }
    }
    out.close();

    delete[] list;
    cout << "Violation removed successfully." << endl;
}