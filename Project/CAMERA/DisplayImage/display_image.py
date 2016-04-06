import cv2 as cv

img = cv.imread("HappyFish.jpg")
#cv.namedWindow("Display window")
cv.imshow("Display window", img)
cv.waitKey(0)
