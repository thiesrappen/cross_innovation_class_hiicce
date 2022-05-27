import time
import random
import yaml

#configuration parameters
poll_rate_seconds = 4
number_of_flower_beds = 2

#return a random value between 0 and 1 for testing purposes
def measure_humidity_levels():
    returnlist =[]
    for i in range(number_of_flower_beds):
        returnlist.append(round(random.uniform(0, 1), 2))
    return returnlist

def watering_routine(flower_bed_index):
    print("Flower bed ", flower_bed_index, " watered")
    return True;

def load_configuration():
    

load_configuration()

#main routine
while(True):
    print(time.asctime())
    humidity_levels = measure_humidity_levels()
    one_container_watered = False
    for index, humidity in enumerate(humidity_levels):
        if(humidity<=0.2):
            print("Trocken")
            if(not one_container_watered):
                watering_routine(index)
                one_container_watered = True
            else:
                print("There already was one watering this loop. Other containers can get water next time. This is to preserve the battery.")
        else:
            #ignore this conatiner
            print("Feucht genug")
    time.sleep(poll_rate_seconds)
    