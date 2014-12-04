==================================
===== GardenManager - readme =====
==================================

=== Source Code File Structure ===

/src/
    /Sketches/
             /GardenManager/ - The GardenManager sketch that runs on the arduino/sensors system.
/src/
    /WWW/                    - The ASP.NET/mono web application which displays the GardenManager data as graphs.
    /GardenManager.Core/     - The GardenManager library, used by the web application (or any .NET/mono project)
                               to parse and interact with GardenMonitor data.

=== Download ===
 - Git
    - Repository: https://github.com/OpenTechWare/GardenManager.git

 - Direct download
    - Source zip file: https://github.com/OpenTechWare/GardenManager/archive/master.zip

=== Install inotool ===
(See http://inotool.org for more information)
 - On Debian/Ubuntu Linux (including Raspbian) run these terminal commands (one line at a time): 
   sudo apt-get install arduino python-configobj python-setuptools git python-jinja2 python-serial python-pip
   sudo pip install glob2
   sudo apt-get install picocom
   git clone git://github.com/amperka/ino.git
   cd ino
   sudo make install

=== Build and Upload Sketch ===
 - Command line (using bash script and inotool)
   1) Install inotool (see above)
   2) Navigate to /src/ folder
   3) Execute one of the commands:
     a) Build:
       sh build.sh
     b) Build and upload to arduino
       sh buildAndUpload.sh

 - Command line (using inotool)
   1) Install inotool (see above)
   2) Open terminal
   3) Navigate to /src/Sketches/GardenManager/
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
 - Bash script
   1) On a Debian/Ubuntu linux OS (including Raspbian) run this command:
      sh build.sh
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

=== Start Web Application ===
 - Command line:
   1) Open terminal
   2) Navigate to /src/WWW/
   3) Run one of the following commands:
     a) Simple (starts on port 8080)
       xsp4
     b) Background process
       screen xsp4
     c) Public (port 80)
       sudo xsp4 --port 80
   4) Open via the browser
     http://127.0.0.1:8080/

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

[ ![Codeship Status for OpenTechWare/GardenManager](https://codeship.com/projects/9268d490-5e3d-0132-feca-5a8b1698743a/status)](https://codeship.com/projects/51303)
