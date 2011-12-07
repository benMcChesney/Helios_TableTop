#ifndef NODE_BUTTON
#define NODE_BUTTON

#include "ofMain.h"
#include "ofxMtObject.h"
#include "Tweenzor.h"
#include "CorePixelHitTest.h"
#include "PixelEvent.h"

class NodeButton : public ofxMtObject , public CorePixelHitTest {
	
public:
    
    bool debug ; 
    ofImage * image ;            //visual image
    ofImage bg ; 
    float rotation ;           //rotation of image to face center
    ofPoint endPoint ;         //end point of carousel relative to center
    ofPoint stagePos ;         //position on the stage 
    
    float angle ;               //angle offset from center
    float angleLength ;         //amount offset radially from the center
    int nodeIndex ; 
    
    ofImage centerDetail ; 
    ofImage introImage ; 
    
    string folderPath ; 
    string slideshowPath ; 
    
    ofPoint * fillaments ; 
    
    float scale ;                   //size
    
    int hexInputID ;                //for ofxPixelHitTest
    
    NodeButton() {

    }
    
    
    //Custom Event Handler
    void hexColorHandler ( const void * sender , PixelEventArgs &args ) 
    {
        if ( args.inputHex == hexInputID ) 
        {
            cout << "Match for Hex color Handler found inside NodeButton!" << endl ; 
        }
    }
    
    void setup( string path , string bgPath ,  int _nodeIndex , string centerPath , string _folderPath, string _slideshowPath , string introPath )
    {
        bounceTime = 0.0f ; 
        scale = 1.0f ; 
        image = new ofImage() ; 
        image->loadImage( path ) ; 
        bg.loadImage ( bgPath ) ; 
        introImage.loadImage ( introPath ) ; 
        
        bg.setAnchorPercent( 0.5f , 0.5f ) ; 
        image->setAnchorPercent( 0.5f , 0.5f ) ; 
        
        fillaments = new ofPoint[5] ; 
        float angleStep = PI * 2 / 5.0f ;
        float radius = 25.0f ;  ; 
            
        for ( int i = 0 ; i < 5 ; i++ ) 
        {
            float theta = angleStep * i ;
            fillaments[i] = ofPoint ( cos( theta ) * radius  , sin ( theta ) * radius ) ; 
        }
        
        centerDetail.loadImage( centerPath ) ; 
        setSize( image->width , image->height ) ; 
        nodeIndex = _nodeIndex ; 
        folderPath = _folderPath ; 
        slideshowPath = _slideshowPath ; 

		col.r = 1 ; 
		col.g = 1 ; 
	    col.b = 1 ; 
        
        debug = false ; 
        
        //Setup global event listener for if a child object gets selected
        ofAddListener( PixelEvent::Instance()->pixelDownEvent , this , &NodeButton::hexColorHandler ) ;
    }
    
    void renderInputMap() 
    {
        ofSetHexColor ( hexInputID ) ; 
        ofCircle ( x , y , bg.width * scale * 0.30f ) ; 
    }
    
    float bounceTime ; 
    void bounceSelectedEffect( float time ) 
    {
        bounceTime = time * .75f ; 
        Tweenzor::add( &scale , scale , 1.25f , 0.0f , time * 0.25f , EASE_IN_QUAD ) ; 
        Tweenzor::addCompleteListener( Tweenzor::getTween( &scale ), this, &NodeButton::bounceEffectComplete );
    }
    
    void bounceEffectComplete ( float * args ) 
    {
        Tweenzor::add( &scale , scale , 1.0f , 0.0f , bounceTime , EASE_OUT_ELASTIC ) ;
    }
    
	void render()
    {
        ofEnableAlphaBlending() ; 
        glColor4f(col.r,col.g,col.b,1) ; 
        glPushMatrix() ; 
        glTranslatef(x, y, 0);
        bg.draw( 0 , 0 , bg.width * scale , bg.height * scale ) ;
        image->draw ( 0, 0 , width * scale , height * scale ) ; 
        
        glPopMatrix() ; 
        
        //Debug stage position text
        if ( debug == true ) 
        {
            ofSetColor ( 255 , 255 , 255 ) ; 
            ofDrawBitmapString( "folderPath: " + folderPath , x + -125 , y - 55 ) ; 
            ofDrawBitmapString( "centerPath: " + slideshowPath , x + -125 , y  -25 ) ; 
        }
    };
	
protected:
	ofColor col;
};

#endif
