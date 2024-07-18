OpenGL Window Rendering with Dynamic Features

Developed using Visual Studio 2022, C++, and the OpenGL framework, this application delivers a vibrant graphical 
presentation of diverse 3D geometries and Skybox, all powered by GLSL.

Functionality:

1. 3D Geometries Display:
   
  -> A 100 floating rotating Sphere forms the backdrop.
  -> Two cubes to represent the point light with their respective color.
  -> skybox on 
  -> sphere to reflect the Skybox

3. Dynamic  Effects:

  -> Sampling Anti-Aliasing (MSAA) is enabled.
  -> mouse scroll wheel for a zoom in and out.

3. Keyboard and Mouse Controls:

   -> Camera Movement: Users can move the camera using the following keys:
  	->'W' and 'S' control movement along the z-axis.
  	->'A' and 'D' control movement along the x-axis.
  	->'Q' and 'E' control movement along the y-axis.
   
4.Cursor Visibility:
 When pressed, the left mouse button hides the cursor and reveals it upon release.

5. Display Mode:
 Press 'L' to toggle between wireframe mode and regular mode.

7.Texture Toggling for the Light: 

-> 1 key for the Point light On/Off
-> 2 key for the Directional light On/Off
-> 3 key for the Spot light On/Off

8.Includes Dependencies:

 -> FREETYPE
 
 -> GLEW
 
 -> GLFW
 
 -> GLM
 
 -> STB
 
Refrence:

OpenGL Tutorial 7 - Going 3D
https://www.youtube.com/watch?v=HiCVXEkkSK4

OpenGL Tutorial 8 - Camera
https://www.youtube.com/watch?v=86_pQCKOIPk&t=2s

Creating an Orthographic Camera | Game Engine series
https://www.youtube.com/watch?v=NjKv-HWstxA&t=119s

Spot Lights // OpenGL Tutorial #23 
https://www.youtube.com/watch?v=MAJqiDll0a8

Basic Lighting And Materials // OpenGL Tutorial #19
https://www.youtube.com/watch?v=YnBhJbQZLuE

Tutorial 12 - Lighting in OpenGL
https://www.youtube.com/watch?v=gFZqzVQrw84
