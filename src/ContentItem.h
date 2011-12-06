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

//This class is the 5 main circles that float around on the main screen
//It extends smallfly's awesome example

class ContentItem : public myMtRotatableScalableItem
{

    public :

        int state ;                     //Dictates Behaivor
        int nextState ;                 //Stores state for after transition

        float scale ;                   //size
        int numTouches ;                //for prize counter

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

        ContentItem()
        {
            state = 0 ;
            touchResetDelay = 5.0f ;
            inTransition = false ;
            scale = 1.0f ;
            inBoundsTransition= false ;
            inFlickTransition = false ;
        };

        ContentItem( string filePath , string bgPath , string _node_bgPath , ofxVec2f pos , int color , int priorityLevel, int _menuIndex , string categoryPath , ofxXmlSettings _xml , ofColor _hexColor )
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

            //Allocate pointers :
            tier2BackImage = new ofImage() ;
            fadeImage1 = new ofImage() ;
            fadeImage2 = new ofImage() ;

            XML = _xml ;
            node_bgPath = _node_bgPath ;

            //Load some images
            menuOpener.loadImage( "sustainable_Menu.png" ) ;
            starterImage.loadImage ( filePath ) ;
            image.loadImage ( filePath ) ;
            bg.loadImage( bgPath ) ;

            bgScale = ( starterImage.width / bg.width ) * 0.95f ;

            minSize = bg.width * .75 ;
            maxSize = bg.width * 1 ;
            setPosAndSize ( pos.x , pos.y ,  starterImage.width  , starterImage.height ) ;

            float padding = bg.width * 0.5f ;
            bounds = ofRectangle ( padding , padding , ofGetWidth() +- padding , ofGetHeight() +- padding ) ;

            contentWidth = image.width ;
            contentHeight = image.height ;

            scale = 1.0f ;
            setColor ( color ) ;
            setPriorityLevel ( priorityLevel ) ;
            state = 0 ;
            nextState = -4 ;
            rotation = 0 ;
            menuIndex = _menuIndex;
            drawLines = true ;
            nItems = DIR.listDir(categoryPath);

            categories = new NodeButton[nItems] ;
            //Calculate angles between sub nodes
            float angleStep = ( PI * 2.0f ) / (float)17 ;
            float angleOffset = -angleStep ;
            float nScale = ( width - minSize ) / ( maxSize - minSize ) ;
            carouselRadius = width * radialFactor + ( -nScale * width * .0625f ) * radialFactor ; //width * radialFactor ;

            string introPath = XML.getValue( "tier1IntroFolder" , "no intros Path" ) ;
            ofxDirList introsDIR ;
            int numIntros = introsDIR.listDir( introPath ) ;

            for ( int i = 0 ; i < nItems ; i++ )
            {
                categories[i] = NodeButton( ) ;
                float theta ;

                //Category Buttons
                if ( i != nItems-1 )
                {
                    theta = angleStep * (i-1) ;
                    categories[i].setup( DIR.getPath(i) , node_bgPath , i , XML.getValue ( "tier1DetailFolder" , "nought" )+"/"+DIR.getName(i) ,
                                        XML.getValue( "menuItem:tier2" , "no name" , i ),
                                        XML.getValue( "menuItem:tier2Content" , "Nought content" , i ) ,
                                        introsDIR.getPath( i ) ) ;
                }

                //Back Button
                else
                {
                    theta = PI * 2.0f * 0.4f ;
                    categories[i].setup( ofToDataPath("back_button_assets/icons_small_leaf.png") , ofToDataPath("back_button_assets/module_small_green.png") , i , XML.getValue ( "tier1DetailFolder" , "nought" )+"/"+DIR.getName(i) ,
                                        XML.getValue( "menuItem:tier2" , "no name" , i ),
                                        XML.getValue( "menuItem:tier2Content" , "Nought content" , i ) ,
                                        introsDIR.getPath( i ) ) ;
                }

                ofPoint endPos = ofPoint ( cos( theta ) * carouselRadius , sin ( theta ) * carouselRadius ) ;
                categories[i].endPoint = endPos ;
                categories[i].setPos( categories[i].x , categories[i].y ) ;
                categories[i].angle = theta ;
                categories[i].angleLength = carouselRadius ;
                categories[i].rotation = theta ;
            }
            touchResetDelay = 5.0f ;

            float min = 0.1f ;
            float max = 0.175f ;
            int xDir = ofRandom ( 0 , 2 ) ;
            int yDir = ofRandom ( 0 , 2 ) ;
            velocity.x = ( xDir == 0 ) ? ofRandom ( -min , -max ) : ofRandom ( min , max ) ;
            velocity.y = ( yDir == 0 ) ? ofRandom ( -min , -max ) : ofRandom ( min , max ) ;
            selectedNodeBG.loadImage ( ofToDataPath ( "misc/module_small_active.png" ) ) ;
            selectedNodeBG.setAnchorPercent ( 0.5f , 0.5f ) ;
        };


        void setRandomAngles ( float minAngle , float maxAngle , int num  );

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
