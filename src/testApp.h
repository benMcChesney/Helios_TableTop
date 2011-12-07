#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxDirList.h"
#include "ofxTuioClient.h"
#include "ofxMultiTouch.h"
#include "ofxMarkerHandler.h"
#include "myMtButton.h"
#include "myMtEventButton.h"
#include "ofxMtActionsHub.h"
#include "myMtRotatableScalableItem.h"
#include "ContentItem.h"

#include "ofxTweenzor.h"
#include "ofxXmlSettings.h"
#include "ImageContainer.h"

#include "PixelHitTestHub.h"
#include "PixelEventArgs.h"
#include "PixelEvent.h"

class testApp : public ofBaseApp{

public:

    void setup();
	void update();
	void draw();

	void keyPressed  (int key);

	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

    //Stuff I added
    void drawRawCursors( );

	//TUIO CALLBACK FUNCTIONS
	void tuioCursorAdded(TuioCursor & tcur);
	void tuioCursorRemoved(TuioCursor & tcur);
	void tuioCursorUpdated(TuioCursor & tcur);

    ofVideoPlayer * videos ;
    
    //XML settings
    ofxXmlSettings XML ;

    //Mouse Debug
    int mouseTouches ;
    float lastMouseX ;
    float lastMouseY ;
    float speedRatio ;
    float maxThrowDistance ;
    bool doFlick ;
    float minFlickSpeed ;
    float flickTweenTime ;

	//TUIO CLIENT
	ofxTuioClient myTuio;

	//MultiTouch Actions Hub [manage priorities and zIndex/depth in the stack of each mtActionObj]
	ofxMtActionsHub mtActionsHub;

    ofImage background ;

    // we will have a dynaimic number of images, based on the content of a director:
    int 		  nItems;
    ContentItem	* menuNodes;
    ofxDirList    DIR;

    //Tween it all
    Tween *tween;

    //Touch Delay Parameters
    float lastTouchTime ;
    float touchTimeDelay ;
    bool isTouchIdle ;
    void toggleIdle () ;
    
    //debug //trace or display helpful developer stuff
    bool debug ;

    ofColor * colors ;
    ofImage nodeGlow ;

    bool firstTouch ;

    //Lines
    int numExtra ;
    ofPoint * extraNodes ;

    //GUI changeable
    float outerFillamentAngle ;
    float outerFillamentDistance ;
    float innerFillamentAngle ;
    float innerFillamentDistance ;
    float nOutsideLines ;
    float nInsideLines;
    float outerAngleOffset ;
    float innerAngleOffset ;
    float outerLinesAlpha ;
    float innerLinesAlpha ;

    float innerMinAngle ;
    float outerMinAngle ;

    bool flipFirstAngle ;
    bool flipSecondAngle ;

    bool mouseEnabled ;

    //For hit detection and bouncing
    float forwardTouchMinRatio ;

    //Sound
    ofSoundPlayer selectedSound ;
    ofSoundPlayer timeOutSound ;

    void setupGUI() ; 
    void parseXML() ; 
    
    //PixelHitTest
    ofPtr<PixelHitTestHub> hitTestHub ;
    
    //Custom Event Handler
    void hexColorHandler ( const void * sender , PixelEventArgs &args ) ;
    
  //  ofPtr<PixelEvent> eventHub ; 
    
};

#endif
