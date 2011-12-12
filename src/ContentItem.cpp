//
//  ContentItem.cpp
//  ofxTuio
//
//  Created by Ben McChesney on 8/1/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//

#include "ContentItem.h"

void ContentItem::changeState ( int newState )
{

    if ( newState == state )
        return ;

    cout << "node " << menuIndex << " | state : " << state << " -> " << newState << endl ;
    switch ( newState )
    {
        //Idle start position on home screen
        case 0 :
            lastTouchTime = ofGetElapsedTimef() ;
            drawLines = true ;
            crossFade.fadeImages( &starterImage , &image , 0.5 , false ) ;
            
            if ( state == 1 )
                transitionOutTierMenu( 1 ) ;
            if ( state == 3 )
            {
                transitionComplete( new float(5.0f) ) ;
                state = 0 ;
            }

            break ;

        //Touched once, first tier menu
        case 1 :
            nextState = 1 ;
            lastTouchTime = ofGetElapsedTimef() ;
            transitionInTierMenu( 1 ) ;

           // drawLines = false ;
            state = newState ;
            break ;


            //Idle Bouncing Around
        case 3 :
            state = newState ;
            break ;

    }
    //cout << "menuIndex : " << menuIndex << " STATE = " << state << endl ;
}

void ContentItem::setup( ) 
{
    //Allocate pointers 
    minSize = bg.width * .75 ;
    maxSize = bg.width * 1 ;

    float padding = bg.width * 0.5f ;
    bounds = ofRectangle ( padding , padding , ofGetWidth() +- padding , ofGetHeight() +- padding ) ;
    contentWidth = image.width ;
    contentHeight = image.height ;
    scale = 1.0f ;
    setPriorityLevel ( priorityLevel ) ;
    state = 0 ;
    nextState = -4 ;
    rotation = 0 ;
    drawLines = true ;
    
    //Setup Nodes
    nodes = new NodeButton[nItems] ;
    //Calculate angles between nodes
    float angleStep = ( PI * 2.0f ) / (float)17 ;
    float angleOffset = -angleStep ;
    float nScale = ( width - minSize ) / ( maxSize - minSize ) ;
    carouselRadius = width * radialFactor + ( -nScale * width * .0625f ) * radialFactor ; //width * radialFactor ;
    
    string introPath = XML.getValue( "tier1IntroFolder" , "no intros Path" ) ;
    hexInputID = hitTestHub->getColorfulUniqueHex() ; 
    
    hitTestHub->addItem( this ) ; 
    
    for ( int i = 0 ; i < nItems ; i++ )
    {
        nodes[i] = NodeButton( ) ; 
        nodes[i].hexInputID = hitTestHub->getColorfulUniqueHex() ; 
        float theta ;
        
        //Category Buttons
        if ( i != nItems-1 )
        {
            theta = angleStep * (i-1) ;
            nodes[i].setup( DIR.getPath(i) , node_bgPath , i , XML.getValue ( "tier1DetailFolder" , "nought" )+"/"+DIR.getName(i) ) ;
        }
        
        //Back Button
        else
        {
            theta = PI * 2.0f * 0.4f ;
            nodes[i].setup( ofToDataPath("back_button_assets/icons_small_back.png") , 
                            ofToDataPath("back_button_assets/module_small_green.png") ,
                           i , XML.getValue ( "tier1DetailFolder" , "nought" )+"/"+DIR.getName(i) ) ;
        }
        
        ofPoint endPos = ofPoint ( cos( theta ) * carouselRadius , sin ( theta ) * carouselRadius ) ;
        nodes[i].endPoint = endPos ;
        nodes[i].setPos( nodes[i].x , nodes[i].y ) ;
        nodes[i].angle = theta ;
        nodes[i].angleLength = carouselRadius ;
        nodes[i].rotation = theta ;
        
        hitTestHub->addItem( &nodes[i] ) ; 
    }
    touchResetDelay = 5.0f ;
    
    float min = 0.1f ;
    float max = 0.175f ;
    int xDir = ofRandom ( 0 , 2 ) ;
    int yDir = ofRandom ( 0 , 2 ) ;
    velocity.x = ( xDir == 0 ) ? ofRandom ( -min , -max ) : ofRandom ( min , max ) ;
    velocity.y = ( yDir == 0 ) ? ofRandom ( -min , -max ) : ofRandom ( min , max ) ;
    
    //Setup global event listener for if a child object gets selected
    ofAddListener(PixelEvent::Instance()->pixelDownEvent , this , &ContentItem::hexColorHandler ) ;
}

