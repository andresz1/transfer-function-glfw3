Instrucciones básicas para el uso de la clase "TransferFunction" para GLFW3.
-----------------------------------------------------

### En "TransferFunction.h": 

Incluir los archivos de cabecera de glfw3 y glm.

### En "TransferFunction.cpp": 

Editar únicamente la forma de cargar las texturas en el método "InitContext", en este caso se usó DevIL.

### En la inicialización de su programa: 
```
TransferFunction *g_pTransferFunc = new TransferFunction();   
g_pTransferFunc->InitContext(GLFWwindow *window, int *windowsW, int *windowsH, int posx, int posy);
```
### En la función callback de glfw3 "MouseButton":
```
double x, y;   
glfwGetCursorPos(g_pWindow, &x, &y);   
g_pTransferFunc->MouseButton(x, y, button, action); 
// Este método retorna un bool dependiendo de si capto un evento o no
```
### En la función callback de glfw3 "CursorPos":
```
g_pTransferFunc->CursorPos(x, y); 
// Este método retorna un bool dependiendo de si capto un evento o no 
```
### En la función de despliegue:
`g_pTransferFunc->Display();`

### En el "Main loop":
```
if(g_pTransferFunc->updateTexture) // Verificar si la paleta de colores cambio    
	UpdateTexture();               // Actualizar la paleta de colores, esta se obtiene haciendo: g_pTransferFunc->colorPalette. Luego hacer: g_pTransferFunc->updateTexture = false;
```
     
[Pulse aquí](https://www.youtube.com/watch?v=2ONEKOq4d0U) para ver las características generales de esta GUI en un video.   