#include "HelloWorldScene.h"

#define ZORDER_SHOW_CARD 1//表示しているカードのオーダー
#define ZORDER_MOVING_CARD 2//移動しているカードのオーダー

#define TAG_TRUSH_CARD 11//捨てられたカードのタグ

#define MOVING_TIME 0.3//カードアニメーションの時間


USING_NS_CC;

/**********************************************************************
*           HelloWorld
**********************************************************************/

Scene* HelloWorld::createScene()
{
    //シーンを作成する。
    auto scene = Scene::create();
    
    //HelloWorldレイヤーを作成する。
    auto layer = HelloWorld::create();

    //シーンに対してレイヤーを追加する。
    scene->addChild(layer);

    //シーンを返す。
    return scene;
}

//HelloWorldクラスのレイヤーの初期か処理を行う
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    //シングルタップイベント収得
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(_swallowsTouches);
    
    //イベント関数の割当
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchCancelled= CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    
    //イベントを追加する
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //ゲームを初期化する
    initGame();
    
    return true;
}

void HelloWorld::initCards(){
    
    _cards.clear();
    
    for(int type = 0; type < CARD_TYPE_NUM; type++)
    {
          for(int number = 1; number <= CARD_NUM; number++)
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
    auto card = CardSprite::create();
    card->setCard(getCard());
    card->setPosIndex(posIndex);
    card->moveToInitPos();
    addChild(card, ZORDER_SHOW_CARD);
}

void HelloWorld::showInitCards()
{
    for(int tag = 1; tag <= 10; tag++)
    {
        auto card = getChildByTag(tag);
        if (card) {
            //カードが残っている場合は削除
            card->removeFromParent();
        }
    }
    
    //5列 x 2行分の行列
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 2; y++) {
            PosIndex posIndex;
            posIndex.x = x;
            posIndex.y = y;
            
            //カードの作成
            createCard(posIndex);
        }
    }
}

void HelloWorld::initGame()
{
    initCards();
    
    showInitCards();
}

CardSprite* HelloWorld::getTouchCard(Touch *touch)
{
    for (int tag = 1; tag <= 10; tag++) {
        //表示されているカードを収得する
        auto card = (CardSprite*)getChildByTag(tag);
        if (card &&
            card != _firstCard &&
            card->getBoundingBox().containsPoint(touch->getLocation()))
        {
            return card;
        }
    }
    return nullptr;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event)
{
    //タップされたカードを収得する
    _firstCard = getTouchCard(touch);
    if (_firstCard) {
        //場に出ているカードがタップされた場合
        
        //Zオーダーを変更する
        _firstCard->setLocalZOrder(ZORDER_SHOW_CARD);
        
        return true;
    }
    
    return false;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *unused_event)
{
    //スワイプしているカードの位置を変更
    _firstCard->setPosition(_firstCard->getPosition() + touch->getDelta());
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
    bool success = false;
    
    //タップしたカードの収得
    auto _secondSprite = getTouchCard((touch));
    if (_secondSprite) {
        //2枚選択
        if (_firstCard->getCard().number + _secondSprite->getCard().number == 13) {
            //2枚のカードを足して13になる
            success = true;
        }
    }
    else
    {
        //1枚せん選択
        if (_firstCard->getCard().number == 13) {
            //1枚のカードで13になる
            success = true;
        }
    }
    
    if (success) {
        //新しいカードを配置する
        if ((int)_cards.size() > 0) {
            createCard(_firstCard->getPosIndex());
        }
        
        //カードを捨てる
        _firstCard->moveToTrash();
        
        if(_secondSprite)
        {
            //もう一枚の新しいカードを配置する
            if ((int)_cards.size() > 0) {
                createCard(_secondSprite->getPosIndex());
            }
            
            //カードを捨てる
            _secondSprite->moveToTrash();
        }
    }
    else
    {
        //カードを元の位置に戻す
        _firstCard->moveBackTOInitPos();
        _firstCard->setLocalZOrder(ZORDER_SHOW_CARD);
    }
    
    //タップしているカードの指定を外す
    _firstCard = nullptr;
}

