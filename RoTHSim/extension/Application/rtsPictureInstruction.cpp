//---------------------------------------------------------------------------
#include "rtsPictureInstruction.h"
//---------------------------------------------------------------------------

//===========================================================================
/*!
 Constructor of rtsPictureInstruction.
 
 \fn     rtsPictureInstruction::rtsPictureInstruction()
 \param
 */
//===========================================================================
rtsPictureInstruction::rtsPictureInstruction(string a_filename, double a_startTime, double a_minDuration, double a_maxDuration, int a_windowW, int a_windowH, 
											string a_responseType, double a_fadeDuration)
{
	initTutorialInstruction(a_startTime, a_minDuration, a_maxDuration, a_responseType, a_fadeDuration);

	cImagePtr img = cImage::create();
	//cLoadPNG(img->getImage(), CHAI3D_LOGO_BLACK_BYTEARRAY, sizeof(CHAI3D_LOGO_BLACK_BYTEARRAY));
	cLoadFileBMP(img->getImage(), a_filename);

    m_picture = new cBitmap();
	m_picture->loadFromImage(img);
    addChild(m_picture);
}

rtsPictureInstruction::~rtsPictureInstruction()
{
    // deallocate memory
    deallocate();
}

void rtsPictureInstruction::deallocate()
{

}