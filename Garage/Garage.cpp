#include <unordered_set>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;



class Vehicle {

protected:
    int id;

    static unordered_set<int> uniqueId;
public:
    
    static bool isIdUnique(int id);

    static const int UNDEFINED_ID = -1;

    Vehicle() {}

    Vehicle(int id) {
        if (isIdUnique(id)) {
            this->id = id;
        }
        else {
            cout << "This vehicle ID has already been registered\n";
            this->id = UNDEFINED_ID;
        }
    };

    int getVehicleId() const {
        return id;
    }

}; 

unordered_set<int> Vehicle::uniqueId;

bool Vehicle::isIdUnique(int id) {
    if (Vehicle::uniqueId.find(id) == Vehicle::uniqueId.end()) {
        Vehicle::uniqueId.insert(id);
        return true;
    }
    return false;
}


class MotorBike : public Vehicle {
public:

    MotorBike(int id): Vehicle(id) {
        
    }

};

class Car : public Vehicle {
public:
    Car(int id): Vehicle(id) {
     
    }

};


class Garage {
private:
    vector<int> spaceInEachLevels;
    int numOfLevels;
    int currentSlotIndex;
    int maxSlots;
    vector<vector<Vehicle*>> spaceState;

public:
    Garage() {
        int initLevels = 0;
        int initSpaces = 0;
        vector<int> initSpaceSlots = vector<int>(initLevels, initSpaces);
        Garage(initLevels, initSpaceSlots);
    }

    Garage(int numOfLevels, vector<int> spaceInEachLevels) {
        this->numOfLevels = numOfLevels;
        this->spaceInEachLevels = spaceInEachLevels;
        this->currentSlotIndex = 0;
        this->maxSlots = 0;
        
        for (int i = 0; i < numOfLevels; i++) {
            maxSlots += spaceInEachLevels[i];
        }
             
        for (int i = 0; i < this->numOfLevels; i++) {
            spaceState.push_back(vector<Vehicle*>(spaceInEachLevels[i], nullptr));
        }
    }

    ~Garage() {
        for (int i = 0; i < this->numOfLevels; i++) {
            for (unsigned int j = 0; j < spaceState[i].size(); j++)
                delete spaceState[i][j];
        }

        this->numOfLevels = 0;

        spaceInEachLevels = vector<int>();

        spaceState = vector < vector<Vehicle*>>();

        maxSlots = 0;
    };

    void enterGarage(Vehicle* vehicle) {
        if (this->currentSlotIndex >= maxSlots) {
            cout << "Sorry, the garage is currently at full capacity\n";
        }
        else if (vehicle->getVehicleId() == Vehicle::UNDEFINED_ID) {
            cout << "Sorry, there's been a vehicle with same Id\n";
        }
        else {
            for (int i = 0; i < numOfLevels; i++) {
                for (Vehicle* slot : spaceState[i]) {
                    if (slot == nullptr) {
                        slot = vehicle;
                        break;
                    }
                }
            }

            cout << "The vehicle has entered the garage with id " << vehicle->getVehicleId() << "\n";

            this->currentSlotIndex += 1;
        }
    }

    void exitGarage(int id) {

        for (int i = 0; i < numOfLevels; i++) {
            for (Vehicle* slot : spaceState[i]) {
                if (slot != nullptr && slot->getVehicleId() == id && id == -1) {
                    Vehicle* p = slot;
                    slot = nullptr;
                    delete p;
                    break;
                }
            }
        }

        cout << "The vehicle has been going out from garage with id " << id << '\n';
        this->currentSlotIndex -= 1;
    }
}; 

void prompt() {
    cout << "Press 1 to create the garage\n";
    cout << "Press 2 to get a vehicle in the garage\n";
    cout << "Press 3 to get a vehicle out from the garage\n";
    cout << "Press 4 to exit the program\n";
}

vector<int> getNumberOfSpaces(string spacesString) {
    int s;
    vector<int> spaces;
    stringstream iss(spacesString);

    while (iss >> s) {
        spaces.push_back(s);
    }
    return spaces;
}

Vehicle* getNewVehicle(char type, int id) {
    
    switch (type) {
    case 'c':
        return new Car(id);
        break;
    case 'm':
        return new MotorBike(id);
        break;
    default:
        return nullptr;
    }
    return nullptr;
}
 
int main() {
    int command;
    int level;
    string spacesStr;
    
    Garage g;

    bool exit = false;

    while (!exit) {
        prompt();
        cin >> command;
        char typeVehicle;
        int Id;
        Vehicle* v;

        switch (command) {
        case 1:
            cout << "Please enter the levels of the garage: ";
            cin >> level;
            cout << "Please enter the space allocated in each level in a space separated line: ";
            getline(cin >> ws, spacesStr);
            g = Garage(level, getNumberOfSpaces(spacesStr));
            break;

        case 2:
            cout << "Please enter the type of vehicle, c for car and m for motorbike: ";
            cin >> typeVehicle;
            cout << "Please enter the vehicle id going in: ";
            cin >> Id;
            v = getNewVehicle(typeVehicle, Id);
            g.enterGarage(v);

            break;
        case 3:
            cout << "Please enter the id of the vehicle: ";
            cin >> Id;
            g.exitGarage(Id);
        case 4:
            exit = true;
        default:
            break;
        }
        cout << "------------------------------------------\n";
   }

    cout << "Exit";
}
