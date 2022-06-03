import logging
import time
import random
import configparser

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