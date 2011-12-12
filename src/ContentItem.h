//
//  ContentItem.h
//  ofxTuio
//
//  Created by Ben McChesney on 7/15/11.
//  Copyright 2011 Helios Interactive. All rights reserved.
//  Use as needed
//

#ifndef CONTENT_ITEM
#define CONTENT_ITEM

#include "myMtRotatableScalableItem.h"
#include "ofxDirList.h"
#include "NodeButton.h"
#include "ofMain.h"
#include "Tweenzor.h"
#include "ofxXmlSettings.h"
#include "PixelHitTestHub.h"
#include "CorePixelHitTest.h"
#include "PixelEvent.h"
#include "CrossFadeImage.h"

//This class is the 5 main circles that float around on the main screen
//It extends smallfly's awesome example

class ContentItem : public myMtRotatableScalableItem , public CorePixelHitTest
{

    public :

        int state ;                     //Dictates Behaivor
        int nextState ;                 //Stores state for after transition

        float scale ;                   //size of main Image

        int hexInputID ;                //for ofxPixelHitTest
        float bgScale ;                 //size of background Image
        int menuIndex ;                 //which menu index ? For directory loading

        float linesAlpha ;              //0-1
        bool drawLines ;

        bool debugActive ;
    
        bool playSubNode ;              //play video behind subnode or behind main app
        float videoAlpha ;              //fade in  / out for transitions
        ofVec2f videoPosition ;         //Where to play the video

        // we will have a dynaimic number of nodes, based on the content of a directory:
        int 		  nItems;
        int           tier1Node ;           //current content tier1 selected
        NodeButton	* nodes ;          //Array of nodeButtons ( TIER1 )
        ofxDirList    DIR;                  //Directory list to load files

        string node_bgPath ;
        ofPoint velocity ;

        //Touch Delay Timer Vars
        float lastTouchTime ;
        float touchResetDelay ;

        //Radius of the carousel
        float carouselRadius ;

        CrossFadeImage crossFade ;

        //Assets
        ofImage starterImage ;
        ofImage image ;                 //icon
        ofImage bg ;                    //color background

        float tweenValue ;
        bool inTransition ;
        bool playVideo ;
        float buttonYOffset ;

        ofxXmlSettings  XML ;
        float radialFactor ;

        int menuNodeDrawTo ;
        int randomNodeDrawTo ;

        float fadeImage1Scale ;

        float * randomAngles ;
        ofRectangle bounds ;

        bool inBoundsTransition ;
        bool inFlickTransition ;
        float flickTweenValue ;
    
        //PixelHitTest
        ofPtr<PixelHitTestHub> hitTestHub ; 

        ContentItem()
        {
            crossFade = CrossFadeImage() ; 
            state = 0 ;
            touchResetDelay = 5.0f ;
            inTransition = false ;
            scale = 1.0f ;
            inBoundsTransition= false ;
            inFlickTransition = false ;
        };

        ContentItem( string filePath , string bgPath , string _node_bgPath , ofxVec2f pos , int priorityLevel, int _menuIndex , string categoryPath , ofxXmlSettings _xml , ofPtr<PixelHitTestHub> _hitTestHub )
        {

            //Initialized Variables
            inBoundsTransition = false ;
            inFlickTransition = false ;
            fadeImage1Scale = 1.0f ;
            autoTransitionFading = false ;
            playSubNode = false ;
            videoAlpha = 0.0f ;
            radialFactor = .675f ;
            lastTouchTime = -2  ;
            linesAlpha = 1.0f ;
            playVideo = false ;
            debugActive = false ; 
            
            tier1Node = -2 ;

            scale = 1.0f ; 
            bgScale = 1.0f ; 
            hitTestHub = _hitTestHub ; 
            menuIndex = _menuIndex ; 
            XML = _xml ;
            node_bgPath = _node_bgPath ;
            
            crossFade = CrossFadeImage() ; 
            setColor ( 0xFFFFFF ) ;
         
            //Load some images
            starterImage.loadImage ( filePath ) ; 
            image.loadImage ( filePath ) ;  
            
            bg.loadImage( bgPath ) ;
            bg.setAnchorPercent( 0.5 , 0.5 ) ; 
            
            setPosAndSize ( pos.x , pos.y ,  starterImage.width  , starterImage.height ) ;
                
            nItems = DIR.listDir(categoryPath);
        };

    
        //Custom Event Handler
        void hexColorHandler ( const void * sender , PixelEventArgs &args )  ;
    
        void drawInputMap() ; 
        void setRandomAngles ( float minAngle , float maxAngle , int num  );
        void setup() ;

        void changeState ( int newState ) ;
        void transitionInTierMenu( int tier ) ;
        void transitionOutTierMenu( int tier ) ;
        void transitionOutComplete ( float * args ) ;

        void drawContent() ;
        void onUpdate() ;
        

        void fadeImages ( ofImage * image1 , ofImage * image2 , float duration = 0.5f , float delay = 0.0f , bool tier2Fade = false ) ;
        void fadeComplete ( float * args ) ;

        void transitionComplete ( float * args ) ;
        void transitionTier1Complete ( float * args ) ;

        void timeOut() ;
        void timeOutTransitionComplete( float * args ) ;

        void linesFadeComplete ( float * args ) ;

        void setDebug( bool _d ) ;

        void delayIncrement ( float * args ) ;
        bool autoTransitionFading ;

        float getRotation() { return rotation ; }
        void setRotation( float angle ) { rotation = angle ; }

        float getMaxSize() ;
        void checkBounds() ;
    
        void boundsTransitionComplete ( float * args ) ;

        void flickTransitionComplete ( float * args ) ;
        void startFlickTween( ) ;
        void onFlickTweenComplete ( float * args ) ;



};

#endif
