//
//  CarbonCalulator.h
//  ofxTuio
//
//  Created by Ben McChesney on 8/8/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//

#ifndef PLEDGE_CHOICES
#define PLEDGE_CHOICES

#include "ofMain.h"
#include "ofxDirList.h"
#include "ofxMtObject.h"
#include "RadialOfImage.h"

class PledgeChoices : public ofxMtObject {
	
public:
    
    
    bool debug ; 
    ofImage image ;            //visual image
       
    float rotation ;           //rotation of image to face center
    
    ofPoint stagePos ;         //position on the stage , used to check aginst click events
    
    int nodeIndex ; 
    float maxSize ;
    float minSize ; 
    
    RadialOfImage * pledges ; 
    int * totals ; 
    float buttonSize ; 
    int numTicks ;  
    ofColor color ; 
    
    PledgeChoices() { }
    
    PledgeChoices( float _maxSize , int numItems ) 
    { 
        maxSize = _maxSize ; 
        width = maxSize ; 
        numTicks = numItems ; 
        
        pledges = new RadialOfImage[numTicks] ; 

        totals = new int[numTicks] ; 
        for ( int i = 0 ; i < numTicks ; i++ )
        {
            totals[i] = 0 ; 
            pledges[i].image_out.loadImage ( ofToDataPath("pledgeAssets/checkBox_off.png") ) ; 
            pledges[i].image_over.loadImage ( ofToDataPath("pledgeAssets/checkBox_on.png") ) ;
        }
        
        float nItems = (float)numTicks ; 
        buttonSize = pledges[0].image_over.getWidth() ; 
        float ySpacing = buttonSize * 2.25f ;  
        int yOffset = ( -width ) + ( nItems * 0.25f ) * ySpacing ;
        
        ofPoint * pledgePos = new ofPoint[numTicks] ; 
        pledgePos[0] = ofPoint ( -202 , -200 ) ;
        pledgePos[1] = ofPoint ( -270 , -93 ) ;
        pledgePos[2] = ofPoint ( -376 , -26 ) ;
        pledgePos[3] = ofPoint ( -365 , 40 ) ;
        pledgePos[4] = ofPoint ( -249 , 107 ) ;
        pledgePos[5] = ofPoint ( -216 , 174 ) ;
        
        for ( int i = 0 ; i < numTicks ; i++ ) 
        {
//            pledgePos[i] = ofPoint ( -300 ,  i * ySpacing + yOffset ) ; 
            
            pledges[i].angle = atan2( pledgePos[i].y , pledgePos[i].x ) ; 
            float angleDistance = ofDist ( pledgePos[i].x , pledgePos[i].y , 0 , 0 ) ;  
            pledges[i].angleDistance = angleDistance ; //ofDist ( userButtonPos[i].x , userButtonPos[i].y , 0 , 0 ) ; 
            pledges[i].originalDistance = angleDistance ; //
        }
    }
    
    
    
    void tallyCurrentTotals() 
    {
        for ( int i = 0 ; i < numTicks ; i++ )
        {
            if ( pledges[i].selected == true ) 
                totals[i] += 1 ;  
        }
    }

    int * getValues ( ) 
    {
        return totals ; 
    }
    
    
    void onMultiTouchDown(float _x, float _y, int touchId, ofxMultiTouchCustomData *data = NULL)
    {
        
        float closestDist = 50000 ; 
        int closestIndex = -4 ; 
        for ( int i = 0 ; i < numTicks ; i++ ) 
        {
            
            float dist = ofDist ( _x , _y , pledges[i].stagePos.x , pledges[i].stagePos.y ) ; 
            //   cout << "dist between " << _x << "," << _y << " and " <<userButtons[i].stagePos.x << "," << userButtons[i].stagePos.y <<endl << "is : " << dist << "   buttonSize : " << buttonSize << endl ; 
            if ( dist < buttonSize ) 
            {
                if ( dist < closestDist ) 
                {
                    closestDist = dist ; 
                    closestIndex = i ; 
                }
            }
        }
        
        if ( closestIndex > -1 ) 
        {
            cout << "user button " << closestIndex << " was selected! " << endl ; 
            pledges[closestIndex].selected = !pledges[closestIndex].selected ; 
            //TODO : fade images here
            
        }

    };
	void onMultiTouchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
        
    };
	void onMultiTouchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
        
	};
    
    void onUpdate( float _rotation, float scale , float nScale ) 
    {
        rotation = _rotation ; 
        float nLength ;
        float factor = 0.45f ; 
        for ( int i = 0 ; i < numTicks ; i++ )
        {
            float angle = pledges[i].angle ; 
            float combinedAngle = rotation + pledges[i].angle ; 
            //            userButtons[i].angleDistance =
            float radius = pledges[i].angleDistance * scale * factor ; 
            
            ofPoint newPos = ofPoint ( cos ( angle ) * radius , sin ( angle ) * radius ) ; 
            ofPoint newStagePos = ofPoint ( x + cos ( combinedAngle ) * radius , y + sin ( combinedAngle ) * radius ) ;
            pledges[i].position = newPos ; 
            pledges[i].stagePos = newStagePos ; 
        }
    }
    
    void draw() 
    {
        
    }
    
	void render( float width , float height , float scale , float rotation )
    {
        scale = 1.0f ; 
        width = ( width > maxSize ) ? maxSize : width ; 
        float nItems = (float)numTicks ; 
        ofSetColor ( 255 , 255 , 255 ) ; 
        for ( int i = 0 ; i < numTicks ; i++ ) 
        {
            pledges[i].draw ( 0 , 0 , buttonSize ) ; 
        }
    };
};

#endif
