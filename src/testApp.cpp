#include "testApp.h"
#include "ofxSimpleGuiToo.h"

ofMutex mutex ;

//--------------------------------------------------------------
void testApp::setup()
{

    ofBackground(0,0,0) ; 
    hitTestHub = (ofPtr<PixelHitTestHub>) new PixelHitTestHub() ; 
    hitTestHub->setup( ofGetWidth() , ofGetHeight() , 0x000000 ) ; 
    doFlick = false ;
    speedRatio = 1.0f ;
	ofSetVerticalSync(true);
	mouseEnabled = true ;

    firstTouch = false ;
    isTouchIdle = false ;
    debug = false ;

    lastTouchTime = -5.0f ;
    
    //Load some initial assets
    background.loadImage( "branding/bg.png" ) ;

	//Connect to Port
	myTuio.connect(3333);

    if ( XML.loadFile( "settings.xml" )  ) {
        cout << "settings.xml loaded!" << endl ;
    }
    else {
        cout << "unable to load settings.xml" << endl ;
    }

    //Setup the TUIO Listeners
  	ofAddListener(myTuio.cursorAdded,this,&testApp::tuioCursorAdded);
	ofAddListener(myTuio.cursorRemoved,this,&testApp::tuioCursorRemoved);
	ofAddListener(myTuio.cursorUpdated,this,&testApp::tuioCursorUpdated);

    parseXML() ; 
    
    Tweenzor::init();

    //Create a video for each ripple to play, in case they have to play asynchronously
    videos = new ofVideoPlayer[nItems] ;

    for ( int i = 0 ; i < nItems ; i++ )
    {
        videos[i].setPixelFormat( OF_PIXELS_RGB ) ;
        videos[i].loadMovie ( "RippleSmall_mp4_"+ofToString(i)+".mov" ) ;
        videos[i].setAnchorPercent( 0.5f , 0.5f ) ;
        videos[i].setLoopState( OF_LOOP_NONE ) ;
    }

    maxThrowDistance = 300.0f ;
  
    //Get random angles for lines
    for ( int i = 0 ; i < nItems ; i++ )
    {
        menuNodes[i].setRandomAngles ( 0.01f, PI * 2.0f  , 80 ) ;
    }

    nodeGlow.loadImage ( ofToDataPath( "misc/module_glow.png" ) ) ;
    nodeGlow.setAnchorPercent( 0.5f , 0.5f ) ;

    forwardTouchMinRatio = 0.7 ;

    selectedSound.loadSound ( ofToDataPath("sound/bounce_in.mp3" ) ) ;
    selectedSound.setMultiPlay( true ) ;
    selectedSound.setVolume ( 0.45f ) ;

    timeOutSound.loadSound ( ofToDataPath("sound/bounce_out.mp3" ) ) ;
    timeOutSound.setMultiPlay ( true ) ;
    timeOutSound.setVolume ( 0.35f ) ;

    mouseTouches = 0 ;

    lastMouseX = 0.0f ;
    lastMouseY = 0.0f ;
    
    setupGUI() ; 
    
    //Setup global event listener for if a child object gets selected
    ofAddListener( PixelEvent::Instance()->pixelDownEvent , this , &testApp::hexColorHandler ) ;
}

void testApp::hexColorHandler ( const void * sender , PixelEventArgs &args ) 
{
    //cout << "HEX COLOR HANDLER ! " << endl ; 
}

