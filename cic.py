import logging
import time
import random
import configparser

# configuration parameters
poll_rate_seconds = 4
number_of_flower_beds = 2
minimum_soil_humidity = 0.5
display_log = True


# return a random value between 0 and 1 for testing purposes
def measure_humidity_levels():
    returns = []
    for i in range(number_of_flower_beds):
        returns.append((round(random.uniform(0, 1)), i))
    return returns


# returns True, when watering was successful
def watering_routine(flower_bed_index):
    #do something
    return True


def log(log_message):
    if display_log:
        print(time.asctime(), "-->", log_message)


# main routine
while True:
    log("Routine started.")
    humidity_levels = measure_humidity_levels()
    log("Humidity levels: " + str(humidity_levels))
    one_container_watered = False
    for flower_box_humidity, flower_bed_index in humidity_levels:
        if flower_box_humidity < minimum_soil_humidity:
            log("Flower box #" + str(flower_bed_index) + " is to dry. Humidity: " + str(flower_box_humidity))
            if watering_routine(flower_bed_index):
                log("Flower bed #" + str(flower_bed_index) + " successfully watered.")
            else:
                log("Error during watering")
        else:
            # ignore this flower bed
            log("Flower box #" + str(flower_bed_index) + " is still wet enough. Humidity: " + str(flower_box_humidity))
    log("Routine finished.\n")
    time.sleep(poll_rate_seconds)
