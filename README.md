# Michael Adu - 7041 Embedded Operating System Project

# Build the Project

- To build the project, download the repository and run the following command in the folder terminal.

```bash
bash build.sh
```

- This should download and build an image, as well as copy source codes into their appropriate folders.
- The image will be found in `project-build-rpi/tmp/deploy/images/raspberrypi3/rpi-test-image-raspberrypi3.wic.bz2`. The image can be unzipped and flashed to the SD card.

# Install the pre-existing image into the SD card

- For testing purposes, there is a pre-existing image that can be downloaded from the following link:
    - [Final Image](https://livecoventryac-my.sharepoint.com/:f:/g/personal/adum3_uni_coventry_ac_uk/EvHbE246uuBBtlitgFbGDGMBwpAHrWI5xzIYGmDJzArlLQ?e=71ZTkS)
- The zipped image is located in `Final Image/raspberrypi3/rpi-test-image-raspberrypi3.wic.bz2`
- This can be unzipped and loaded into an SD card for the raspberry pi.

# Running the Application Module

- Upon bootup, register the hcsr04 module with the major and minor number.

```bash
mknod /dev/hcsr04 c <MAJORNUMBER> <MINORNUMBER>
```

- To run the program, type ultrasonic into the terminal followed by the number of times the ultrasonic sensor should read for.

```bash
ultrasonic 8 #runs 8 times
```

- If the number is 0, the ultrasonic sensor will be read infinitely until the user cancels the operation.

```bash
ultrasonic 0 #runs infinitely
```

- To retrieve the last read value, run the command below.

```bash
cat /sys/kernel/hcsr04/hcsr04
```