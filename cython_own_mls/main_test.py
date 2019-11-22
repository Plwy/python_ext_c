import cv2
import numpy as np
import time
import mls 

img = cv2.imread('/home/zsl/Test_datas/00FACE_PICS/difshape_face/068.png')
img = np.asarray(img, dtype=np.uint8)
h,w,c = img.shape

dst_data = np.zeros(img.shape, dtype=np.uint8)
x1 = np.asarray([85, 101, 140, 326, 310, 273])
y1 = np.asarray([302, 339, 376, 300, 343, 377])
x2 = np.asarray([105, 119, 156, 305, 296, 261])
y2 = np.asarray([304, 335, 370, 299, 335, 368])
n = 6

start_time = time.time()
mls.mls_deformation(h, w, img, dst_data, x1, y1, n, x2, y2)
end_time = time.time()
print("total time: {0:}".format(end_time - start_time))
cv2.imshow("dst_data", dst_data)
cv2.waitKey(0)