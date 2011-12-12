#ifndef _MYMTROTATABLESCALABLEITEM
#define _MYMTROTATABLESCALABLEITEM

#include "ofMain.h"
#include "ofxMtActionsObject.h"

class myMtRotatableScalableItem : public ofxMtActionsObject{

public:
	myMtRotatableScalableItem(){
		//franklinBook.loadFont("frabk.ttf", 26);
		rotation = ofRandom(1.0f, PI*2.0f);
	}

	int getZIndex() {
		return getSessionID();
	}

	void setColor(int hexColor) {
		itemColor = hexColor;
	}

	void drawContent() {
		ofSetHexColor(itemColor);
		ofRect(-width/2, -height/2, width, height);

		ofSetHexColor(0x000000);
		int yOffset = -height/2 + 20;
		if(draggable) {
			franklinBook.drawString("drag me", -width/2,yOffset);
			yOffset += 30;
		}

		if(rotatable) {
			franklinBook.drawString("rotate me", -width/2,yOffset);
			yOffset += 30;
		}

		if(scalable) {
			franklinBook.drawString("scale me", -width/2,yOffset);
			yOffset += 30;
		}

		char tempString[255];
		sprintf(tempString,"priority:%i", priorityLevel);
		franklinBook.drawString(tempString, -width/2,yOffset);

	}

	int itemColor;
	ofTrueTypeFont  franklinBook;
};

#endif
