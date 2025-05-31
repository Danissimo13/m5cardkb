#include <unistd.h>
#include <linux/uinput.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>
#include <cstring>
#include <map>
#include <stdexcept>
#include <vector>

using namespace std;

map<int, vector<unsigned short>> SIGNAL_TO_KEYS = {
        // number row
        {0x1B, {KEY_ESC}},
        {0x31, {KEY_1}},
        {0x32, {KEY_2}},
        {0x33, {KEY_3}},
        {0x34, {KEY_4}},
        {0x35, {KEY_5}},
        {0x36, {KEY_6}},
        {0x37, {KEY_7}},
        {0x38, {KEY_8}},
        {0x39, {KEY_9}},
        {0x30, {KEY_0}},
        {0x08, {KEY_BACKSPACE}},

        // top row
        {0x09, {KEY_TAB}},
        {0x71, {KEY_Q}},
        {0x77, {KEY_W}},
        {0x65, {KEY_E}},
        {0x72, {KEY_R}},
        {0x74, {KEY_T}},
        {0x79, {KEY_Y}},
        {0x75, {KEY_U}},
        {0x69, {KEY_I}},
        {0x6F, {KEY_O}},
        {0x70, {KEY_P}},

        // home row
        {0x61, {KEY_A}},
        {0x73, {KEY_S}},
        {0x64, {KEY_D}},
        {0x66, {KEY_F}},
        {0x67, {KEY_G}},
        {0x68, {KEY_H}},
        {0x6A, {KEY_J}},
        {0x6B, {KEY_K}},
        {0x6C, {KEY_L}},
        {0x0D, {KEY_ENTER}},

        // bottom row
        {0x7A, {KEY_Z}},
        {0x78, {KEY_X}},
        {0x63, {KEY_C}},
        {0x76, {KEY_V}},
        {0x62, {KEY_B}},
        {0x6E, {KEY_N}},
        {0x6D, {KEY_M}},
        {0x2C, {KEY_COMMA}},
        {0x2E, {KEY_DOT}},
        {0x20, {KEY_SPACE}},

        // arrow keys
        {0xB4, {KEY_LEFT}},
        {0xB5, {KEY_UP}},
        {0xB6, {KEY_DOWN}},
        {0xB7, {KEY_RIGHT}},

        // number row symbols
        {0x21, {KEY_LEFTSHIFT, KEY_1}},
        {0x40, {KEY_LEFTSHIFT, KEY_2}},
        {0x23, {KEY_LEFTSHIFT, KEY_3}},
        {0x24, {KEY_LEFTSHIFT, KEY_4}},
        {0x25, {KEY_LEFTSHIFT, KEY_5}},
        {0x5E, {KEY_LEFTSHIFT, KEY_6}},
        {0x26, {KEY_LEFTSHIFT, KEY_7}},
        {0x2A, {KEY_LEFTSHIFT, KEY_8}},
        {0x28, {KEY_LEFTSHIFT, KEY_9}},
        {0x29, {KEY_LEFTSHIFT, KEY_0}},
        {0x7B, {KEY_LEFTSHIFT, KEY_LEFTBRACE}},
        {0x7D, {KEY_LEFTSHIFT, KEY_RIGHTBRACE}},

        // top row symbols
        {0x5B, {KEY_LEFTBRACE}},
        {0x5D, {KEY_RIGHTBRACE}},
        {0x2f, {KEY_SLASH}},
        {0x5C, {KEY_BACKSLASH}}, 
        {0x7C, {KEY_LEFTSHIFT, KEY_BACKSLASH}},
        {0x7E, {KEY_LEFTSHIFT, KEY_GRAVE}},
        {0x27, {KEY_APOSTROPHE}},
        {0x22, {KEY_LEFTSHIFT, KEY_APOSTROPHE}},

        // bottom row symbols
        {0x3B, {KEY_SEMICOLON}},
        {0x3A, {KEY_LEFTSHIFT, KEY_SEMICOLON}},
        {0x60, {KEY_GRAVE}},
        {0x2B, {KEY_LEFTSHIFT, KEY_EQUAL}},
        {0x2D, {KEY_MINUS}},
        {0x5F, {KEY_LEFTSHIFT, KEY_MINUS}},
        {0x3D, {KEY_EQUAL}},
        {0x3F, {KEY_LEFTSHIFT, KEY_SLASH}},
        {0x3C, {KEY_LEFTSHIFT, KEY_COMMA}},
        {0x3E, {KEY_LEFTSHIFT, KEY_DOT}},

        // top row capitals
        {0x51, {KEY_LEFTSHIFT, KEY_Q}},
        {0x57, {KEY_LEFTSHIFT, KEY_W}},
        {0x45, {KEY_LEFTSHIFT, KEY_E}},
        {0x52, {KEY_LEFTSHIFT, KEY_R}},
        {0x54, {KEY_LEFTSHIFT, KEY_T}},
        {0x59, {KEY_LEFTSHIFT, KEY_Y}},
        {0x55, {KEY_LEFTSHIFT, KEY_U}},
        {0x49, {KEY_LEFTSHIFT, KEY_I}},
        {0x4F, {KEY_LEFTSHIFT, KEY_O}},
        {0x50, {KEY_LEFTSHIFT, KEY_P}},

        // home row capitals
        {0x41, {KEY_LEFTSHIFT, KEY_A}},
        {0x53, {KEY_LEFTSHIFT, KEY_S}},
        {0x44, {KEY_LEFTSHIFT, KEY_D}},
        {0x46, {KEY_LEFTSHIFT, KEY_F}},
        {0x47, {KEY_LEFTSHIFT, KEY_G}},
        {0x48, {KEY_LEFTSHIFT, KEY_H}},
        {0x4A, {KEY_LEFTSHIFT, KEY_J}},
        {0x4B, {KEY_LEFTSHIFT, KEY_K}},
        {0x4C, {KEY_LEFTSHIFT, KEY_L}},

        // bottom row capitals
        {0x5A, {KEY_LEFTSHIFT, KEY_Z}},
        {0x58, {KEY_LEFTSHIFT, KEY_X}},
        {0x43, {KEY_LEFTSHIFT, KEY_C}},
        {0x56, {KEY_LEFTSHIFT, KEY_V}},
        {0x42, {KEY_LEFTSHIFT, KEY_B}},
        {0x4E, {KEY_LEFTSHIFT, KEY_N}},
        {0x4D, {KEY_LEFTSHIFT, KEY_M}},

        // number row Ctrls 
        {0x81, {KEY_LEFTCTRL, KEY_1}},
        {0x82, {KEY_LEFTCTRL, KEY_2}},
        {0x83, {KEY_LEFTCTRL, KEY_3}},
        {0x84, {KEY_LEFTCTRL, KEY_4}},
        {0x85, {KEY_LEFTCTRL, KEY_5}},
        {0x86, {KEY_LEFTCTRL, KEY_6}},
        {0x87, {KEY_LEFTCTRL, KEY_7}},
        {0x88, {KEY_LEFTCTRL, KEY_8}},
        {0x89, {KEY_LEFTCTRL, KEY_9}},
        {0x8A, {KEY_LEFTCTRL, KEY_0}},
        {0x8B, {KEY_LEFTCTRL, KEY_BACKSPACE}},

        // top row Ctrls
        {0x8C, {KEY_LEFTCTRL, KEY_TAB}},
        {0x8D, {KEY_LEFTCTRL, KEY_Q}},
        {0x8E, {KEY_LEFTCTRL, KEY_W}},
        {0x8F, {KEY_LEFTCTRL, KEY_E}},
        {0x90, {KEY_LEFTCTRL, KEY_R}},
        {0x91, {KEY_LEFTCTRL, KEY_T}},
        {0x92, {KEY_LEFTCTRL, KEY_Y}},
        {0x93, {KEY_LEFTCTRL, KEY_U}},
        {0x94, {KEY_LEFTCTRL, KEY_I}},
        {0x95, {KEY_LEFTCTRL, KEY_O}},
        {0x96, {KEY_LEFTCTRL, KEY_P}},

        // home row Ctrls
        {0x9A, {KEY_LEFTCTRL, KEY_A}},
        {0x9B, {KEY_LEFTCTRL, KEY_S}},
        {0x9C, {KEY_LEFTCTRL, KEY_D}},
        {0x9D, {KEY_LEFTCTRL, KEY_F}},
        {0x9E, {KEY_LEFTCTRL, KEY_G}},
        {0x9F, {KEY_LEFTCTRL, KEY_H}},
        {0xA0, {KEY_LEFTCTRL, KEY_J}},
        {0xA1, {KEY_LEFTCTRL, KEY_K}},
        {0xA2, {KEY_LEFTCTRL, KEY_L}},
        {0xA3, {KEY_LEFTCTRL, KEY_ENTER}},

        // bottom row Ctrls
        {0xA6, {KEY_LEFTCTRL, KEY_Z}},
        {0xA7, {KEY_LEFTCTRL, KEY_X}},
        {0xA8, {KEY_LEFTCTRL, KEY_C}},
        {0xA9, {KEY_LEFTCTRL, KEY_V}},
        {0xAA, {KEY_LEFTCTRL, KEY_B}},
        {0xAB, {KEY_LEFTCTRL, KEY_N}},
        {0xAC, {KEY_LEFTCTRL, KEY_M}},
        {0xAD, {KEY_LEFTCTRL, KEY_COMMA}},
        {0xAE, {KEY_LEFTCTRL, KEY_DOT}},
        {0xAF, {KEY_LEFTCTRL, KEY_SPACE}},

        // arrow key Ctrls
        {0x98, {KEY_LEFTCTRL, KEY_LEFT}},
        {0x99, {KEY_LEFTCTRL, KEY_UP}},
        {0xA4, {KEY_LEFTCTRL, KEY_DOWN}},
        {0xA5, {KEY_LEFTCTRL, KEY_RIGHT}}
    };

