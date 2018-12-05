#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#include <wchar.h>
#include <string.h>



#define MAX_STR 255

int main(int argc, char* argv[])
{
    int res;
    unsigned char buf[65];
    wchar_t wstr[MAX_STR];
    hid_device *handle;
    int i;
    struct hid_device_info *devs, *cur_dev;

    if (hid_init())
            return -1;

    printf(" 运行中\n");

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;
    while (cur_dev) {
        printf("Device Found\n  type: %04hx %04hx\n  path: %s\n  serial_number: %ls", cur_dev->vendor_id, cur_dev->product_id, cur_dev->path, cur_dev->serial_number);
        printf("\n");
        printf("  Manufacturer: %ls\n", cur_dev->manufacturer_string);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Release:      %hx\n", cur_dev->release_number);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");
        cur_dev = cur_dev->next;
    }


    // Open the device using the VID, PID,
    // and optionally the Serial number.
//      handle = hid_open(cur_dev->vendor_id, cur_dev->product_id, NULL);
      handle = hid_open(0x46d,0xc31c, NULL);
      if (!handle) {
          printf("unable to open device\n");
          getchar();
          return 1;
      }
//    // Read the Manufacturer String
 //     res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
//      wprintf(L"Manufacturer String: %s\n", wstr);

//    // Read the Product String
//    res = hid_get_product_string(handle, wstr, MAX_STR);
//    wprintf(L"Product String: %s\n", wstr);

//    // Read the Serial Number String
//    res = hid_get_serial_number_string(handle, wstr, MAX_STR);
//    wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);

//    // Read Indexed String 1
//    res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
//    wprintf(L"Indexed String 1: %s\n", wstr);

//    // Toggle LED (cmd 0x80). The first byte is the report number (0x0).
//    buf[0] = 0x0;
//    buf[1] = 0x80;
//    res = hid_write(handle, buf, 65);

//    // Request state (cmd 0x81). The first byte is the report number (0x0).
//    buf[0] = 0x0;
//    buf[1] = 0x81;
//    res = hid_write(handle, buf, 65);

//    // Read requested state
//    hid_read(handle, buf, 65);

//    // Print out the returned buffer.
//    for (i = 0; i < 4; i++)
//        printf("buf[%d]: %d\n", i, buf[i]);

    printf(" 运行结束\n");

//    Sleep(5000);
    getchar();
    return 0;
}
