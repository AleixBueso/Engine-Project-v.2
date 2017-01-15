
# EngineProject v.2

A simple game engine developed by AleixBueso.     
This has been created for learning purposes, during 2016 at the CITM university.   

Source code can be found at:        
https://github.com/AleixBueso/EngineProject_v.2/

You can find out more on the [Web](https://aleixbueso.github.io/EngineProject_v.2/)!

## Usage

Drag and drop any 3d file into the engine viewport to load it. Note that only files located inside the "Assets" folder will be handled.


There still isn't a UI or any output when the engine is importing files, so it may appear frozen. Just wait, it may take a moment
for it to create all the needed files to handle the information.  
The first execution, since it's importing all the files in the assets folder, will take the longest to import.  



Keybindings:  

Camera movement  
	click and hold right mouse button over any viewport to rotate the camera.  
	Note that the default orthogonal views won't rotate.  
	The keys will move the "active viewport", the last one that was interacted with.  

	W forward  
	S backward  
	A Left  
	D Right  
	Q Down  
	E Up  
	Shift (Hold) Increase camera movement speed  
	
Change Viewports:  
	Space Switch between a single viewport or 4 of them.  
	-Even though you can bind any camera to any viewport through the Camera menu, it is recommended to be careful when changing them.  
		Deleting a camera linked to an active viewport will cause the engine to crash.  



Notes:  
-Avoid messing(modifying) with the library files unless you know what you're doing.  

-There's a known crash that will occur occasionally when importing certain images that the engine can't handle.  
Since it will try to import them on every execution, it is recommended to remove the image from the assets folder. After it's
imported other files, try to add it back and refresh.  
	If the problem persists, give up on the file and, preferably, erase all the files and folders inside "Library". A fix will be released. Hopefully.  
 
-There's a known bug when selecting certain objects with volume 0, such as planes. Sometimes they won't select, and sometimes they'll be selected even if they're behind other objects. We're working on it.  

-There's a known crash that will occur occasionally when deleting all the objects in a scene(included when loading a new scene from scratch).

## LICENSE

 Copyright 2016 Aleix Bueso

   Licensed under the Apache License, Version 2.0 (the "License");    
   you may not use this file except in compliance with the License.    
   You may obtain a copy of the License at    

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software     
   distributed under the License is distributed on an "AS IS" BASIS,    
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    
   See the License for the specific language governing permissions and    
   limitations under the License.

## ADITIONAL INFO (ASIGNMENT 3)

As I couldn't repair the motor to work properly saving/loading scenes does not work properly, i prepared an special scene so that it doesn't crash. 
To load the scene click on Load Scene option and type "Assignment". 
Three emitters should appear. On those, at the component Material, just select the texture you want to work with (recomended smoke). 
Blending and alpha are already prepared. 
Press "1" to fire a randomized Firework.

### TUTORIAL VIDEOS

[link to all videos](https://drive.google.com/open?id=0B9SRT_gp33sHUmU5M3FlbmI3NFk)
