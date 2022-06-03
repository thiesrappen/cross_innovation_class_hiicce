import logging
import time
import random
import configparser

# configuration parameters
POLL_RATE_SECONDS = 4
FLOWER_BEDS = 2
SOIL_HUMIDITY = 0.5
WATERING_TIME = 5
DISPLAY_LOG = True


# return a random value between 0 and 1 for testing purposes
def measureHumidityLevels():
    humidities = []
    for i in range(FLOWER_BEDS):
        # TODO sensor-using
        # Random values
        humidities.append(round(random.uniform(0, 1), 2))
    return humidities


# returns True, when watering was successful
def watering(bedIndex):
    # start pump
    # open valve with bedIndex
    time.sleep(WATERING_TIME)
    # stop pump
    # close all valves
    # check if water is flowed    
    return True


def log(message):
    if DISPLAY_LOG:
        print(time.asctime(), "-->", message)


def main():
    while True:
        log("Routine started.")
        humidities = measureHumidityLevels()
        log("Humidity levels: " + str(humidities))
        for bedIndex, humidity in enumerate(humidities):
            if humidity < SOIL_HUMIDITY:
                # water the bed
                log("Flower box #" + str(bedIndex) + " is to dry. Humidity: " + str(humidity))
                if watering(bedIndex):
                    log("Flower bed #" + str(bedIndex) + " successfully watered.")
                else:
                    log("Error during watering")
            else:
                # ignore this flower bed
                log("Flower box #" + str(bedIndex) + " is still wet enough. Humidity: " + str(humidity))
        log("Routine finished.\n")
        time.sleep(POLL_RATE_SECONDS)

if __name__ == "__main__":
    main()