import torch
import torch.nn as nn
from torchvision import transforms
import matplotlib.pyplot as plt
import numpy as np
import cv2
import math
import matplotlib
matplotlib.use('webagg')

# Vytvoření jednoduché konvoluční vrstvy
# 1 vstupní kanál, několik výstupních kanálů (filtrů), kernel 3x3
out_channels = 4
conv_layer = nn.Conv2d(in_channels=1, out_channels=out_channels, kernel_size=3, bias=False)

# Přístup k váhám vrstvy
print("Tvar vah:", conv_layer.weight.shape)  
# torch.Size([3, 1, 3, 3]) - [out_channels, in_channels, height, width]
print("\nVáhy (filtry):")
print(conv_layer.weight.data)

# Načtení obrazu a převod na grayscale
image = cv2.imread('09839.ppm')
image_gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
image_gray = cv2.resize(image_gray, (256, 256))

# Použití ToTensor() - automaticky převede na (1, H, W) a normalizuje na [0, 1]
transform = transforms.ToTensor()
image_tensor = transform(image_gray)  # shape: (1, 256, 256)

# Přidání batch dimenze
image_tensor = image_tensor.unsqueeze(0)  # shape: (1, 1, 256, 256)

# Aplikace konvoluce
output = conv_layer(image_tensor)

pocet_priznaku = output[0].numel()
print("Počet příznaků (vstup pro lineární vrstvu):", pocet_priznaku)

# Je definována velikost mřížky (např. 6 sloupců)
cols = 6
total_plots = out_channels + 1
rows = math.ceil(total_plots / cols)

# Je vytvořena mřížka grafů
fig, axes = plt.subplots(rows, cols, figsize=(16, 2.5 * rows))
axes = axes.flatten()

# Na první pozici je vykreslen vstupní obraz
axes[0].imshow(image_gray, cmap='gray')
axes[0].set_title('Vstup', fontsize=12, fontweight='bold')
axes[0].axis('off')

# Do dalších pozic jsou postupně vykresleny mapy příznaků
for i in range(out_channels):
    feature_map = output[0, i, :, :].detach().numpy()
    
    # Je aplikována absolutní hodnota a normalizace pro lepší viditelnost
    feature_map = np.abs(feature_map)
    f_min = np.min(feature_map)
    f_max = np.max(feature_map)
    if f_max > f_min:
        feature_map = (feature_map - f_min) / (f_max - f_min)
        
    axes[i+1].imshow(feature_map, cmap='gray')
    axes[i+1].set_title(f"kernel-{i}", fontsize=10)
    axes[i+1].axis('off')

# Zbylé nevyužité grafy v mřížce jsou skryty
for j in range(total_plots, len(axes)):
    axes[j].axis('off')

plt.tight_layout()
plt.show()
