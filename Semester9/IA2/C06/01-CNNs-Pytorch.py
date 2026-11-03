# https://pytorch.org/hub/pytorch_vision_alexnet/
# https://pytorch.org/vision/main/models/generated/torchvision.models.alexnet.html
# https://discuss.pytorch.org/t/why-does-alexnet-in-torch-vision-use-average-pooling/84313
# https://discuss.pytorch.org/t/alexnet-input-size-224-or-227/41272/3

import numpy as np
import cv2 as cv
from torchvision import transforms
import torchvision.models as models
import torch

preprocess = transforms.Compose([
    transforms.ToTensor(),
    transforms.Resize(256),
    transforms.CenterCrop(224),   
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])


model = models.alexnet(weights='DEFAULT')
# dalsi architektury (nizsi varianty; vzdy jen jednu odkomentovat):
#model = models.vgg11(weights='DEFAULT')
#model = models.resnet18(weights='DEFAULT')
#model = models.densenet121(weights='DEFAULT')
#model = models.googlenet(weights='DEFAULT')          # Inception v1
#model = models.mobilenet_v2(weights='DEFAULT')       # MobileNet v1 v torchvision neni
#model = models.mobilenet_v3_small(weights='DEFAULT')
# inception_v3 chce vstup 299x299 -> v preprocess Resize(342) a CenterCrop(299)
#model = models.inception_v3(weights='DEFAULT') # https://arxiv.org/pdf/1512.00567

print(model)

image = cv.imread("dog.jpg")
image_rgb = cv.cvtColor(image, cv.COLOR_BGR2RGB)

input_tensor = preprocess(image_rgb)
#print(input_tensor.shape)

input_tensor = input_tensor.unsqueeze(0)
#print(input_tensor.shape)

model.eval()
with torch.no_grad():
    output = model(input_tensor)
probabilities = torch.nn.functional.softmax(output[0], dim=0)
#print(probabilities)

with open("imagenet_classes.txt", "r") as f:
    categories = [s.strip() for s in f.readlines()]
# Show top categories per image
top5_prob, top5_catid = torch.topk(probabilities, 5)
for i in range(top5_prob.size(0)):
    print(categories[top5_catid[i]], top5_prob[i].item())









