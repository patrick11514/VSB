import cv2
import torch
import torchvision
import torchvision.transforms as transforms

coco_names = [
    '__background__', 'person', 'bicycle', 'car', 'motorcycle', 'airplane', 'bus',
    'train', 'truck', 'boat', 'traffic light', 'fire hydrant', 'N/A', 'stop sign',
    'parking meter', 'bench', 'bird', 'cat', 'dog', 'horse', 'sheep', 'cow',
    'elephant', 'bear', 'zebra', 'giraffe', 'N/A', 'backpack', 'umbrella', 'N/A', 'N/A',
    'handbag', 'tie', 'suitcase', 'frisbee', 'skis', 'snowboard', 'sports ball',
    'kite', 'baseball bat', 'baseball glove', 'skateboard', 'surfboard', 'tennis racket',
    'bottle', 'N/A', 'wine glass', 'cup', 'fork', 'knife', 'spoon', 'bowl',
    'banana', 'apple', 'sandwich', 'orange', 'broccoli', 'carrot', 'hot dog', 'pizza',
    'donut', 'cake', 'chair', 'couch', 'potted plant', 'bed', 'N/A', 'dining table',
    'N/A', 'N/A', 'toilet', 'N/A', 'tv', 'laptop', 'mouse', 'remote', 'keyboard', 'cell phone',
    'microwave', 'oven', 'toaster', 'sink', 'refrigerator', 'N/A', 'book',
    'clock', 'vase', 'scissors', 'teddy bear', 'hair drier', 'toothbrush'
]

cv2.namedWindow("result", 0)
one_img = cv2.imread("Gemini_Generated_Image-01.jpeg")
one_img_rgb = cv2.cvtColor(one_img, cv2.COLOR_BGR2RGB)
# FPN (Feature Pyramid Network, 2017)
#   Spojuje priznakove mapy z ruznych vrstev site (v ruznem rozliseni), aby se dobre detekovaly objekty ruznych velikosti.
model = torchvision.models.detection.fasterrcnn_mobilenet_v3_large_320_fpn(weights='DEFAULT')
# dalsi dvoufazove detektory:
#model = torchvision.models.detection.fasterrcnn_mobilenet_v3_large_fpn(weights='DEFAULT')
#model = torchvision.models.detection.fasterrcnn_resnet50_fpn(weights='DEFAULT')
#model = torchvision.models.detection.fasterrcnn_resnet50_fpn_v2(weights='DEFAULT')
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
# self.device = "cpu"
model.eval().to(device)
print("device", device)
print("model", model)
transformRCNN = transforms.Compose([
    transforms.ToTensor()
])
tensor = transformRCNN(one_img_rgb)
print(tensor.shape)
#add batch size
tensor = tensor.unsqueeze(0).to(device)
with torch.no_grad():
    outputsRCNN = model(tensor)
pred_classes = [coco_names[i] for i in outputsRCNN[0]['labels'].cpu().numpy()]
pred_scores = outputsRCNN[0]['scores'].detach().cpu().numpy()
pred_bboxes = outputsRCNN[0]['boxes'].detach().cpu().numpy()

print("pred_bboxes", pred_bboxes)
print("pred_scores", pred_scores)
print("pred_classes", pred_classes)

for (x1, y1, x2, y2), score, name in zip(pred_bboxes, pred_scores, pred_classes):
    if score < 0.5: continue
    cv2.rectangle(one_img, (int(x1), int(y1)), (int(x2), int(y2)), (0, 255, 0), 3)
    cv2.putText(one_img, f"{name}:{score:.2f}", (int(x1), int(y1) - 5), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0, 255, 0), 3)

cv2.imshow("result", one_img)
cv2.waitKey()


# Faster R-CNN neni konec vetve, dvoufazove detektory se vyvijeji dal:
#
# Mask R-CNN (2017)
#   Stejna sit jako Faster R-CNN, ktera ke kazdemu ramecku navic predikuje masku objektu (segmentace).
#
# Cascade R-CNN (2018)
#   Za RPN nasleduje nekolik hlav za sebou, kazda zpresnuje ramecky z predchozi a je trenovana s prisnejsim
#   prahem IoU (napr. 0.5 -> 0.6 -> 0.7). Vysledne ramecky jsou presnejsi, aniz by se zmenila zakladni sit.
#
# Sparse R-CNN (2021) + Dynamic Sparse R-CNN
#   Misto anchoru a RPN pouziva male pevne mnozstvi naucenych navrhu (napr. 100) a trenuje se tak, ze kazdy
#   objekt dostane prave jednu predikci. Diky tomu nepotrebuje NMS.
