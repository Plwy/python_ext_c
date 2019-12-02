import cv2
import numpy as np
import time
import pymls 
import dlib

img = cv2.imread('/home/zsl/Test_datas/00FACE_PICS/difshape_face/0068.png')
h,w,c = img.shape
# cv2.imshow("img", img)
# cv2.waitKey(0)
# print(type(img))

dst_data = np.zeros(img.shape, dtype=np.uint8)

x1 = np.array([85, 101, 140, 326, 310, 273])
y1 = np.array([302, 339, 376, 300, 343, 377])
x2 = np.array([105, 119, 156, 305, 296, 261])
y2 = np.array([304, 335, 370, 299, 335, 368])
for i in range(6):
    cv2.circle(img, (x1[i],y1[i]), 2, (255,0,0),-1)
for i in range(6):
    cv2.circle(img, (x2[i],y2[i]), 2, (0,255,0),-1)
cv2.imshow('img_circle', img)
cv2.waitKey(0)

n = 6
start_time = time.time()
pymls.mls_deformation(h, w, img, dst_data, x1, y1, n, x2, y2)
end_time = time.time()
print("total time: {0:}".format(end_time - start_time))
cv2.imshow("dst_data", dst_data)
cv2.waitKey(0)