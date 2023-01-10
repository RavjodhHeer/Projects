from vininfo import Vin
import cv2
import imutils
import numpy as np
import pytesseract

pytesseract.pytesseract.tesseract_cmd = r'/usr/local/bin/tesseract'

def VinDecoder(S):
    try:
        vin = Vin(S)
        country = vin.country
        manufacturer = vin.manufacturer
        region = vin.region
        model = vin.wmi
        serial = vin.vis
        year = vin.years
        print('Vehicle details for', vin, ':')
        print('Model Year:', year)
        print('Manufacturer:', manufacturer)
        print('Model:', model)
        print('Vehicle Serial Number:', serial)
        print('Country Manufactured:', country)
        print('Region Manufactured:', region)
    except:
        print("ERROR: VIN not found, please try again")


def PlateDetector(img_input):
    try:
        img = cv2.imread(img_input, cv2.IMREAD_COLOR)
        img = cv2.resize(img, (600, 400))

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        gray = cv2.bilateralFilter(gray, 13, 15, 15)

        edged = cv2.Canny(gray, 30, 200)
        contours = cv2.findContours(edged.copy(), cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        contours = imutils.grab_contours(contours)
        contours = sorted(contours, key=cv2.contourArea, reverse=True)[:10]
        screenCnt = None

        for c in contours:

            peri = cv2.arcLength(c, True)
            approx = cv2.approxPolyDP(c, 0.018 * peri, True)

            if len(approx) == 4:
                screenCnt = approx
                break

        if screenCnt is None:
            detected = 0
            print("No contour detected")
        else:
            detected = 1

        if detected == 1:
            cv2.drawContours(img, [screenCnt], -1, (0, 0, 255), 3)

        mask = np.zeros(gray.shape, np.uint8)
        new_image = cv2.drawContours(mask, [screenCnt], 0, 255, -1, )
        new_image = cv2.bitwise_and(img, img, mask=mask)

        (x, y) = np.where(mask == 255)
        (topx, topy) = (np.min(x), np.min(y))
        (bottomx, bottomy) = (np.max(x), np.max(y))
        Cropped = gray[topx:bottomx + 1, topy:bottomy + 1]

        text = pytesseract.image_to_string(Cropped, config='--psm 11')

        img = cv2.resize(img, (500, 300))
        Cropped = cv2.resize(Cropped, (400, 200))

        cv2.imshow('car', img)
        cv2.imshow('Cropped', Cropped)

        cv2.waitKey(1000)
        cv2.destroyAllWindows()

        return text
    except:
        print("ERROR: invalid file path, please try again")
        exit()


if __name__ == '__main__':
    inputted_task = input('Would you like to detect license plate from image or decode number via input? (please respond with "detect" or "decode")\n')
    inputted_task = inputted_task.lower()
    if inputted_task == 'decode':
        inputted_vin = input('Please provide the VIN or License Plate number:\n')
        Data = VinDecoder(inputted_vin)
        exit()

    elif inputted_task == 'detect':
        image_input = input('Please provide image file path:\n')
        Data = PlateDetector(image_input)
        if not Data:
            print('ERROR: Cannot detect license plate, please retry with clearer image')
        else:
            print('The detected license plate number is:', Data)
        exit()
    else:
        print('ERROR: Invalid input, please try again.')
        exit()