void ContentItem::setRandomAngles ( float minAngle , float maxAngle , int num  )
{
    randomAngles = new float[num] ;
    for ( int i = 0 ; i < num ; i++ )
    {
        randomAngles[i] = ofRandom ( minAngle , maxAngle ) ;
    }
}

void ContentItem::transitionInTierMenu( int tier )
{
    playVideo = true ;
    playSubNode = true ;

    Tweenzor::add( &videoAlpha , 0.0f , 1.0f , 0.0f , 0.25f , EASE_OUT_QUAD ) ;

    float nScale = ( width - minSize ) / ( maxSize - minSize ) ;

    float nLength = width * radialFactor + ( -nScale * minSize * ( radialFactor * radialFactor * .95f ) );
    float transitionSpeed = 0.45f ;

    if ( tier == 1 )
    {
        minSize = bg.width * .75 ;

        if ( state == 0 || state == 3 )
        {
            playSubNode = false ;
            Tweenzor::add( &fadeImage1Scale , 0.1f , 1.0f , 0.1f , transitionSpeed , EASE_OUT_QUAD ) ;
        }
        if ( width < minSize )
        {
            nScale = ( minSize - minSize ) / ( maxSize - minSize ) ;
            nLength = minSize * radialFactor + ( -nScale * minSize * .0625f ) * radialFactor ; //width * radialFactor
        }

        //Set the max / min number of touches to trigger scaling
        int * scaleRange = new int[2] ;
        scaleRange[0] = 2 ;
        scaleRange[1] = 3 ;
        setIsScalable( true , scaleRange , 2 ) ;
        delete [] scaleRange ;


        nLength = maxSize * radialFactor + ( -maxSize * .0625f ) * radialFactor ; //width * radialFactor ;
        inTransition = true ;
        for ( int i = 0 ; i < nItems ; i++ )
        {
            float combinedAngle = rotation + nodes[i].angle ;
            nodes[i].angleLength = nLength ; //width * radialFactor ;
            float radius = nodes[i].angleLength ;
            ofPoint newEndPoint ( cos ( nodes[i].angle ) * radius , sin ( nodes[i].angle ) * radius ) ;
            nodes[i].endPoint = newEndPoint ;

            float delay = (float)i * .06f ;
            Tweenzor::add( &nodes[i].x , 0, nodes[i].endPoint.x, delay, transitionSpeed, EASE_OUT_QUAD );
            Tweenzor::add( &nodes[i].y , 0, nodes[i].endPoint.y, delay, transitionSpeed, EASE_OUT_QUAD );
        }
        if (   Tweenzor::getTween( &nodes[nItems-1].y ) != NULL )
            Tweenzor::removeCompleteListener( Tweenzor::getTween( &nodes[nItems-1].y ) ) ; 
        
        Tweenzor::add( &bgScale , bgScale , 1.0f , (nItems-1)*.02f , transitionSpeed , EASE_OUT_QUAD ) ;

        if ( width < maxSize )
        {
            Tweenzor::add( &width , width , maxSize , 0.0f , transitionSpeed , EASE_OUT_QUAD ) ;
            Tweenzor::add( &height , height , maxSize , 0.0f , transitionSpeed , EASE_OUT_QUAD ) ;
        }
    }
}

void ContentItem::transitionTier1Complete ( float * args )
{
    
    if ( Tweenzor::getTween( &nodes[nItems-1].y ) != NULL )
        Tweenzor::removeCompleteListener( Tweenzor::getTween( &nodes[nItems-1].y ) ) ; 
    
    transitionComplete( args ) ;
}

void ContentItem::transitionComplete( float * args )
{
    inTransition = false ;
    playVideo = false ;
    playSubNode = false ;
    cout << "@TRANSITION COMPLETE : menuIndex = " << menuIndex << " newState " << nextState << " STATE = " << state << endl ;

    if ( nextState > -1 )
        state = nextState ;

    nextState = -4 ;
}

void ContentItem::transitionOutComplete ( float * args )
{
    playVideo = false ;
    playSubNode = false ;
}

