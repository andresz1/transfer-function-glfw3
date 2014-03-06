#include "TransferFunction.h"

bool operator<( const specialPoint &point1, const specialPoint &point2 ) 
{
	return point1.point.x > point2.point.x;
}

TransferFunction::TransferFunction(void):imageW( IMAGEWIDHT ), imageH( IMAGEHEIGHT ), ptsCounter( 0 ), lastPicking( 0 ), lastPickingColor(0), dragDrop( false ), dragDropColor( false ), dragDropPicker( false ), indicatorSC( 0 ), pointSelected( 1 ), palleteCreated( false ), updateTexture( true )
{
	baseColors[ 0 ] = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	baseColors[ 1 ] = glm::vec4( 1.0f, 0.0f, 1.0f, 1.0f );
	baseColors[ 2 ] = glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f );
	baseColors[ 3 ] = glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f);
	baseColors[ 4 ] = glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f );
	baseColors[ 5 ] = glm::vec4( 1.0f, 1.0f, 0.0f, 1.0f );

	currentColor = glm::vec4( 1.0f, 0.0f, 0.0f, 1.0 );
	currentColorPickerPos = glm::ivec2( MINWPC + 6, MINHPC + 6 );
	
	alpha = 0.5;
}

TransferFunction::~TransferFunction(void)
{
}

void TransferFunction::InitContext( GLFWwindow *window, int *windowsW, int *windowsH, int posx, int posy )
{
	unsigned int tId;

	ilInit();

	tId = ILoadImage((std::string)"assets/transferFunction.jpg");
	if(tId > 1)
	{ 
		ilBindImage(tId);
		ILoadText(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT), ilGetData(), txtId, GL_FALSE, GL_BGRA_EXT, GL_RGBA, 0, 0);	
	}

	tId = ILoadImage((std::string)"assets/point.png");
	if(tId > 1)
	{ 
		ilBindImage(tId);
		ILoadText(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),ilGetData(), ptsId, GL_FALSE, GL_BGRA_EXT, GL_RGBA, 0, 0);	
	}

	tId = ILoadImage((std::string)"assets/selector.png");
	if(tId > 1)
	{ 
		ilBindImage(tId);
		ILoadText(ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),ilGetData(),sltId, GL_FALSE, GL_BGRA_EXT, GL_RGBA, 0, 0);	
	}

	isVisible = true;

	this->window = window;

	this->windowsW = windowsW;
	this->windowsH = windowsH;

	this->posx = (*windowsW - posx < 365 || posx < 0)? 0: posx;
	this->posy = (*windowsH - posy < 455 || posy < 0)? 0: posy;

	this->MouseButton( MAXW + this->posx, MINH + this->posy, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS );					
	this->pointSelected = 0;							
	
	this->MouseButton( MINWPC + this->posx, MAXHPC + this->posy, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS );					
	this->MouseButton( MINW + this->posx, MAXH + this->posy, GLFW_MOUSE_BUTTON_LEFT,  GLFW_PRESS );						
	
	this->MouseButton( MAXW + this->posx, MINH + this->posy, GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE );					

	this->UpdatePallete();

	isVisible = false;
}

