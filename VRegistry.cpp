#include <iostream>
#include <string>
#include <limits>
using namespace std;

class Vehicle {
private:
    int vehicleID;
    string manufacturer;
    string model;
    int year;
    bool active;

    static int totalVehicles;

    void ensureActive() { if (!active) { active = true; totalVehicles++; } }

public:
    Vehicle() : vehicleID(0), manufacturer(""), model(""), year(0), active(false) {}
    Vehicle(int id, const string& manu, const string& mod, int yr)
        : vehicleID(id), manufacturer(manu), model(mod), year(yr), active(true) {
        totalVehicles++;
    }
    virtual ~Vehicle() { if (active) totalVehicles--; }

    // setters/getters (encapsulation)
    void setVehicleID(int id) { ensureActive(); vehicleID = id; }
    int getVehicleID() const { return vehicleID; }

    void setManufacturer(const string& m) { ensureActive(); manufacturer = m; }
    string getManufacturer() const { return manufacturer; }

    void setModel(const string& m) { ensureActive(); model = m; }
    string getModel() const { return model; }

    void setYear(int y) { ensureActive(); year = y; }
    int getYear() const { return year; }

    static int getTotalVehicles() { return totalVehicles; }

    virtual void displayDetails() const {
        cout << "ID: " << vehicleID
             << ", Manufacturer: " << manufacturer
             << ", Model: " << model
             << ", Year: " << year;
    }
};

int Vehicle::totalVehicles = 0;

/* Single inheritance */
class Car : public Vehicle {
private:
    string fuelType;
public:
    Car(int id = 0, const string& manu = "", const string& mod = "", int yr = 0, const string& fuel = "")
        : Vehicle(id, manu, mod, yr), fuelType(fuel) {}
    virtual ~Car() {}

    void setFuelType(const string& f) { fuelType = f; }
    string getFuelType() const { return fuelType; }

    void displayDetails() const override {
        Vehicle::displayDetails();
        cout << ", Fuel: " << fuelType;
    }
};

/* Multilevel inheritance */
class ElectricCar : public Car {
private:
    int batteryCapacity; // kWh
public:
    ElectricCar(int id = 0, const string& manu = "", const string& mod = "", int yr = 0,
                const string& fuel = "", int batt = 0)
        : Car(id, manu, mod, yr, fuel), batteryCapacity(batt) {}
    virtual ~ElectricCar() {}

    void setBatteryCapacity(int b) { batteryCapacity = b; }
    int getBatteryCapacity() const { return batteryCapacity; }

    void displayDetails() const override {
        Car::displayDetails();
        cout << ", Battery: " << batteryCapacity << " kWh";
    }
};

/* Aircraft - base for multiple inheritance */
class Aircraft {
private:
    int flightRange; // km
public:
    Aircraft(int range = 0) : flightRange(range) {}
    virtual ~Aircraft() {}

    void setFlightRange(int r) { flightRange = r; }
    int getFlightRange() const { return flightRange; }

    virtual void displayAircraftDetails() const {
        cout << ", Range: " << flightRange << " km";
    }
};

/* Multiple inheritance */
class FlyingCar : public Car, public Aircraft {
public:
    FlyingCar(int id = 0, const string& manu = "", const string& mod = "", int yr = 0,
              const string& fuel = "", int range = 0)
        : Car(id, manu, mod, yr, fuel), Aircraft(range) {}
    virtual ~FlyingCar() {}

    void displayDetails() const override {
        Car::displayDetails();
        Aircraft::displayAircraftDetails();
    }
};

/* Multilevel derived from ElectricCar */
class SportsCar : public ElectricCar {
private:
    int topSpeed; // km/h
public:
    SportsCar(int id = 0, const string& manu = "", const string& mod = "", int yr = 0,
              const string& fuel = "", int batt = 0, int speed = 0)
        : ElectricCar(id, manu, mod, yr, fuel, batt), topSpeed(speed) {}
    virtual ~SportsCar() {}

    void setTopSpeed(int s) { topSpeed = s; }
    int getTopSpeed() const { return topSpeed; }

    void displayDetails() const override {
        ElectricCar::displayDetails();
        cout << ", Top Speed: " << topSpeed << " km/h";
    }
};

/* Hierarchical inheritance */
class Sedan : public Car {
public:
    Sedan(int id = 0, const string& manu = "", const string& mod = "", int yr = 0, const string& fuel = "")
        : Car(id, manu, mod, yr, fuel) {}
    virtual ~Sedan() {}
    void displayDetails() const override {
        cout << "[Sedan] ";
        Car::displayDetails();
    }
};