//--------------------------------------------------------------
void testApp::update()
{
    if ( isTouchIdle == false && ofGetElapsedTimef() > lastTouchTime + touchTimeDelay )
        toggleIdle() ;

    //	mtActionsHub.update();
    Tweenzor::update( ofGetElapsedTimeMillis() ) ;

    for(int i = 0; i < nItems; i++)
    {
        menuNodes[i].onUpdate() ;
        videos[i].update() ;

        if ( menuNodes[i].playSelectedSoundFlag == true )
        {
            selectedSound.play() ;
            menuNodes[i].playSelectedSoundFlag = false ;
            cout << "playing selected sound! " << endl ;
        }
        if ( menuNodes[i].playTimeOutSoundFlag == true )
        {
           // timeOutSound.setSpeed ( -0.5f ) ;
            timeOutSound.play() ;
            menuNodes[i].playTimeOutSoundFlag = false ;
            cout << "playing time out sound! " << endl ;
        }

    }

    //Idle Update
    if ( isTouchIdle == true )
    {
        float minDistance = menuNodes[0].width ;
        int w = ofGetWidth() +- minDistance * 0.5f ;
        int h = ofGetHeight() +- minDistance * 0.5 ;
        for(int i = 0; i < nItems; i++)
        {
            menuNodes[i].onUpdate() ;
            float _x = menuNodes[i].x ;
            float _y = menuNodes[i].y ;
            float speed = ( menuNodes[i].velocity.x + menuNodes[i].velocity.y ) * -.025f  ;
            menuNodes[i].setRotation( menuNodes[i].getRotation() + speed ) ;
            for ( int a = 0 ; a < nItems ; a++ )
            {
                float dist = ofDist ( _x , _y , menuNodes[a].x , menuNodes[a].y );
                if ( a != i && dist < minDistance )
                {
                    ofPoint iPos = ofPoint ( menuNodes[i].x , menuNodes[i].y ) ;
                    ofPoint aPos = ofPoint ( menuNodes[a].x , menuNodes[a].y ) ;
                    ofPoint sub1 = iPos - aPos ;
                    sub1.limit ( .25f ) ;
                    menuNodes[i].velocity = sub1 ;
                    ofPoint sub2 = aPos - iPos ;
                    sub2.limit( .25f ) ;
                    menuNodes[a].velocity = sub2 ;
                }
            }
            if ( _x < minDistance * 0.5f  )
            {
                menuNodes[i].velocity.x *= -1 ;
                menuNodes[i].x = minDistance * 0.5f ;
            }
            if ( _x > w )
            {
                menuNodes[i].velocity.x *= -1 ;
                menuNodes[i].x = _x ;
            }
            if ( _y < minDistance * 0.5f )
            {
                menuNodes[i].velocity.y *= -1 ;
                menuNodes[i].y = minDistance * 0.5f ;
            }
            if ( _y > h )
            {
                menuNodes[i].velocity.y *= -1 ;
                menuNodes[i].y = _y ;
            }
        }
    }


    ofSoundUpdate() ;

    lastMouseX = mouseX ;
    lastMouseY = mouseY ;
}