void TransferFunction::Display()
{
	if(!isVisible)
		return;

	glViewport(posx, -posy,  *windowsW, *windowsH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, *windowsW, *windowsH, 0, 0, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_DEPTH_TEST );

	glBindTexture( GL_TEXTURE_2D, this->txtId );
	glEnable( GL_TEXTURE_2D );

	float w = getX( imageW ); 
	float h = getY( imageH );

	glColor4f( 1.0, 1.0, 1.0, alpha );
	glBegin( GL_QUADS );
		glTexCoord2f(0, 1);
		glVertex3f( -1.0f, 1.0f, -0.01f );

		glTexCoord2f(0, 0);
		glVertex3f( -1.0f, h, -0.01f );

		glTexCoord2f(1, 0);
		glVertex3f( w, h, -0.01f );

		glTexCoord2f(1, 1);
		glVertex3f( w, 1.0f, -0.01f );
	glEnd();

	glDisable( GL_TEXTURE_2D );

	float divW = ( -1.0 - w ) / 6.0;
	float h2 = 1.0 - getY( 2 );
	float h3 = 1.0 - getY( 30 );

	for( int i=0; i<6; i++ )
	{
		glBegin( GL_QUADS );

			glColor3fv(glm::value_ptr(baseColors[i]));
			glVertex3f( -1.0 - divW * i, h - h2, 0.0f );

			glColor3fv(glm::value_ptr(baseColors[ i ]));
			glVertex3f( -1.0 - divW * i, h - h3, 0.0f );

			glColor3fv(glm::value_ptr(baseColors[ (i + 1) % 6 ]));
			glVertex3f( -1.0 - divW * ( i + 1 ), h - h3, 0.0f );

			glColor3fv(glm::value_ptr(baseColors[ (i + 1) % 6 ]));
			glVertex3f( -1.0 - divW * ( i + 1 ), h - h2, 0.0f );

		glEnd();
	}

	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
	glBindTexture( GL_TEXTURE_2D, this->sltId );
	glEnable( GL_TEXTURE_2D );

	float baseW = getX( this->indicatorSC );

	glBegin( GL_QUADS );
		glTexCoord2f(0, 1);
		glVertex3f( baseW, h - h2, 0.0f );

		glTexCoord2f(0, 0);
		glVertex3f( baseW, h - h3, 0.0f );

		glTexCoord2f(1, 0);
		glVertex3f( baseW - (divW / 6), h - h3, 0.0f );

		glTexCoord2f(1, 1);
		glVertex3f( baseW - (divW / 6), h - h2, 0.0f );
	glEnd();

	glDisable( GL_TEXTURE_2D );

	float h4 = h + h2 + h2 + h3;
	float h5 = h - ( h3 + h2 + h2 + ( 1.0 - h ) );
	glColor4f( 1.0f, 1.0f, 1.0f, alpha );

	glBegin( GL_QUADS );

		glColor4f( 1.0f, 1.0f, 1.0f, alpha );
		glVertex3f( -1.0, h - ( h3 + h2 + h2 ), 0.0f );

		glColor4f( 0.0f, 0.0f, 0.0f, alpha );
		glVertex3f( -1.0f, h5, 0.0f );

		glColor4f( 0.0f, 0.0f, 0.0f, alpha );
		glVertex3f( w, h5, 0.0f );

		glColor4fv( glm::value_ptr(this->currentColor) );
		glVertex3f( w, h - ( h3 + h2 + h2 ), 0.0f );

	glEnd();


	static float GrsfAlpha = 0.5;
	float maxH = getY( MAXH );

	for( int point = 1; point < this->ptsCounter; point++ ){

		float PW1 = getX( this->pointList[ point - 1 ].x );
		float PH1 = getY( this->pointList[ point - 1 ].y );

		float PW2 = getX( this->pointList[ point ].x );
		float PH2 = getY( this->pointList[ point ].y );

		glBegin( GL_QUADS );
			glColor4f( this->colorList[ point - 1 ].x, this->colorList[ point - 1 ].y, this->colorList[ point - 1 ].z, GrsfAlpha );
			glVertex2f( PW1, PH1 );
			glColor4f( this->colorList[ point - 1 ].x, this->colorList[ point - 1 ].y, this->colorList[ point - 1 ].z, GrsfAlpha );
			glVertex2f( PW1, maxH );
			glColor4f( this->colorList[ point ].x, this->colorList[ point ].y, this->colorList[ point ].z, GrsfAlpha );
			glVertex2f( PW2, maxH );
			glColor4f( this->colorList[ point ].x, this->colorList[ point ].y, this->colorList[ point ].z, GrsfAlpha );
			glVertex2f( PW2, PH2 );
		glEnd();

	}

	float plusX = 1.0 + getX( 5 );
	float plusY = 1.0 - getY( 5 );

	glBindTexture( GL_TEXTURE_2D, this->ptsId );
	glEnable( GL_TEXTURE_2D );

	for( int point = 0; point < this->ptsCounter; point++ )
	{
		float PW = getX( this->pointList[ point ].x );
		float PH = getY( this->pointList[ point ].y );

		if( point != this->pointSelected - 1 ) glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );
		else glColor4f( 0.0f, 0.8f, 1.0f, alpha );

		glBegin( GL_QUADS );	

			glTexCoord2f(0, 1);
			glVertex2f( PW - plusX, PH + plusY );

			glTexCoord2f(0, 0);
			glVertex2f( PW - plusX, PH - plusY );

			glTexCoord2f(1, 0);
			glVertex2f( PW + plusX, PH - plusY );

			glTexCoord2f(1, 1);
			glVertex2f( PW + plusX, PH + plusY );
		glEnd();
	}

	glColor4f( 1.0f, 1.0f, 1.0f, alpha );
	float PWCP = getX( this->currentColorPickerPos.x );
	float PHCP = getY( this->currentColorPickerPos.y );

	glBegin( GL_QUADS );	
		glTexCoord2f(0, 1);
		glVertex2f( PWCP - plusX, PHCP + plusY );

		glTexCoord2f(0, 0);
		glVertex2f( PWCP - plusX, PHCP - plusY );

		glTexCoord2f(1, 0);
		glVertex2f( PWCP + plusX, PHCP - plusY );

		glTexCoord2f(1, 1);
		glVertex2f( PWCP + plusX, PHCP + plusY );
	glEnd();

	glDisable(GL_BLEND);

	glViewport(0, 0,  *windowsW, *windowsH);
}