class SUV : public Car {
public:
    SUV(int id = 0, const string& manu = "", const string& mod = "", int yr = 0, const string& fuel = "")
        : Car(id, manu, mod, yr, fuel) {}
    virtual ~SUV() {}
    void displayDetails() const override {
        cout << "[SUV] ";
        Car::displayDetails();
    }
};

/* VehicleRegistry: manages array of Vehicle* */
class VehicleRegistry {
private:
    Vehicle* vehicles[100];
    int total;

public:
    VehicleRegistry() : total(0) {
        // preload 3 sample records so "View All" shows output immediately
        vehicles[total++] = new Car(201, "Toyota", "Corolla", 2019, "Petrol");
        vehicles[total++] = new ElectricCar(202, "Tesla", "Model 3", 2021, "Electric", 75);
        vehicles[total++] = new FlyingCar(203, "AeroMakers", "SkyRider", 2024, "Hybrid", 500);
    }

    ~VehicleRegistry() {
        for (int i = 0; i < total; ++i) delete vehicles[i];
    }

    void addVehicleInteractive() {
        if (total >= 100) { cout << "Registry full.\n"; return; }

        cout << "\nSelect type to add:\n";
        cout << "1. Car\n2. Electric Car\n3. Sports Car\n4. Flying Car\n5. Sedan\n6. SUV\nEnter: ";
        int type; if (!(cin >> type)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Bad input.\n"; return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        int id, year, battery = 0, speed = 0, range = 0;
        string manu, mod, fuel;

        cout << "ID: "; cin >> id; cin.ignore();
        cout << "Manufacturer: "; getline(cin, manu);
        cout << "Model: "; getline(cin, mod);
        cout << "Year: "; cin >> year; cin.ignore();

        switch (type) {
            case 1:
                cout << "Fuel Type: "; getline(cin, fuel);
                vehicles[total++] = new Car(id, manu, mod, year, fuel);
                break;
            case 2:
                cout << "Fuel Type: "; getline(cin, fuel);
                cout << "Battery (kWh): "; cin >> battery; cin.ignore();
                vehicles[total++] = new ElectricCar(id, manu, mod, year, fuel, battery);
                break;
            case 3:
                cout << "Fuel Type: "; getline(cin, fuel);
                cout << "Battery (kWh): "; cin >> battery; cin.ignore();
                cout << "Top Speed (km/h): "; cin >> speed; cin.ignore();
                vehicles[total++] = new SportsCar(id, manu, mod, year, fuel, battery, speed);
                break;
            case 4:
                cout << "Fuel Type: "; getline(cin, fuel);
                cout << "Flight Range (km): "; cin >> range; cin.ignore();
                vehicles[total++] = new FlyingCar(id, manu, mod, year, fuel, range);
                break;
            case 5:
                cout << "Fuel Type: "; getline(cin, fuel);
                vehicles[total++] = new Sedan(id, manu, mod, year, fuel);
                break;
            case 6:
                cout << "Fuel Type: "; getline(cin, fuel);
                vehicles[total++] = new SUV(id, manu, mod, year, fuel);
                break;
            default:
                cout << "Invalid type.\n";
                return;
        }
        cout << "Added.\n";
    }

    void displayAll() const {
        if (total == 0) { cout << "No vehicles.\n"; return; }
        cout << "\n-- All Vehicles (" << Vehicle::getTotalVehicles() << ") --\n";
        for (int i = 0; i < total; ++i) {
            cout << i+1 << ". ";
            vehicles[i]->displayDetails();
            cout << "\n";
        }
    }

    void searchById() const {
        cout << "Enter ID to search: ";
        int id; if (!(cin >> id)) { cout << "Bad input.\n"; return; }
        for (int i = 0; i < total; ++i) {
            if (vehicles[i]->getVehicleID() == id) {
                cout << "Found: ";
                vehicles[i]->displayDetails();
                cout << "\n";
                return;
            }
        }
        cout << "Vehicle with ID " << id << " not found.\n";
    }
};

int main() {
    VehicleRegistry registry;
    while (true) {
        cout << "\n--- Vehicle Registry ---\n";
        cout << "1. Add Vehicle\n2. View All Vehicles\n3. Search by ID\n4. Exit\n";
        cout << "Choice: ";

        int ch; if (!(cin >> ch)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Invalid.\n"; continue; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (ch == 1) registry.addVehicleInteractive();
        else if (ch == 2) registry.displayAll();
        else if (ch == 3) registry.searchById();
        else if (ch == 4) { cout << "Goodbye.\n"; break; }
        else cout << "Choose 1-4.\n";
    }
    return 0;
}
