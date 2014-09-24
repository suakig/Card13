#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <random>

#define CARD_NUM 13//１種類あたりのカードの枚数
#define CARD_TYPE_NUM 4//カードの種類

#define CARD_1_POS_X 200
#define CARD_1_POS_Y 320
#define CARD_DISTANCE_X 140
#define CARD_DISTANCE_Y 200

#define ZORDER_SHOW_CARD 1//表示しているカードのZオーダー

USING_NS_CC;

//カードの種類
enum CardType
{
    Clubs =0,
    Diamonds,
    Hearts,
    Spades
};

//カード
struct Card
{
    int number;
    CardType type;
};

//カードを表示する位置
struct PosIndex{
    int x;
    int y;
};


class HelloWorld : public cocos2d::Layer
{
protected:
    std::vector<Card> _cards;//カード情報
    
    void initCards();//カードを初期化する
    Card getCard();//カードを習得する
    void createCard(PosIndex posIndex);//カードを生成
    void showInitCards();//ゲーム開始時にカードを10枚表示する。
    void initGame();//ゲームを初期化する。
    
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