//--------------------------------------------------------------
void testApp::draw(){

    //Draw input resolution
    ofDisableAlphaBlending() ; 
    ofSetColor ( 255 , 255 , 255 ) ; 
    hitTestHub->beginFbo() ; 
        for ( int k = 0 ; k < nItems; k++ )
        {
            menuNodes[k].drawInputMap() ; 
        }
    hitTestHub->endFbo() ; 
    
    ofSetCircleResolution(125);
    //ofSetLineResolution ( 65 ) ;

    ofSetColor ( 255 , 255 ,255 ) ;
    background.draw(0,0) ;

    drawNodeLines( ) ; 
   
    ofSetColor ( 255 , 255 , 255 ) ;
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE);

    float largeSize = 850.0f ;
    for ( int i = 0 ; i < nItems ; i++ )
    {
        ContentItem * iContent ;
        iContent = &menuNodes[i] ;

        if ( iContent->playVideo == true )
        {
            ofVideoPlayer * iVideo ;
            iVideo = &videos[i] ;
            int indexToDraw = -4 ;

            if ( iContent->playVideoFlag == true )
            {
                if ( iContent->playSubNode == true )
                {
                    cout << "play sub Node " << endl ; 
                    iVideo->setSpeed ( 1.75f ) ;
                    ofPoint subStagePos ;
                    if ( iContent->state == 1 )
                    {
                        indexToDraw = ( iContent->tier1Node < 0 ) ? iContent->nItems-1 : iContent->tier1Node ;
                        subStagePos = iContent->nodes[indexToDraw].stagePos ;
                        cout << "state == 1 " << endl ; 
                    }
                    else
                    {
                        cout << "PlaySub Node state was not 1 " << endl ; 
                    }
                }
                else
                {
                    iVideo->setSpeed ( 1.1f ) ;
                }
                iContent->playVideoFlag = false;
                iVideo->setPosition ( 0.0f ) ;
                iVideo->play() ;
            }

            if ( iContent->playSubNode == true )
            {
                ofPoint subStagePos ;
                if ( iContent->state == 1 )
                {
                    indexToDraw = ( iContent->tier1Node < 0 ) ? iContent->nItems-1 : iContent->tier1Node ;
                    subStagePos = iContent->nodes[indexToDraw].stagePos ;
                    cout << "subStagePos : " << subStagePos << endl ; 
                    iVideo->draw ( subStagePos.x , subStagePos.y ) ;
                }
            }
            else
            {
                cout << "not play sub node!" << endl ; 
                iVideo->draw ( menuNodes[i].x , menuNodes[i].y , largeSize , largeSize ) ;
            }
        }
    }
    glDisable ( GL_BLEND ) ;
    ofEnableAlphaBlending() ;

    for ( int i = 0 ; i < nItems ; i++ )
    {
        if ( menuNodes[i].state != 0 && menuNodes[i].state != 3 )
        {
            ofSetColor ( 255 , 255 , 255 , 255.0f * menuNodes[i].glowAlpha ) ;
            float scale = menuNodes[i].scale ;
            float glowSize = menuNodes[i].width * 2.0f ;
            nodeGlow.draw ( menuNodes[i].x , menuNodes[i].y , glowSize , glowSize) ;
        }
    }

    ofSetColor ( 255 , 255 , 255 ) ;

    mutex.lock() ;
  	mtActionsHub.renderAsStack();
    mutex.unlock() ;

    ofFill() ;
    if ( debug == true )
    {
        //render TUIO Cursors and Objects
        drawRawCursors() ;
    }

    if ( hitTestHub->debugDraw == true ) 
    {
        ofPushMatrix() ; 
        ofTranslate( ofGetWidth() * .7 , ofGetHeight() * .65 , 0 ) ; 
        hitTestHub->drawMap( ) ; 
        ofPopMatrix() ; 
    }
    
    gui.draw();
    ofDisableSmoothing() ;

    ofSetColor ( 255 , 255 , 255 , 255 ) ;
}

