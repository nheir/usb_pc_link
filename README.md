usb_pc_link
===========

Complete the connection protocol with Philips USB HiFi on Linux

It works with Philips MC-M530. I guess it will work with any other USB PC Link Hifi but I can not test.

Compile
------- 
Dependency: 

    libusb-dev

Compile:

    g++ -Wall usb_pc_link.cpp -o usb_pc_link -lusb


Use
---
Switch on and plug the HiFi

Run `./usb_pc_link`

Choose `Set [Philips Audio Set]` as sound card (alsa or pulseaudio)

Working
-------
The snd-usb-audio Linux module lacks something that the Widows driver do. Appearently, the Windows driver 'switches on' the USB device on the Micro-System.
I find two usb packet that seems to do it.

    40 04 00 00 a4 ef 01 00 01
    |  |  |     |     |     |
    |  |  value index |     data
    |  request        length
    requesttype


    40 04 00 00 a4 f0 01 00 ff
    |  |  |     |     |     |
    |  |  value index |     data
    |  request        length
    requesttype

`usb_pc_link` uses VendorID and ProductID to select the USB device. If these IDs are not 0471:0111, you may change the lines :

    #define VENDOR_ID  0x0471
    #define PRODUCT_ID 0x0111
