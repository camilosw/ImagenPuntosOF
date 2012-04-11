#include "ofMain.h"
#include "ImagenPuntos.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    ofAppGlutWindow window;
	//ofSetupOpenGL(&window, 800,600, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(&window, 512,240, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ImagenPuntos());

}
