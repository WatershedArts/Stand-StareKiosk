File System
===

There are specific folders for specific parts of the app.

They are fairly self explainatory.
Video files go into the video files folder.

##Gif Generator
When a new video is added to the video folder, a shell script will generate a animated gif of the video mainly for preview. The script will launch on RPi boot.

The Gif Generator uses ffmpeg and imagemagick.

```

```

##Overview
Here is an overview of the folder structure.



```
|-- Stand&StareJukebox
|	|-- addons.make
|	|-- config.make   
|	|-- Makefile
|	|-- ReadMe.md
|	|---|  bin
|	|	|--  executable
|	|	|---|  libs
|	|   |---|  data
|	|	|---|---|  gifMaker
|	|	|	|	|	|-- folderWatcher.sh
|	|	|	|	|	|-- makeGifFromVideo.sh
|	|	|	|	|	|---| tempImg
|	|   |   |   |-- config.json
|	|   |   |   |---| gifs
|	|   |   |   |		|-- files
|	|   |   |   |---| videos
|	|   |   |   |		|-- files
|	|   |   |   |---| ofxdatgui_assets 
|	|   |   |   |		|-- font-verdana.ttf
|	|   |   |   |       |-- icon-dropdown.png
|	|   |   |   |		|-- icon-radio-off.png
|	|   |   |	|		|-- icon-radio-on.png
|	|   |   |	|		|-- picker-rainbow.png
|	|---|  obj
|	|---|  src
|	|   |-- main.cpp
|	|   |-- ofApp.cpp
|	|   |-- ofApp.h
|	|   |---|  local_addons
|   |   |   |   |-- Calibration.cpp
|   |   |   |   |-- Calibration.hpp
|   |   |   |   |-- DonationReader.cpp
|   |   |   |   |-- DonationReader.hpp
|   |   |   |   |-- Enticer.cpp
|   |   |   |   |-- Enticer.hpp
|   |   |   |   |-- ExtraInfo.cpp
|   |   |   |   |-- ExtraInfo.hpp
|   |   |   |   |-- LoadSettings.cpp
|   |   |   |   |-- LoadSettings.hpp
|   |   |   |   |-- MachineSecurity.cpp
|   |   |   |   |-- MachineSecurity.hpp
|   |   |   |   |-- MyTimer.cpp
|   |   |   |   |-- MyTimer.hpp
|   |   |   |   |-- ObjectHandler.cpp
|   |   |   |   |-- ObjectHandler.hpp
|   |   |   |   |-- PostData.cpp
|   |   |   |   |-- PostData.hpp
|   |   |   |   |-- ProjectorControl.cpp
|   |   |   |   |-- ProjectorControl.hpp
|   |   |   |   |-- RFIDReader.cpp
|   |   |   |   |-- RFIDReader.hpp
|   |   |   |   |-- SimpleButton.h
|   |   |   |   |-- VideoPlayer.cpp
|   |   |   |   |-- VideoPlayer.hpp
|   |   |   |   |-- VideoPreview.cpp
|   |   |   |   |-- VideoPreview.hpp
|   |   |   |   |-- terminalListener.h

```