void ContentItem::hexColorHandler ( const void * sender , PixelEventArgs &args ) 
{ 
    int inputHex = args.inputHex ; 
    //If the color is on the main content
    if ( inputHex == hexInputID ) 
    {
        cout << "Main Node was hit" << endl ; 
        //Toggle off all other buttons
        if ( state == 0 || state == 3 )
        {
            changeState ( 1 ) ;
            lastTouchTime = ofGetElapsedTimef() ; 
            TransitionEventArgs args = TransitionEventArgs ( menuIndex , ofVec2f ( x , y ) , false ) ; 
            ofNotifyEvent(PixelEvent::Instance()->playTransitionMovieEvent , args , this ) ; 
        }
    }
    else
    {
        for ( int i = 0 ; i < nItems ; i++ )
        {
            if ( inputHex == nodes[i].hexInputID ) 
            {
                if ( state != 0 && state != 3 )
                    lastTouchTime = ofGetElapsedTimef() ;
                
                TransitionEventArgs args = TransitionEventArgs ( menuIndex , ofVec2f ( nodes[i].stagePos.x , nodes[i].stagePos.y ) , true ) ; 
                ofNotifyEvent(PixelEvent::Instance()->playTransitionMovieEvent , args , this ) ; 
                
                //Back Button was selected
                if ( i == nItems-1 )
                {
                    cout << "Back button was selected!" << endl ; 
                    nextState = 0 ;
                    tier1Node = i ;
                    timeOut() ;
                    playSubNode = true ;
                }
                //An image was selected
                else
                {
                    cout << "Image button selected!" << endl ; 
                    int lastIndex = tier1Node ; 
                    tier1Node = i ;
                    if ( lastIndex < 0 ) 
                        crossFade.fadeImages ( &nodes[i].centerDetail ,  &image ) ;
                    else
                        crossFade.fadeImages ( &nodes[i].centerDetail , &nodes[lastIndex].centerDetail ) ; 
                }

                //If a valid node was selected
                if ( tier1Node > -2 )
                {
                    Tweenzor::add( &videoAlpha , 0.0f , 1.0f , 0.0f , 0.25f , EASE_OUT_QUAD ) ;
                    playSubNode = true ;
                    playVideo = true ;
                    nodes[i].bounceSelectedEffect( .55f ) ;
                }
            }
        }
    }
} 

void ContentItem::drawInputMap() 
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(180.0f*rotation/PI, 0, 0, 1);

       
    for ( int i = 0 ; i < nItems ; i++ ) 
    {
        nodes[i].renderInputMap() ; 
    }
    
    ofSetHexColor( hexInputID ) ;
    ofCircle( 0 , 0 , image.width * scale * .5 ) ; 

    
    glPopMatrix();
}

void ContentItem::transitionOutTierMenu( int tier )
{
    cout << "transitionOut tier" << tier << endl ;

    float transitionSpeed = .25f ;
    if ( tier == 1 )
    {
        inTransition = true ;
      //  tier1Node = -2 ;
        for ( int i = 0 ; i < nItems ; i++ )
        {
            float delay = (float) i * .04f + transitionSpeed ;
            Tweenzor::add( &nodes[i].x , nodes[i].endPoint.x , 0, delay, transitionSpeed, EASE_IN_QUAD );
            Tweenzor::add( &nodes[i].y , nodes[i].endPoint.y , 0, delay, transitionSpeed, EASE_IN_QUAD );
        }
        
        Tweenzor::addCompleteListener(  Tweenzor::getTween( &nodes[nItems-1].y ) , this, &ContentItem::transitionOutComplete ) ; 
    }
}

void ContentItem::linesFadeComplete ( float * args )
{
    drawLines = false ;
}