vector<unsigned short> SUPPORTED_KEYS = {
    KEY_ESC,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_BACKSPACE,
    KEY_UP,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_DOWN,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_ENTER,
    KEY_LEFT,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_DOT,
    KEY_SPACE,
    KEY_RIGHT,
    KEY_LEFTSHIFT,
    KEY_LEFTCTRL,
    KEY_APOSTROPHE,
    KEY_LEFTBRACE,
    KEY_RIGHTBRACE,
    KEY_SLASH,
    KEY_BACKSLASH,
    KEY_SEMICOLON,
    KEY_GRAVE,
    KEY_EQUAL,
    KEY_MINUS,
    KEY_NUMERIC_POUND,
    KEY_LEFTALT
};

class UInputDevice {
private:
    int fd;

    void change_key_state(unsigned short key, int value) {
        struct input_event ev;

        memset(&ev, 0, sizeof(ev));
        ev.type = EV_KEY;
        ev.code = key;
        ev.value = value;
        write(fd, &ev, sizeof(ev));
    }

    void syn() {
        struct input_event ev;

        memset(&ev, 0, sizeof(ev));
        ev.type = EV_SYN;
        ev.code = SYN_REPORT;
        ev.value = 0;
        write(fd, &ev, sizeof(ev));
    }
public:
    UInputDevice(const vector<unsigned short>& keys) {
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
        if (fd < 0) {
            throw runtime_error("Failed to open uinput device");
        }

        struct uinput_user_dev uidev;
        memset(&uidev, 0, sizeof(uidev));
        snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "M5Stack/CardKB");
        uidev.id.bustype = BUS_USB;
        uidev.id.vendor = 0x1;
        uidev.id.product = 0x1;
        uidev.id.version = 0x1;

