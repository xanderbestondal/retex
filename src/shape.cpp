#include "shape.h"
#include <iostream>

#include <GL/gl.h>
#include <map>

//--------------------------------------------------------------
void shape::setup(){
    reverseNormal = false;
}


void shape::setSteps(int st){
    steps = st;
    testMesh.clear();
    vertsBasePos.resize(0);
    vertsDeformedPos.resize(0);
    vertsDeformeFrom.resize(0);


    img.loadImage("img_02.jpg");
    //img.grabScreen(0,0,ofGetWidth(),ofGetHeight());

    //texData.textureTarget()
    //cylinder(steps,sgments,height);
}

int shape::getFileObjFaceCount(const char* filename){
    ifstream in(filename, ios::in);
    int faceCount=0;
    string line;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "f ")
        {
            faceCount ++;
        }
    }
    return faceCount;
}
ofMesh shape::loadObj(const char* filename){


    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Cannot open " << filename << endl;
        //exit(1);
    }
    int faceCount = getFileObjFaceCount(filename);
    vertsTris.resize(faceCount);
    vertNormals.resize(faceCount);

    string line;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "v ")
        {
            cout << line << endl;
            istringstream s(line.substr(2));

            ofPoint v(0,0,0);
            s >> v.x;
            s >> v.y;
            s >> v.z;
            cout << v << endl;

            testMesh.addVertex(v);
            vertsBasePos.push_back(v);
            vertsDeformedPos.push_back(v);
            vertMap.push_back(0);
        }
        else if (line.substr(0,2) == "f ")
        {
            istringstream s(line.substr(2));
            GLushort a,b,c;
            s >> a; s >> b; s >> c;
            a--; b--; c--;
            //cout << a << b << c << endl;
            makeTri(a,b,c);
        }
        else if (line[0] == '#')
        {
            /* ignoring this line */
        }
        else
        {
            /* ignoring this line */
        }
    }

    solidShading();
    // TODO CHANGE VERTDEFORMED LIST after smooth normales...
    return testMesh;

}
void shape::circle(int steps, float radius, ofVec3f pos){
    float angle = 0;
    for (int r = 0; r < steps; r++){

        float x = sin(angle)*radius;
        float z = cos(angle)*radius;

        ofPoint newPos(x+pos.x,0+pos.y,z+pos.z);
        testMesh.addVertex(newPos);
        vertsBasePos.push_back(newPos);
        vertsDeformedPos.push_back(newPos);
        vertMap.push_back(0);

        testMesh.addTexCoord(ofVec2f(0,0));
        //testMesh.addColor(ofFloatColor(0.5,0.5,0.5));
        angle += TWO_PI / steps;//ofLog(OF_LOG_NOTICE, "the number is " + ofToString(yPos));
        //cout << yPos << endl;
    }
    //segment_count += 1;
}
ofMesh shape::cylinder(int s,int heightSegments, float height, float radius,bool sphere, bool cone){
    //float radius = 300;
    float angle = 0;
    //float yStep = height/heightSegments;

    tris = 0;
    steps = s;
    ofVec3f pos(0,0,0);

    //testMesh.setMode( OF_PRIMITIVE_LINES );

    vertsTris.resize(heightSegments*steps);
    vertNormals.resize(heightSegments*steps);

    if(sphere){
        for (int i = 0; i < heightSegments; i++){
            float sphereRadius = sin(float(i)/heightSegments*PI)*radius;
            float circlePos = cos(float(i)/heightSegments*PI)*radius;
            pos.y = -circlePos;
            circle(steps,sphereRadius,pos);
        }
    }
    if(cone){
        for (int i = 0; i < heightSegments; i++){
            pos.y = float(i)/heightSegments*height;
            circle(steps,-float(heightSegments-1-i)/heightSegments*radius,pos);
        }
    }

    for (int i = 0; i < (heightSegments-1)*steps; i++){
        if(i!=0 && (i+1)%steps == 0){
            //ofLog(OF_LOG_NOTICE, "the modulo " + ofToString(i));
            makeTri(i,i+steps,i+1-steps);
            makeTri(i+1,i+1-steps,i+steps);
        }else{
            makeTri(i,i+steps,i+1);
            makeTri(i+1,i+steps,i+1+steps);
        }
    }
    //tex.loadScreenData(0, 0, 1, 1);
    setNormals(testMesh);
    vertsDeformeFrom = vertsBasePos;
    vertsBaseNormals = testMesh.getNormals();
    return testMesh;
}
void shape::setNormals( ofMesh &mesh ){
	//The number of the vertices
	int nV = mesh.getNumVertices();

	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;

	vector<ofPoint> norm( nV );	//Array for the normals

	//Scan all the triangles. For each triangle add its
	//normal to norm's vectors of triangle's vertices
    polyNormals.resize(0);
	for (int t=0; t<nT; t++) {
		//Get indices of the triangle t

		int i1 = mesh.getIndex( 3 * t );
		int i2 = mesh.getIndex( 3 * t + 1 );
		int i3 = mesh.getIndex( 3 * t + 2 );

		//Get vertices of the triangle
		const ofPoint &v1 = mesh.getVertex( i1 );
		const ofPoint &v2 = mesh.getVertex( i2 );
		const ofPoint &v3 = mesh.getVertex( i3 );

		//Compute the triangle's normal
		ofPoint dir(0,0,0);
		if(!reverseNormal){
            dir = ( (v3 - v1).crossed( v2 - v1 ) ).normalized();
		}else{
            dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
		}


		polyNormals.push_back(dir);

		//Accumulate it to norm array for i1, i2, i3
		norm[ i1 ] += dir;
		norm[ i2 ] += dir;
		norm[ i3 ] += dir;
	}

	//Normalize the normal's length
	for (int i=0; i<nV; i++){
		norm[i].normalize();
	}

	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals( norm);
 }
