#include <iostream>
#include <string>
#include <mysql.h>
using namespace std;

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

void connectDB() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        cerr << "mysql_init() failed\n";
        exit(EXIT_FAILURE);
    }

    // Replace "root", "your_password", "hospital_db" as needed
    if (mysql_real_connect(conn, "localhost", "root", "krushna", "hospital_db", 0, NULL, 0) == NULL) {
        cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }
}

void addPatient() {
    string name, gender, disease;
    int age;

    cin.ignore();
    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter age: ";
    cin >> age;
    cin.ignore();
    cout << "Enter gender: ";
    getline(cin, gender);
    cout << "Enter disease: ";
    getline(cin, disease);

    string query = "INSERT INTO patients(name, age, gender, disease) VALUES('" +
                   name + "'," + to_string(age) + ",'" + gender + "','" + disease + "')";

    if (mysql_query(conn, query.c_str())) {
        cerr << "Insert failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Patient added successfully!\n";
    }
}

void viewPatients() {
    if (mysql_query(conn, "SELECT * FROM patients")) {
        cerr << "Query failed: " << mysql_error(conn) << endl;
        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL) {
        cerr << "Result error: " << mysql_error(conn) << endl;
        return;
    }

    cout << "\n--- Patient List ---\n";
    while ((row = mysql_fetch_row(res))) {
        cout << "ID: " << row[0]
             << " | Name: " << row[1]
             << " | Age: " << row[2]
             << " | Gender: " << row[3]
             << " | Disease: " << row[4] << endl;
    }

    mysql_free_result(res);
}

void updatePatient() {
    int id;
    string disease;
    cout << "Enter patient ID to update disease: ";
    cin >> id;
    cin.ignore();
    cout << "Enter new disease: ";
    getline(cin, disease);

    string query = "UPDATE patients SET disease='" + disease +
                   "' WHERE patient_id=" + to_string(id);

    if (mysql_query(conn, query.c_str())) {
        cerr << "Update failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Patient disease updated successfully!\n";
    }
}

void deletePatient() {
    int id;
    cout << "Enter patient ID to delete: ";
    cin >> id;

    string query = "DELETE FROM patients WHERE patient_id=" + to_string(id);

    if (mysql_query(conn, query.c_str())) {
        cerr << "Delete failed: " << mysql_error(conn) << endl;
    } else {
        cout << "Patient record deleted.\n";
    }
}

int main() {
    connectDB();

    int choice;
    do {
        cout << "\n--- Hospital Management System ---\n";
        cout << "1. Add Patient\n";
        cout << "2. View Patients\n";
        cout << "3. Update Patient Disease\n";
        cout << "4. Delete Patient\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: updatePatient(); break;
            case 4: deletePatient(); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 5);

    mysql_close(conn);
    return 0;
}