        ioctl(fd, UI_SET_EVBIT, EV_KEY);
        for (int key : keys) {
            ioctl(fd, UI_SET_KEYBIT, key);
        }

        write(fd, &uidev, sizeof(uidev));
        if (ioctl(fd, UI_DEV_CREATE) < 0) {
            close(fd);
            throw runtime_error("Failed to create uinput device");
        }
    }

    void emit_combo(const vector<unsigned short>& keys) {
        for (unsigned short key : keys) {
            change_key_state(key, 1);
        }
        
        for (unsigned short key : keys) {
            change_key_state(key, 0);
        }

        syn();
    }

    ~UInputDevice() {
        ioctl(fd, UI_DEV_DESTROY);
        close(fd);
    }
};

int main(int argc, char* argv[]) {
    const int i2c_address = 0x5f;

    int i2c_bus = 5;
    if (argc > 1) {
        i2c_bus = stoi(argv[1]);
    }

    string i2c_device = "/dev/i2c-" + to_string(i2c_bus);

    int i2c_fd = open(i2c_device.c_str(), O_RDWR);
    if (i2c_fd < 0) {
        cerr << "Failed to open I2C bus" << endl;
        return 1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, i2c_address) < 0) {
        cerr << "Failed to set I2C slave address" << endl;
        close(i2c_fd);
        return 1;
    }

    try {
        UInputDevice device(SUPPORTED_KEYS);

        const useconds_t pull_sleep_interval = 10000;
        while (true) {

            unsigned char signal = 0x00;

            if (read(i2c_fd, &signal, 1) != 1) {
                cerr << "Error: " << "I2C read failed" << endl;
                signal = 0x00;
            }

            if (SIGNAL_TO_KEYS.find(signal) != SIGNAL_TO_KEYS.end()) {
                const auto& keys = SIGNAL_TO_KEYS[signal];
                device.emit_combo(keys);
            }
            else {
                usleep(pull_sleep_interval);
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        close(i2c_fd);
        return 1;
    }

    close(i2c_fd);
    return 0;
}
