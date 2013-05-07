#include "testApp.h"
//--------------------------------------------------------------
void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
	ofVec3f normal = ((b - a).cross(c - a)).normalize();
	mesh.addNormal(normal);
	mesh.addTexCoord(a);
	mesh.addVertex(a);
	mesh.addNormal(normal);
	mesh.addTexCoord(b);
	mesh.addVertex(b);
	mesh.addNormal(normal);
	mesh.addTexCoord(c);
	mesh.addVertex(c);
}

//--------------------------------------------------------------
void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
	addFace(mesh, a, b, c);
	addFace(mesh, a, c, d);
}

//--------------------------------------------------------------
ofVec3f getVertexFromImg(ofFloatImage& img, int x, int y) {
	float scaleFactor = 100;
	return ofVec3f(x, y, 100 * img.getColor(x, y).getBrightness());
}

//--------------------------------------------------------------
void testApp::setup(){
	//	ofSetFullscreen(true);
	shader.load("shaders/depth");
	
	grabber.initGrabber(640,480);
	fbo.allocate(ofGetWidth()*0.5,ofGetHeight()*0.5);
	
	shader.begin();
	shader.setUniform1f("brightness", 1);
	shader.setUniform1f("contrast", 1);
	shader.setUniform1f("saturation", 0.5);
	shader.setUniform1i("invert",false);
	shader.setUniform1f("alpha", 1.0 );
	shader.setUniform1i("tex", 0);
//	shader.setUniformTexture("tex", fbo.getTextureReference(), 1);
	shader.end();
	
	int width = fbo.getWidth();
	int height = fbo.getHeight();
	int step = 8;
	mainMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	//add one vertex to the mesh for each pixel
	for (int y = 0; y < height; y+=step){
		for (int x = 0; x<width; x+=step){
			//			mainMesh.addVertex(ofPoint(x,y,0)); // 0
			//			mainMesh.addTexCoord(ofPoint(x,y,0));
			//
			//			mainMesh.addVertex(ofPoint(x+step,y,0)); //1
			//			mainMesh.addTexCoord(ofPoint(x+step,y,0));
			//
			//			mainMesh.addVertex(ofPoint(x,y+step,0)); //10
			//			mainMesh.addTexCoord(ofPoint(x,y+step,0));
			//
			//
			//			mainMesh.addVertex(ofPoint(x+step,y,0)); //1
			//			mainMesh.addTexCoord(ofPoint(x+step,y,0));
			//
			//			mainMesh.addVertex(ofPoint(x+step,y+step,0)); //11
			//			mainMesh.addTexCoord(ofPoint(x+step,y+step,0));
			//
			//			mainMesh.addVertex(ofPoint(x,y+step,0)); //10
			//			mainMesh.addTexCoord(ofPoint(x,y+step,0));
			ofVec3f nw = ofVec3f( x, y , 0);
			ofVec3f ne = ofVec3f( x + step, y, 0);
			ofVec3f sw = ofVec3f( x, y + step, 0);
			ofVec3f se = ofVec3f( x + step, y + step, 0);
			
			addFace(mainMesh, nw, ne, se, sw);
			
		}
	}
	
	
	particle.setup(fbo.getWidth(),fbo.getHeight());
	
	gui.setup("Settings", "defaultGuiSettings.xml");
    
    gui.add(maxHeight.setup("MAX_HEIGHT", ofxParameter<float>(),0,100));
    gui.add(enableShader.setup("ENABLE_SHADER", ofxParameter<bool>()));
	gui.add(mode.setup("SHADER_MODE", ofxParameter<int>(),0,5));
	gui.add(enableCamera.setup("ENABLE_CAMERA", ofxParameter<bool>()));
	gui.add(enableRipple.setup("ENABLE_RIPPLE", ofxParameter<bool>()));
	gui.add(enableParticle.setup("ENABLE_PARTICLE", ofxParameter<bool>()));
	for(int i = 0; i < 9 ; i++)
	{
		gui.add(color[i].setup("COLOR_"+ofToString(i), ofxParameter<float>(),0,1));
	}
	gui.loadFromFile("defaultGuiSettings.xml");
}

//--------------------------------------------------------------
void testApp::update(){
	//	ripple.makeRipples(loc.x, loc.y , 5000);
	if(enableParticle)particle.update();
	if(enableCamera)
	{
		grabber.update();
	}
	fbo.begin();
	ofClear(0);
	
	if(enableCamera){
		ofPushStyle();
		ofSetColor(255);
		grabber.draw(0, 0,fbo.getWidth(),fbo.getHeight());
		ofPopStyle();
	}
	ofPushStyle();
	if(enableRipple)
	{
		ripple.render();
		ripple.draw();
	}
	ofPopStyle();
	ofPushStyle();
	ofSetColor(255);
	if(enableParticle)particle.draw();
	ofPopStyle();
	fbo.end();
	
}

//--------------------------------------------------------------
void testApp::draw(){
	glDisable(GL_DEPTH_TEST);
	fbo.draw(0,0,ofGetWidth(),ofGetHeight());
	glEnable(GL_DEPTH_TEST);
	cam.begin();
	ofPushMatrix();
	ofScale (1,-1,1);
	glPushMatrix();
	glTranslatef(-fbo.getWidth()*0.5, -fbo.getHeight()*0.5, 0);
	if(enableShader)
	{


		shader.begin();
		shader.setUniform1i("maxHeight",maxHeight);
		shader.setUniform1i("mode",mode);
            shader.setUniform1f("iGlobalTime", ofGetElapsedTimef());
		for(int i =0 ; i< 9 ;i ++)
		{
			ofColor c = ofColor::fromHex(color[i]*255*255*255);
			string uniformName="c"+ofToString(i+1);
			shader.setUniform3f(uniformName.c_str(), c.r/255.0, c.g/255.0, c.b/255.0);
		}
		
	}
	//	fbo.draw(0, 0);
	//
	fbo.getTextureReference().bind();
	mainMesh.drawFaces();
//	mainMesh.drawWireframe();
	fbo.getTextureReference().unbind();
	//fbo.unbind();
	if(enableShader)
	{
		shader.end();
	}
	glPopMatrix();
	ofPopMatrix();
	cam.end();
	glDisable(GL_DEPTH_TEST);
	gui.draw();
	//	mainMesh.drawFaces();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	//	shader.unload();
	//	shader.load("shaders/depth");
	//	shader.setUniform1i("tex", 0);
	if(key=='f')
	{
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}