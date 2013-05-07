//
//  ofxParallaxScroll.h
//  ParallaxScrolling
//
//  Created by james KONG on 7/8/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#pragma once
//#ifndef ParallaxScrolling_StackParticle_h
//#define ParallaxScrolling_StackParticle_h
//#include "WarpGlobal.h"
//#include "StackObject.h"
//#include "MSAPerlin.h"
#include "ofMain.h"

#include "ofxTween.h"
//#include "ofxGlow.h"
//#include "ofxSimpleGuiToo.h"
#define MAX_PARTICLE 100
enum Mode
{
    MODE1,
    MODE2,
};
class Particle : public ofRectangle , public ofxTween
{
public:
    float increasement,_increasment;
    Particle()
    {
        set(0,0,0,0);
        currentIndex = 0;
        increasement = 0;
        _increasment = 0;
    }
    void setup(float _width, float _height)
    {
		//        gui.addPage("StackParticle");
		//        gui.addSlider("Increasment", _increasment, 0, 1);
        width = _width;
        height = _height;
        billboards.getVertices().resize(MAX_PARTICLE);
        billboards.getColors().resize(MAX_PARTICLE);
        billboards.getNormals().resize(MAX_PARTICLE,ofVec3f::zero());
        int _scale = 32;
        for(int i = 0 ; i < MAX_PARTICLE ; i++)
        {
            pos[i] = ofPoint(ofRandomf()*width,ofRandomf()*height);
            acc[i] = ofVec3f::zero();//ofPoint(ofRandomf()*0.01,ofRandomf()*0.01,0);
            vel[i] = ofVec3f::zero();//ofPoint(ofRandom(-1,1),ofRandom(-1,1),0);
            per[i] = ofVec3f::zero();
            fic[i] = ofRandom(0.98f,0.99f);
            age[i] = 100;
            decay[i]  = (abs(i-MAX_PARTICLE*0.5f)/MAX_PARTICLE*0.5f)*0.7;//ofRandom(10,50);
            
            scale[i]  = ofRandom(_scale/2,_scale*10);
            //noiseStrength[i]  = ofRandom(50,100);
            stepSize[i]   = ofRandomf()*0.1;
            gravity[i]  = 0;//0.05;
            billboards.setColor(i,color[i]);
			
        }
        billboards.setUsage( GL_DYNAMIC_DRAW );
        billboards.setMode(OF_PRIMITIVE_POINTS);
        
//        perlin = new MSA::Perlin();
		
        ofEnableAlphaBlending();
        
        billboardShader.load("shaders/Billboard");
        
        // we need to disable ARB textures in order to use normalized texcoords
        ofDisableArbTex();
        texture.loadImage("dot.png");
		//        ofEnableAlphaBlending();
		//        glow.allocate(WIDTH,HEIGHT,GL_RGBA);
		
		//        blur.allocate(WIDTH,HEIGHT,GL_RGBA);
    }
    void update()
    {
        x = ofxTween::update();
        
        updateParticle();
        
    }
    void draw()
    {
        
		//        blur.begin();
        ofClearAlpha();
        ofEnableAlphaBlending();
		
        ofSetColor(255);
        
        
		//        ofRect(0,0,width,height);
		//
        
        glPushMatrix();
        glTranslatef(x,y,0);
        drawParticle();
        glPopMatrix();
		//        ofClearAlpha();
		//        blur.end();
		//        blur.setRadius(1.7);
		//        blur.update();
		//        glow.setTexture(blur.getTextureReference());
		//        glow.setRadius(12);
		//        glow.update();
		//        ofSetColor(255);
		//        ofEnableAlphaBlending();
		//
		//
		//        glow.draw(0,0,WIDTH,HEIGHT);
        
        //fbo.draw(0,0,320,240);
        
    }
    float getWidth()
    {
        return width;
    }
    int size()
    {
        return 0;
    }
    void reset()
    {
        ofVec2f center(ofGetWidth()*0.5-x,height*0.5);
        for(int p = 0 ; p < MAX_PARTICLE;  p++)
        {
            ofVec2f n = ofVec2f(center-pos[p]).normalize();
            setupParticle( pos[p]  , n , 100);
			
        }
    }
    void setupParticle( ofVec2f vertex  , ofVec2f normal , float _age)
    {
        
        int i = currentIndex;
        pos[i] = vertex;
        acc[i] = ofPoint(ofRandomf()*0.01,ofRandomf()*0.01,ofRandomf()*0.01);
        vel[i] = normal*ofRandom(1,10);
        
        per[i] = ofVec3f::zero();
        fic[i] = ofRandom(0.8,0.99f);
        age[i] = _age;
        decay[i]  = (abs(i-MAX_PARTICLE*0.5f)/MAX_PARTICLE*0.5f)*0.7;
        scale[i]  = ofRandom(16,1000);
        //color[i].set(0.5,ofRandom(0.5,1),ofRandom(0.5,1),age[i]*0.01);
        billboards.setColor(i,color[i]);
        stepSize[i]   = ofRandomf()*0.5;
        gravity[i]  = 0.1;
        currentIndex++;
        currentIndex%=MAX_PARTICLE;
        
    }
    