void HelloWorld::onTouchCancelled(Touch *touch, Event *unused_event)
{
    //タップ終了と同じ処理を行う
    onTouchEnded(touch, unused_event);
}

/**********************************************************************
 *           CardSprite
 **********************************************************************/

bool CardSprite::init()
{
    if (!Sprite::init())
    {
        return false;
    }
    return true;
}

void CardSprite::onEnter()
{
    Sprite::onEnter();
    
    //画像の表示
    setTexture("card_back.png");
    
    //カード位置とタグを指定
    setTag(_posIndex.x + _posIndex.y * 5 +1);
}

std::string CardSprite::getFileName(CardType cardType)
{
    std::string filename;
    switch (cardType) {
        case Clubs: filename = "card_clubs.png"; break;
        case Diamonds: filename = "card_diamonds.png"; break;
        case Hearts: filename = "card_hearts.png"; break;
        default:    filename = "card_spades.png"; break;
    }
    
    return filename;
}

void CardSprite::showNumber()
{
    //表示する数字の収得
    std::string numberString;
    switch (_card.number) {
        case 1:  numberString = "A"; break;
        case 11: numberString = "J"; break;
        case 12: numberString = "Q"; break;
        case 13: numberString = "K"; break;
        default: numberString = StringUtils::format("%d", _card.number);  break;
    }
    
    //表示する文字の収得
    Color4B textColor;
    switch (_card.type) {
        case Clubs:
        case Spades:
            textColor = Color4B::BLACK;
            break;
            
        case Diamonds:
        case Hearts:
            textColor = Color4B::RED;
            break;
    }
    
    //ラベルの生成
    auto number = Label::createWithSystemFont(numberString, "Arial", 96);
    number->setPosition((Point(getContentSize() / 2)));
    number->setTextColor(textColor);
    addChild(number);
}

void CardSprite::moveBackTOInitPos()
{
    float posX = CARD_1_POS_X + CARD_DISTANCE_X * _posIndex.x;
    float posY = CARD_1_POS_Y + CARD_DISTANCE_Y * _posIndex.y;
    auto move = MoveTo::create(MOVING_TIME, Point(posX, posY));

    runAction(move);
}

void CardSprite::moveToTrash()
{
    //移動アニメーションの作成
    float posX = CARD_1_POS_X + CARD_DISTANCE_X * 4;
    float posY = CARD_1_POS_Y - CARD_DISTANCE_Y;
    auto move = MoveTo::create(MOVING_TIME, Point(posX, posY));
    
    //アニメーション後に呼び出す関数の作成
    auto func = CallFunc::create([&](){
        this->setTag(TAG_TRUSH_CARD);
    });
    
    //アクションの直列結合
    auto seq = Sequence::create(move, func, nullptr);
    
    //アニメーションの実行
    runAction(seq);
}

void CardSprite::moveToInitPos()
{
    //移動アニメーションの作成
    float posX = CARD_1_POS_X + CARD_DISTANCE_X * _posIndex.x;
    float posY = CARD_1_POS_Y + CARD_DISTANCE_Y * _posIndex.y;
    auto move = MoveTo::create(MOVING_TIME, Point(posX, posY));
    
    runAction(move);
    
    //カード回転アニメーションの作成
    auto scale1 = ScaleTo::create(MOVING_TIME / 2, 0, 1);
    auto func1 = CallFunc::create([&]{
        //画像の表示
        setTexture(getFileName(_card.type));
        
        //数字の表示
        showNumber();
    });
    auto scale2 = ScaleTo::create(MOVING_TIME /2 , 1, 1);
    auto seq1 = Sequence::create(scale1, func1, scale2, nullptr);
    
    //アクションの並列実行
    auto spawn = Spawn::create(move, seq1, nullptr);
    
    //アニメーションの実行
    runAction(spawn);
}

