#pragma once

#include "Image.h"
#include "headers\glfw\glfw3.h"
#include "headers\glm\glm.hpp"
#include "headers\glm\gtc\matrix_transform.hpp"
#include "headers\glm\gtx\transform2.hpp"
#include "headers\glm\gtc\type_ptr.hpp"
#include <queue>

enum PickingValidation
{ 
	GRAFIC, 
	SELECTCOLOR, 
	PICKERCOLOR 
};

#define getX(x) (GLfloat)(((x)<<1)-*windowsW)/(GLfloat)*windowsW
#define getY(y) (GLfloat)(*windowsH-((y)<<1))/(GLfloat)*windowsH

#define NULLNUM 0
#define NUMOFCOLORS 256

#define IMAGEWIDHT 360
#define IMAGEHEIGHT 210
#define MINW 47
#define MINH 26
#define MAXW 328
#define MAXH 155
#define MAXPOINT 100

#define MINWSC 0
#define MINHSC 212
#define MAXWSC 355
#define MAXHSC 232

#define MINWPC 0
#define MINHPC 241
#define MAXWPC 360
#define MAXHPC 448

struct specialPoint
{
	glm::ivec2 point;
	glm::vec4 color;
	glm::ivec2 pickerPos;
	int selectedColorPos;
	bool drag;
	bool selected;

	specialPoint(){}
	specialPoint( glm::ivec2 point, glm::vec4 color, glm::ivec2 pickerPos, int selectedColorPos, bool drag, bool selected ):point( point ), color( color ), pickerPos( pickerPos ), selectedColorPos( selectedColorPos ), drag( drag ), selected( selected ){}
};

class TransferFunction
{
private:
	GLuint txtId, ptsId, sltId;	
	float alpha;
	GLFWwindow *window;
	int *windowsW, *windowsH;							
	int imageW, imageH, ptsCounter;						
	int lastPicking, posx , posy, antx, anty, lastPickingColor;					
	int pointSelected, indicatorSC;									
	bool palleteCreated, dragDropWindow;		

	int colorPosList[ MAXPOINT ];	
	glm::vec4 currentColor;	
	glm::vec4 colorList[ MAXPOINT ];	
	glm::vec4 baseColors[ 6 ];	
	glm::ivec2 currentColorPickerPos;					
	glm::ivec2 pointList[ MAXPOINT ];	
	glm::ivec2 colorPickerPosList[ MAXPOINT ];
				
	bool Picking( int x, int y, PickingValidation type  );
	void SortPoints( int jumpPoint = -1 );
	void UpdateColorPoint();
	bool DeletePoint( int w, int h );

public:
	TransferFunction(void);
	~TransferFunction(void);

	GLuint pallete, indextemppallete;
	GLfloat colorPalette[256][4];
	bool updateTexture, dragDrop, dragDropColor, dragDropPicker, isVisible;		

	void Display();
	void InitContext( GLFWwindow *window, int *windowsW, int *windowsH, int posx, int posy );
	bool MouseButton( int w, int h, int button, int action );
	bool CursorPos ( int w, int h );
	void UpdatePallete();
};