void testApp::drawNodeLines( ) 
{
    /* Draw test lines between nodes */
    ofEnableAlphaBlending() ;
    
    ofNoFill() ;
    ofSetLineWidth( 1 ) ;
    
    float numLinesPerNode =   nOutsideLines ;
    
    ofEnableSmoothing() ;
    
    ofPoint menuPos ;
    ofPoint hereToThere , thereToHere , mid , targetPos , hereCP , thereCP ;
    ContentItem * iContent ;
    
    for ( int i = 0 ; i < nItems; i++ )
    {
        //Get position of the menu
        menuPos = ofPoint ( menuNodes[i].x , menuNodes[i].y ) ;
        iContent = &menuNodes[i] ;
        
        if ( menuNodes[i].drawLines == true )
        {
            int otherNodeIndex = menuNodes[i].menuNodeDrawTo ;
            ofPoint offscreenPosition = ofPoint() ;
            if( menuNodes[i].randomNodeDrawTo > 0 )
                offscreenPosition = extraNodes[ menuNodes[i].randomNodeDrawTo ] ;
            float alphaToUse = menuNodes[i].linesAlpha ;
            float angleFactor1 = ( flipFirstAngle == true ) ? -1 : 1 ;
            float angleFactor2 = ( flipSecondAngle == true ) ? -1 : 1 ;
            
            alphaToUse = ( menuNodes[otherNodeIndex].linesAlpha > menuNodes[i].linesAlpha ) ? menuNodes[i].linesAlpha : menuNodes[otherNodeIndex].linesAlpha ;
            
            float targetDistance = 100.0f ;
            float dist = ofDist ( menuPos.x , menuPos.y , targetPos.x , targetPos.y ) ;
            float distRatio =  ( dist / targetDistance ) ;
            float normalizedAnchorDistance = outerFillamentDistance ;
            //normalizedAnchorDistance = ( dist < targetDistance ) ? normalizedAnchorDistance * distRatio : normalizedAnchorDistance ;
            
            for ( int p = 0 ; p < numLinesPerNode ; p++ )
            {
                //Don't draw lines to itself and only if it's not selected
                if ( i != otherNodeIndex && menuNodes[otherNodeIndex].drawLines == true )
                {
                    ofSetColor( 255 , 255 , 255 , alphaToUse * outerLinesAlpha ) ;
                    targetPos = ofPoint ( menuNodes[otherNodeIndex].x , menuNodes[otherNodeIndex].y ) ;
                    
                    float ratioAngle = outerMinAngle + outerFillamentAngle * ( iContent->randomAngles[p] ) ;
                    
                    //Data to draw from here to menuNodes[k]
                    hereToThere = menuPos +- targetPos ;
                    float hereAngle = atan2 ( hereToThere.y , hereToThere.x ) + ratioAngle * angleFactor1 ;
                    hereCP = ofPoint ( menuPos.x + cos ( hereAngle ) * normalizedAnchorDistance , menuPos.y + sin ( hereAngle ) * normalizedAnchorDistance ) ;
                    
                    //Data to draw from menuNodes[k] to here
                    thereToHere = targetPos +- menuPos ;
                    float thereAngle = atan2 ( thereToHere.y , thereToHere.x ) + ratioAngle * angleFactor2 ;
                    thereCP = ofPoint ( targetPos.x + cos ( thereAngle ) * normalizedAnchorDistance , menuPos.y + sin ( thereAngle ) * normalizedAnchorDistance ) ;
                    
                    ofBeginShape() ;
                    ofVertex(menuPos.x, menuPos.y) ;
                    ofBezierVertex(menuPos.x, menuPos.y, hereCP.x, hereCP.y, hereCP.x, hereCP.y ) ;
                    ofBezierVertex(hereCP.x, hereCP.y, thereCP.x, thereCP.y, targetPos.x, targetPos.y ) ;
                    ofEndShape() ;
                }
                
            }
            
            
            //Draw offscreen at predetermined random angles
            if( menuNodes[i].randomNodeDrawTo > 0 )
            {
                alphaToUse =  menuNodes[i].linesAlpha ;
                ofSetColor( 255 , 255 , 255 , alphaToUse * innerLinesAlpha ) ;
                float targetDistance = 800.0f ;
                float targetDistance2 = 40.0f ;
                float dist = ofDist ( menuPos.x , menuPos.y , targetPos.x , targetPos.y ) ;
                float normalizedAnchorDistance = ( dist / targetDistance ) * innerFillamentDistance ;
                //float normalizedAnchorDistance = innerFillamentDistance ;
                float normalizedAnchorDistance2 = ( dist / targetDistance2 ) * innerFillamentDistance ;
                
                for ( int p = 0 ; p < nInsideLines ; p++ )
                {
                    
                    targetPos = offscreenPosition ;
                    float ratioAngle = innerMinAngle + innerFillamentAngle * ( iContent->randomAngles[p] ) ;
                    
                    //Data to draw from here to menuNodes[k]
                    hereToThere = menuPos +- targetPos ;
                    float hereAngle = atan2 ( hereToThere.y , hereToThere.x ) + ratioAngle * angleFactor1 ;
                    hereCP = ofPoint ( menuPos.x + cos ( hereAngle ) * normalizedAnchorDistance , menuPos.y + sin ( hereAngle ) * normalizedAnchorDistance ) ;
                    
                    //Data to draw from menuNodes[k] to here
                    thereToHere = targetPos +- menuPos ;
                    float thereAngle = atan2 ( thereToHere.y , thereToHere.x ) + ratioAngle * angleFactor2 ;
                    thereCP = ofPoint ( targetPos.x + cos ( thereAngle ) * normalizedAnchorDistance2 , menuPos.y + sin ( thereAngle ) * normalizedAnchorDistance2 ) ;
                    ofBeginShape() ;
                    ofVertex(menuPos.x, menuPos.y) ;
                    //      ofBezierVertex(hereCP.x, hereCP.y, thereCP.x, thereCP.y, targetPos.x, targetPos.y ) ;
                    ofBezierVertex(menuPos.x, menuPos.y, hereCP.x, hereCP.y, hereCP.x, hereCP.y ) ;
                    ofBezierVertex(hereCP.x, hereCP.y, thereCP.x, thereCP.y, targetPos.x, targetPos.y ) ;
                    ofEndShape() ;
                    //  ofBezier( menuPos.x , menuPos.y , hereCP.x , hereCP.y , thereCP.x , thereCP.y , targetPos.x , targetPos.y ) ;
                }
            }
        }
    }
}

