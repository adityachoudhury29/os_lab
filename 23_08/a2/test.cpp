#include <iostream>
#include <unistd.h>
using namespace std;

int main() {
    cout << "Sleeping for 5 seconds..." << endl;
    sleep(5);
    cout << "Woke up." << endl;

    cout << "Calling pause. Waiting for any signal..." << endl;
    pause();

    return 0;
}