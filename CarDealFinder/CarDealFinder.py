#!/bin/env/ python
import datetime
from datetime import datetime
import re
from selenium import webdriver
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys

#   'Buy New' or 'Buy Used' -> x
#   'By Body Style
#       ->  SUV/Crossover, Sedan, Pickup Truck, Coupe, Hatchback, Convertible, Wagon, Minivan
#   'To' Price
#       -> Round Down to the nearest price range
#   'NEAR' Zip Code
#   'Search'
#   Oldest Model Year?  -
#   Hide vehicles with Accidents, Frame Damage, Theft, Fleet, Lemon, Salvage
#   Return Vehicle Information
#       -> Year:
#       -> Make:
#       -> Model:
#       -> Trim: 
#       -> Mileage:
#       -> Price:
#       -> Savings (Cargurus Deal Rating):
#       -> Link:
cars = ['suv', 'sedan', 'truck', 'coupe', 'hatchback', 'convertible', 'wagon', 'minivan']



def SearchVehicle(x, bodystyle, price, zipcode, minyear):
    year = 2017
    make = 'Mercedes-Benz'
    model = 'c300'
    mileage = '70,000'
    defprice = '$24,000'
    savings = '$3,300'
    link = 'www.ravjodhheer.com'

    if price < 10000:
        price = price // 1000
        price *= 1000
    elif price < 20000:
        price = price // 2000
        price *= 2000
    elif price < 100000:
        price = price // 5000
        price *= 5000
    else:
        price = price // 10000
        price *= 10000

    print(price)

    driver = webdriver.Chrome()
    driver.get('https://www.cargurus.com/')

    if x == 'used':
        driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/label[2]/span").click()

        #Select "By body style"
        driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[2]/div/div/label[2]/span").click()

        #Select Body Style
        for i in range(len(cars)):
            if bodystyle == cars[i]:
                xpath = "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[2]/div/div/div[2]/div/form/fieldset/div[1]/div/div[" + str(i) + "]"
                driver.find_element(By.XPATH, xpath).click()
        
        #Select option div xpath by directly inputting price var
        #driver.find_element(By.CSS_SELECTOR, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[3]/div/div/div[2]/div/form/fieldset/div[1]/div/div[2]/div[2]/div/h3").click()

        #Enter Zipcode
        zip = driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[2]/div/div/div[2]/div/form/fieldset/div[2]/div[3]/div/input")
        zip.send_keys(zipcode)

        #Click Submit
        driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[2]/div/div/div[2]/div/form/fieldset/div[2]/input").click()

        #Continue search on vehicle
        
    elif x == 'new':
        driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/label[3]/span").click()

        #Select "By body style"
        driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[3]/div/div/label[2]/span").click()

        #Select Body Style
        for i in range(len(cars)):
            if bodystyle == cars[i]:
                xpath = "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[3]/div/div/div[2]/div/form/fieldset/div[1]/div/div[" + str(i) + "]"
                driver.find_element(By.XPATH, xpath).click()
        
        #Select option div xpath by directly inputting price var
        #driver.find_element(By.CSS_SELECTOR, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[3]/div/div/div[2]/div/form/fieldset/div[1]/div/div[2]/div[2]/div/h3").click()

        #Enter Zipcode
        zip = driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[3]/div/div/div[2]/div/form/fieldset/div[2]/div[3]/div/input")
        zip.send_keys(zipcode)

        #Click Submit
        driver.find_element(By.XPATH, "/html/body/main/div[2]/div[2]/div[1]/section/div/div/div[3]/div/div/div[2]/div/form/fieldset/div[2]/input").click()

        #Continue search on vehicle

    else:
        print('Error!\n')
        return
    

    print('---------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n')
    print('We found this vehicle for you!:\n')
    print(f'Year:{year}\nMake:{make}\nModel:{model}\nMileage:{mileage}\nPrice:{price}\nSavings{savings}\nLink:{link}\n')
    print('---------------------------------------------------------------------------------------------------------------------------------------------------------------------------')





if __name__ == '__main__':
    suvs = ['suv', 'suv crossover', 'suv/crossover', 'crossover']
    trucks = ['pickup', 'truck', 'pickup truck', 'pickup/truck']
    print('---------------------------------------------------------------------------------------------------------------------------------------------------------------------------')
    print('Welcome to CarDealFinder, an interactive application built to help you easily search for and save money on your next car purchase!\n')

    while True:
        x = str(input('Are you looking for a new or used vehicle today? (e.g. Used)\n').lower())
        if x == 'used' or x == 'new':
            break
        else:
            print('Please only reply with "Used" or "New"!\n')
            continue

    while True:
        bodystyle = str(input('Which body style of vehicle are you searching for? (Pick one: SUV/Crossover, Sedan, Pickup Truck, Coupe, Hatchback, Convertible, Wagon, Minivan)\n').lower())
        if bodystyle not in cars:
            print('Please choose one of the body style options above!\n')
            continue
        else:
            break

    price = str(input('What is your maximum price range? (e.g. 25,000)\n').lower())
    price = re.findall(r'\d+', price)
    s = ''
    price = s.join(price)
    price = int(price)

    while True:
        if x == 'used':   
            minyear = str(input('What is the oldest year you wish to search for? (e.g. 2014)\n'))
            minyear = re.findall(r'\d+', minyear)
            s = ''
            minyear = s.join(minyear)
            minyear = int(minyear)
            curr = datetime.now().year + 1
            if minyear > curr or minyear < 1930:
                print(f'Please pick a valid year between 1930 and {curr}!\n')
                continue
            else:
                break
        else:
            minyear = datetime.now().year
            break

    while True:
        zipcode = str(input('What zipcode will you be searching in? (e.g. 12345)\n'))
        s = ''
        zipcode = s.join(zipcode)
        zipcode = int(zipcode)
        if zipcode > 99950 or zipcode < 501:
            print(f'Please pick a valid zipcode between 00501 and 99950!\n')
            continue
        else:
            break
    
    if bodystyle in suvs:
        bodystyle = 'suv'
    elif bodystyle in trucks:
        bodystyle = 'truck'

    print(x, bodystyle, price, minyear, zipcode)

    SearchVehicle(x, bodystyle, price, minyear, zipcode)

    exit()