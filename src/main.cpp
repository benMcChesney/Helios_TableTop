#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){

    //Make this fullscreen to your resolution when using touch points
    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1920 ,1080, OF_FULLSCREEN );			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new testApp());

}
