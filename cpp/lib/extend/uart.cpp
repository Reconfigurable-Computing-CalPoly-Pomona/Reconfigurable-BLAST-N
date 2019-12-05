#include <stdio.h>
#include <string.h>

#include "uart.hpp"

#ifdef _WIN32

#include <Windows.h>
#include <string>
HANDLE hComm;
std::string ComPortName = "\\\\.\\";
BOOL Status;

#endif

/**
 * Initialize the serial port connection depending on the operating system.
 */

int uart_init(const char *path, size_t baud_rate)
{
#ifdef _WIN32
    ComPortName += path;

    // opening the serial port
    hComm = CreateFile(
        ComPortName.c_str(),
        GENERIC_READ | GENERIC_WRITE,   // read/write access
        0,                              // no sharing, ports can't be shared
        NULL,                           // no security
        OPEN_EXISTING,                  // open existing port only
        0,                              // non overlapped I/O
        NULL                            // null for comm devices
    );

    if (hComm == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Serial Port Error: Port %s can't be opened\n", ComPortName.c_str());
        exit(-1);
    }

    // initializing DCB structure
    DCB dcb_params = { 0 };
    dcb_params.DCBlength = sizeof(dcb_params);

    // retrieve current settings
    Status = GetCommState(hComm, &dcb_params);

    if (!Status) {
        fprintf(stderr, "Serial Port Error: Could not obtain COM port state\n");
        exit(-1);
    }

    //dcb_params.BaudRate = CBR_256000;   // BaudRate = 256000
    dcb_params.BaudRate = baud_rate;
    dcb_params.ByteSize = 8;            // ByteSize = 8
    dcb_params.StopBits = ONESTOPBIT;   // StopBits = 1
    dcb_params.Parity   = NOPARITY;     // Parity   = None

    // configure the port according to settings in DCB
    Status = SetCommState(hComm, &dcb_params);

    if (!Status) {
        fprintf(stderr, "Serial Port Error: Could not set DCB structure\n");
        exit(-1);
    }

    // timeouts
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        fprintf(stderr, "Serial Port Error: Could not set timeouts\n");
        exit(-1);
    }
#endif
    return 1;
}

/**
 * Serial port tear-down depending on the operating system
 */

int uart_close()
{
#ifdef _WIN32
    return CloseHandle(hComm);
#else
    return 0;
#endif
}

/**
 * Write data from the computer to the serial port
 */

void uart_write(unsigned char *buf, size_t size)
{
#ifdef _WIN32
    DWORD  bytes_written = 0;
    Status = WriteFile(
        hComm,          // serial port handle
        buf,            // data to be written
        (DWORD)size,    // num bytes to write
        &bytes_written, // num bytes written
        NULL
    );

    if (!Status) {
        fprintf(stderr, "Serial Port Error: Code %d received in writing to serial port\n", GetLastError());
        exit(-1);
    }
#endif
}

/**
 * Read data from the serial port to the computer
 */

void uart_read(unsigned char *buf, size_t size)
{
#ifdef _WIN32
    // set receive mask to configure Windows to monitor the serial device for character reception
    Status = SetCommMask(hComm, EV_RXCHAR);

    if (!Status) {
        fprintf(stderr, "Serial Port Error: Could not set CommMask\n");
        exit(-1);
    }

    // wait for the character to be received from even_mask trigger
    DWORD event_mask;
    Status = WaitCommEvent(hComm, &event_mask, NULL);

    // wait until a character is received
    char rx_byte;
    DWORD bytes_read;
    int i = 0;

    // read rx data
    if (!Status) {
        fprintf(stderr, "Serial Port Error: Could not set WaitCommEvent\n");
        exit(-1);
    }
    else {
        do {
            Status = ReadFile(hComm, &rx_byte, sizeof(rx_byte), &bytes_read, NULL);
            if (i < size)
                buf[i++] = rx_byte;
            else
                break;
        } while (bytes_read > 0);
    }
#endif
}
