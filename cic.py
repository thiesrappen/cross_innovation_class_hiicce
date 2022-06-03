import logging
import time
import random
import configparser
import os

# configuration parameters
POLL_RATE_SECONDS = 4
FLOWER_BEDS = 2
SOIL_HUMIDITY = 0.5
WATERING_TIME = 5
MINIMUM_WATERING_LEVEL = 0.2
DISPLAY_LOG = True


# return a random value between 0 and 1 for testing purposes
def measureHumidityLevels():
    humidities = []
    for i in range(FLOWER_BEDS):
        # TODO sensor-using
        humidities.append(round(random.uniform(0, 1), 2))
    return humidities


# returns True, when watering was successful
def watering(bedIndex):
    # check if container is not empty
    # TODO sensor-using
    waterLevel = 0.35
    if waterLevel <= MINIMUM_WATERING_LEVEL: 
        return False
    # start pump
    # TODO pump-control
    # open valve with bedIndex
    # TODO valve-control
    time.sleep(WATERING_TIME)
    # stop pump
    # TODO pump-control
    # close all valves
    # TODO valve-control
    # check if water is flowed
    # TODO sensor-using
    newWaterLevel = 0.32
    if newWaterLevel < waterLevel:
        return True
    else: 
        return False

def log(message):
    if DISPLAY_LOG:
        print(time.asctime(), "-->", message)


# stolen from stack overflow "https://raspberrypi.stackexchange.com/questions/7621/accessing-a-usb-drive-after-start-up-with-python"
def mountUsbDrives():
    partitionsFile = open("/proc/partitions")
    lines = partitionsFile.readlines()[2:]  # Skips the header lines
    for line in lines:
        words = [x.strip() for x in line.split()]
        minorNumber = int(words[1])
        deviceName = words[3]
        if minorNumber % 16 == 0:
            path = "/sys/class/block/" + deviceName
        if os.path.islink(path):
            if os.path.realpath(path).find("/usb") > 0:
                print("/dev/%s1 -> /mnt/usb" % (deviceName))
                os.system('mkdir /mnt/usb 2>/dev/null')  # if already created don't output error.
                os.system("mount /dev/%s1 /mnt/usb 2>/dev/null" % (deviceName))


def searchForNewConfig():
    mountUsbDrives()
    os.chdir("/media/pi")
    drives = os.listdir(".")
    output = ""
    for drive in drives:
        print(drive)

    return output


def updateConfiguration():
    file = "" #searchForNewConfig()
    # if no config is found, the defaults will be loaded
    if file == "":
        file = "defaultConfig.ini"
    configParser = configparser.ConfigParser()
    configParser.read(file)
    POLL_RATE_SECONDS = configParser['config']['POLL_RATE_SECONDS']
    FLOWER_BEDS = configParser['config']['FLOWER_BEDS']
    SOIL_HUMIDITY = configParser['config']['SOIL_HUMIDITY']
    WATERING_TIME = configParser['config']['WATERING_TIME']
    MINIMUM_RESERVOIR_LEVEL = configParser['config']['MINIMUM_RESERVOIR_LEVEL']
    DISPLAY_LOG = configParser['config']['DISPLAY_LOG']


# main routine
while True:
    log("Routine started.")
    updateConfiguration()
    humidities = measureHumidityLevels()
    log("Humidity levels: " + str(humidities))
    for bedIndex, humidity in enumerate(humidities):
        if humidity < SOIL_HUMIDITY:
            # water the bed
            log("Flower box #" + str(bedIndex) + " is to dry. Humidity: " + str(humidity))
            if watering(bedIndex):
                log("Flower bed #" + str(bedIndex) + " successfully watered.")
            else:
                # ignore this flower bed
                log("Flower box #" + str(bedIndex) + " is still wet enough. Humidity: " + str(humidity))
        log("Routine finished.\n")
        time.sleep(POLL_RATE_SECONDS)

if __name__ == "__main__":
    main()