import cv2
import numpy as np

img = cv2.VideoCapture(0)
cv2.putText(img," OPENCV ",(300,200),cv2.FONT_HERSHEY_COMPLEX,1,(0,150,0),3)

while True:
    success, img = cap.read()
    cv2.imshow("Video",img)
    if cv2.waitKey(1) & 0XFF ==ord("q"):
        break









