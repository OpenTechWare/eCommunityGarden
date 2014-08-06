==================================
===== GardenManager - readme =====
==================================

=== Download ===
 - Git
    - Repository: https://github.com/OpenTechWare/GardenManager.git

 - Direct download
    - Source zip file: https://github.com/OpenTechWare/GardenManager/archive/master.zip

=== Build and Upload Sketch ===
 - Command line (using bash script)
   1) Install inotool (see http://inotool.org)
   2) Navigate to /src/ folder
   3) Execute one of the commands:
     a) Build:
       sh build.sh
     b) Build and upload to arduino
       sh buildAndUpload.sh

 - Command line (using inotool)
   1) Install inotool (see http://inotool.org/)
   2) Open terminal
   3) Navigate to /src/Sketches/GardenMonitor/
   4) Type one of the following commands to build sketch:
      a) Basic
        ino build 
      b) Specify model of arduino (change "nano328" for the model you're using)
        ino build -m nano328
   5) Type one of the following commands to upload sketch
      a) Basic
        sudo ino upload
      b) Specify model of arduino (change "nano328" for the model you're using)
        sudo ino upload -m nano328
   6) Open serial monitor (type the following command)
      sudo ino serial
   7) To exit serial monitor press the following two key combinations (without releasing the CTRL key)
        CTRL + A
        CTRL + Q     

=== Build Web Application ===
 - xbuild
   1) Open terminal
   2) Navigate to /src/ directory
   3) Type one of the following commands:
      a) Debug mode
           xbuild
      b) Release mode
           xbuild /p:Configuration=Release

 - IDE (VS.NET/MonoDevelop/SharpDevelop)
   1) Navigate to /src/ directory in file manager/browser
   2) Open GardenManager.sln in your preferred IDE
   3) Start build

=== Start Serial Capture ===
The serial capture script will capture all serial data from the connected garden monitor and save it as a "serialLog.txt" file in the /WWW/ directory. The web application uses this file to populate the graphs.
While the serial capture script is running and the garden monitor is connected the web application will display the latest data each time it's reloaded.

 - Command line
    1) Open terminal
    2) Navigate to:
      /src/WWW/
    3) Type one of the following command:
      a) Standard
        sudo sh captureSerial.sh
      b) Background
        screen sudo sh captureSerial.sh
    5) To exit serial monitor press the following two key combinations (without releasing the CTRL key)
        CTRL + A
        CTRL + Q     
