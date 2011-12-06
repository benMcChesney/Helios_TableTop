//
//  CarbonCalulator.h
//  ofxTuio
//
//  Created by Ben McChesney on 8/8/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//

#ifndef CARBON_CALULATOR
#define CARBON_CALULATOR

#include "ofMain.h"
#include "ofxDirList.h"
#include "ofxMtObject.h"
#include "SlidingDoorImage.h"
#include "RadialOfImage.h"

class CarbonCalulator : public ofxMtObject {

public:


    bool debug ;

    float numUsers ;
    float cloudPercent ;
    float percSavings ;

    SlidingDoorImage barA ;
    SlidingDoorImage barB ;

    float rotation ;           //rotation of image to face center

    ofPoint stagePos ;         //position on the stage , used to check aginst click events

    int nodeIndex ;
    float maxSize ;
    float minSize ;

    RadialOfImage * userButtons ;
    RadialOfImage * platformButtons ;

    float buttonSize ;
    int numTicks ;

    ofTrueTypeFont * vaground ;
    ofTrueTypeFont * vagroundBig ;

    int barAIndex , barBIndex ;


    CarbonCalulator() { }

    CarbonCalulator( float _maxSize )
    {
        maxSize = _maxSize ;
        width = maxSize ;
        numTicks = 3 ;


        float * newValues = new float[3] ;
        newValues = lookUpCarbonCalcTable( 0 , 0 ) ;
        percSavings = newValues[2] ;
        percSavings = .97f ;
        barA = SlidingDoorImage( 20, -15, 45.0f , 28.0f, 115.0f, 271.0f , VERTICAL ) ;
        barB = SlidingDoorImage( 94 , -15, 45.0f , 128.0f, 115.0f, 271.0f , VERTICAL ) ;

        barA.size = ( newValues[0] / 271.0f ) * barA.maxSize ;
        barA.currentValue = newValues[0] ;

        barA.color = ( 0x0b7acd ) ;
        barB.size = 4.0f ; //( newValues[1] / 271.0f ) * barB.maxSize ;
        barB.currentValue = newValues[1] ;

       // barA.loadImages( ofToDataPath("barGraph/start.png") , ofToDataPath("barGraph/stretch.png") , ofToDataPath("barGraph/end.png") ) ;
       // barB.loadImages( ofToDataPath("barGraph/start.png") , ofToDataPath("barGraph/stretch.png") , ofToDataPath("barGraph/end.png") ) ;        //barA.width = 35 ;
        barB.color = ( 0x8cba00 ) ;

        userButtons = new RadialOfImage[numTicks] ;
        platformButtons = new RadialOfImage[numTicks] ;

        barAIndex = 0 ;
        barBIndex = 0 ;

        for ( int i = 0 ; i < numTicks ; i++ )
        {
            userButtons[i].image_out.loadImage ( ofToDataPath("barGraph/graph_off.png") ) ;
            userButtons[i].image_over.loadImage ( ofToDataPath("barGraph/graph_on.png") ) ;
            platformButtons[i].image_out.loadImage ( ofToDataPath("barGraph/graph_off.png") ) ;
            platformButtons[i].image_over.loadImage ( ofToDataPath("barGraph/graph_on.png") ) ;
        }

        float nItems = (float)numTicks ;
        buttonSize = platformButtons[0].image_over.getWidth() * .35f ;
        float xSpacing = buttonSize * 6.25f ;
        int xOffset = ( -width ) + ( nItems * 0.175f ) * xSpacing +- 10 ;

        ofPoint * userButtonPos = new ofPoint[numTicks] ;
        ofPoint * platformButtonPos = new ofPoint[numTicks] ;
        for ( int i = 0 ; i < numTicks ; i++ )
        {
            userButtonPos[i] = ofPoint ( xOffset + i * xSpacing + 65 , -buttonSize * 2.2f + 10 ) ;
            platformButtonPos[i] = ofPoint ( xOffset + i * xSpacing + 65 , buttonSize * 2.2f + 95 ) ;

            userButtons[i].angle = atan2( userButtonPos[i].y , userButtonPos[i].x ) ;
            float angleDistance = ofDist ( userButtonPos[i].x , userButtonPos[i].y , 0 , 0 ) ;
            userButtons[i].angleDistance = angleDistance ; //ofDist ( userButtonPos[i].x , userButtonPos[i].y , 0 , 0 ) ;
            userButtons[i].originalDistance = angleDistance ; //
            userButtons[i].value = interpolate ( 100.0f , 10000.0f , (float)i/(nItems-1.0f) ) ;


            platformButtons[i].angle = atan2( platformButtonPos[i].y , platformButtonPos[i].x ) ;
            angleDistance = ofDist ( platformButtonPos[i].x , platformButtonPos[i].y , 0 , 0 ) ;
            platformButtons[i].angleDistance = angleDistance ;
            platformButtons[i].originalDistance = angleDistance ;
            platformButtons[i].value = interpolate ( 0.1 , 1.0f , (float)i/(nItems-1.0f) ) ;

        }

        numUsers = userButtons[0].value ;
        cloudPercent = platformButtons[0].value ;
     //    cout << " v1 : " << userButtons[0].value << " | v2 : " << platformButtons[0].value << endl;
     //   updateValues (numUsers ,cloudPercent ) ;
    //    updateValues( 0 , 0 ) ;
        userButtons[0].selected = true ;
        platformButtons[0].selected = true ;

        /* double x, y, result;
         x = -10.0;
         y = 10.0;
         result = atan2 (y,x) * 180 / PI;
         printf ("The arc tangent for (x=%lf, y=%lf) is %lf degrees\n", x, y, result );\*/
    }

