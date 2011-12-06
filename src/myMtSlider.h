#ifndef _MYMTSLIDER
#define _MYMTSLIDER

#include "ofMain.h"
#include "ofxMtActionsObjectEvents.h"

class myMtSlider : public ofxMtActionsObjectEvents{

public:

	int rLevel, gLevel, bLevel;

	myMtSlider(){
		stopActionOnRollOut = false;
	}

	int getZIndex() {
		return getSessionID();
	}

	void setColor(int _r, int _g, int _b) {
		rLevel = _r;
		gLevel = _g;
		bLevel = _b;
	}

	void drawContent() {
		ofSetColor(rLevel, gLevel, bLevel);
		ofRect(0, 0, width, height);
	}

	void onMultiTouchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){
		ofNotifyEvent(eMultiTouchMoved, voidEventArgs, this);
	};
};

#endif
