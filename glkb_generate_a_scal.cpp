#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

// IoT Device Interface
class IoTDevice {
public:
    virtual void connect() = 0;
    virtual void disconnect() = 0;
    virtual void send(char* data, int size) = 0;
    virtual char* receive(int& size) = 0;
};

// IoT Device Controller
class IoTDeviceController {
private:
    vector<IoTDevice*> devices;
    mutex deviceMutex;

public:
    void addDevice(IoTDevice* device) {
        lock_guard<mutex> lock(deviceMutex);
        devices.push_back(device);
    }

    void removeDevice(IoTDevice* device) {
        lock_guard<mutex> lock(deviceMutex);
        for (auto it = devices.begin(); it != devices.end(); ++it) {
            if (*it == device) {
                devices.erase(it);
                break;
            }
        }
    }

    void connectAll() {
        for (auto device : devices) {
            device->connect();
        }
    }

    void disconnectAll() {
        for (auto device : devices) {
            device->disconnect();
        }
    }

    void sendData(char* data, int size) {
        for (auto device : devices) {
            device->send(data, size);
        }
    }

    char* receiveData(int& size) {
        char* data = nullptr;
        for (auto device : devices) {
            char* buffer = device->receive(size);
            if (buffer != nullptr) {
                data = buffer;
                break;
            }
        }
        return data;
    }
};

// Sample IoT Device Implementation
class MyIoTDevice : public IoTDevice {
private:
    string deviceId;
    bool connected;

public:
    MyIoTDevice(string deviceId) : deviceId(deviceId), connected(false) {}

    void connect() override {
        cout << "Connecting to device " << deviceId << endl;
        connected = true;
    }

    void disconnect() override {
        cout << "Disconnecting from device " << deviceId << endl;
        connected = false;
    }

    void send(char* data, int size) override {
        cout << "Sending data to device " << deviceId << endl;
        // Implement sending data logic here
    }

    char* receive(int& size) override {
        cout << "Receiving data from device " << deviceId << endl;
        // Implement receiving data logic here
        return new char[10]; // Return a sample received data
    }
};

int main() {
    IoTDeviceController controller;

    MyIoTDevice device1("device1");
    MyIoTDevice device2("device2");

    controller.addDevice(&device1);
    controller.addDevice(&device2);

    controller.connectAll();

    char data[] = "Hello, IoT!";
    controller.sendData(data, sizeof(data));

    int size;
    char* receivedData = controller.receiveData(size);
    cout << "Received data: " << receivedData << endl;

    controller.disconnectAll();

    return 0;
}