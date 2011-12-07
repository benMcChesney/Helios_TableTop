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

//This class is the 5 main circles that float around on the main screen
//It extends smallfly's awesome example

class ContentItem : public myMtRotatableScalableItem , public CorePixelHitTest
{

    public :

        int state ;                     //Dictates Behaivor
        int nextState ;                 //Stores state for after transition

        float scale ;                   //size

        int hexInputID ;                //for ofxPixelHitTest
        ofImage image ;                 //icon
        ofImage bg ;                    //color background
        float bgScale ;
        int menuIndex ;                 //which menu index ? For directory loading

        float linesAlpha ;              //0-1
        bool drawLines ;

        bool debugActive ;

        bool playVideoFlag ;            //read by testApp to play movie from the beginning
        bool playSubNode ;              //play video behind subnode or behind main app
        float videoAlpha ;              //fade in  / out for transitions

        // we will have a dynaimic number of categories, based on the content of a directory:
        int 		  nItems;
        int           tier1Node ;           //current content tier1 selected
        NodeButton	* categories ;          //Array of nodeButtons ( TIER1 )
        ofxDirList    DIR;                  //Directory list to load files

        string node_bgPath ;
        ofPoint velocity ;

        //Touch Delay Timer Vars
        float lastTouchTime ;
        float touchResetDelay ;

        //Radius of the carousel
        float carouselRadius ;

        //Image Fading Variables
        bool isFading ;
        ofImage * fadeImage1 ;
        ofImage * fadeImage2 ;
        float fadeAlpha1 ;
        float fadeAlpha2 ;
        float glowAlpha ;

        ofPoint nextStagePos , prevStagePos ;   //position of UI Images compared to the stage

        //Assets
        ofImage menuOpener ;
        ofImage starterImage ;
        float tweenValue ;
        bool inTransition ;
        bool playVideo ;
        float buttonYOffset ;

        ofxXmlSettings  XML ;

        float radialFactor ;

        ofImage selectedNodeBG ;
        ofColor hexColor ;

        int menuNodeDrawTo ;
        int randomNodeDrawTo ;

        float fadeButtonAlpha1 ;
        float fadeButtonAlpha2 ;
        float fadeImage1Scale ;

        //Interchangable backButton Image on tier2 back button
        ofImage * tier2BackImage ;


        float * randomAngles ;

        ofRectangle bounds ;

        bool playTimeOutSoundFlag ;
        bool playSelectedSoundFlag ;

        bool inBoundsTransition ;
        float slideshowFadeTime ;
        bool inFlickTransition ;
        float flickTweenValue ;
    
        //PixelHitTest
        ofPtr<PixelHitTestHub> hitTestHub ; 

        ContentItem()
        {
            state = 0 ;
            touchResetDelay = 5.0f ;
            inTransition = false ;
            scale = 1.0f ;
            inBoundsTransition= false ;
            inFlickTransition = false ;
        };

        ContentItem( string filePath , string bgPath , string _node_bgPath , ofxVec2f pos , int color , int priorityLevel, int _menuIndex , string categoryPath , ofxXmlSettings _xml , ofColor _hexColor , 
                ofPtr<PixelHitTestHub> _hitTestHub )
        {

            //Initialized Variables
            inBoundsTransition = false ;
            inFlickTransition = false ;
            playTimeOutSoundFlag = false ;
            playSelectedSoundFlag = false ;
            glowAlpha = 0.0f ;
            fadeImage1Scale = 1.0f ;
            autoTransitionFading = false ;
            playSubNode = false ;
            fadeButtonAlpha1 = 0.0f ;
            fadeButtonAlpha2 = 0.0f ;
            videoAlpha = 0.0f ;
            radialFactor = .675f ;
            hexColor = _hexColor ;
            lastTouchTime = -2  ;
            linesAlpha = 1.0f ;
            playVideo = false ;
            buttonYOffset = 55 ;
            tier1Node = -1 ;
            inTransition = false ;
            isFading = false ;
            hitTestHub = _hitTestHub ; 
            debugActive = false ; 
            setColor ( color ) ;

            menuIndex = _menuIndex ; 
            
            XML = _xml ;
            node_bgPath = _node_bgPath ;
         
            //Load some images
            menuOpener.loadImage( "sustainable_Menu.png" ) ;
            starterImage.loadImage ( filePath ) ;
            image.loadImage ( filePath ) ;
            bg.loadImage( bgPath ) ;
            
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

        void onMultiTouchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) ;
        int forwardedTouchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) ;
        void onMultiTouchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) ;
        void forwardedTouchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) ;
        void onMultiTouchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL) ;
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

        float getRotation()
        {
            return rotation ;
        }

        void setRotation( float angle )
        {
            rotation = angle ;
        }

        float getMaxSize() ;
        void checkBounds() ;
        void boundsTransitionComplete ( float * args ) ;

        void flickTransitionComplete ( float * args ) ;
        void startFlickTween( ) ;
        void onFlickTweenComplete ( float * args ) ;

        float slideshowDelayTime ;



};

#endif