void testApp::drawRawCursors()
{
    const int w = ofGetWidth() ;
    const int h = ofGetHeight() ;
    std::list<TuioCursor*> cursorList = myTuio.client->getTuioCursors();
    std::list<TuioCursor*>::iterator cursor;
    myTuio.client->lockCursorList() ;
    ofNoFill () ;

    for ( cursor = cursorList.begin() ; cursor != cursorList.end() ; cursor++ )
    {
        TuioCursor * cur = ( *cursor ) ;
        glColor4f( 1.0f , 1.0f , 1.0f , 1.0f ) ;
        ofCircle(cur->getX() * w, cur->getY() * h, 8 ) ;
        string id = ofToString( (int)(cur->getCursorID() )) ;
        ofDrawBitmapString( id ,  cur->getX() * w , cur->getY() * h +- 25 ) ;
    }
    ofFill () ;
    myTuio.client->unlockCursorList() ;
}

void testApp::toggleIdle()
{
    isTouchIdle = !isTouchIdle ;
    if ( isTouchIdle )
    {
        for ( int i = 0 ; i < nItems ; i++ )
        {
            menuNodes[i].nextState = 3 ;
            menuNodes[i].changeState ( 3 ) ;
        }
    }
};

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    cout << "key pressed : " << key <<endl ;
    switch ( key )
    {
        case 's':
        case 'S':
            //Save XML Settings, even though they should auto-save
            XML.saveFile("settings.xml");
            break ;

        //Toggle on the input Map
        case 'i':
        case 'I':
            hitTestHub->debugDraw = !hitTestHub->debugDraw ; 
            break ; 
            
        case 'd':
        case 'D':
            //Turn all Debugging
            debug = !debug ;
           
            for ( int i = 0 ; i < nItems ; i++ ) 
                menuNodes[i].setDebug ( debug ) ;
        
            break ;

        case 'g':
        case 'G':
            //Bring up the GUI Menu
            gui.toggleDraw() ;
            break ;

        case 'f':
        case 'F':
            ofToggleFullscreen() ;
            break ;

        case 'm':
        case 'M':
            mouseEnabled = !mouseEnabled ;
            if ( mouseEnabled == false ) {
                ofHideCursor() ;
            }
            else {
                ofShowCursor() ;
            }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    hitTestHub->getHexAt( ofVec2f( x , y ) ) ; 
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    if ( debug == true || mouseEnabled == true )
    {
        TuioCursor tc = TuioCursor ( ofGetElapsedTimef() , 22 , mouseTouches , (float)x / ofGetWidth() , (float)y / ofGetHeight() ) ;
        tuioCursorUpdated( tc ) ;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    if ( debug == true || mouseEnabled == true )
    {
        mouseTouches++ ;
        TuioCursor tc = TuioCursor (  ofGetElapsedTimef() , 22 , mouseTouches, (float)x / ofGetWidth() , (float)y / ofGetHeight() ) ;
        tuioCursorAdded( tc ) ;
    }
}


//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if ( debug == true || mouseEnabled == true )
    {
        TuioCursor tc = TuioCursor (  ofGetElapsedTimef() , 22 , mouseTouches , (float)x / ofGetWidth() , (float)y / ofGetHeight() ) ;
        tuioCursorRemoved( tc ) ;
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

void testApp::tuioCursorAdded(TuioCursor & tcur)
{
    ofPoint screenPos = ofPoint ( tcur.getX() * ofGetWidth() , tcur.getY() * ofGetHeight() ) ;

	ofxMultiTouchCustomDataSF multiTouchCustomData;
	multiTouchCustomData.sessionID = tcur.getSessionID();

	mtActionsHub.touchDown(tcur.getX(), tcur.getY(), tcur.getCursorID(), &multiTouchCustomData);
    
    int currentHex = hitTestHub->getHexAt( ofVec2f( screenPos.x , screenPos.y ) ) ; 
    if ( currentHex > -1 )
    {
        PixelEventArgs args = PixelEventArgs ( currentHex ) ;
        ofNotifyEvent( PixelEvent::Instance()->pixelDownEvent , args , this ) ; 
    }
    //forward the touch events to ofxMultiTouch for the InteractiveObjects
    //ofxMultiTouch.touchDown(tcur.getX(), tcur.getY(), tcur.getCursorID(), &multiTouchCustomData);
}

void testApp::tuioCursorRemoved(TuioCursor & tcur)
{
    mtActionsHub.touchUp(tcur.getX(), tcur.getY(), tcur.getCursorID(), NULL);
    lastTouchTime = ofGetElapsedTimef() ;

    int currentHex = hitTestHub->getHexAt( ofVec2f( (int)tcur.getX(), (int)tcur.getY() ) ) ; 
    if ( currentHex > -1 )
    {
        //Check input up for "flick" release event
        for ( int i = 0 ; i < nItems ; i++ ) 
        {
            if ( currentHex = menuNodes[i].hexInputID && 
                 doFlick == true &&
                 menuNodes[i].inBoundsTransition == false )
            {
                menuNodes[i].inFlickTransition = true ;
                //menuNodes[i].setIsDraggable( false ) ;
                float speedAngle = 0.0f ;
                ofPoint newPosition ;
                float speedX , speedY , length ;

                //cout << "non-scaled speed -- x: " << tcur.getXSpeed() << " y: " << tcur.getYSpeed() << endl ;
                if ( abs( tcur.getXSpeed() ) > minFlickSpeed || abs( tcur.getYSpeed() ) > minFlickSpeed )
                {
                    speedX = tcur.getXSpeed() * speedRatio ;
                    speedY = tcur.getYSpeed() * speedRatio ;

                    length = ( speedX * speedX + speedY * speedY ) ;
                    length = ( length > maxThrowDistance ) ? maxThrowDistance : length ;

                    speedAngle = atan2( speedY , speedX ) ;
                    newPosition = ofPoint ( menuNodes[i].x + cos ( speedAngle ) * length , menuNodes[i].y + sin ( speedAngle ) * length ) ;
                
                    Tweenzor::add ( &menuNodes[i].x , menuNodes[i].x , newPosition.x , 0.0f , flickTweenTime , EASE_OUT_QUAD ) ;
                    Tweenzor::add ( &menuNodes[i].y , menuNodes[i].y , newPosition.y , 0.0f , flickTweenTime , EASE_OUT_QUAD ) ;

                    menuNodes[i].startFlickTween() ;
                    return ;
                }
            }
        }
    }
}

void testApp::tuioCursorUpdated(TuioCursor & tcur)
{
	ofxMultiTouchCustomDataSF multiTouchCustomData;
	multiTouchCustomData.sessionID = tcur.getSessionID();
	mtActionsHub.touchMoved(tcur.getX(), tcur.getY(), tcur.getCursorID(), &multiTouchCustomData);

	//forward the touch events to ofxMultiTouch for the InteractiveObjects
//	ofxMultiTouch.touchMoved(tcur.getX(), tcur.getY(), tcur.getCursorID(), NULL);
}

void testApp::setupGUI() 
{
    gui.addTitle("GUI");
    //Between Node Lines
    gui.addPage ("Nodal Fillaments").setXMLName( "guiSettings.xml" ) ;
    gui.addTitle( "Between Nodes" ) ;
    gui.addSlider( "angleSpread" , outerFillamentAngle , -PI * 2.0f ,PI * 2.0f ) ;
    gui.addSlider( "distance from Node" , outerFillamentDistance , 25.0f , 800.0f ) ;
    gui.addSlider ( "number of connections" , nOutsideLines , 0.0f , 80.0f  ) ;
    gui.addSlider ( "angleSpread offset" , outerMinAngle , -PI * 1.0f , PI * 1.0f ) ;
    gui.addSlider ( "transparency" , outerLinesAlpha , 0.0 , 255.0f ) ;
    
    //Out of bounds lines
    gui.addTitle ( "Out of Bounds Fillaments" ) ;
    gui.addSlider( "bounds Angle Spread" , innerFillamentAngle , -PI * 2.0f ,PI * 2.0f ) ;
    gui.addSlider( "controlPoint distance" , innerFillamentDistance , 0.0f , 400.0f ) ;
    gui.addSlider ( "bounds number of connections" , nInsideLines , 0.0f , 80.0f  ) ;
    gui.addSlider ( "boundsAngle offset" , innerMinAngle , -PI * 1.0f , PI * 1.0f ) ;
    gui.addSlider ( "out of bounds transparency" , innerLinesAlpha , 0.0 , 255.0f ) ;
    gui.addToggle ( "Flip 1st Angle" , flipFirstAngle ) ;
    gui.addToggle ( "Flip 2nd Angle" , flipSecondAngle ) ;

    //Flick Parameters
    gui.addTitle( "Touch Parameters" ) ;
    gui.addSlider ( "throwSpeed" , speedRatio , XML.getValue( "minTouchSpeedRatio" , 1.0f ) ,  XML.getValue( "maxTouchSpeedRatio" , 1.0f ) ) ;
    gui.addSlider ( "maxThrowDistance" , maxThrowDistance , 10.0f , 800.0f ) ;
    gui.addSlider ( "flickTweenTime" , flickTweenTime , 0.1f , 1.5f ) ;
    gui.addSlider ( "minFlickSpeed" , minFlickSpeed , 0.1f , 3.0f ) ;
    gui.addToggle ( "Enable Flicks" , doFlick ) ;
    
    gui.loadFromXML() ;
    gui.show() ;
    gui.toggleDraw() ;
}

void testApp::parseXML() 
{
    //color pool
    colors = new ofColor[3] ;

    colors[0] = ofColor ( 0 , 110 , 183 ) ; 
    colors[1] = ofColor ( 245 , 124 , 33 ) ; 
    colors[2] = ofColor ( 124 , 40 , 141 ) ; 
    
    //Set the max / min number of touches to trigger dragging
    int * dragRange = new int[2];
    dragRange[0] = 1;
    dragRange[1] = -4; //negative means the abs value and up
    
    //Set the max / min number of touches to trigger rotation
    int * rotateRange = new int[2] ;
    rotateRange[0] = 2 ;
    rotateRange[1] = 3 ;
    
    //Set the max / min number of touches to trigger scaling
    int * scaleRange = new int[2] ;
    scaleRange[0] = 2 ;
    scaleRange[1] = 3 ;
    
    //Get folder paths from XML
    string menuFolderPath = XML.getValue( "tier1Folder" , "nothing" ) ;
    string starterFolderPath = XML.getValue ( "startGFXFolder" , "nothing" ) ;
    string largeBGPath = XML.getValue ( "bigColorFolder" , "nothing_small" ) ;
    string smallBGPath = XML.getValue ( "smallColorFolder" , "nothing_large" ) ;
    nItems = XML.getNumTags ( "menuItem" ) ;
    
    //Create ofxDirList for large Color images
    ofxDirList  largeBGDir ;
    largeBGDir.listDir( largeBGPath ) ;
    ofxDirList  smallBGDir ;
    smallBGDir.listDir( smallBGPath ) ;
    
    float nodeDelay = XML.getValue ( "nodeIdleInSeconds" , 30.0f ) ;
    touchTimeDelay = XML.getValue ( "tableIdleInSeconds" , 50.0f ) ;
    //Set the directory
    DIR.listDir( starterFolderPath );
    
    //Allocate the Menu Nodes
 	menuNodes = new ContentItem[nItems];
    
    //Create starter locations
    numExtra = 3 ;
    extraNodes = new ofPoint[numExtra] ;
    extraNodes[0] = ofPoint ( -400 , -400 ) ;
    extraNodes[1] = ofPoint ( 2570 , -650 ) ;
    extraNodes[2] = ofPoint ( 2970 , 1530 ) ;
    
    ofPoint * startLocs = new ofPoint[nItems] ;
    startLocs[2] = ofPoint ( 200 , 750 ) ;
    startLocs[0] = ofPoint ( 1475 , 770 ) ;
    startLocs[1] = ofPoint ( 1550 , 165 ) ;
    
    //you can now iterate through the files as you like
    for(int i = 0; i < nItems; i++)
    {
        //Setup Paths and XML for creating it's subNodes
        menuNodes[i] = ContentItem( DIR.getPath(i) , largeBGDir.getPath(i) , smallBGDir.getPath(i) , startLocs[i] , 0xFFFFFF, nItems-i , i ,menuFolderPath , XML , colors[i] , hitTestHub ) ;
        
        //Combine these into setupSlideshow
        menuNodes[i].touchResetDelay = nodeDelay ;
        menuNodes[i].setup( ) ; 

        //Skip indices for nicer looking paths
        int nodeIndexToDraw = i-2 ;
        int maxIndex = nItems;
        //Loop Values if beyond length
        nodeIndexToDraw = ( nodeIndexToDraw > maxIndex ) ? nodeIndexToDraw-maxIndex : nodeIndexToDraw ;
        nodeIndexToDraw = ( nodeIndexToDraw < 0 ) ? nodeIndexToDraw +maxIndex : nodeIndexToDraw ;
        menuNodes[i].menuNodeDrawTo = nodeIndexToDraw ;
        if ( i % 2 != 0 || i == 0 )
            menuNodes[i].randomNodeDrawTo = i ;
        else
            menuNodes[i].randomNodeDrawTo = -2 ;
        
        menuNodes[i].registerForMultitouchEvents() ;
        menuNodes[i].setIsDraggable( true , dragRange , 2 );
        menuNodes[i].setIsRotatable( true , rotateRange , 2 ) ;
        mtActionsHub.addObject( &menuNodes[i] ) ;
    }
    
    //Do Some Cleanup
    DIR.reset();
    largeBGDir.reset() ;
    smallBGDir.reset() ;
    delete [] dragRange;
    delete [] rotateRange ;
    delete [] scaleRange ;
    delete [] startLocs ;
    delete [] colors ;

}
