import os
import torch
import torchvision
import torchvision.transforms as transforms
import torchvision.datasets as datasets
import torch.nn.functional as F

import matplotlib.pyplot as plt
import matplotlib.patches as patches
import numpy as np
from PIL import Image

def my_imshow(img):
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))
    plt.show()

class LeNet(torch.nn.Module):
    def __init__(self):
        super(LeNet, self).__init__()
        # Convolution (In LeNet-5, 32x32 images are given as input. Hence padding of 2 is done below)
        self.conv1 = torch.nn.Conv2d(in_channels=1, out_channels=6, kernel_size=5, stride=1, padding=2, bias=True)
        # Max-pooling
        self.max_pool_1 = torch.nn.MaxPool2d(kernel_size=2)
        # Convolution
        self.conv2 = torch.nn.Conv2d(in_channels=6, out_channels=16, kernel_size=5, stride=1, padding=0, bias=True)
        # Max-pooling
        self.max_pool_2 = torch.nn.MaxPool2d(kernel_size=2)
        # Fully connected layer
        self.fc1 = torch.nn.Linear(16*5*5, 120)   # convert matrix with 16*5*5 (= 400) features to a matrix of 120 features (columns)
        self.fc2 = torch.nn.Linear(120, 84)       # convert matrix with 120 features to a matrix of 84 features (columns)
        self.fc3 = torch.nn.Linear(84, 10)        # convert matrix with 84 features to a matrix of 10 features (columns)
        
    def forward(self, x):
        # convolve, then perform ReLU non-linearity
        x = F.relu(self.conv1(x))  
        # max-pooling with 2x2 grid
        x = self.max_pool_1(x)
        # convolve, then perform ReLU non-linearity
        x = F.relu(self.conv2(x))
        # max-pooling with 2x2 grid
        x = self.max_pool_2(x)
        # first flatten 'max_pool_2_out' to contain 16*5*5 columns
        x = x.view(-1, 16*5*5)
        # FC-1, then perform ReLU non-linearity
        x = F.relu(self.fc1(x))
        # FC-2, then perform ReLU non-linearity
        x = F.relu(self.fc2(x))
        # FC-3
        x = self.fc3(x)
        
        return x

def train(data, model):
    model.train()

    learning_rate = 0.01
    criterion = torch.nn.CrossEntropyLoss()
    optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)

    num_epochs = 5
    p = 1
    print("Starting training...")
    with open("loss.txt", "wt") as f:
        for epoch in range(num_epochs):
            running_loss = 0.0
            for i, sample in enumerate(data, 0):
                optimizer.zero_grad()            
                inputs = sample[0]
                labels = sample[1]
                
                output = model(inputs)
                loss = criterion(output, labels)
                
                loss.backward()
                optimizer.step()
                running_loss += loss.item()
                if i % 500 == 499:    # print every 500 mini-batches
                    print('[%d, %d] loss: %.3f' % (epoch + 1, i + 1, running_loss / 500))
                    s = "{0} {1}\n".format(p, running_loss / 500)
                    f.write(s)
                    p += 1
                    running_loss = 0.0

    torch.save(model.state_dict(), './model.pth')

def validation(data, model):
    model.eval()
    print("Validating...")
    show_image = False

    size = len(data.dataset)
    num_incorrect = 0
    i = 0
    with torch.no_grad():
        for sample in data:
            images, labels = sample
            output = model(images)
            predicted = torch.max(output.data, 1)
            num_incorrect += (predicted[1] != labels).sum().item()
            
            # Simple implementation for viewing the first incorrect sample if enabled
            if show_image and i == 0: 
                pass
            i += 1
            
    print("Validation Error: {:.2f} %".format(100.0 * num_incorrect / size))

def sliding_window(model, image, size=(28, 28)):
    """
    Implement a sliding window to recognize numbers in any location in a given image.
    We do not expect numbers to be rotated, so this is much simplified.
    """
    model.eval()
    transform = torchvision.transforms.Compose([
        torchvision.transforms.Grayscale(),
        torchvision.transforms.Resize(size[0]), # Size could be dynamic
        torchvision.transforms.ToTensor()
    ])
    
    width, height = image.size
    window_w, window_h = size
    stride = window_w // 2 # stride for sliding
    
    fig, ax = plt.subplots(1)
    ax.imshow(np.asarray(image.convert('RGB')))
    
    print(f"Running sliding window on {width}x{height} image with window {window_w}x{window_h}...")
    
    with torch.no_grad():
        for y in range(0, height - window_h + 1, stride):
            for x in range(0, width - window_w + 1, stride):
                window = image.crop((x, y, x + window_w, y + window_h))
                tensor_img = transform(window).unsqueeze(0) # Shape: [1, 1, H, W]
                
                # Check for completely empty/white or black background
                # Assuming background is uniform, we only run inference if there's enough variance
                if tensor_img.std().item() < 0.1:
                    continue
                
                output = model(tensor_img)
                prob = F.softmax(output, dim=1)
                max_prob, predicted = torch.max(prob, 1)
                
                # If high confidence
                if max_prob.item() > 0.95:
                    rect = patches.Rectangle((x, y), window_w, window_h, linewidth=2, edgecolor='r', facecolor='none')
                    ax.add_patch(rect)
                    ax.text(x, max(0, y-5), str(predicted.item()), color='red', fontsize=12, fontweight='bold', backgroundcolor='white')
                    
    print("Sliding window processing complete. Close the plot to continue.")
    # In headless environment we might just want to save it instead
    plt.savefig("sliding_window_output.png", bbox_inches='tight')
    plt.close()

def main():
    transform = torchvision.transforms.Compose([
        torchvision.transforms.Grayscale(), 
        torchvision.transforms.Resize(28), 
        torchvision.transforms.ToTensor()
    ])
    
    batch_size_train = 16

    train_loader = torch.utils.data.DataLoader(
        torchvision.datasets.MNIST('./data', train=True, download=True, transform=transform), 
        batch_size=batch_size_train, shuffle=True
    )
    test_loader = torch.utils.data.DataLoader(
        torchvision.datasets.MNIST('./data', train=False, download=True, transform=transform)
    )

    model = LeNet()
    
    if os.path.exists('./model.pth'):
        print("Loading existing model...")
        model.load_state_dict(torch.load('./model.pth'))
    else:
        print("Training new model...")
        train(train_loader, model)
    
    validation(test_loader, model)

    # run sliding window if image exists
    img_path = 'numbers.png'
    if not os.path.exists(img_path):
        img_path = 'numbers_rgb.png'
        
    if os.path.exists(img_path):
        print(f"Found {img_path}, running sliding window test...")
        img = Image.open(img_path)
        sliding_window(model, img, (28, 28))
    else:
        print("Warning: Neither numbers.png nor numbers_rgb.png found. Skipping sliding window.")

if __name__ == '__main__':
    main()
