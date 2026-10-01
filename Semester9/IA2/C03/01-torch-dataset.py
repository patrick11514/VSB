import numpy as np
import torch
import torchvision
import torchvision.transforms as transforms

IMG_SIZE = 64

label_map__ = {
    0: "T-Shirt",
    1: "Trouser",
    2: "Pullover",
    3: "Dress",
    4: "Coat",
    5: "Sandal",
    6: "Shirt",
    7: "Sneaker",
    8: "Bag",
    9: "Ankle Boot",
}

label_map = {
    0: "Speed limit (20km/h)",
    1: "Speed limit (30km/h)",
    2: "Speed limit (50km/h)",
    3: "Speed limit (60km/h)",
    4: "Speed limit (70km/h)",
    5: "Speed limit (80km/h)",
    6: "End of speed limit (80km/h)",
    7: "Speed limit (100km/h)",
    8: "Speed limit (120km/h)",
    9: "No passing",
    10: "No passing for vehicles over 3.5 metric tons",
    11: "Right-of-way at the next intersection",
    12: "Priority road",
    13: "Yield",
    14: "Stop",
    15: "No vehicles",
    16: "Vehicles over 3.5 metric tons prohibited",
    17: "No entry",
    18: "General caution",
    19: "Dangerous curve to the left",
    20: "Dangerous curve to the right",
    21: "Double curve",
    22: "Bumpy road",
    23: "Slippery road",
    24: "Road narrows on the right",
    25: "Road work",
    26: "Traffic signals",
    27: "Pedestrians",
    28: "Children crossing",
    29: "Bicycles crossing",
    30: "Beware of ice/snow",
    31: "Wild animals crossing",
    32: "End of all speed and passing limits",
    33: "Turn right ahead",
    34: "Turn left ahead",
    35: "Ahead only",
    36: "Go straight or right",
    37: "Go straight or left", 
    38: "Keep right",
    39: "Keep left",
    40: "Roundabout mandatory",
    41: "End of no passing",
    42: "End of no passing by vehicles over 3.5 metric tons"
}

transform = transforms.Compose(
    [transforms.ToTensor(),
     transforms.Resize((IMG_SIZE, IMG_SIZE), antialias=True),
     ])

#trainset = torchvision.datasets.FashionMNIST(root='./data', train=True,
#                                        download=True, transform=transform)

trainset = torchvision.datasets.GTSRB(root='./data', split='train', download=True, transform=transform)

#testset = torchvision.datasets.FashionMNIST(root='./data', train=False,
#                                       download=True, transform=transform)

testset = torchvision.datasets.GTSRB(root='./data', split='test', download=True, transform=transform)


import matplotlib.pyplot as plt
import numpy as np



BATH_SIZE = 8

testloader = torch.utils.data.DataLoader(testset, batch_size=BATH_SIZE,
                                         shuffle=False)

trainloader = torch.utils.data.DataLoader(trainset, batch_size=BATH_SIZE,
                                          shuffle=True)
                                          
                                          
class Model_1(torch.nn.Module):
    def __init__(self, img_size):
        super().__init__()
        self.layers = torch.nn.Sequential(
            torch.nn.Flatten(),
            torch.nn.Linear(img_size*img_size*3, 8),
            torch.nn.Linear(8, 43)
        )
                
    def forward(self, x):
        return self.layers(x) 


net = Model_1(IMG_SIZE)
print(net)

import torch.optim as optim
criterion = torch.nn.CrossEntropyLoss()
optimizer = optim.SGD(net.parameters(), lr=0.001)

for epoch in range(4):  # loop over the dataset multiple times
    
    running_loss = 0.0
    for i, data in enumerate(trainloader, 0):
        # get the inputs; data is a list of [inputs, labels]
        inputs, labels = data

        # zero the parameter gradients
        optimizer.zero_grad()

        # forward + backward + optimize
        outputs = net(inputs)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()

        # print statistics
        running_loss += loss.item()
        if i % 20 == 19:    # print every 20 mini-batches
            print(f'[{epoch + 1}, {i + 1:5d}] loss: {running_loss / 20:.3f}')
            running_loss = 0.0
            
print('Finished Training')


net.eval()

"""
Finished Training
Traceback (most recent call last):
  File "/home/patrick115/Projects/VSB/Semester9/IA2/C03/01-torch-dataset.py", line 152, in <module>
    plt.imshow(img.squeeze(), cmap='gray')
    ~~~~~~~~~~^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/home/patrick115/ENVS/ImageAnalysis/lib/python3.14/site-packages/matplotlib/pyplot.py", line 3784, in imshow
    __ret = gca().imshow(
        X,
    ...<16 lines>...
        **kwargs,
    )
  File "/home/patrick115/ENVS/ImageAnalysis/lib/python3.14/site-packages/matplotlib/__init__.py", line 1531, in inner
    return func(
        ax,
        *map(cbook.sanitize_sequence, args),
        **{k: cbook.sanitize_sequence(v) for k, v in kwargs.items()})
  File "/home/patrick115/ENVS/ImageAnalysis/lib/python3.14/site-packages/matplotlib/axes/_axes.py", line 6377, in imshow
    im.set_data(X)
    ~~~~~~~~~~~^^^
  File "/home/patrick115/ENVS/ImageAnalysis/lib/python3.14/site-packages/matplotlib/image.py", line 716, in set_data
    self._A = self._normalize_image_array(A)
              ~~~~~~~~~~~~~~~~~~~~~~~~~~~^^^
  File "/home/patrick115/ENVS/ImageAnalysis/lib/python3.14/site-packages/matplotlib/image.py", line 684, in _normalize_image_array
    raise TypeError(f"Invalid shape {A.shape} for image data")
TypeError: Invalid shape (3, 64, 64) for image data
"""

# Render the images in grid with their predicted and true labels
for i in range(1,26):
    img, label = testset[np.random.randint(0, len(testset))]
    with torch.no_grad():
        output = net(img.unsqueeze(0))
        pred = torch.argmax(output, dim=1).item()
    plt.title(f"Pred: {label_map[pred]}\nTrue: {label_map[label]}")
    plt.subplot(5, 5, i)
    plt.imshow(img.permute(1, 2, 0))  # Change the shape from (C, H, W) to (H, W, C) for displaying     

plt.show()
    