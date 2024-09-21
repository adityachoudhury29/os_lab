#include <iostream>
#include <csignal>
using namespace std;

int terminationCount = 0;

void terminationHandler0(int signalId) {
    cout << "Received signal: " << signalId << endl;
    cout << "Terminating itself." << endl;
    exit(0);
}

void terminationHandler1(int signalId) {
    cout << "Received signal: " << signalId << endl;
    cout << "Ignoring default action and continuing to run." << endl;
    terminationCount++;
}

void terminationHandler2(int signalId) {
    cout << "Received signal: " << signalId << endl;

    if (terminationCount < 3) {
        cout << "Ignoring default action and continuing to run." << endl;
        terminationCount++;
    } else {
        cout << "Terminating itself." << endl;
        exit(0);
    }
}

int main() {
    signal(SIGINT, terminationHandler2);
    while (true) {}
    return 0;
}
