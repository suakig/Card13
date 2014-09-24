#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    //HelloWorldクラスのシーンを作成する。
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    //初期か処理を行う
    virtual bool init();
    
    // implement the "static create()" method manually
    //create関数作成マクロ
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
