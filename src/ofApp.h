/*----------------------数字をここで指定----------------------*/
#define NUM 10

/*-----------------------ofxOsc(受信のやつ)-----------------------*/

#include "ofxOsc.h"
#define PORT 8020//OSCを受信しりポート番号を設定

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void draw_particle();
    void osc_word();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    /*-----------------------ofxOsc(受信のやつ)-----------------------*/
    ofxOscReceiver receiver;
    
    /*----------------------フォントを指定----------------------*/
    ofTrueTypeFont type_word;
    
    /*----------------------描画される文字（テスト）----------------------*/
    //std::vector<std::string> test_NUM = {"test_zero" };
     
    string text;
    //std::vector<std::string> text_array;                  //出力する文字列
    
    /*----------------------色の指定（ランダム）----------------------*/
    float red[NUM];
    float green[NUM];
    float blue[NUM];
    
    
    float rect_x[NUM];
    float rect_y[NUM];
    float rect_w[NUM];
    float rect_h[NUM];
    
    /*----------------------3D空間に----------------------*/
    ofEasyCam cam;
    
    
    bool once;
    
    
    
    vector<ofColor> color; //パーティクルの色
    vector<ofPoint> cpos;  //パーティクルの現在位置
    vector<ofPoint> gpos;  //パーティクルの目標位置
    vector<ofVec2f> vel;   //パーティクルの速度
    
    bool isKeyPressed;           //キーを押下した/していない
    int radius;                  //パーティクルの半径
    
    
    int time = 0;
    
    
    bool draw_rect =  false;

    
};
