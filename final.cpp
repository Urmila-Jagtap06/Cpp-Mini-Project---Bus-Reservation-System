#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <vector>
using namespace std;

class Bus
{
public:
    int seatCapacity_;
    vector<bool> seatStatus_;

    int bus_num;
    string driver, from, to;
    int ARRIVAL, DEPARTURE;
    long long phone_number;
    int deleted_bus_number = -1;

public:
    Bus(int capacity)
    {
        seatCapacity_ = capacity;
        seatStatus_ = vector<bool>(capacity, false);
    }

    ~Bus()
    {
        cout << "\nDone Successfully !" << endl;
    }

    void add_bus()
    {
        cout << "***************************************************************************************************************" << endl;
        cout << " Please fill reservation details" << endl;

        cout << "Enter Bus no (4 digits): ";
        while (!(cin >> bus_num) || bus_num < 1000 || bus_num > 9999)
        {
            cout << "Invalid input. Enter a valid 4-digit bus number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter Driver name: ";
        cin.ignore();
        getline(cin, driver);
        while (!isAlphabetic(driver))
        {
            cout << "Invalid input. Enter a valid driver name: ";
            getline(cin, driver);
        }

        cout << "Enter source: ";
        getline(cin, from);
        while (!isAlphabetic(from))
        {
            cout << "Invalid input. Enter a valid source: ";
            getline(cin, from);
        }

        cout << "Enter destiny: ";
        getline(cin, to);
        while (!isAlphabetic(to))
        {
            cout << "Invalid input. Enter a valid destiny: ";
            getline(cin, to);
        }

        cout << "Enter ARRIVAL (24HR format): ";
        while (!(cin >> ARRIVAL) || ARRIVAL < 0 || ARRIVAL > 25)
        {
            cout << "Invalid input. Enter a valid 24HR format time: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter DEPARTURE (24HR format): ";
        while (!(cin >> DEPARTURE) || DEPARTURE < 0 || DEPARTURE > 25)
        {
            cout << "Invalid input. Enter a valid 24HR format time: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter Phone Number: ";
        while (!(cin >> phone_number) || to_string(phone_number).length() != 10)
        {
            cout << "Invalid input. Enter a valid 10-digit phone number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Your details saved successfully....!" << endl;
        cout << "***************************************************************************************************************" << endl;

        fstream f("BUS.txt", ios::app);
        if (!f.is_open())
        {
            cerr << "Error opening file 'BUS.txt'. Exiting..." << endl;
            exit(EXIT_FAILURE);
        }

        f << bus_num << " " << driver << " " << from << " " << to << " " << DEPARTURE << " " << ARRIVAL << " " << phone_number << "\n";
        f.close();
    }
    // Add this method to the Bus class
    bool reserveBus()
    {
        int busNumber;
        cout << "Enter the bus number to reserve: ";
        cin >> busNumber;

        fstream f("BUS.txt", ios::in | ios::out);

        if (!f.is_open())
        {
            cerr << "Error opening file 'BUS.txt'. Exiting..." << endl;
            exit(EXIT_FAILURE);
        }

        bool found = false;
        while (f >> bus_num >> driver >> from >> to >> DEPARTURE >> ARRIVAL >> phone_number)
        {
            if (bus_num == busNumber)
            {
                found = true;
                break;
            }
        }

        f.close();

        if (!found)
        {
            cout << "Bus with number " << busNumber << " not found. Reservation failed." << endl;
            return false;
        }

        displayAvailableSeats();
        reserveSeat();
        // displayAvailableSeats();

        return true;
    }

    void display_info()
    {
        cout << setw(8) << "BUS NO"
             << "|" << setw(12) << "SOURCE"
             << "|" << setw(13) << "DESTINATION"
             << "|" << setw(8) << "DRIVER"
             << "|" << setw(9) << "ARRIVAL"
             << "|" << setw(11) << "DEPARTURE"
             << "|" << setw(12) << "PHONE NO"
             << "|" << endl;

        fstream f("BUS.txt", ios::in | ios::out);
        if (!f.is_open())
        {
            cerr << "Error opening file 'BUS.txt'. Exiting..." << endl;
            exit(EXIT_FAILURE);
        }

        fstream temp_file("temp.txt", ios::out);
        while (f >> bus_num >> driver >> from >> to >> DEPARTURE >> ARRIVAL >> phone_number)
        {
            if (bus_num != deleted_bus_number)
            {
                temp_file << bus_num << " " << driver << " " << from << " " << to << " " << DEPARTURE << " " << ARRIVAL << " " << phone_number << endl;
            }
            else
            {
                continue;
            }
        }

        f.close();
        temp_file.close();
        remove("BUS.txt");
        rename("temp.txt", "BUS.txt");

        f.open("BUS.txt", ios::in);
        while (f >> bus_num >> driver >> from >> to >> DEPARTURE >> ARRIVAL >> phone_number)
        {
            cout << setw(8) << bus_num << "|" << setw(12) << from << "|" << setw(13) << to << "|" << setw(8) << driver << "|" << setw(9) << ARRIVAL << "|" << setw(11) << DEPARTURE << "|" << setw(12) << phone_number << "|" << endl;
        }

        f.close();
    }

    void displayAvailableSeats()
    {
        cout << "Available Seats: ";
        for (int i = 0; i < seatStatus_.size(); ++i)
        {
            if (!seatStatus_[i])
            {
                cout << i + 1 << ", ";
            }
        }
        cout << endl;
    }

    bool reserveSeat(int seatNumber)
    {
        if (seatNumber < 1 || seatNumber > seatStatus_.size())
        {
            cout << "Invalid seat number. Please choose a seat between 1 and " << seatStatus_.size() << "." << endl;
            return false;
        }

        if (seatStatus_[seatNumber - 1])
        {
            cout << "Seat " << seatNumber << " is already booked. Please choose another seat." << endl;
            return false;
        }

        seatStatus_[seatNumber - 1] = true;
        cout << "Seat " << seatNumber << " booked successfully!" << endl;
        return true;
    }

    void reserveSeat()
    {
        cout << "Enter the seat number you want to reserve: ";
        int chosenSeat;
        cin >> chosenSeat;

        reserveSeat(chosenSeat);
    }

    void delete_bus()
    {
        int bus_number_to_delete;

        cout << "Enter the bus number to delete: ";
        cin >> bus_number_to_delete;
        fstream f("BUS.txt", ios::in | ios::out);

        if (!f.is_open())
        {
            cerr << "Error opening file 'BUS.txt'. Exiting..." << endl;
            exit(EXIT_FAILURE);
        }

        fstream temp_file("temp.txt", ios::out);
        while (f >> bus_num >> driver >> from >> to >> DEPARTURE >> ARRIVAL >> phone_number)
        {
            if (bus_num != bus_number_to_delete)
            {
                temp_file << bus_num << " " << driver << " " << from << " " << to << " " << DEPARTURE << " " << ARRIVAL << " " << phone_number << endl;
            }
            else
            {
                deleted_bus_number = bus_num;
            }
        }

        f.close();
        temp_file.close();
        remove("BUS.txt");
        rename("temp.txt", "BUS.txt");

        cout << "Bus " << bus_number_to_delete << " deleted successfully!" << endl;
    }

    bool isAlphabetic(const string &str)
    {
        for (char c : str)
        {
            if (!isalpha(c) && c != ' ')
            {
                return false;
            }
        }
        return true;
    }
};

int main()
{
    fstream fileCheck("BUS.txt");
    if (!fileCheck.is_open())
    {
        cout << "File 'BUS.txt' does not exist. Creating the file..." << endl;
        ofstream createFile("BUS.txt");
        createFile.close();
    }
    fileCheck.close();

    Bus bus(32);
    int ch = 1;

    cout << "\n\t\t\tWELCOME TO BUS RESERVATION SYSTEM MAHARASTRA ! \n\n\n";
    while (ch != 3)
    {
        cout << "\n\t\t\t\t-----------------";
        cout << "\n\t\t\t\t| 1. Add Bus   |"
             << "\n\t\t\t\t| 2. Display   |"
             << "\n\t\t\t\t| 3. Exit      |"
             << "\n\t\t\t\t| 4. Reserve Seat |"
             << "\n\t\t\t\t| 5. Delete Bus |"
             << "\n\t\t\t\t-----------------"
             << "\n\t\t\t\t"
             << "\n\t\t\t\t"
             << "\n\t\t\t\tEnter your choice-> ";
        cin >> ch;
        cout << endl;

        switch (ch)
        {
        case 1:
            bus.add_bus();
            break;
        case 2:
            cout << "***************************************************************************************************************" << endl;
            cout << " STAFF MEMBER AND BUS INFORMATION" << endl;
            cout << "***************************************************************************************************************" << endl;
            bus.display_info();
            cout << endl;
            break;
        case 3:
            cout << "Exiting the program." << endl;
            break;
        case 4:
            bus.reserveBus();
            bus.displayAvailableSeats();
            break;
        case 5:
            bus.delete_bus();
            break;
        case 6:
            // bus.reserveBus();
            break;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
        }
    }

    return 0;
}
