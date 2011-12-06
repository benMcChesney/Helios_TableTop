//
//  SlidingDoorImage.h
//  ofxTuio
//
//  Created by Ben McChesney on 8/8/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//

#ifndef SLIDING_DOOR_IMAGE ;
#define SLIDING_DOOR_IMAGE ;


#define HORIZONTAL 19
#define VERTICAL 21

#include "ofMain.h"
#include "ofxMtObject.h"
#include "ofxTweenzor.h"

class SlidingDoorImage : public ofxMtObject {

public:

    ofImage start ;
    ofImage stretch ;
    ofImage end ;

    float width , height , x , y ;
    float size ;
    float maxSize ;
    float minSize ;

    int orientation ;
    bool imagesSet ;

    float capHeight ;

    int color ;
    float cornerRoundness ;

    float currentValue ;
    float maxValue ;

    float scaleFactor ;

    SlidingDoorImage( )
    {
        imagesSet = false ;
        cornerRoundness = 4.0f ;
        size = 4.0f ;
        minSize = 4.0f ;
        maxSize = 8.0f ;
    }


    SlidingDoorImage( float _x , float _y , float _width , float _height , float _maxSize , float _maxValue , int _orientation )
    {
        cornerRoundness = 8.0f ;
        color = 0xffffff ;
        x = _x ;
        y = _y ;
        width = _width ;
        height = _height ;
        imagesSet = false ;

        capHeight = 5.0f ;

        maxValue = _maxValue ;
        maxSize = _maxSize ;
        size = minSize ;
        minSize = 4.0f ;
        size = maxSize ;
        orientation = _orientation ;

        scaleFactor = maxSize / maxValue ;

        size = cornerRoundness * 2 ;
        currentValue = 80.0f ;



        if ( orientation == HORIZONTAL )
            size = width ;
        if ( orientation == VERTICAL )
            size = height ;
    }

    float map ( float v , float a , float b , float x = 0 , float y = 1 )
    {
        return ( v == a ) ? x : ( v - a ) * ( y - x ) / ( b - a ) + x ;
    }

    void loadImages ( string startPath , string stretchPath , string endPath )
    {
        imagesSet = true ;
        start.loadImage( startPath ) ;
        stretch.loadImage ( stretchPath ) ;
        end.loadImage ( endPath ) ;

        minSize = start.height + stretch.height + end.height ;
    }

    void updateSize( float _value , float _size )
    {
      //  float nSize = map ( _size , 0 , maxValue , minSize , maxSize ) ;

        float newSize = ( _size < minSize ) ? minSize : _size ;
        cout << "passed Size : " << _size << " | newSize : " << newSize << " | minSize is : " << minSize << endl ;
        Tweenzor::add( &currentValue , currentValue , _value , 0.0f , 0.75f , EASE_OUT_QUAD ) ;
        Tweenzor::add( &size , size , newSize , 0.0f , 0.75f , EASE_OUT_QUAD ) ;
//        size = _size ;

    }

	void onMultiTouchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){

	};
	void onMultiTouchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){

    };
	void onMultiTouchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){

	};

    void update()
    {
      //  size = (currentValue / maxValue) * maxSize ;
    }

    void draw( float scale , float rotation )
    {
        float _y = ( -maxSize * .5 ) * scale + (y * (1.0f - scale) ) + 15  ; //* (1.0f - scale ) ;
        float _x = x * ( 1.0f * scale );
        float sizeOffset = ( maxSize +- size ) ;
     /*
        if ( imagesSet == false )
        {
           if ( orientation == VERTICAL )
            {
                ofSetColor ( 0x7c2883 ) ;
                ofRect ( _x , _y + sizeOffset , width , capHeight ) ;
                ofSetColor ( 0x7c2883  * .66f ) ;
                ofRect ( _x , _y + sizeOffset + capHeight , width , height +-  (capHeight * 2.0f )  ) ;
                ofSetColor ( 0x7c2883  * .33f ) ;
                ofRect ( _x , _y + sizeOffset + height-capHeight , width , capHeight ) ;
                ofSetColor ( 255 , 255 , 255 ) ;
            }
        }
        else
        {
       */

            ofSetHexColor ( color ) ;
            start.draw ( _x , _y + sizeOffset , width , capHeight ) ;
            stretch.draw ( _x , _y + sizeOffset + capHeight , width , size +-  (capHeight * 2.0f )  ) ;
            end.draw ( _x , _y + sizeOffset + size-capHeight , width , capHeight ) ;

            /*
            ofSetColor ( 15 , 15 , 15 ) ;
            glPushMatrix() ;
            glTranslatef(x, y, 0);
            glRotatef(180.0f*rotation/PI, 0, 0, 1);
            glColor4f ( 0.5f , 0.5f , 0.5f , 1 ) ;
            ofDrawBitmapString( ofToString( (int)(size) ) , -20 , -50 ) ;

            glPopMatrix() ;
            */
//        }

    }

	void render()
    {

    };
};

#endif
