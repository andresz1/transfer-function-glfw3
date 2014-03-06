Transfer-Function-GUI
=====================

Graphical user interface for transfer function used in volume rendering, originally created by Luiyit Hernandez and adapted to glfw3 by Andrés Alvarez.   
The main steps to include the class in your code are:

#### In "TransferFunction.h": 

Include glfw3 and glm header files.

#### In "TransferFunction.cpp": 

Edit the way to load the textures in the "initContext" method, in this case was used DevIL.

#### In the program initialization: 
```
TransferFunction *g_pTransferFunc = new TransferFunction();   
g_pTransferFunc->InitContext(GLFWwindow *window, int *windowsW, int *windowsH, int posx, int posy);
```
#### In the glfw3 callback function "MouseButton":
```
double x, y;   
glfwGetCursorPos(g_pWindow, &x, &y);   
g_pTransferFunc->MouseButton(x, y, button, action); 
// This method returns a bool depending on whether an event is captured or not
```
#### In the glfw3 callback function "CursorPos":
```
g_pTransferFunc->CursorPos(x, y); 
// This method returns a bool depending on whether an event is captured or not
```
#### In the display function:
`g_pTransferFunc->Display();`

#### In the "Main loop":
```
if(g_pTransferFunc->updateTexture) // Check if the color palette changed    
	UpdateTexture();               // Update the color palette, this is obtained by: g_pTransferFunc->colorPalette. Then do: g_pTransferFunc->updateTexture = false;
```
     
[Click here](https://www.youtube.com/watch?v=2ONEKOq4d0U) to see the general characteristics of this GUI in a video.