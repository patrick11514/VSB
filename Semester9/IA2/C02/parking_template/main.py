#!/usr/bin/python

import sys
import cv2
import numpy as np
import math
import struct
from datetime import datetime
from time import time

import glob

import matplotlib.pyplot as plt

from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_auc_score

import skimage as ski

import joblib


feature_types = ['type-2-x', 'type-2-y']

def extract_feature_image(img, feature_type, feature_coord=None):
    """Extract the haar feature for the current image"""
    ii = ski.transform.integral_image(img)
    return ski.feature.haar_like_feature(
        ii,
        0,
        0,
        ii.shape[0],
        ii.shape[1],
        feature_type=feature_type,
        feature_coord=feature_coord,
    )

def order_points(pts):
    # initialzie a list of coordinates that will be ordered
    # such that the first entry in the list is the top-left,
    # the second entry is the top-right, the third is the
    # bottom-right, and the fourth is the bottom-left
    rect = np.zeros((4, 2), dtype = "float32")
    # the top-left point will have the smallest sum, whereas
    # the bottom-right point will have the largest sum
    s = pts.sum(axis = 1)
    rect[0] = pts[np.argmin(s)]
    rect[2] = pts[np.argmax(s)]
    # now, compute the difference between the points, the
    # top-right point will have the smallest difference,
    # whereas the bottom-left will have the largest difference
    diff = np.diff(pts, axis = 1)
    rect[1] = pts[np.argmin(diff)]
    rect[3] = pts[np.argmax(diff)]
    # return the ordered coordinates
    return rect

def extract_rectangle(image, one_c):
    #https://www.pyimagesearch.com/2014/08/25/4-point-opencv-getperspective-transform-example/
    
    pts = [((float(one_c[0])), float(one_c[1])),
            ((float(one_c[2])), float(one_c[3])),
            ((float(one_c[4])), float(one_c[5])),
            ((float(one_c[6])), float(one_c[7]))]
    
    # obtain a consistent order of the points and unpack them
    # individually
    rect = order_points(np.array(pts))
    (tl, tr, br, bl) = rect
    # compute the width of the new image, which will be the
    # maximum distance between bottom-right and bottom-left
    # x-coordiates or the top-right and top-left x-coordinates
    widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
    widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
    maxWidth = max(int(widthA), int(widthB))
    # compute the height of the new image, which will be the
    # maximum distance between the top-right and bottom-right
    # y-coordinates or the top-left and bottom-left y-coordinates
    heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
    heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
    maxHeight = max(int(heightA), int(heightB))
    # now that we have the dimensions of the new image, construct
    # the set of destination points to obtain a "birds eye view",
    # (i.e. top-down view) of the image, again specifying points
    # in the top-left, top-right, bottom-right, and bottom-left
    # order
    dst = np.array([
	    [0, 0],
	    [maxWidth - 1, 0],
	    [maxWidth - 1, maxHeight - 1],
	    [0, maxHeight - 1]], dtype = "float32")
    # compute the perspective transform matrix and then apply it
    M = cv2.getPerspectiveTransform(rect, dst)
    warped = cv2.warpPerspective(image, M, (maxWidth, maxHeight))
    # return the warped image
    return warped

    
def main(argv):

    pkm_file = open('parking_map_python.txt', 'r')
    pkm_lines = pkm_file.readlines()
    pkm_coordinates = []
   
    for line in pkm_lines:
        st_line = line.strip()
        sp_line = list(st_line.split(" "))
        pkm_coordinates.append(sp_line)

    model = joblib.load("../clf_face.joblib")

      
    test_images = [img for img in glob.glob("test_images/*.jpg")]
    test_images.sort()

    cv2.namedWindow("parking_template", cv2.WINDOW_FREERATIO)    
    cv2.namedWindow("parking_solo", cv2.WINDOW_FREERATIO)
    cv2.namedWindow("parking_edge", cv2.WINDOW_FREERATIO)



    for i in test_images:
        img = cv2.imread(i)

        
        for coord in pkm_coordinates:
            place = extract_rectangle(img, coord)
            grayscale = cv2.cvtColor(place, cv2.COLOR_BGR2GRAY)
            grayscale = cv2.resize(grayscale, (25, 25))
            features = extract_feature_image(grayscale, feature_types)

            result = model.predict_proba(features.reshape(1, -1))

            place = cv2.resize(place, (200, 200))

            blurred = cv2.GaussianBlur(place, (5, 5), 0)
            edge_place = cv2.Canny(blurred, 80, 140)

            edge_pix_count = cv2.countNonZero(edge_place)

            p1 = (int(coord[0]), int(coord[1]))
            p2 = (int(coord[4]), int(coord[5]))

            if edge_pix_count > 500:
                cv2.line(img, p1, p2, (0, 0, 255), 2)
            else: 
                cv2.line(img, p1, p2, (0, 255, 0), 2)


            cv2.imshow("parking_solo", place)
            cv2.putText(edge_place, str(result), (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255), 2)

            cv2.imshow("parking_edge", edge_place)
            cv2.imshow("parking_template", img)


            cv2.waitKey(200)

        cv2.imshow("parking_template", img)
        cv2.waitKey(0)
    
if __name__ == "__main__":
   main(sys.argv[1:])     
