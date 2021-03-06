
#include "ofApp.h"

int cover = 0;

//--------------------------------------------------------------

void ofApp::setup(){
    /*-----------------------ofxOsc(受信のやつ)-----------------------*/
    receiver.setup(PORT);
    /*----------------------フォントを指定----------------------*/
    type_word.load("Anders.ttf", 50,true, true, true, 0.3, 0);
    type_word_small.load("Anders.ttf", 20,true, true, true, 0.3, 0);

    
    // フレーム更新時に前のフレームで描画したものを消さない
    //ofSetBackgroundAuto(false);
    /*----------------------文字(particle)----------------------*/
    isKeyPressed = false;
}

//--------------------------------------------------------------

void ofApp::update(){
    /*-----------------------文字(particle)-----------------------*/
    for(int i=0; i<cpos.size(); i++){
        if(isKeyPressed){
            if(abs(gpos[i].x - cpos[i].x) > abs(vel[i].x)){
                cpos[i].x += vel[i].x;
            }else{
                cpos[i].x = gpos[i].x;
                vel[i].x = 0;
            
            }
            if(abs(gpos[i].y - cpos[i].y) > abs(vel[i].y)){
                cpos[i].y += vel[i].y;
            }else{
                cpos[i].y = gpos[i].y;
                vel[i].y = 0;
            }
        }else{
            if(cpos[i].x > ofGetWidth()){
                cpos[i].x =ofGetWidth();
                vel[i].x *= (-1);
            }else if(cpos[i].x < 0){
                cpos[i].x = 0;
                vel[i].x *= (-1);
            }else{
                cpos[i].x += vel[i].x;
            }
            if(cpos[i].y > ofGetHeight()){
                cpos[i].y = ofGetHeight();
                vel[i].y *= (-1);
            }else if(cpos[i].y < 0){
                cpos[i].y = 0;
                vel[i].y *= (-1);
            }else{
                cpos[i].y += vel[i].y;
            }
        }
    }
}

//--------------------------------------------------------------

void ofApp::draw(){
    /*----------------------文字(particle)----------------------*/
     ofBackground(0, 0, 0);
    
    /*-----------------------OSC部分-----------------------*/
    //関数:osc通信で文字を受信
    osc_word();
    
    /*-----------------------いっぱいの円の描画-----------------------*/
    for(int i=0; i<cpos.size(); i++){
        ofSetColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
        ofDrawRectangle(cpos[i].x, cpos[i].y, radius, radius);
    }
    
    if(cover > 0){
        word_draw = true;
    }
    
    if(word_draw){
        ofSetColor(255,255,255);
        type_word_small.drawString("please press space key", ofGetWidth()/2-180 ,  80);
    }
    
    /*
    //int ms = ofGetElapsedTimeMillis() % 1000;
    int s = ofGetSeconds();
    cout << s;
    time_now = s % 3;
    
    const static int ftime= 100;
    
    if(time_now == 1 ){
        particle_atumaru = true;
    }else if(time_now == 0 ) {
        particle_atumaru = false;
    }
    
    if(particle_atumaru){
        isKeyPressed = !isKeyPressed;
        for(int i=0; i<vel.size(); i++){
            vel[i].x = (isKeyPressed) ? (gpos[i].x-cpos[i].x)/ftime : ofRandom(-1, 1);
            vel[i].y = (isKeyPressed) ? (gpos[i].y-cpos[i].y)/ftime : ofRandom(-1, 1);
        }

    }
    */
    
}

//--------------------------------------------------------------

