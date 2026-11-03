# pip install opencv-contrib-python
# https://stackoverflow.com/questions/57427233/module-cv2-cv2-has-no-attribute-ximgproc

# http://www.huppelen.nl/publications/selectiveSearchDraft.pdf

import os
import cv2

from torchvision import transforms
import torchvision.models as models
import torch
import torchvision

model = models.resnet18(weights='DEFAULT').eval()

with open("imagenet_classes.txt", "r") as f:
    categories = [s.strip() for s in f.readlines()]

preprocess = transforms.Compose([
    transforms.ToTensor(),
    transforms.Resize((224, 224)),
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])

one_img = cv2.imread("Gemini_Generated_Image-02.jpeg")
one_img_res = cv2.resize(one_img, None, fx = 0.7, fy = 0.7 ) 
one_img_blur = cv2.medianBlur(one_img_res, 7)
one_img_res_paint = one_img_res.copy()
one_img_res_nms = one_img_res.copy()
s_search = cv2.ximgproc.segmentation.createSelectiveSearchSegmentation()
s_search.setBaseImage(one_img_blur)
#s_search.switchToSelectiveSearchQuality()  
#s_search.switchToSelectiveSearchFast()              
s_search.switchToSingleStrategy()

out_dir = "crops"
os.makedirs(out_dir, exist_ok=True)

img_h, img_w = one_img_res.shape[:2]
max_area_ratio = 0.5   # zahodit navrhy zabirajici vic nez tuto cast obrazku

nms_iou_thr = 0.3     # rámečky stejné třídy s vyšším překryvem se potlačí

det_boxes, det_scores, det_ids = [], [], []

rects = s_search.process()
print(len(rects))
for i in range(0, len(rects)):
    x, y, w, h = rects[i]
    if h < 30 or w < 30: continue
    if w * h > max_area_ratio * img_w * img_h: continue
    cv2.rectangle(one_img_blur, (x, y), (x + w, y + h), (0, 255, 0), 3)

    ### 1 ###
    cropp = one_img_res[y:y+h, x:x+w]
    cropp_rgb = cv2.cvtColor(cropp, cv2.COLOR_BGR2RGB)
    
    cv2.imwrite(os.path.join(out_dir, f"crop_{i:04d}_x{x}_y{y}_w{w}_h{h}.jpg"), cropp)
    
    cv2.imshow("cropp", cropp)
    cv2.waitKey(30)     
    ### ###
    
    
    ### 2 ###
    input_tensor = preprocess(cropp_rgb) 
    input_tensor = input_tensor.unsqueeze(0) 
    with torch.no_grad():
        output = model(input_tensor)
    output = output.squeeze(0)
    probabilities = torch.nn.functional.softmax(output, dim=0)
    
    #print("output", probabilities)   
    
    top_prob, top_catid = torch.topk(probabilities, 1)
    #print("top_prob", top_prob)
    cat_name, cat_prob = categories[top_catid.item()], top_prob.item()    
    print(cat_name, cat_prob)
    if cat_prob < 0.6: continue
    det_boxes.append([x, y, x + w, y + h])
    det_scores.append(cat_prob)
    det_ids.append(top_catid.item())
    cv2.rectangle(one_img_res_paint, (x, y), (x + w, y + h), (0, 255, 0), 3)    
    cv2.putText(one_img_res_paint, f"{cat_name}:{cat_prob:.2f}", (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 3)

    ### ###


### 3 ###
# NMS (Non-Maximum Suppression): jeden objekt najde mnoho rámečků.
# 1) vezmi rámeček s nejvyšším skóre a ponech ho,
# 2) zahoď všechny rámečky stejné třídy, které se s ním překrývají (IoU) víc než nms_iou_thr,
# 3) opakuj se zbylými rámečky.
# IoU = plocha průniku / plocha sjednocení dvou rámečků (0 = nedotýkají se, 1 = totožné).
# S rámečkem s nejvyšším skóre se porovná každý další rámeček stejné třídy a pokud je jejich IoU
# větší než nms_iou_thr, zahodí se. Při 0.3 se potlačí i rámečky, které se překrývají jen mírně,
# při 0.9 jen ty téměř totožné (vyšší nms_iou_thr = méně potlačování).
print("detekci pred NMS:", len(det_boxes))
if det_boxes:
    keep = torchvision.ops.batched_nms(
        torch.tensor(det_boxes, dtype=torch.float32),
        torch.tensor(det_scores, dtype=torch.float32),
        torch.tensor(det_ids),
        nms_iou_thr)
    print("detekci po NMS:", len(keep))
    for k in keep.tolist():
        x1, y1, x2, y2 = det_boxes[k]
        label = f"{categories[det_ids[k]]}:{det_scores[k]:.2f}"
        print(label)
        cv2.rectangle(one_img_res_nms, (x1, y1), (x2, y2), (0, 255, 0), 3)
        cv2.putText(one_img_res_nms, label, (x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 3)
### ###

cv2.imshow("one_img_res", one_img_res)
cv2.imshow("one_img_blur", one_img_blur)
cv2.imshow("one_img_res_paint", one_img_res_paint)
cv2.imshow("one_img_res_nms", one_img_res_nms)
cv2.waitKey()
