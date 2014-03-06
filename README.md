Instrucciones b�sicas para el uso de la clase "TransferFunction" para GLFW3.
-----------------------------------------------------

### En "TransferFunction.h": 

Incluir los archivos de cabecera de glfw3 y glm.

### En "TransferFunction.cpp": 

Editar �nicamente la forma de cargar las texturas en el m�todo "InitContext", en este caso se us� DevIL.

### En la inicializaci�n de su programa: 
```
TransferFunction *g_pTransferFunc = new TransferFunction();   
g_pTransferFunc->InitContext(GLFWwindow *window, int *windowsW, int *windowsH, int posx, int posy);
```
### En la funci�n callback de glfw3 "MouseButton":
```
double x, y;   
glfwGetCursorPos(g_pWindow, &x, &y);   
g_pTransferFunc->MouseButton(x, y, button, action); 
// Este m�todo retorna un bool dependiendo de si capto un evento o no
```
### En la funci�n callback de glfw3 "CursorPos":
```
g_pTransferFunc->CursorPos(x, y); 
// Este m�todo retorna un bool dependiendo de si capto un evento o no 
```
### En la funci�n de despliegue:
`g_pTransferFunc->Display();`

### En el "Main loop":
```
if(g_pTransferFunc->updateTexture) // Verificar si la paleta de colores cambio    
	UpdateTexture();               // Actualizar la paleta de colores, esta se obtiene haciendo: g_pTransferFunc->colorPalette. Luego hacer: g_pTransferFunc->updateTexture = false;
```
     
[Pulse aqu�](https://www.youtube.com/watch?v=2ONEKOq4d0U) para ver las caracter�sticas generales de esta GUI en un video.   