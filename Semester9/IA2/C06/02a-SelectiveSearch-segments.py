# pip install opencv-contrib-python

import cv2
import numpy as np

rng = np.random.default_rng(0)

img = cv2.resize(cv2.imread("Gemini_Generated_Image-02.jpeg"), None, fx=0.5, fy=0.5)
#img = cv2.medianBlur(img, 3)

# segmentace obrazu na oblasti (prvni krok selective search): kazdy pixel dostane cislo segmentu
g_seg = cv2.ximgproc.segmentation.createGraphSegmentation(sigma=0.8, k=300, min_size=50)
labels = g_seg.processImage(img)
print("segmentu:", labels.max() + 1)

# kazdy segment zabarvime nahodnou barvou
palette = rng.integers(0, 255, (labels.max() + 1, 3), dtype=np.uint8)

cv2.imshow("original", img)
cv2.imshow("graph segmentation - segmenty", palette[labels])
cv2.waitKey()
