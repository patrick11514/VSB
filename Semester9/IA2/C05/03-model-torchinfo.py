# Instalace: pip install torchinfo
import torch
import torch.nn as nn
from torchinfo import summary

class MiniCNN(nn.Module):
    def __init__(self, bias=True):
        super().__init__()
        # Cin=3, Cout=16, kernel=3x3 -> param=16*(3*3*3)+(bias 16)=432+(+16) = 448 pokud je bias=True
        self.conv = nn.Conv2d(in_channels=3, out_channels=16, kernel_size=3, padding=1, bias=bias)     

    def forward(self, x):
        return self.conv(x)

# Je automaticky detekováno dostupné výpočetní zařízení (GPU nebo CPU)
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

# Model je inicializován a přesunut na příslušné zařízení
model = MiniCNN(bias=True).to(device)  # Je možné vyzkoušet i bias=False

# Je vygenerován přehled modelu
# pro velikost dávky (batch_size)je použita hodnota 1
summary(model, input_size=(1, 3, 28, 28), device=device)
