#ifndef _CUSTOMEVENTBUTTON
#define _CUSTOMEVENTBUTTON

#include "ofMain.h"
#include "ofxMtObjectEvents.h"

class customEventButton : public ofxMtObjectEvents{
	
public:
    
	customEventButton(){
		col.r = .3;
		col.g = .3;
		col.b = .3;
	}
	void onMultiTouchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		col.r = ofRandom(0,1);
		col.g = ofRandom(0,1);
	    col.b = ofRandom(0,1);
	};
	void onMultiTouchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){};
	void onMultiTouchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		col.r = .3;
		col.g = .3;
	    col.b = .3;
		ofNotifyEvent(eMultiTouchUp, voidEventArgs, this);
	};
	virtual void onMultiTouchRollOut(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		col.r = .3;
		col.g = .3;
	    col.b = .3;
	};
	virtual void onMultiTouchRollOver(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		col.r = ofRandom(0,1);
		col.g = ofRandom(0,1);
		col.b = ofRandom(0,1);
	};
	void render(){
		glColor3f(col.r, col.g, col.b);
		ofRect(x, y, width, height);
		//glColor3f(1,1,1);
		//ofDrawBitmapString("Bg Color", x, y+height+15);
	};
	
protected:
	ofColor col;
};

#endif
