echo "Retrieving required libraries..."

echo "Installing certificates..."
mozroots --import --sync

if [ ! -f nuget.exe ]; then
    echo "nuget.exe not found. Downloading..."
    wget http://nuget.org/nuget.exe
fi

echo "Installing libraries..."

mono nuget.exe install NUnit
mono nuget.exe install Sider

echo "Installation complete!"