    void roundedRect(float x, float y, float w, float h, float r) {
        ofBeginShape();
            ofVertex(x+r, y);
            ofVertex(x+w-r, y);
            quadraticBezierVertex(x+w, y, x+w, y+r, x+w-r, y);
            ofVertex(x+w, y+h);
//            quadraticBezierVertex(x+w, y+h, x+w-r, y+h, x+w, y+h-r);
            ofVertex(x, y+h);
//            quadraticBezierVertex(x, y+h, x, y+h-r, x+r, y+h);
            ofVertex(x, y+r);
            quadraticBezierVertex(x, y, x+r, y, x, y+r);
        ofEndShape();
    }
    void quadraticBezierVertex(float cpx, float cpy, float x, float y, float prevX, float prevY) {
        float cp1x = prevX + 2.0/3.0*(cpx - prevX);
        float cp1y = prevY + 2.0/3.0*(cpy - prevY);
        float cp2x = cp1x + (x - prevX)/3.0;
        float cp2y = cp1y + (y - prevY)/3.0;

        // finally call cubic Bezier curve function
        ofBezierVertex(cp1x, cp1y, cp2x, cp2y, x, y);
    };

    float interpolate ( float y1,float y2, float mu)
    {
        return(y1*(1-mu)+y2*mu);
    }

    void updateValues ( int barAIndex , int barBIndex )
    {
        float * newValues = new float[3] ;
        newValues = lookUpCarbonCalcTable( barAIndex , barBIndex ) ;
      //  cout << "new readout : A : " << newValues[0] << " B : " << newValues[1] << endl ;
        barA.updateSize ( newValues[0] , ( newValues[0] / 271.0f ) * barA.maxSize ) ;
        barB.updateSize ( newValues[1] , ( newValues[1] / 271.0f ) * barB.maxSize ) ;
        Tweenzor::add ( &percSavings , percSavings , newValues[2] , 0.0f , 0.45f , EASE_OUT_QUAD ) ;
//        percSavings = newValues[2] * 100 ;

    }

    void onMultiTouchDown(float _x, float _y, int touchId, ofxMultiTouchCustomData *data = NULL)
    {

        for ( int i = 0 ; i < numTicks ; i++ )
        {

            float dist = ofDist ( _x , _y , userButtons[i].stagePos.x , userButtons[i].stagePos.y ) ;
         //   cout << "dist between " << _x << "," << _y << " and " <<userButtons[i].stagePos.x << "," << userButtons[i].stagePos.y <<endl << "is : " << dist << "   buttonSize : " << buttonSize << endl ;
            if ( dist < buttonSize * 2 )
            {
                if ( userButtons[i].selected == false )
                {
                    cout << "user button " << i << " was selected! " << endl ;
                    for ( int a = 0 ; a < numTicks ; a++ )
                    {
                        if ( a != i )
                        {
                            userButtons[a].selected = false ;
                        }
                    }
                    userButtons[i].selected = true ;

                    barAIndex = i ;


                    updateValues( barAIndex, barBIndex ) ;
                    //return ;

                }
            }
        }

        for ( int k = 0 ; k < numTicks ; k++ )
        {

            float dist = ofDist ( _x , _y , platformButtons[k].stagePos.x , platformButtons[k].stagePos.y ) ;
            if ( dist < buttonSize )
            {
                cout << "platform button " << k << " was selected! " << endl ;
                if ( platformButtons[k].selected == false )
                {
                    for ( int b = 0 ; b < numTicks ; b++ )
                    {
                        if ( b != k )
                        {
                            platformButtons[b].selected = false ;
                        }
                    }
                    barBIndex = k ;
                    platformButtons[k].selected = true ;
                     updateValues( barAIndex, barBIndex ) ;

                }
            }

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
            float angle = userButtons[i].angle ;
            float combinedAngle = rotation + userButtons[i].angle ;
//            userButtons[i].angleDistance =
            float radius = userButtons[i].angleDistance * scale * factor ;


            ofPoint newPos = ofPoint ( cos ( angle ) * radius , sin ( angle ) * radius ) ;
            ofPoint newStagePos = ofPoint ( x + cos ( combinedAngle ) * radius , y + sin ( combinedAngle ) * radius ) ;
            userButtons[i].position = newPos ;
            userButtons[i].stagePos = newStagePos ;

            angle = platformButtons[i].angle ;
            combinedAngle = rotation + platformButtons[i].angle ;
            radius = platformButtons[i].angleDistance * scale * factor;
            newPos = ofPoint ( cos ( angle ) * radius , sin ( angle ) * radius ) ;
            newStagePos = ofPoint ( x + cos ( combinedAngle ) * radius , y + sin ( combinedAngle ) * radius ) ;
            platformButtons[i].position = newPos ;
            platformButtons[i].stagePos = newStagePos ;
        }
    }

