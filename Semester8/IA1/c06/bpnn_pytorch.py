import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import Dataset, DataLoader

# --- Dataset ---
train_data = ((0.111, 0.935), (0.155, 0.958), (0.151, 0.960), (0.153, 0.955),  # # - square
              (0.715, 0.924), (0.758, 0.964), (0.725, 0.935), (0.707, 0.913),  # * - star
              (0.167, 0.079), (0.215, 0.081), (0.219, 0.075), (0.220, 0.078),) # ## - rectangle

train_labels = ((1.0, 0.0, 0.0), (1.0, 0.0, 0.0), (1.0, 0.0, 0.0), (1.0, 0.0, 0.0), # # - square
              (0.0, 1.0, 0.0), (0.0, 1.0, 0.0), (0.0, 1.0, 0.0), (0.0, 1.0, 0.0),   # * - star
              (0.0, 0.0, 1.0), (0.0, 0.0, 1.0), (0.0, 0.0, 1.0), (0.0, 0.0, 1.0),)  # ## - rectangle

test_data = ((0.120, 0.950), (0.730, 0.940), (0.200, 0.080)) # Example test features

# --- Dataset Loader ---
class MyDataset(Dataset):
    def __init__(self, features, labels):
        self.labels = labels
        self.features = features

    def __len__(self):
        return len(self.features)

    def __getitem__(self, index):
        X = self.features[index]
        y = self.labels[index]
        return X, y

# --- Feed Forward Neural Network ---
class FeedforwardNeuralNetModel(torch.nn.Module):
    def __init__(self, input_dim, hidden_dim, output_dim):
        super(FeedforwardNeuralNetModel, self).__init__()
        self.fc1 = nn.Linear(input_dim, hidden_dim)
        self.sigmoid = nn.Sigmoid()
        self.fc2 = nn.Linear(hidden_dim, output_dim)

    def forward(self, x):
        out = self.fc1(x)
        out = self.sigmoid(out)
        out = self.fc2(out)
        out = self.sigmoid(out)
        return out

# --- Training Your Model ---
def train(dataloader, model, criterion, optimizer, epochs=5000):
    model.train()
    for epoch in range(epochs):
        epoch_loss = 0.0
        for batch_idx, (data, target) in enumerate(dataloader):
            # Zero the parameter gradients
            optimizer.zero_grad()
            
            # Forward pass
            outputs = model(data)
            
            # Compute loss
            loss = criterion(outputs, target)
            
            # Backward pass and optimize
            loss.backward()
            optimizer.step()
            
            epoch_loss += loss.item()
            
        if (epoch+1) % 100 == 0:
            print(f'Epoch [{epoch+1}/{epochs}], Loss: {epoch_loss/len(dataloader):.4f}')

# --- Validating Your Model ---
def validation(model, test_features):
    model.eval()
    with torch.no_grad():
        test_tensor = torch.stack([torch.Tensor(i) for i in test_features])
        outputs = model(test_tensor)
        print("\nTest predictions:")
        for i, out in enumerate(outputs):
            predicted_class = torch.argmax(out).item()
            print(f"Input: {test_features[i]}, Output probs: {out.numpy()}, Predicted Class: {predicted_class}")

# --- Running ---
if __name__ == "__main__":
    tensor_x = torch.stack([torch.Tensor(i) for i in train_data])
    tensor_y = torch.stack([torch.Tensor(i) for i in train_labels])    

    dataset_train = MyDataset(tensor_x, tensor_y)
    dataloader_train = DataLoader(dataset_train, batch_size=4, shuffle=True)

    input_dim = 2
    hidden_dim = 4
    output_dim = 3
    learning_rate = 0.5

    # create model
    model = FeedforwardNeuralNetModel(input_dim, hidden_dim, output_dim)
    
    # Loss and optimizer
    criterion = nn.MSELoss()
    optimizer = optim.SGD(model.parameters(), lr=learning_rate)

    print("Starting training...")
    train(dataloader_train, model, criterion, optimizer, epochs=5000)
    
    print("\nStarting validation...")
    validation(model, test_data)
