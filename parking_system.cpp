// Colluci Smart Parking System - Kenya
// Interactive Version

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;
using namespace std::chrono;

const int TOTAL_SLOTS = 20;
const string CURRENCY = "KES";

struct Slot {
    int id;
    bool occupied = false;
    string plate;
};

struct ParkedVehicle {
    string plate;
    system_clock::time_point entryTime;
    int slotId;
};

int calculateFee(long long minutes) {
    if (minutes <= 30)  return 0;
    if (minutes <= 120) return 50;
    if (minutes <= 240) return 100;
    if (minutes <= 360) return 300;
    return 500;
}

class ParkingSystem {
private:
    vector<Slot> slots;
    map<string, ParkedVehicle> parked;

    string timeToString(system_clock::time_point tp) {
        time_t t = system_clock::to_time_t(tp);
        tm tm{};
        localtime_s(&tm, &t);
        ostringstream oss;
        oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

public:
    ParkingSystem() {
        slots.resize(TOTAL_SLOTS);
        for (int i = 0; i < TOTAL_SLOTS; ++i) {
            slots[i].id = i + 1;
        }
    }

    void displayAvailability() const {
        cout << "\n========== PARKING AVAILABILITY ==========\n";
        int freeCount = 0;
        for (const auto& s : slots) {
            if (s.occupied)
                cout << "[X] Slot " << setw(2) << s.id << " : " << s.plate << "\n";
            else {
                cout << "[ ] Slot " << setw(2) << s.id << " : FREE\n";
                freeCount++;
            }
        }
        cout << "------------------------------------------\n";
        cout << "Free slots: " << freeCount << " / " << TOTAL_SLOTS << "\n";
        cout << "==========================================\n";
    }

    bool vehicleEntry(const string& plate) {
        if (parked.count(plate)) {
            cout << "[ERROR] Vehicle " << plate << " is already parked.\n";
            return false;
        }

        auto it = find_if(slots.begin(), slots.end(),
                          [](const Slot& s) { return !s.occupied; });

        if (it == slots.end()) {
            cout << "[ERROR] Parking is FULL. No free slots.\n";
            return false;
        }

        it->occupied = true;
        it->plate = plate;

        ParkedVehicle v;
        v.plate = plate;
        v.entryTime = system_clock::now();
        v.slotId = it->id;
        parked[plate] = v;

        cout << "\n[ENTRY SUCCESS]\n";
        cout << "Plate     : " << plate << "\n";
        cout << "Slot      : " << it->id << "\n";
        cout << "Entry Time: " << timeToString(v.entryTime) << "\n";
        cout << "[BARRIER] Entry barrier OPENED. Welcome!\n";
        return true;
    }

    bool vehicleExit(const string& plate) {
        auto it = parked.find(plate);
        if (it == parked.end()) {
            cout << "[ERROR] Vehicle " << plate << " not found in the parking.\n";
            return false;
        }

        auto exitTime = system_clock::now();
        auto duration = duration_cast<minutes>(exitTime - it->second.entryTime);
        long long mins = max(0LL, duration.count());
        int fee = calculateFee(mins);

        cout << "\n=============== EXIT RECEIPT ===============\n";
        cout << "Plate          : " << plate << "\n";
        cout << "Entry Time     : " << timeToString(it->second.entryTime) << "\n";
        cout << "Exit Time      : " << timeToString(exitTime) << "\n";
        cout << "Duration       : " << mins << " minutes\n";
        cout << "Amount to Pay  : " << fee << " " << CURRENCY << "\n";
        cout << "============================================\n";

        cout << "Processing payment of " << fee << " " << CURRENCY << "... ";
        cout << "PAID SUCCESSFULLY!\n";

        int slotId = it->second.slotId;
        slots[slotId - 1].occupied = false;
        slots[slotId - 1].plate.clear();
        parked.erase(it);

        cout << "[EXIT] Slot " << slotId << " is now FREE.\n";
        cout << "[BARRIER] Exit barrier OPENED. Thank you!\n";
        return true;
    }

    void listParkedVehicles() const {
        cout << "\nCurrently Parked Vehicles:\n";
        if (parked.empty()) {
            cout << "  (No vehicles currently parked)\n";
        } else {
            for (const auto& p : parked) {
                cout << "  " << p.first << "  -->  Slot " << p.second.slotId << "\n";
            }
        }
    }
};

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    ParkingSystem park;
    int choice;
    string plate;

    cout << "=============================================\n";
    cout << "     COLLUCI SMART PARKING SYSTEM - KENYA\n";
    cout << "=============================================\n";

    while (true) {
        cout << "\n----------- MAIN MENU -----------\n";
        cout << "1. View Available Slots\n";
        cout << "2. Vehicle Entry\n";
        cout << "3. Vehicle Exit & Payment\n";
        cout << "4. List Parked Vehicles\n";
        cout << "5. Exit Program\n";
        cout << "---------------------------------\n";
        cout << "Enter your choice (1-5): ";

        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                park.displayAvailability();
                break;

            case 2:
                cout << "Enter vehicle number plate: ";
                clearInput();
                getline(cin, plate);
                if (!plate.empty())
                    park.vehicleEntry(plate);
                else
                    cout << "Plate cannot be empty.\n";
                break;

            case 3:
                cout << "Enter vehicle number plate: ";
                clearInput();
                getline(cin, plate);
                if (!plate.empty())
                    park.vehicleExit(plate);
                else
                    cout << "Plate cannot be empty.\n";
                break;

            case 4:
                park.listParkedVehicles();
                break;

            case 5:
                cout << "\nThank you for using Colluci Smart Parking. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please select 1-5.\n";
        }
    }

    return 0;
}