    void draw()
    {

    }

    //This is a total shitshow because salesforce couldn't give us the right formula or what we asked for.
    float* lookUpCarbonCalcTable ( int barAIndex , int barBIndex )
    {
        float * lookup = new float[3] ;
        switch ( barAIndex )
        {
            case 0 :
                switch ( barBIndex )
                {
                    case 0 :
                        lookup[0] = 13.0f ;
                        lookup[1] = 0.25f ;
                        lookup[2] = 0.98f ;
                        break ;

                    case 1 :
                        lookup[0] = 6.0f ;
                        lookup[1] = 0.25f ;
                        lookup[2] = 0.96f ;
                        break ;

                    case 2 :
                        lookup[0] = 0.69f ;
                        lookup[1] = 0.25f ;
                        lookup[2] = 0.96f ;
                        break ;
                }
                break ;

            case 1 :
                switch ( barBIndex )
                {
                    case 0 :
                        lookup[0] = 134.0f ;
                        lookup[1] = 12.0f ;
                        lookup[2] = 0.90f ;
                        break ;

                    case 1 :
                        lookup[0] = 85.0f ;
                        lookup[1] = 12.00f ;
                        lookup[2] = 0.85f ;
                        break ;

                    case 2 :
                        lookup[0] = 35.0f ;
                        lookup[1] = 12.0f ;
                        lookup[2] = 0.63f ;
                        break ;
                }
                break ;

            case 2 :
                switch ( barBIndex )
                {
                    case 0 :
                        lookup[0] = 271.0f ;
                        lookup[1] = 25.0f ;
                        lookup[2] = 0.90f ;
                        break ;

                    case 1 :
                        lookup[0] = 163.0f ;
                        lookup[1] = 25.0f ;
                        lookup[2] = 0.85f ;
                        break ;

                    case 2 :
                        lookup[0] = 69.0f ;
                        lookup[1] = 25.0f ;
                        lookup[2] = 0.63f ;
                        break ;
                }
                break ;
        }

        return lookup ;
    }
	void render( float width , float height , float scale , float rotation )
    {
        scale = 1.0f ;
        width = ( width > maxSize ) ? maxSize : width ;
        float nItems = (float)numTicks ;
        ofSetColor ( 255 , 255 , 255 ) ;
        for ( int i = 0 ; i < 3 ; i++ )
        {
            userButtons[i].draw ( 0 , 0 , buttonSize ) ;
//          userButtons[i].image.draw( -width/2 + userButtons[i].position.x +-size , userButtons[i].position.y +-size , size*2 , size*2 ) ;
//          platformButtons[i].image.draw( -width/2 + platformButtons[i].position.x +-size , platformButtons[i].position.y +-size , size*2 , size*2 ) ;
            platformButtons[i].draw ( 0 , 0 , buttonSize ) ;
        }


        float _y = ( -barA.maxSize * .5 ) * scale + (y * (1.0f - scale) ) + 20  ; //* (1.0f - scale ) ;
        float sizeOffset = ( barA.maxSize +- barA.size ) ;

//      start.draw ( _x , _y + sizeOffset , width , capHeight ) ;
//      stretch.draw ( _x , _y + sizeOffset + capHeight , width , size +-  (capHeight * 2.0f )  ) ;

        ofSetHexColor ( 0x0b7acd ) ;
        roundedRect( barA.x , _y + sizeOffset + -1  , barA.width , barA.size , barA.cornerRoundness ) ;

        _y = ( -barB.maxSize * .5 ) * scale + (y * (1.0f - scale) ) + 20  ; //* (1.0f - scale ) ;
        sizeOffset = ( barB.maxSize +- barB.size ) ;

        ofSetHexColor ( 0x8cba00 ) ;
        roundedRect( barB.x , _y + sizeOffset + -1, barB.width , barB.size , barB.cornerRoundness ) ;


        ofSetHexColor ( 0x57656d ) ;
        float offsetXA = ( barA.currentValue > 99.9f ) ? barA.x +- 6 : barA.x ;
        string barAString = ( barA.currentValue < 1.0f ) ? "<1" : ofToString( (int)barA.currentValue ) ;
        string barBString = ( barB.currentValue < 1.0f ) ? "<1" : ofToString( (int)barB.currentValue ) ;

        vaground->drawString( barAString , offsetXA , -61 ) ;
        vaground->drawString( barBString , barB.x , -61 ) ;

        ofSetHexColor ( 0x8cba00 ) ;
        vagroundBig->drawString ( ofToString((int)(percSavings*100.0f)) , -77 , 101 ) ;



      };
};

#endif
