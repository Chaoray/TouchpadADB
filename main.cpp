#include <windows.h>

#include <cstdio>
#include <iostream>

#define popen _popen
#define pclose _pclose

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define BUFFER_SIZE 1024

struct point {
    int x, y;
};

int main() {
    std::string target_device;
    std::cout << "Enter target device: ";
    std::cin >> target_device;

    char buffer[BUFFER_SIZE];

    // execute logcat on target

    //// clear logcat
    std::string cmd = "adb -s " + target_device; + " logcat -c";
    popen(cmd.c_str(), "r");

    //// start logcat
    cmd = "adb -s " + target_device + " logcat -s touch_event:I -v raw";
    FILE* adb_pipe = popen(cmd.c_str(), "r");

    //// is pipe open success
    if (!adb_pipe) {
        printf("popen() failed!\n");
        return EXIT_FAILURE;
    }

    point p;
    ULONG_PTR dwExtraInfo;
    // get output data
    while (fgets(buffer, BUFFER_SIZE, adb_pipe) != nullptr) {
        sscanf(buffer, "%d %d", &p.x, &p.y);
        // output example: -18 7

        // move mouse
        mouse_event(
            MOUSEEVENTF_MOVE,
            p.x,
            p.y,
            0,
            dwExtraInfo);
    }

    pclose(adb_pipe);
}
