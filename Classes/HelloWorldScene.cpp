#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    //シーンを作成する。
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    //HelloWorldレイヤーを作成する。
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    //シーンに対してレイヤーを追加する。
    scene->addChild(layer);

    // return the scene
    //シーンを返す。
    return scene;
}

// on "init" you need to initialize your instance
//HelloWorldクラスのレイヤーの初期か処理を行う
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    return true;
}