void ofApp::osc_word(){
    /*-----------------------OSC部分-----------------------*/
    //現在順番待ちをのOSCメッセージがある間は受信を続ける
    while(receiver.hasWaitingMessages()){
        //次のメッセージを取得(決まり文句的な)
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        //メッセージが "/key/sentences"なら受信
        if(m.getAddress() == "/key/sentences"){
            text = "";
            cover ++;
            //numという変数をおいて初期値を0としてそこに送信されてきた数字を代入
            int num = m.getArgAsInt(0);
            //ポインタ＝より効率的にメモリを使用できる
            string *sentences = new string[num];
            for (int i = 0; i < num; i++) {
                
                //sentences[i]に初期値をi+1としてそこに送信されてきた文字を代入
                sentences[i] = m.getArgAsString(i+1);
                //コンソールに表示
                printf("%s", sentences[i].c_str());
                //cout << i << endl;
                
                //真ん中に表示する文字を代入
                text = text + sentences[i].c_str();
            }
            printf("\n--------------------------------");

        
            //関数:円をたくさん描画
            draw_particle();
        }
    }
    
    /*
    int hue = ofGetElapsedTimef();
    if(hue % 5 == 3){
        const static int ftime= 100;
        isKeyPressed = !isKeyPressed;
        for(int i=0; i<vel.size(); i++){
            vel[i].x = (isKeyPressed) ? (gpos[i].x-cpos[i].x)/ftime : ofRandom(-2, 2);
            vel[i].y = (isKeyPressed) ? (gpos[i].y-cpos[i].y)/ftime : ofRandom(-2, 2);
        }
    }
     
     */
}

//--------------------------------------------------------------

void ofApp::draw_particle(){
    //パーティクルの半径の設定
    radius = 4;
    cpos.clear();
    gpos.clear();
    
    vector<ofTTFCharacter> sentences = type_word.getStringAsPoints(text);
    //文字列の中心座標を計算
    ofPoint center = ofPoint(type_word.stringWidth(text)/2, type_word.stringHeight(text)/2 - 130);
    
    //画面中央に文字列を出力する為にシフト量を計算
    ofPoint shift = ofPoint(ofGetWidth()/2, ofGetHeight()/2) - center;
    //文字列の１文字ずつのループ
    for(int i = 0; i < sentences.size(); i++){
        //文字の輪郭のループ
        for(int j = 0; j < sentences[i].getOutline().size(); j++){
            ofPoint first, prev;
            //文字の輪郭を形成する点のループ
            for(int k = 0; k <= sentences[i].getOutline()[j].size(); k++){
                ofVec2f pos;
                if(k < sentences[i].getOutline()[j].size()){
                    //点の位置座標を設定
                    pos = ofPoint(sentences[i].getOutline()[j].getVertices()[k].x,
                                  sentences[i].getOutline()[j].getVertices()[k].y);
                }else{
                    //輪郭が一周したら先頭の点に戻る
                    pos = first;
                }
                if(k == 0){
                    //先頭の点を記憶しておく
                    first = pos;
                }else{
                    //一つ前の点(prev)と現在の点(pos)の距離を計算する
                    float length = sqrt(pow((pos.x - prev.x),2) + pow((pos.y - prev.y),2));
                    //前の点と現在の点をパーティクルで繋げるときに
                    //どのくらいの間隔でパーティクルを配置してゆくかを計算する
                    float addX = (pos.x - prev.x) * (radius / length);
                    float addY = (pos.y - prev.y) * (radius / length);
                    //前の点の位置にパーティクルを置く
                    gpos.push_back(prev + shift);
                    //前の点と現在の点との間に一定間隔でパーティクルを置く
                    for(int n = 0; n < (length / radius) - 1; n++){
                        prev.x += addX;
                        prev.y += addY;
                        gpos.push_back(prev + shift);
                    }
                }
                
                //現在の点の位置座標を記憶する
                prev = pos;
            }
        }
    }
    
    for(int i  = 0; i < gpos.size(); i++){
        cpos.push_back(ofPoint(ofRandom(ofGetWidth()/2 - 200, ofGetWidth()/2 + 200), ofRandom(ofGetHeight()/2 - 200, ofGetHeight()/2 + 200)));
        vel.push_back(ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1)));
    }
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key){
    /*----------------------spaceをおしたら文字に！----------------------*/
    
    const static int ftime= 100;
    
    switch(key){
        case ' ':
            isKeyPressed = !isKeyPressed;
            for(int i=0; i<vel.size(); i++){
                vel[i].x = (isKeyPressed) ? (gpos[i].x-cpos[i].x)/ftime : ofRandom(-1, 1);
                vel[i].y = (isKeyPressed) ? (gpos[i].y-cpos[i].y)/ftime : ofRandom(-1, 1);
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