bool TransferFunction::MouseButton( int w, int h, int button, int action )
{
	if(!isVisible)
		return false;

	w-=posx;
	h-=posy;

	static int DIVBASESC = MAXWSC / 6;
	
	if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
	{
		if( w >= MINW - 5 && w <= MAXW + 5 && h >= MINH - 5 && h <= MAXH + 5 )
		{
			if( !this->Picking( w, h, GRAFIC ) )
			{
				if( this->ptsCounter < MAXPOINT - 1 )
				{
					this->pointList[ this->ptsCounter ].x  = w;
					this->pointList[ this->ptsCounter ].y  = h;
					this->pointSelected = ++this->ptsCounter;

					this->UpdateColorPoint();
				}
			}
			else
			{
				this->indicatorSC = this->colorPosList[ this->pointSelected - 1 ];

				int completeSize =  ( this->indicatorSC / DIVBASESC )  * DIVBASESC;
				int currentSize = this->indicatorSC - completeSize;
				float t = ( float )currentSize / ( float ) DIVBASESC;

				this->currentColor = this->baseColors[ completeSize / DIVBASESC ] * ( 1.0 - t ) + this->baseColors[ ( ( completeSize / DIVBASESC ) + 1 ) % 6 ] * t;

				this->currentColorPickerPos = this->colorPickerPosList[ this->pointSelected - 1 ];
			}
			this->SortPoints();
			this->UpdatePallete();

			return true;
		}
		else if( w >= MINWSC && w <= MAXWSC && h >= MINHSC && h <= MAXHSC )
		{
			this->Picking( w, h, SELECTCOLOR );
			this->indicatorSC = w;

			int completeSize =  ( this->indicatorSC / DIVBASESC )  * DIVBASESC;
			int currentSize = this->indicatorSC - completeSize;
			float t = ( float )currentSize / ( float ) DIVBASESC;

			this->currentColor = this->baseColors[ completeSize / DIVBASESC ] * ( 1.0 - t ) + this->baseColors[ ( ( completeSize / DIVBASESC ) + 1 ) % 6 ] * t;

			if( this->pointSelected != 0 ) this->UpdateColorPoint();
			this->UpdatePallete();

			return true;
		}
		else if( w >= MINWPC && w <= MAXWPC && h >= MINHPC && h <= MAXHPC )
		{
			this->Picking( w, h, PICKERCOLOR );

			if( w < MINWPC + 6 ) w = MINWPC + 6; if( w > MAXWPC - 6 ) w = MAXWPC - 6;
			if( h < MINHPC + 6 ) h = MINHPC + 6; if( h > MAXHPC - 6 ) h = MAXHPC - 6;

			this->currentColorPickerPos.x = w;
			this->currentColorPickerPos.y = h;

			if( this->pointSelected != 0 ) this->UpdateColorPoint();
			this->UpdatePallete();

			return true;
		}
		else if( w >= 0 && w <= 365 && h >= 0 && h <= 455 )
		{
			antx = posx + w;
			anty = posy + h;
			dragDropWindow = true;
			alpha = 0.7f;

			return true;
		}
	}
	else if( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		return DeletePoint(w,h);
	}
	else if ( action == GLFW_RELEASE )
	{
		this->dragDrop = false;
		this->dragDropColor = false;
		this->dragDropPicker = false;
		this->dragDropWindow = false;
		alpha = 0.5f;
	}

	return false;
}

