# LED Wall Video Player

This project is compatible with OF v0.8.4, Mac OS 10.10 and Xcode 6.1

## Video Wall

### Hardware

The Moment installation consists of two large walls made up of 4 x 2 __modules__ of 36 cubes each (6 x 6). This makes the "resolution" of each wall 24 x 12. Each wall can function independently of the other, however, this application treats them as a pair with a combined "resolution" of 48 x 12.

Each module has 36 cubes filled with 16-40 Adafruit NeoPixel LEDs daisy chained together into four separate strips. These four strips each share one data connection. All LEDs in each module share one 12V 60A power connection as well as a common ground (with the exception of the third module from the left on the right wall (B3) which shares a power supply connection with the module directly above it and the module directly to its right). Each module has 5 Fadecandy chips, 4 of which provide data output from all of their output pins (providing data to 8 cubes each) and 1 of which provides data from only half of its output pins (output to 4 only cubes). There are 120 Fadecandies in total.

Two Raspberry Pis and one wireless router rest behind each wall. One RPi controls the left 144 cubes (12x12, i.e. 2 x 2 modules) and the other controls the right 144 cubes of each wall. The Raspberry Pis are connected via ethernet to their respective wall's router and the two routers are bridged to create one LAN. Each Raspberry Pi is connected via USB to a 10-1 powered USB hub to allow it to connect to the 20 Fadecandies on each 1/4 of each wall.

### Software

#### Network Info

```
# Master Router, running DHCP server
SSID: Moment
IP: 192.168.2.1
Subnet: 255.255.255.0

# Bridged Router, not running DHCP
SSID: Moment Extender # Hidden SSID, not broadcasted
IP: 192.168.2.2

# Left Wall Left RPi
IP: 192.168.2.100 # static IP

# Left Wall Left RPi
IP: 192.168.2.101 # static IP

# Right Wall Left RPi
IP: 192.168.2.103 # static IP

# Right Wall Right RPi
IP: 192.168.2.102 # static IP

# Mac Mini
IP: 192.168.2.200 # static IP
```

#### Application

This OF project was developed with OpenFrameworks v0.8.4. Modified versions of `ofxCv`, `ps3eye` and `ofxPhysics3d` are in the `src` folder.

- `LEDWallVideoPlayer.app`: 
Startup:  
The Mac Mini is scheduled to restart daily at 7am.
On system restart, LEDWallVideoPlayer.app is set to launch via Users>Login Items. LEDWallVideoPlayer.app is located in `/Volumes/SYSTEM/Users/moment/Desktop/bin/LEDWallVideoPlayer.app`

On system startup, LEDWallVideoPlayer.app launches and goes through the following steps (controlled by `StartupController.h`):  

- reboots the RPis
- waits 2 minutes
- checks for `fcserver` availability

If these steps are successful the Application proceeds to play different "apps" that are switched every hour. This schedule is controlled via `Scheduler.h`. The Scheduler will set a shutdown flag that causes the app to clear the wall and terminate the application at 19:00.

Apps:   
"Apps" is an internal term to describe different implementations running in `LEDWallVideoPlayer.app`. The following are currently active

OpenCVBurstApp:   
Uses cameras/blob tracking to paint each wall

OpenCVMinimalParticle:   
Uses cameras/blob tracking to guide entities towards blob location

OpenCVParticleApp:   
Uses cameras/blob tracking to guide entities towards blob location. Background color palettes are chosen on random on each app switch

VideoPlayerApp:   
Plays random video chosen from inside `bin/data/videos` folder

Internal Structure:
A single `ofFbo` is allocated and passed to each application. Each application responsible for clearing and drawing into it. The main application (`ofApp`) then sends the pixels to the fadecandy servers.

`OpenCVEngine` is a Singleton that is shared among apps that use the blob tracking and installed PS3Eye cameras. This manages 2 `TrackedWall` instances that each have a single video source (camera or video) and an FBO that they can draw content into.

`TrackedWall` also contains an `ofxDynamicBackgroundCV` instance which updates the background matte and provides blobs and contours via `ofxCv::ContourFinder`. Each TrackedWall has it's own FBO which can be used to draw into/extract content from (helps simplify wall coordinates for per-wall behavior)

-ofApp
-creates main FBO and applications
-creates `Scheduler` and `StartupController`
-creates `OpenCVEngine`
-creates active applications and manages switching
-creates FadeCandyController and sends pixels from main FBO

--WallBaseApp
--provides common setup/update
--has own FBO
---WallBaseApp subclass (app implementation, e.g. OpenCVBurstApp)
---can have extra assets, FBOs, textures, etc
---accesses any needed OpenCV via `OpenCVEngine::getInstance`
---draws into shared FBO created by ofApp
---can implement `onAppSwitch` for specific switching behavior (e.g. change colors, load different video)


#### Fadecandy Servers and Pixel Mapping

Each RPi has a fadecandy server located in `/usr/local/bin` (don't edit the one in `~/`) that is automatically run at startup via `/etc/rc.local`. This server loads the `/usr/local/bin/fcserver.json` configuration file and listens for TCP connections on port `7890`. Pixel mapping is complete for the most part, however, there are a few errors that I noticed on each wall. To whoever works on Dolby project next, I suggest correcting the errors. Copies of each config file reside in this repo in `pixel_maps` and some info about mapping is available in [this Adafruit tutorial](https://learn.adafruit.com/1500-neopixel-led-curtain-with-raspberry-pi-fadecandy/fadecandy-server-setup). A mapping utility app exists for this project as well. [Email me](mailto:brannon@brannondorsey.com) and I will send it to you with a description of how it can be used.






