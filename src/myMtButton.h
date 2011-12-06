#ifndef _MYMTBUTTON
#define _MYMTBUTTON

#include "ofMain.h"
#include "ofxMtObject.h"

class myMtButton : public ofxMtObject{
	
public:
	myMtButton(){
		col.r = 0;
		col.g = 0;
		col.b = 0;
	}
	
	void onMultiTouchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		col.r = ofRandom(0,1);
		col.g = ofRandom(0,1);
	    col.b = ofRandom(0,1);
	};
	void onMultiTouchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){};
	void onMultiTouchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		this->x = ofRandom(0,ofGetWidth()-width);
		this->y = ofRandom(0,ofGetHeight()-height);
		col.r = 0;
		col.g = 0;
	    col.b = 0;
	};
	virtual void onMultiTouchRollOut(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		col.r = 0;
		col.g = 0;
	    col.b = 0;
	};
	virtual void onMultiTouchRollOver(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		col.r = ofRandom(0,1);
		col.g = ofRandom(0,1);
		col.b = ofRandom(0,1);
	};
	
	void render(){
		glColor3f(col.r, col.g, col.b);
		ofRect(x,y, width, height);
		glColor3f(1,1,1);
		//ofDrawBitmapString("I am a Multitouchbutton", x, y+height+15);
	};
	
protected:
	ofColor col;
};

#endif
