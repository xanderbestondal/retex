#pragma once

#include "ofMain.h"

class shape : public ofBaseApp{

	public:
		void setup();
		void draw();

        ofMesh testMesh;


        ofImage img;
        ofImage displaceImg;
        ofImage img_2;
        ofTexture tex;
        ofTextureData texData;

        bool reverseNormal;
        //int segment_count;
        int steps;
        int segments;
        int tris;
        int iterations;
        int noiseStrength;

        vector<ofVec3f> vertsBasePos; // used to be able to alwys go back to the original shape
        vector<ofVec3f> vertsDeformedPos;
        vector<ofVec3f> vertsDeformeFrom;

        vector<ofVec3f> vertsBaseNormals;
        vector<ofVec3f> normalsDeformeFrom;

        vector<ofVec3f> polyNormals;
        vector<ofVec3f> vertNormals;
        vector<float> vertMap;

        vector<vector<int> > vertsTris;

        vector<ofIndexType > originalIndices;// used to store verts before division
        vector<ofVec3f > dividedIndices; // used to store ALL verts after division
        //vector<ofVec3f > dividedIndices; // used to store only new verts after division

        vector<ofVec2f > dividedList; // used to store vert pairs that are divided
        vector<int > dividedVertList; // used together with dividedList to find vert number that was already divided

        //std::map<char,ofPoint> lightnessList;

        void makeTri(int vert1,int vert2,int vert3);
        void show();
        void setSteps(int st);
        void setNoiseStrength(int s);

        void setNormals( ofMesh &mesh );
        void solidShading();

        ofVec3f makeTriNormal(ofVec3f p1,ofVec3f p2,ofVec3f p3);
        void updateNormals();
        void reverseNormals();
        void resetMesh();
        void setVertColor();

        ofCamera cam;

        ofMesh piramid();
        ofMesh piramidSharp();
        ofMesh cylinder(int s,int heightSegments, float height,float radius,bool sphere, bool cone);


ofMesh loadObj(const char* filename);
int getFileObjFaceCount(const char* filename);


        //ofMesh sphere(int s,int heightSegments, float radius);

        void noiseMap(float frequency, ofVec3f scale);
        void displaceMap(ofCamera cam,ofImage dispImg);
        void gradientMap();

        void solidDeform(int iterations);
        void deform(int iterations);
        void drawNormals(bool faces=false);

        void mapPlannar(ofCamera cam);

void mapByLight(ofVec3f point,ofImage positionMap);
ofPoint findPixel(ofImage texture,float lightness);

        void changeColor();
        void loopTexture(int xOffset,int yOffset,float scale);
        void setTexture(ofImage texImg);

        void circle(int steps, float radius, ofVec3f pos);


};
