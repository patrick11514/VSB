import torch
import torch.nn as nn
import torch.optim as optim
import torchvision
import torchvision.transforms as transforms
from torch.utils.data import DataLoader

class LeNet5(nn.Module):
    def __init__(self, num_classes=43):
        super(LeNet5, self).__init__()
        self.conv1 = nn.Conv2d(3, 6, kernel_size=5, padding=2)
        self.relu1 = nn.ReLU()
        self.pool1 = nn.AvgPool2d(kernel_size=2, stride=2)

        self.conv2 = nn.Conv2d(6, 16, kernel_size=5)
        self.relu2 = nn.ReLU()
        self.pool2 = nn.AvgPool2d(kernel_size=2, stride=2)

        self.flatten = nn.Flatten()

        self.fc1 = nn.LazyLinear(120)
        self.relu3 = nn.ReLU()
        self.fc2 = nn.Linear(120, 84)
        self.relu4 = nn.ReLU()
        self.fc3 = nn.Linear(84, num_classes)

    def forward(self, x):
        x = self.pool1(self.relu1(self.conv1(x)))
        x = self.pool2(self.relu2(self.conv2(x)))
        x = self.flatten(x)
        x = self.relu3(self.fc1(x))
        x = self.relu4(self.fc2(x))
        x = self.fc3(x)
        return x

# Device configuration
device = torch.device('cpu')

# Data transformations including resize to 32x32 and normalization
transform = transforms.Compose([
    transforms.Resize((32, 32)),
    transforms.ToTensor(),
    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))
])

# Load GTSRB dataset (train and test)
#train_dataset = torchvision.datasets.GTSRB(root='./data', split='train', download=True, transform=transform)
#test_dataset = torchvision.datasets.GTSRB(root='./data', split='test', download=True, transform=transform)

train_dataset = torchvision.datasets.ImageFolder(root="train_images", transform=transform)

train_loader = DataLoader(train_dataset, batch_size=64, shuffle=True)
test_loader = DataLoader(train_dataset, batch_size=64, shuffle=False)

# Initialize model, loss, and optimizer
model = LeNet5(num_classes=2).to(device)
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(), lr=0.1)

# Training loop
num_epochs = 15  # Adjust as needed
for epoch in range(num_epochs):
    model.train()
    running_loss = 0.0
    for images, labels in train_loader:
        images = images.to(device)
        labels = labels.to(device)

        # Forward pass
        outputs = model(images)
        loss = criterion(outputs, labels)

        # Backward and optimize
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        running_loss += loss.item()
        _, predicted = outputs.max(1)

    print(f"Epoch [{epoch+1}/{num_epochs}], Loss: {running_loss/len(train_loader):.4f}")

# Evaluation on test dataset
model.eval()
correct = 0
total = 0
with torch.no_grad():
    for images, labels in test_loader:
        images = images.to(device)
        labels = labels.to(device)

        outputs = model(images)
        _, predicted = outputs.max(1)
        total += labels.size(0)
        correct += predicted.eq(labels).sum().item()

print(f"Test Accuracy: {100.*correct/total:.2f}%")

import matplotlib.pyplot as plt
import random
import numpy as np

# Function to denormalize and display an image
def imshow(img):
    img = img / 2 + 0.5  # Denormalize: convert from [-1,1] to [0,1] range
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))  # Convert CHW to HWC for matplotlib
    plt.axis('off')  # Hide axis for better visualization

# Class names from the dataset if available, otherwise numeric labels
classes = train_dataset.classes if hasattr(train_dataset, 'classes') else [str(i) for i in range(2)]

# Select 10 random images from the first batch of test_loader
model.eval()
dataiter = iter(test_loader)
images_all, labels_all = next(dataiter)

random_indices = random.sample(range(len(images_all)), 10)
images = images_all[random_indices]
labels = labels_all[random_indices]

# Move images to device and run through the model to get predictions
images = images.to(device)
outputs = model(images)
_, predicted = outputs.max(1)

# Move tensors back to CPU for visualization
images = images.cpu()
labels = labels.cpu()
predicted = predicted.cpu()

# Plot the 10 images with true and predicted labels
plt.figure(figsize=(15, 6))
for i in range(10):
    plt.subplot(2, 5, i + 1)
    imshow(images[i])
    true_label = classes[labels[i].item()]
    pred_label = classes[predicted[i].item()]
    plt.title(f"True: {true_label}\nPred: {pred_label}")
plt.tight_layout()
plt.show()

# Select test image for visualization
model.eval()
dataiter = iter(test_loader)
images_all, labels_all = next(dataiter)

idx = random.randint(0, len(images_all) - 1)
image = images_all[idx].unsqueeze(0).to(device)
label = labels_all[idx]


# Global list to store feature maps from multiple layers
feature_maps = []

# Hook function that appends outputs to the list
def hook_fn(module, input, output):
    feature_maps.append(output.detach().cpu())

hooks = []
hooks.append(model.relu1.register_forward_hook(hook_fn))
hooks.append(model.relu2.register_forward_hook(hook_fn))

# Forward pass with the selected image
with torch.no_grad():
    model(image)

# Remove hooks
for hook in hooks:
    hook.remove()

# Visualization of feature maps from both layers
plt.figure(figsize=(20, 10))

# Max 6 filters from the first layer
for i in range(min(6, feature_maps[0].shape[1])):
    plt.subplot(2, 6, i + 1)
    plt.imshow(feature_maps[0][0, i], cmap='gray')
    plt.title(f'Conv1 Filter {i+1}')
    plt.axis('off')

# Max 6 filters from the second layer
for i in range(min(6, feature_maps[1].shape[1])):
    plt.subplot(2, 6, i + 7)
    plt.imshow(feature_maps[1][0, i], cmap='gray')
    plt.title(f'Conv2 Filter {i+1}')
    plt.axis('off')

plt.tight_layout()
plt.show()



