#pragma once

#include "ofMain.h"

class Ripple : public ofVec2f
{
public:
	Ripple()
	{
        xSize = ofGetWidth();
        ySize = ofGetHeight();
        block  = 8;
        yRes = ySize/block;
        xRes = xSize/block;
        r0                 = new float*[xRes];
        r1                 = new float*[xRes];
        r2                 = new float*[xRes];
        for(int i = 0 ; i < xRes ; i++)
        {
            r0[i]=	 new float[yRes];
            r1[i]=	 new float[yRes];
            r2[i]=	 new float[yRes];
            
        }
        _decay              = .9;  // the speed at which the waves decay (1.0 is no decay) (0.0 is instant decay to nothing)
        heightMulti        = .02;
        for(int i = 0 ; i < xRes ; i++)
        {
            for(int j = 0 ; j < yRes ; j++)
            {
                r0[i][j] = 0;
                r1[i][j] = 0;
                r2[i][j] = 0;
            }
        }
	}
    void findRipples(){
        for (int y=1; y<yRes-1; y++){
            for (int x=1; x<xRes-1; x++){
                r0[x][y] = (r1[x-1][y] + r1[x+1][y] + r1[x][y-1] + r1[x][y+1]) / 4.0;
                r0[x][y] = r0[x][y] * 2.0 - r2[x][y];
                r0[x][y] *= _decay;
            }
        }
    }
    
    void swapBuffers(){
        for (int y=0; y<yRes; y++){
            for (int x=0; x<xRes; x++){
                r2[x][y] -= (r2[x][y] - r1[x][y]) * _decay;
                r1[x][y] -= (r1[x][y] - r0[x][y]) * _decay;
            }
        }
    }
    void makeRipples(int x,  int y){
        makeRipples(x,  y , 2000);
    }
	void makeRipples(int x,  int y , float power){
        xm = ofMap(x,0,ofGetWidth(),0,xRes-1);
        ym = ofMap(y,0,ofGetHeight(),0,yRes-1);
        for (int y=1; y<yRes-1; y++){
            for (int x=1; x<xRes-1; x++){
                float d = ofDist(xm,ym,x,y);
                if (d < 3){
                    
                    r1[x][y] -= pow(((3 - d)/3.0),2) * power;
                }
            }
        }
    }
	void render()
	{
        if(ofGetFrameNum()%2==0)
        {
            makeRipples(ofRandom(0,xSize*0.5),ofRandom(0,ySize));
        }
		findRipples();
		swapBuffers();
    }
    void draw()
    {
        for (int y=1; y<yRes-1; y++){
            for (int x=1; x<xRes-1; x++){
                int c = ofMap(r0[x][y],0,100,0,255);
                ofSetColor(c,c,c,c);
                ofRect(x*block,y*block,1*block,1*block);
            }
        }
    }
    float **r0;
    float **r1;
    float **r2;
    float _decay;
    float heightMulti;
    int yRes,xRes;
    float xm, ym;
    int xSize,ySize;
    int block;
};