void ContentItem::timeOut()
{
    //cout << "# " << menuIndex << " @ time out!" << endl ;
    inTransition = true ;

    drawLines = true ;
    linesAlpha = 0.0f ;
    if ( Tweenzor::getTween( &linesAlpha ) != NULL )
    {
        Tweenzor::removeCompleteListener(Tweenzor::getTween( &linesAlpha )) ; 
    }

    Tweenzor::add( &linesAlpha , linesAlpha , 1.0f , 0.0f , 1.0f , EASE_IN_QUAD ) ;

    float transOutSpeed  = 0.35f ;

    if ( state == 1 )
    {
        for ( int i = 0 ; i < nItems ; i++ )
        {
            float delay = (float)i * .04f ;
            Tweenzor::add( &nodes[i].x , nodes[i].endPoint.x , 0, delay, transOutSpeed, EASE_IN_QUAD );
            Tweenzor::add( &nodes[i].y , nodes[i].endPoint.y , 0, delay, transOutSpeed, EASE_IN_QUAD );
        }
        Tweenzor::addCompleteListener( Tweenzor::getTween( &nodes[nItems-1].y ) ,  this, &ContentItem::timeOutTransitionComplete ) ; 
        Tweenzor::add( &fadeImage1Scale , fadeImage1Scale , 0.25f , transOutSpeed , transOutSpeed , EASE_IN_QUAD ) ;
        crossFade.fadeImages ( &starterImage , &image , transOutSpeed ) ; 
    }

    Tweenzor::add( &videoAlpha , 0.0f , 1.0f , 0.0f , 0.25f , EASE_OUT_QUAD ) ;
    playVideo = true ;

    tier1Node = -4 ;

    Tweenzor::add( &minSize , minSize , starterImage.width , 0.0f , 0.75f , EASE_IN_QUAD ) ;
    Tweenzor::add( &bgScale , bgScale , 0.5f , 0.0f , 0.35f , EASE_IN_QUAD ) ;
    setIsScalable( false ) ;
}

void ContentItem::timeOutTransitionComplete( float * args)
{
    transitionComplete ( new float(-10.0f) ) ;
    playSubNode = false ;
    state = 0 ;

}

void ContentItem::setDebug ( bool d )
{
    debugActive = d ;
    if ( state == 1 )
    {
        for ( int i = 0 ; i < nItems ; i++ )
        {
            nodes[i].debug = d ;
        }
    }
};


void ContentItem::checkBounds()
{
    float _w = width * 0.425f ;
    if ( x < _w || x > ofGetWidth()+- _w || y < _w || y > ofGetHeight()+- _w )
    {

        if ( inBoundsTransition == false )
        {
            cout << "Content Item bouncing off of the edge!" << endl ;
            if ( inFlickTransition == true )
            {
                if ( Tweenzor::getTween( &y ) != NULL )
                    Tweenzor::removeCompleteListener ( Tweenzor::getTween( &y ) ) ; 
            }
            inBoundsTransition = true ;

            setIsDraggable ( false ) ;
            ofPoint pos = ofPoint ( x , y ) ;
            ofPoint center = ofPoint ( ofGetWidth() / 2 , ofGetHeight() /2 ) ;
            ofPoint desiredPosition = center +- pos ; //* .2f ;
            desiredPosition  = pos + desiredPosition.limit ( maxSize * 0.5f ) ;

            Tweenzor::add( &x , x , desiredPosition.x , 0.0f , .75f , EASE_OUT_QUAD ) ;
            Tweenzor::add( &y , y , desiredPosition.y , 0.0f , .75f , EASE_OUT_QUAD ) ;
            if ( Tweenzor::getTween( &y ) != NULL )
                Tweenzor::removeCompleteListener( Tweenzor::getTween( &y ) ) ; 
            Tweenzor::addCompleteListener( Tweenzor::getTween( &y ) , this , &ContentItem::boundsTransitionComplete ) ; 
        }
    }
}

void ContentItem::boundsTransitionComplete ( float * args )
{
    int * dragRange = new int[2];
    dragRange[0] = 1;
    dragRange[1] = -4; //negative means the abs value and up
    setIsDraggable ( true , dragRange , 2 ) ;

    delete [] dragRange ;

    inBoundsTransition = false ;
}

