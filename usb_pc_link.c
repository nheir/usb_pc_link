/**
 * Complete the connection with Philips 
 * MCM-530 USB PC Link
 *
 * License: GPL
 *
 * You need to have libusb-dev installed.
 * apt-get install libusb-dev
 *
 * Compile:
 * gcc -Wall usb_pc_link.c -o usb_pc_link -lusb
 *
 * Run as normal user:
 * ./usb_pc_link
 *
 */

#include <usb.h>
#include <stdio.h>

#define VENDOR_ID  0x0471
#define PRODUCT_ID 0x0111

// get a handle to the USB device and configure it
usb_dev_handle*
get_handle(struct usb_device *usbDevice)
{
  usb_dev_handle *deviceHandle = NULL;
  if( usbDevice == NULL ) {
    return NULL;
  }

  deviceHandle = usb_open( usbDevice );

  if (deviceHandle == NULL)
  {
    fprintf(stderr, "Unable to open USB device %s\n", usb_strerror());
    usb_close(deviceHandle);
    exit(-1);
  }
  return deviceHandle;
}

// find the right USB device
struct usb_device*
findUSBDevice(void) {
  struct usb_bus *bus;
  struct usb_device *usbDevice;
  struct usb_device *mcmUSBDevice = NULL;

  usb_init();
  usb_find_busses();
  usb_find_devices();

  for (bus = usb_busses; bus; bus = bus->next) {
    for (usbDevice = bus->devices; usbDevice;
         usbDevice = usbDevice->next)
    {
      if (usbDevice->descriptor.idVendor == VENDOR_ID &&
          usbDevice->descriptor.idProduct == PRODUCT_ID)
      {
        mcmUSBDevice = usbDevice;
        break;
      }
    }
  }

  if (mcmUSBDevice == NULL) {
    fprintf(stderr, "Device %04x:%04x not found\n", VENDOR_ID, PRODUCT_ID);
    exit(-1);
  }
  return mcmUSBDevice;
}

/**
 * Connection.
 *
 * send
 * 40 04 00 00 a4 ef 01 00 01
 * |  |  |     |     |     |
 * |  |  value index |     data
 * |  request        length
 * requesttype
 *
 * send
 * 40 04 00 00 a4 f0 01 00 ff
 * |  |  |     |     |     |
 * |  |  value index |     data
 * |  request        length
 * requesttype
 */
int
connection(usb_dev_handle *deviceHandle) {

  int requesttype = 0x40;
  int request = 0x04;
  int value = 0x00;

  int index[2] = { 0xa4ef, 0xa4f0 };
  char buffer[2] = { 1, -1 };
  int size = 1;
  int timeout = 1000;

  int ret = 0;

  ret += usb_control_msg(
    deviceHandle, requesttype, request, value,
    index[0], buffer, size, timeout);

  ret += usb_control_msg(
    deviceHandle, requesttype, request, value,
    index[1], buffer+1, size, timeout);

  if (ret < 2) {
    fprintf(stderr, "An error occurs in the requests...\n");
    return -1;
  }
  return 0;
}


int
main(void) {
  struct usb_device *mcmUSBDevice = findUSBDevice();

  usb_dev_handle *deviceHandle = NULL;
  deviceHandle = get_handle(mcmUSBDevice);

  if(connection(deviceHandle) < 0)
    return -1;

  return 0;
}