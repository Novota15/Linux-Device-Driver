# Linux-Device-Driver

### Grant Novota

grno9650@colorado.edu

A simple linux device character driver.

## Setup

1. `cd` into the modules directory, which should contain **simple_char_driver.c** and the **Makefile**

2. Create the module object:
`make -C /lib/modules/$(uname -r)/build M=$PWD modules` you should now see a file named **simple_char_driver.ko** within the modules directory.

3. Create the device file:
`sudo mknod -m 777 /dev/simple_char_driver c 240 0`. 240 is the major number of the driver and it is possible that this number may be taken, so some other major number may need to be picked.

4. Install the module:
`sudo insmod simple_char_driver.ko`

5. Confirm installation:
`lsmod`. You should see simple_char_driver listed.

6. Compile the test app:
`gcc -o test driver_test.c`

7. Run the test app:
`./test`

8. View module kernel outputs with:
`dmesg`

9. You can uninstall the module with:
`sudo rmmod simple_char_driver`

## Using the test application

The interactive test app gives the following options:

* Press r to read from the device.
* Press w to write to the device.
* Press s to seek into the device.
* Press e to exit from the device/test app.

If the user presses `r` then they will be asked for the number of bytes to read and a buffer of that size will be created to read the data.

Data read from the device file is printed starting from the current position.

If the user presses `w` then the app will ask for the data to be written from the user.

If the user presses `s` then they will be promped for an offset value and and value for whence.

If the user presses `e` then the app is shut down.