    void updateParticle()
    {
        
		//        float noiseStrength = 0.7;
        increasement +=_increasment;
        for(int p = 0 ; p < MAX_PARTICLE;  p++)
        {
            //if(pos[p].x+x >-50 && pos[p].x+x<ofGetWidth()+50 && pos[p].y+y >-50 &&pos[p].y+y < ofGetHeight()+50)
            {
				if(age[p]>0)
				{
                    float angle = ofSignedNoise(pos[p].x,pos[p].y,ofGetElapsedTimef()) * noiseStrength;
                    
                    per[p].x = sin(angle) * stepSize[p] * age[p]* 0.01 ;
                    per[p].y = sin(angle) * stepSize[p] * age[p]* 0.01 ;
                    //per[p].z = sin(angle)*cos(angle) * stepSize[p] * age[p]* 0.01 ;
//					if(isCompleted())
//					{
//						
//						//float increasement_ = increasement*(p/MAX_PARTICLE*1.0f);
//						float decreasment = (abs(MAX_PARTICLE*0.5-p)*1.0f/MAX_PARTICLE*1.0f);
//						pos[p].y -= (pos[p].y-(height*0.5+((sin(increasement*decay[p])*200))))*0.1f;
//						pos[p].x -= (pos[p].x-(width*0.5+((sin(increasement*decay[p])*200))))*0.1f;
//						if(pos[p].y>height*0.5f-10 && pos[p].y<height*0.5f+10)
//						{
//							age[p] = 500;
//						}
//						age[p]+=(100-age[p])*0.1f;
//					}
//					else
					{
                    
						vel[p] += per[p];
						vel[p] += acc[p];
						
						
						vel[p] *= fic[p];
						
						
						pos[p] += vel[p];
					}
					if(pos[p].x<0)
					{
						pos[p].x = width;
					}else if(pos[p].y<0)
					{
						pos[p].y = height;
					}else if(pos[p].x>width)
					{
						pos[p].x = 0;
					}else if(pos[p].y>height)
					{
						pos[p].y = 0	;
					}
					//                age[p] -= timeDiff*decay[p];
					billboards.setNormal(p,ofVec2f(age[p]*0.01*scale[p],0));
					
					billboards.getVertices()[p] = pos[p];
					color[p].a = age[p]*0.01;
					billboards.setColor(p,color[p]);
					
				}
            }
        }
        
    }
    void drawParticle()
    {
        
        ofEnableAlphaBlending();
        //int numConnect=  0;
        for (int p=0; p<MAX_PARTICLE; p++){
//			//            if(numConnect>(MAX_PARTICLE*MAX_PARTICLE)*0.5)
//			//            {
//			//                break;
//			//            }
			float gDist = pos[p].distance(ofVec2f(width*0.5,height*0.5));
			if (gDist > 0.1) {
				float xGrav = width*0.5;
				float yGrav = height*0.5;
				float gravity = 0.1;
				float gxv   = gravity * (xGrav - pos[p].x) / gDist;
				float gyv   = gravity * (yGrav - pos[p].y) / gDist;
				
				vel[p].x         += gxv;
				vel[p].y         += gyv;
			}
            for (int i=0; i<MAX_PARTICLE; i++){
                if (i != p){
                    
                    float R             = pos[p].distance(pos[i]);
                    
                    int radius = 15;
                    if (R < radius){
//                        //numConnect++;
//						vel[p].y = -(pos[p].y-(height*0.5))*0.1f;
//						vel[p].x = -(pos[p].x-(width*0.5))*0.1f;
						
						
                        if(R<radius)
                        {
                            vel[p].x           +=  (pos[p].x - pos[i].x) / R*0.8;
                            vel[p].y           +=  (pos[p].y - pos[i].y) / R*0.8;
                            vel[p].x           +=  (pos[p].z - pos[i].z) / R*0.8;
							scale[p] = 16*(radius/R);
                        }
						else
						{
						scale[p] = 48;
						}
						
                        ofPushStyle();
                        ofSetLineWidth(2);
                        ofSetColor(225,255,255, ofMap(R,0,radius,255,0));
                        ofLine(pos[i].x, pos[i].y, pos[i].z, pos[p].x, pos[p].y, pos[p].z);
                        ofPopStyle();
                    }
                }
            }
        }
        ofPushStyle();
        //ofEnableAlphaBlending();
        ofEnableBlendMode(OF_BLENDMODE_ADD);
        glEnable(GL_POINT_SMOOTH);
        glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
        
        billboardShader.begin();
        ofEnablePointSprites();
        texture.getTextureReference().bind();
        billboards.draw();
        texture.getTextureReference().unbind();
        ofDisablePointSprites();
        billboardShader.end();
        ofDisableBlendMode();
        //ofDisableAlphaBlending();
        ofPopStyle();
        
		
    }
	
private:
    ofVboMesh billboards;
	ofShader billboardShader;
	ofImage texture;
	
    ofPoint pos[MAX_PARTICLE];
	ofPoint acc[MAX_PARTICLE];
	ofPoint vel[MAX_PARTICLE];
	ofPoint frc[MAX_PARTICLE];
	ofPoint per[MAX_PARTICLE];
	ofFloatColor color[MAX_PARTICLE];
	float fic[MAX_PARTICLE];
	float age[MAX_PARTICLE];
	float gravity[MAX_PARTICLE];
	float scale[MAX_PARTICLE];
	float decay[MAX_PARTICLE];
    
	float noiseStrength;
	float stepSize[MAX_PARTICLE];
	
	//bool isDrawMeah;
	int currentIndex;
//    MSA::Perlin *perlin;
	//    ofxGlow glow;
	//    ofxBlur blur;
    //ofFbo fbo;
};

//#endif
