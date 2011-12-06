#ifndef NODE_BUTTON
#define NODE_BUTTON

#include "ofMain.h"
#include "ofxMtObject.h"
#include "Tweenzor.h"

class NodeButton : public ofxMtObject {
	
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
    
    float scale ; 
    
    
    NodeButton() {

    }
    
    //categories[i].setup( DIR.getPath(i) , i , XML.getValue( "menuItem:tier2" , "no name" , menuIndex ) , filePath ) ; 
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
       // cout << "centerPath is " << centerPath << endl ; 
       // cout << "folder path is : " << _folderPath <<endl ; 
       // cout << "intro Path is : " << introPath << endl ; 
        
        centerDetail.loadImage( centerPath ) ; 
      //  image.resize( image.width /2 , image.height /2 ) ; 
        setSize( image->width , image->height ) ; 
        nodeIndex = _nodeIndex ; 
        folderPath = _folderPath ; 
        slideshowPath = _slideshowPath ; 
       // cout << "slideshow path is : " <<slideshowPath << endl ; 
		col.r = 1 ; 
		col.g = 1 ; 
	    col.b = 1 ; 
        
        debug = false ; 
    }
    
    void onMultiTouchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){

	};
	void onMultiTouchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){

    };
	void onMultiTouchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
	};
    
    void update() 
    {
     //   cout << "updating!!" ; 
    }
    
    void draw() 
    {
        
    }
    
    float bounceTime ; 
    void bounceSelectedEffect( float time ) 
    {
        bounceTime = time * .75f ; 
        Tweenzor::add( &scale , scale , 1.25f , 0.0f , time * 0.25f , EASE_IN_QUAD ) ; 
//        Tweenzor::addCompleteListener( Tweenzor::getTween(&_x1), this, &testApp::onComplete);
        Tweenzor::addCompleteListener( Tweenzor::getTween( &scale ), this, &NodeButton::bounceEffectComplete );
        //Tweenzor::addCompleteListener( Tweenzor::getTween(&_x1), this, &testApp::onComplete);
   //     Tweenzor::getTween( &scale ) ->addListener(Tween::COMPLETE , this , &NodeButton::bounceEffectComplete ) ; 
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
      //  glRotatef(180.0f*rotation/PI, 0, 0, 1);
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
