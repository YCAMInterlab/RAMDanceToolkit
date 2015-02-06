#pragma once

class ofxUIToggleMini : public ofxUIToggle
{
public:
    ofxUIToggleMini(string _name, bool *_value, float w, float h) : ofxUIToggle()
    {
        useReference = true;                 
        rect = new ofxUIRectangle(0,0,w,h); 
        init(_name, _value, w, h);
    }
	
    void init(string _name, bool *_value, float w, float h)
    {
        name = string(_name);
		kind = OFX_UI_WIDGET_TOGGLE;
        
		paddedRect = new ofxUIRectangle(-padding, -padding, w+padding*2.0, h+padding*2.0);
		paddedRect->setParent(rect);
        
        label = new ofxUILabel(w+padding,0, (name+" LABEL"), name, OFX_UI_FONT_MEDIUM);
		label->setParent(label);
		label->setRectParent(rect);
        label->setEmbedded(true);
        drawLabel = false;
        label->setVisible(drawLabel);
        
        if(useReference)
        {
            value = _value;
        }
        else
        {
            value = new bool();
            *value = *_value;
        }
        
        setValue(*_value);
    }
    
    virtual ~ofxUIToggleMini()
    {
    }
    
    virtual void drawBack()
    {
        if(draw_back && !draw_fill)
        {
            ofFill();
            ofSetColor(color_back);
            ofRect(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
        }
    }
    
    virtual void drawFill()
    {
        if(draw_fill)
        {
            ofFill();
            ofSetColor(color_fill);
            ofRect(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
        }
    }
    
    virtual void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
            ofFill();
            ofSetColor(color_fill_highlight);
            ofRect(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
        }
    }
    
    virtual void drawOutlineHighlight()
    {
        if(draw_outline_highlight)
        {
            ofNoFill();
            ofSetColor(color_outline_highlight);
            ofRect(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
        }
    }
}; 
