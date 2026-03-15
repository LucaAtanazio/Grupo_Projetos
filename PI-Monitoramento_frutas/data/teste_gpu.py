import torch

print("PyTorch:", torch.__version__)
print("CUDA disponível:", torch.cuda.is_available())

if torch.cuda.is_available():
    print("GPU:", torch.cuda.get_device_name(0))


device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

print("Device:", device)

x = torch.rand(10000,10000).to(device)
y = torch.mm(x,x)

print("Operação concluída na GPU 🚀")