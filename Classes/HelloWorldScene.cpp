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
    
    initGame();
    
    return true;
}

void HelloWorld::initCards(){
    
    _cards.clear();
    
    for(int type = 0; type < CARD_TYPE_NUM; type++)
    {
          for(int number = 0; number < CARD_NUM; number++)
          {
              Card card;
              card.number = number;
              card.type = (CardType)type;
              
              _cards.push_back(card);
          }
    }
}

Card HelloWorld::getCard()
{
    std::random_device rd;
    std::mt19937 rand = std::mt19937(rd());
    
    int index = std::uniform_int_distribution<int>(0, (int)_cards.size() - 1)(rand);
    
    //カードの値を習得する
    auto card = _cards[index];
    
    //一時カードから削除
    _cards .erase(_cards.begin() + index);
    
    return card;
}

void HelloWorld::createCard(PosIndex posIndex)
{
    //新しいカードを作成する
    auto card = Sprite::create("card_spades.png");
    card->setPosition(CARD_1_POS_X + CARD_DISTANCE_X * posIndex.x,
                      CARD_1_POS_Y + CARD_DISTANCE_Y * posIndex.y);
    addChild(card, ZORDER_SHOW_CARD);
}

void HelloWorld::showInitCards()
{
    for(int tag = 1; tag <= 10; tag++)
    {
        auto card = getChildByTag(tag);
        if (card) {
            card->removeFromParent();
        }
    }
    
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 2; y++) {
            PosIndex posIndex;
            posIndex.x = x;
            posIndex.y = y;
            
            createCard(posIndex);
        }
    }
}

void HelloWorld::initGame()
{
    initCards();
    
    showInitCards();
}
