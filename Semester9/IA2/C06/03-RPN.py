# Navrhy oblasti pomoci RPN (Region Proposal Network) z predtrenovaneho Faster R-CNN.
# Stejny krok jako selective search ve skriptu 02, ale pocita ho sit nad mapou priznaku.

import time
import cv2
import torch
import torchvision
import torchvision.transforms as transforms

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model = torchvision.models.detection.fasterrcnn_mobilenet_v3_large_320_fpn(weights='DEFAULT')
model.eval().to(device)

top_n = 20   # kolik nejlepsich navrhu vykreslit

one_img = cv2.imread("Gemini_Generated_Image-01.jpeg")
one_img_res = cv2.resize(one_img, None, fx = 0.7, fy = 0.7 )
one_img_paint = one_img_res.copy()
img_h, img_w = one_img_res.shape[:2]

tensor = transforms.ToTensor()(cv2.cvtColor(one_img_res, cv2.COLOR_BGR2RGB)).to(device)

t0 = time.time()
with torch.no_grad():
    # transform: normalizace + zmena velikosti (model pracuje na jine velikosti nez originalni obrazek)
    images, _ = model.transform([tensor])
    # backbone + FPN: priznakove mapy v ruznych rozlisenich
    features = model.backbone(images.tensors)
    # RPN: navrhy oblasti (objectness + posun anchoru + NMS)
    proposals, _ = model.rpn(images, features)
print("cas RPN [s]:", time.time() - t0)

boxes = proposals[0]
print("pocet navrhu:", len(boxes))

# navrhy jsou v souradnicich obrazku po transform -> prepocet na puvodni obrazek
new_h, new_w = images.image_sizes[0]
scale_x, scale_y = img_w / new_w, img_h / new_h

# navrhy jsou serazene podle skore (objectness) od nejlepsiho
for x1, y1, x2, y2 in boxes[:top_n].cpu().numpy():
    x1, x2 = int(x1 * scale_x), int(x2 * scale_x)
    y1, y2 = int(y1 * scale_y), int(y2 * scale_y)
    cv2.rectangle(one_img_paint, (x1, y1), (x2, y2), (0, 255, 0), 3)

cv2.imshow("one_img_res", one_img_res)
cv2.imshow("one_img_paint", one_img_paint)
cv2.waitKey()
