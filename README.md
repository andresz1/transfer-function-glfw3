Transfer-Function-GUI
=====================

Graphical user interface for transfer function used in volume rendering.

### In "TransferFunction.h": 

Include header files glfw3 and glm..

### In "TransferFunction.cpp": 

Edit the way to load the textures in the "initContext" method in this case was used DevIL.

### In the program initialization: 
```
TransferFunction *g_pTransferFunc = new TransferFunction();   
g_pTransferFunc->InitContext(GLFWwindow *window, int *windowsW, int *windowsH, int posx, int posy);
```
### In the glfw3 callback function "MouseButton":
```
double x, y;   
glfwGetCursorPos(g_pWindow, &x, &y);   
g_pTransferFunc->MouseButton(x, y, button, action); 
// This method returns a bool depending on whether an event is captured or not
```
### In the glfw3 callback function "CursorPos":
```
g_pTransferFunc->CursorPos(x, y); 
// This method returns a bool depending on whether an event is captured or not
```
### In the display function:
`g_pTransferFunc->Display();`

### In the "Main loop":
```
if(g_pTransferFunc->updateTexture) // Verificar si la paleta de colores cambio    
	UpdateTexture();               // Actualizar la paleta de colores, esta se obtiene haciendo: g_pTransferFunc->colorPalette. Luego hacer: g_pTransferFunc->updateTexture = false;
```
     
[Click here](https://www.youtube.com/watch?v=2ONEKOq4d0U) to see the general characteristics of this GUI in a video.