bool TransferFunction::DeletePoint( int w, int h )
{
	int saveLastSelectedPoint = this->pointSelected;
	
	if( this->Picking( w, h, GRAFIC ) )
	{
		if( this->lastPicking != 1 && this->lastPicking != this->ptsCounter )
		{
			if( saveLastSelectedPoint == this->lastPicking ) 
				this->pointSelected = 0;
			else
				this->pointSelected = saveLastSelectedPoint;
			
			SortPoints( this->lastPicking - 1 );
			this->UpdatePallete();
			this->pointSelected = 1;
			this->lastPicking = -1;
			this->ptsCounter--;
		}else
			this->pointSelected = 0;

		this->dragDrop = false;
		return true;
	}

	return false;
}

bool TransferFunction::Picking( int x, int y, PickingValidation type )
{
	GLuint buff[64] = {0};
	GLint  hits, view[4];
	this->lastPicking = -1;
	this->lastPickingColor = 0;

	glSelectBuffer(64, buff);
	glGetIntegerv( GL_VIEWPORT, view );
	glRenderMode( GL_SELECT );
	glInitNames();
	glPushName(0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();

		gluPickMatrix(x, view[3]-y, 1.0, 1.0, view );
		glOrtho(0, *this->windowsW, *this->windowsH, 0, 0, 0);

		glMatrixMode(GL_MODELVIEW);
		glfwSwapBuffers(this->window);

		if( type == GRAFIC )
		{
			float plusX = 1.0 + getX( 7 );
			float plusY = 1.0 - getY( 7 );

			for( int point = 0; point < this->ptsCounter; point++ )
			{
				glLoadName( point + 1 );

				float PW = getX( this->pointList[ point ].x );
				float PH = getY( this->pointList[ point ].y );

				glBegin( GL_QUADS );	
					glVertex2f( PW - plusX, PH + plusY );
					glVertex2f( PW - plusX, PH - plusY );
					glVertex2f( PW + plusX, PH - plusY );
					glVertex2f( PW + plusX, PH + plusY );
				glEnd();

			}
		}
		else if( type ==  SELECTCOLOR )
		{
			float h = getY( imageH );
			float divW = ( -1.0 - getX( imageW ) ) / 6.0;
			float h2 = 1.0 - getY( 2 );
			float h3 = 1.0 - getY( 30 );

			for( int i=0; i<6; i++ )
			{
				glLoadName( i + 1 );

				glBegin( GL_QUADS );
					glVertex3f( -1.0 - divW * i, h - h2, 0.0f );
					glVertex3f( -1.0 - divW * i, h - h3, 0.0f );
					glVertex3f( -1.0 - divW * ( i + 1 ), h - h3, 0.0f );
					glVertex3f( -1.0 - divW * ( i + 1 ), h - h2, 0.0f );
				glEnd();
			}
		}
		else if( type == PICKERCOLOR )
		{
			float plusX = 1.0 + getX( 7 );
			float plusY = 1.0 - getY( 7 );

			float PWCP = getX( this->currentColorPickerPos.x );
			float PHCP = getY( this->currentColorPickerPos.y );

			float w = getX( imageW );
			float h = getY( imageH );			
			float divW = ( -1.0 - w ) / 6.0;
			float h2 = 1.0 - getY( 2 );
			float h3 = 1.0 - getY( 30 );
			float alpha = 1.0;
			float h4 = h + h2 + h2 + h3;
			float h5 = h - ( h3 + h2 + h2 + ( 1.0 - h ) );

			glBegin( GL_QUADS );
				glVertex3f( -1.0, h - ( h3 + h2 + h2 ), 0.0f );
				glVertex3f( -1.0f, h5, 0.0f );
				glVertex3f( w, h5, 0.0f );
				glVertex3f( w, h - ( h3 + h2 + h2 ), 0.0f );
			glEnd();
		}

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	hits = glRenderMode( GL_RENDER );
	for( int i = 0; i < hits; i++ )
	{
		if( i == hits - 1 )
		{
			if( type == GRAFIC )
			{
				this->lastPicking = (GLubyte) buff[ i * 4 + 3 ];
				this->pointSelected = this->lastPicking;
				this->dragDrop = true;
				break;
			}
			else if( type == SELECTCOLOR )
			{
				this->lastPickingColor = (GLubyte) buff[ i * 4 + 3 ];
				this->dragDropColor = true;
				break;
			}
			else if( type == PICKERCOLOR )
			{
				this->dragDropPicker = true;
				break;
			}
		}
	}

	glMatrixMode(GL_MODELVIEW);
	return this->dragDrop;
}

void TransferFunction::SortPoints( int jumpPoint )
{
	std::priority_queue< specialPoint > pilaState;
	int less = ( jumpPoint == -1 )?0:1;

	for( int point = 0; point < this->ptsCounter; point++ ){
		if( point != jumpPoint ){
			bool selectPicking =  (( this->dragDrop ) && ( point == this->lastPicking - 1 ));
			bool selectPoint = ( point == this->pointSelected - 1 );
			pilaState.push( specialPoint( this->pointList[ point ], this->colorList[ point ], this->colorPickerPosList[ point ], this->colorPosList[ point ], selectPicking, selectPoint ) );
		}
	}
	
	for( int point = 0; point < this->ptsCounter - less; point++ ){

		this->pointList[ point ] = pilaState.top().point;
		this->colorList[ point ] = pilaState.top().color;
		this->colorPosList[ point ] = pilaState.top().selectedColorPos;
		this->colorPickerPosList[ point ] = pilaState.top().pickerPos;

		if( pilaState.top().drag ) this->lastPicking = point + 1;
		if( pilaState.top().selected ) this->pointSelected = point + 1;
		pilaState.pop();

	}
}

bool TransferFunction::CursorPos( int w, int h )
{
	if(!isVisible)
		return false;

	w-=posx;
	h-=posy;

	static int DIVBASESC = MAXWSC / 6;

	if( this->dragDrop )
	{
		if( w > MAXW ) w = MAXW;	if( h > MAXH ) h = MAXH;
		if( w < MINW ) w = MINW;	if( h < MINH ) h = MINH;		

		if( this->lastPicking - 1 != 0 && this->lastPicking != ptsCounter )this->pointList[ this->lastPicking - 1 ].x  = w;
		this->pointList[ this->lastPicking - 1 ].y = h;

		this->SortPoints();
		if( this->pointSelected != 0 ) this->UpdateColorPoint();
		this->UpdatePallete();

		return true;
	}
	else if( this->dragDropColor )
	{
		if( w > MAXWSC - 5 ) w = MAXWSC - 5;	if( w < MINWSC ) w = MINWSC;

		this->indicatorSC = w;

		int completeSize =  ( w / DIVBASESC )  * DIVBASESC;
		int currentSize = w - completeSize;
		float t = ( float )currentSize / ( float ) DIVBASESC;

		this->currentColor = this->baseColors[ completeSize / DIVBASESC ] * ( 1.0 - t ) + this->baseColors[ ( ( completeSize / DIVBASESC ) + 1 ) % 6 ] * t;

		if( this->pointSelected != 0 ) this->UpdateColorPoint();
		this->UpdatePallete();

		return true;
	}
	else if( this->dragDropPicker )
	{
		if( w > MAXWPC ) w = MAXWPC;	if( h > MAXHPC ) h = MAXHPC;
		if( w < MINWPC ) w = MINWPC;	if( h < MINHPC ) h = MINHPC;

		if( w < MINWPC + 5 ) w = MINWPC + 5; if( w > MAXWPC - 5 ) w = MAXWPC - 5;
		if( h < MINHPC + 5 ) h = MINHPC + 5; if( h > MAXHPC) h = MAXHPC;

		this->currentColorPickerPos.x = w;
		this->currentColorPickerPos.y = h;

		if( this->pointSelected != 0 ) this->UpdateColorPoint();
		this->UpdatePallete();

		return true;
	}
	else if( this ->dragDropWindow )
	{
		int tx = 2 * posx + w - antx;
		int ty = 2 * posy + h - anty; 

		antx = posx + w;
		anty = posy + h;

		if(tx < *this->windowsW - 360 && tx > 0)
			posx = tx;
		
		if(ty < *this->windowsH - 455 && ty > 0)
			posy = ty; 	
		
		return true;
	}

	return false;
}

void TransferFunction::UpdateColorPoint()
{
	static glm::vec4 whiteColor( 1.0f, 1.0f, 1.0f, 1.0 );
	static glm::vec4 blackColor( 0.0f, 0.0f, 0.0f, 1.0 );
	float alpha = 1.0 - (( float )( pointList[ this->pointSelected - 1 ].y - MINH ) / ( float )( MAXH - MINH ));
	glm::vec4 copyCurrentColor( this->currentColor.x, this->currentColor.y, this->currentColor.z, alpha );

	float tw =  (float)this->currentColorPickerPos.x / (float)MAXWPC;
	float th = (float)( this->currentColorPickerPos.y - MINHPC ) / (float)( MAXHPC - MINHPC );

	glm::vec4 colorW = whiteColor * ( 1.0 - tw ) + copyCurrentColor * tw;
	this->colorList[ this->pointSelected - 1 ] = colorW * ( 1.0 - th ) + blackColor * th;
	this->colorList[ this->pointSelected - 1 ].w = alpha;

	this->colorPosList[ this->pointSelected - 1 ] = this->indicatorSC;
	this->colorPickerPosList[ this->pointSelected - 1 ] = this->currentColorPickerPos;
}

void TransferFunction::UpdatePallete()
{
	static float completeAreaSize = MAXW - MINW;
	static float stepSizeBox = completeAreaSize / NUMOFCOLORS;
	int indexOfPallete = 0;

	for( int point = 1; point < this->ptsCounter; point++ )
	{
		float dist = pointList[ point ].x - pointList[ point - 1 ].x;
		int stepsNumber = dist / stepSizeBox;

		float floatStepSize = stepSizeBox / dist;

		for( int step = 0; step < stepsNumber; step++, indexOfPallete++ )
		{
			glm::vec4 currentColor = colorList[ point - 1 ] * ( 1.0 - ( floatStepSize * (float) step ) ) + colorList[ point ] * ( floatStepSize * (float) step );
			this->colorPalette[ indexOfPallete ][ 0 ] = currentColor.x;
			this->colorPalette[ indexOfPallete ][ 1 ] = currentColor.y;
			this->colorPalette[ indexOfPallete ][ 2 ] = currentColor.z;
			this->colorPalette[ indexOfPallete ][ 3 ] = currentColor.w;
		}
	}

	if( indexOfPallete < 255 )
	{
		glm::vec4 currentColor = colorList[ this->ptsCounter - 1 ];

		for( int step = indexOfPallete; step < 256; step++, indexOfPallete++ )
		{
			this->colorPalette[ indexOfPallete ][ 0 ] = currentColor.x;
			this->colorPalette[ indexOfPallete ][ 1 ] = currentColor.y;
			this->colorPalette[ indexOfPallete ][ 2 ] = currentColor.z;
			this->colorPalette[ indexOfPallete ][ 3 ] = currentColor.w;
		}
	}

	this->updateTexture = true;
}