void shape::makeTri(int vert1,int vert2,int vert3){

    testMesh.addIndex(vert1);
    testMesh.addIndex(vert2);
    testMesh.addIndex(vert3);
    vertsTris[vert1].push_back(tris);
    vertsTris[vert2].push_back(tris);
    vertsTris[vert3].push_back(tris);
//solidShading();
    tris += 1;
// TEMP NORMALS
    testMesh.addNormal(ofPoint(0,0,0));
}
void shape::solidShading(){
    vertsBasePos.resize(0);
    testMesh.smoothNormals(0.0f);
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        int index = testMesh.getIndex(i);
        vertsBasePos.push_back(testMesh.getVertex(index));
        //vertsDeformedPos.push_back(v);
    }
}
ofVec3f shape::makeTriNormal(ofVec3f p1,ofVec3f p2,ofVec3f p3){
    ofVec3f U = (p3 - p1);
    ofVec3f V = (p2 - p1);

    ofVec3f n(0,0,0);
    n.x = ( U.y * V.z) - ( U.z * V.y);
    n.y = ( U.z * V.x) - ( U.x * V.z);
    n.z = ( U.x * V.y) - ( U.y * V.x);

    //n = U.cross(V);
    n.normalize();
    return n;
}
void shape::reverseNormals(){
    reverseNormal = true;
}
void shape::updateNormals(){
    polyNormals.resize(0);
   // testMesh.removeNormal
    for(int i = 0; i < testMesh.getNumIndices(); i+=3){
        polyNormals.push_back(makeTriNormal(testMesh.getVertex(testMesh.getIndex(i)),testMesh.getVertex(testMesh.getIndex(i+1)),testMesh.getVertex(testMesh.getIndex(i+2))));
    }

    for(int i = 0; i < vertsTris.size(); i++){
        vertNormals[i] = ofPoint(0,0,0);
        for(int j = 0; j < vertsTris[i].size(); j++){

            vertNormals[i] += polyNormals[vertsTris[i][j]];
        }
        testMesh.setNormal(i,vertNormals[i] /= vertsTris[i].size());
    }
}
void shape::drawNormals(bool faces){

    float len = 10;
    if(faces){
        for(int i = 0; i < polyNormals.size(); i++){
            ofPoint mid = (testMesh.getVertex(testMesh.getIndex(i*3))+testMesh.getVertex(testMesh.getIndex(i*3+1))+testMesh.getVertex(testMesh.getIndex(i*3+2)))/3;
            //polyNormals
            //ofSetColor(0,0,255);
            ofLine(mid.x,mid.y,mid.z,mid.x+polyNormals[i].x*len,mid.y+polyNormals[i].y*len,mid.z+polyNormals[i].z*len);
            //ofLine(x,polyNormals[i].y,polyNormals[i].z,0,0,0);
        }
    }else{
        for(int i = 0; i < vertNormals.size(); i++){
            //ofSetColor(255,255,255);
            ofLine(testMesh.getVertex(i).x,testMesh.getVertex(i).y,testMesh.getVertex(i).z,testMesh.getVertex(i).x+testMesh.getNormal(i).x*len,testMesh.getVertex(i).y+testMesh.getNormal(i).y*len,testMesh.getVertex(i).z+testMesh.getNormal(i).z*len);
        }
    }
}
void shape::mapPlannar(ofCamera cam){
    testMesh.clearTexCoords();
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        int index = testMesh.getIndex(i);
        ofVec2f uv = cam.worldToScreen(testMesh.getVertex(index));
        testMesh.addTexCoord(ofPoint(uv.x/ofGetWidth(),uv.y/ofGetHeight()));
    }
}
void shape::mapByLight(ofVec3f point,ofImage positionMap){
    //testMesh.clearTexCoords();
    float len = 2.0f;
    //cout << polyNormals.size() << endl;
    for(int i = 0; i < polyNormals.size(); i++){
        ofPoint mid = (testMesh.getVertex(testMesh.getIndex(i*3))+testMesh.getVertex(testMesh.getIndex(i*3+1))+testMesh.getVertex(testMesh.getIndex(i*3+2)))/3;
        //ofPoint lightDir(-1,0,0);

        float dot(((polyNormals[i].dot(point))+1)/2);
        ofPoint uvPos(findPixel(positionMap,dot*255.0f));
        //cout << dot << endl;
        for(int t = 0; t < 3; t++){
            int index = testMesh.getIndex(i*3+t);
            ofPoint vert = testMesh.getVertex(index);
            //float distance = mid.distance( vert );


            ofPoint tangent = (vert-mid).normalize();
            ofPoint perp = tangent.crossed(polyNormals[i]);
            ofVec3f localVert = vert.getMapped(mid,tangent,polyNormals[i],perp);

            if(i==2){
                ofSetColor(255,255,0);
                ofLine(vert.x,vert.y,vert.z, localVert.x, localVert.y, localVert.z);
            }

//            if(t==2){
                ofSetColor(255,0,0);
                ofLine(mid.x,mid.y,mid.z, mid.x+tangent.x*len,mid.y+tangent.y*len,mid.z+tangent.z*len);
                ofSetColor(0,255,0);
                ofLine(mid.x,mid.y,mid.z, mid.x+perp.x*len, mid.y+perp.y*len, mid.z+perp.z*len);
//            }

            //testMesh.addTexCoord(ofPoint(dot,ofRandom(1)));
            testMesh.setTexCoord(index,ofPoint(dot+ofRandom(1),ofRandom(1)));
            //testMesh.setTexCoord(testMesh.getIndex(i*3+t),ofPoint(uvPos.x+ofRandom(1.0f),uvPos.y+ofRandom(1.0f)));
        }
        ofSetColor(0,0,255);
        ofLine(mid.x,mid.y,mid.z, mid.x+polyNormals[i].x*len,mid.y+polyNormals[i].y*len,mid.z+polyNormals[i].z*len);
        //ofLine(x,polyNormals[i].y,polyNormals[i].z,0,0,0);
    }
    ofSetColor(255,255,255);

}
ofPoint shape::findPixel(ofImage texture, float lightness){
    ofPixels pixels = texture.getPixelsRef();
    ofPoint returnPos(0.0f,0.0f);
    int tol = 10;
    for(int y = 0; y < texture.getHeight(); y++){
		for(int x = 0; x < texture.getWidth(); x++){
                ofColor pixel = pixels.getColor(x,y);
                unsigned int diff = pixel.getLightness()-lightness;
                //lightnessList.push ['a']=ofPoint(x,y);
                if(diff < tol){
                    returnPos.x = x/texture.getWidth();
                    returnPos.y = y/texture.getHeight();
                }
		}
    }
    return returnPos;// NORMALISED
}
void shape::noiseMap(float frequency,ofVec3f scale){
    vertMap.resize(0);
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        vertMap.push_back( ofNoise(vertsBasePos[i].x*frequency*scale[0],vertsBasePos[i].y*frequency*scale[1],vertsBasePos[i].z*frequency*scale[2]));
    }
}
void shape::displaceMap(ofCamera cam,ofImage dispImg){
    vertMap.resize(0);
    for(int i = 0; i < testMesh.getNumVertices(); i++){

        ofVec2f uv = cam.worldToScreen(vertsBasePos[i]);
        //cout << uv.x << endl;
        //ofColor col =
//        ofColor col(0,0,0);
//        if(uv.x < width){
//            cout << uv.x << endl;
        ofColor col = dispImg.getColor(uv.x,uv.y);
//        }
        //vertMap.push_back( col[0] );
        vertMap.push_back(col.getBrightness()/255.0f);
    }
}
void shape::gradientMap(){
    vertMap.resize(0);
    float height = 500;
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        vertMap.push_back( vertsBasePos[i].y/height);
    }
}
void shape::solidDeform(int iterations){

//    for(int iter = 0; iter < iterations; iter++){
    for(int i = 0; i < testMesh.getNumVertices(); i+=3){
        //float rand=  ofRandom(30);
        ofVec3f deformedPos(0,ofRandom(30),0);

        testMesh.setVertex(i,(testMesh.getVertex(i)+deformedPos));
        testMesh.setVertex(i+1,(testMesh.getVertex(i+1)+deformedPos));
        testMesh.setVertex(i+2,(testMesh.getVertex(i+2)+deformedPos));
    }
//    }
}
void shape::deform(int iterations){
    vertsDeformeFrom = vertsBasePos;
    normalsDeformeFrom = vertsBaseNormals;
    for(int iter = 0; iter < iterations; iter++){
        vertsDeformedPos.resize(0);
        for(int i = 0; i < testMesh.getNumVertices(); i++){
            ofVec3f deformedPos((vertMap[i]-.5f) * noiseStrength * normalsDeformeFrom[i] + vertsDeformeFrom[i] + normalsDeformeFrom[i]*noiseStrength*.5f);
            testMesh.setVertex(i,deformedPos);
            vertsDeformedPos.push_back(deformedPos);
        }
        vertsDeformeFrom = vertsDeformedPos;
        setNormals(testMesh);
        normalsDeformeFrom = testMesh.getNormals();
    }
}
void shape::setNoiseStrength(int s){
    iterations = 0;
    noiseStrength = s;
}
void shape::resetMesh(){

}
void shape::setTexture(ofImage texImg){
    img = texImg;
}
void shape::setVertColor(){
    for(int i = 0; i < testMesh.getNumVertices(); i++){
        testMesh.setColor(i,ofFloatColor(1-vertMap[i],0,0));
    }
}
void shape::changeColor(){
    //int pixels = (img.getHeight()*img.getWidth())
    //ofColor green(0, 255, 0);
    //img.loadImage("img_02.jpg");
    ofPixelsRef pixels = img.getPixelsRef();

	//let's manipulate the pixels by iterating through them
    //or a rough bluescreen
    for(int y = 0; y < img.getHeight(); y++){
		for(int x = 0; x < img.getWidth(); x++){

			//if the blue is greater than the combination of  red and green together, delete that pixel
			ofColor color = img.getColor(x, y);
			if(color.r > 10){
                pixels.setColor(x, y, color*ofColor::red);
			}
			else{
                pixels.setColor(x, y, color*ofColor::blue);
			}
		}
	}

    //after we're done we need to put the pixels into the keyed image
    img.setFromPixels(pixels);
}

void shape::loopTexture(int xOffset,int yOffset,float scale){

    //int offset = 10;
    img.grabScreen(0+xOffset+scale,0+yOffset+scale,ofGetWidth()-scale,ofGetHeight()-scale);
}
void shape::show(){

    setNormals(testMesh);
    //drawNormals();
    //drawNormals(true);

    //ofTranslate(-20,0,0);
    //img.getTextureReference().bind();

    //ofSetColor( 0, 128, 0 );
    //mapPlannar();
//    ofColor green(0, 255, 0);
//    img.setColor(green);

//    img.loadImage("img_01.png");
//    changeColor();
    //img.rotate90(5);
    img.bind();

    testMesh.draw();

//    testMesh.drawWireframe();
    img.unbind();

    //img.
    //setVertColor();

}
//--------------------------------------------------------------
void shape::draw(){

}
