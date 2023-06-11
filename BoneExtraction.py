import cv2
import os
import numpy as np



if __name__ == '__main__':

    
    gray = cv2.imread("ct_images/P25_S86_00315.jpg", cv2.IMREAD_GRAYSCALE)
    gray = cv2.resize(gray, (0, 0), fx=0.5, fy=0.5)

    _, thresh = cv2.threshold(gray, 200, 255, cv2.THRESH_BINARY)
    #dilate image
    thresh = cv2.dilate(thresh, cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3)), iterations=1)

    contours, hierarchy = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    #rgb_img = cv2.cvtColor(thresh, cv2.COLOR_GRAY2RGB)
    rgb_img = np.zeros((thresh.shape[0], thresh.shape[1], 3), np.uint8)

    cv2.drawContours(rgb_img, contours, -1, (0, 255, 0), 3)
    cv2.drawContours(rgb_img, contours, -1, (255,255,255), thickness=cv2.FILLED)
    cv2.fillPoly(rgb_img, pts=contours, color=(255,255,0))

    cv2.imshow("Original", rgb_img)
    cv2.waitKey(0)
    
    cv2.imshow("Output", thresh)
    cv2.waitKey(0)
    cv2.destroyAllWindows()


