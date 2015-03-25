echo "Retrieving required libraries..."

if [ ! -f nuget.exe ]; then
    echo "nuget.exe not found. Downloading..."
    wget http://nuget.org/nuget.exe
fi

echo "Installing libraries..."

mono nuget.exe install NUnit
mono nuget.exe install Sider
mono nuget.exe install Quartz

echo "Installation complete!"
