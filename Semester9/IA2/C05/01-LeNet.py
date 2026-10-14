import torch
import torch.nn as nn

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
        
        self.fc1 = nn.Linear(16 * 6 * 6, 120)
        self.relu3 = nn.ReLU()
        self.fc2 = nn.Linear(120, 84)
        self.relu4 = nn.ReLU()
        self.fc3 = nn.Linear(84, num_classes)
      
    def forward(self, x):
        print(f"Input shape: {x.shape}")
        x = self.conv1(x)
        print(f"After conv1: {x.shape}")
        x = self.relu1(x)
        # ReLU does not change the shape
        x = self.pool1(x)
        print(f"After pool1: {x.shape}")
        
        x = self.conv2(x)
        print(f"After conv2: {x.shape}")
        x = self.relu2(x)
        x = self.pool2(x)
        print(f"After pool2: {x.shape}")
        
        x = self.flatten(x)
        print(f"After flatten: {x.shape}")
        
        x = self.fc1(x)
        print(f"After fc1: {x.shape}")
        x = self.relu3(x)
        x = self.fc2(x)
        print(f"After fc2: {x.shape}")
        x = self.relu4(x)
        x = self.fc3(x)
        print(f"After fc3 (output): {x.shape}")
        
        return x

# Example input: batch 1, 3 channels, 32×32 pixels
x = torch.randn(1, 3, 32, 32)
model = LeNet5(num_classes=43)
out = model(x)