void ContentItem::flickTransitionComplete ( float * args )
{
     int * dragRange = new int[2];
    dragRange[0] = 1;
    dragRange[1] = -4; //negative means the abs value and up
    setIsDraggable ( true , dragRange , 2 ) ;

    delete [] dragRange ;

      inFlickTransition = false ;

}
void ContentItem::onUpdate()
{
    if ( state != 3 )
        checkBounds() ;
    if ( state != 0 && state != 3 )
    {
        Tweenzor::add( &scale , scale , width/maxSize , 0.0f , 0.05f , EASE_OUT_QUAD ) ;
    }
    else
        Tweenzor::add( &scale , scale , 1.0f , 0.0f , 0.15f , EASE_OUT_QUAD ) ;

    //This is not getting reset
//    if ( inTransition == true )
//        return ;

    //normalized scale 0.0 <-> 1.0
    float nScale = ( width - minSize ) / ( maxSize - minSize ) ;
    float nLength = width * radialFactor + ( -nScale * width * .0625f ) * radialFactor ;
    if ( state == 1 )
    {
        for ( int i = 0 ; i < nItems ; i++ )
        {
            float combinedAngle = rotation + nodes[i].angle ;
            nodes[i].angleLength = nLength ;

            float radius = nodes[i].angleLength ;
            ofPoint newEndPoint ( cos ( nodes[i].angle ) * radius , sin ( nodes[i].angle ) * radius ) ;
            ofPoint newStagePos ( x + cos ( combinedAngle ) * radius , y + sin ( combinedAngle ) * radius ) ;
            nodes[i].stagePos = newStagePos ;
            nodes[i].endPoint = newEndPoint ;

            if ( inTransition == false )
            {
                if ( Tweenzor::getTween( &nodes[i].y ) != NULL )
                    Tweenzor::removeCompleteListener ( Tweenzor::getTween( &nodes[i].y ) ) ; 
                  //  Tweenzor::getTween( &nodes[i].y )->removeListener( Tween::COMPLETE ) ;
                Tweenzor::add( &nodes[i].x , nodes[i].x , newEndPoint.x , 0.0f , 0.1f , EASE_OUT_QUAD ) ;
                Tweenzor::add( &nodes[i].y , nodes[i].y , newEndPoint.y , 0.0f , 0.1f , EASE_OUT_QUAD ) ;
            }
        }
    }
    if ( state == 3 )
    {
        x += velocity.x ;
        y += velocity.y ;
    }
}

void ContentItem::drawContent()
{
    if ( lastTouchTime > 0 && ofGetElapsedTimef() > lastTouchTime + touchResetDelay )
    {
        if ( state == 1 )
        {
            if ( nextState != 0 )
            {
                lastTouchTime = -2.0f ;
                nextState = 0 ;
                timeOut() ;
            }
        }
    }

    ofEnableAlphaBlending () ;
    ofSetColor ( 255 , 255 , 255 , 255 ) ;
    ofSetLineWidth( 3 ) ;
    if ( state == 1 )
    {
        for ( int i = 0 ; i < nItems ; i++ )
        {
            ofSetColor ( 255 , 255 , 255 , 180 ) ;
            for ( int k = 0 ; k < 5 ; k++ )
            {
                ofLine( nodes[i].fillaments[k].x , nodes[i].fillaments[k].y , nodes[i].x , nodes[i].y ) ;
            }
            ofSetColor ( 255 , 255 , 255 ) ;
            nodes[i].render( ) ;
        }
    }

    ofSetColor ( 255 , 255 , 255 ) ;

    ofPushMatrix() ; 
        ofScale ( bgScale , bgScale , 1 ) ; 
        bg.draw ( -width * 0.5f , -height * 0.5f , width , height ) ;
    ofPopMatrix() ; 
    ofSetHexColor( itemColor ) ;

    if ( tier1Node < 0 ) 
        image.draw( -image.width * scale  * 0.5f , -image.height * scale * 0.5f , image.width * scale , image.height * scale ) ;

    ofPushMatrix() ; 
        ofScale ( scale , scale , 1 ) ; 
        crossFade.draw() ; 
    ofPopMatrix() ; 

    if ( debugActive == true )
    {
        ofSetColor ( 255 , 255 , 255 ) ;
        ofDrawBitmapString ( "STATE: " + ofToString( state ) , -width * .45 , -width * .45 ) ;
        ofDrawBitmapString ( "inTransition: " + ofToString( inTransition)  , -width * .45 , -width * .35 ) ;
        ofDrawBitmapString ( "Index #: " + ofToString( menuIndex )  , -width * .45 , -width * .25 ) ;
        ofDrawBitmapString ( "autoTrans: " + ofToString( autoTransitionFading )  , -width * .45 , -width * .05 ) ;
    }
}

void ContentItem::startFlickTween( )
{
    Tweenzor::add( &tweenValue , tweenValue , 1.0f , 0.0f , 0.25f , EASE_OUT_QUAD ) ;
    Tweenzor::addCompleteListener(  Tweenzor::getTween( &tweenValue ) , this , &ContentItem::onFlickTweenComplete ) ;

}

void ContentItem::onFlickTweenComplete( float * args )
{
    inFlickTransition = false ;
    cout << "FLICK IS FINISHED!" << endl ;
}

float ContentItem::getMaxSize()
{
    return maxSize ;
}
