#include "HelloWorldScene.h"
#include "GAF.h"
#include "SlotMachine.h"

USING_NS_CC;
USING_NS_GAF;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto asset = GAFAsset::create("slot_machine/slot_machine.gaf", nullptr);
    auto machine = asset->createObjectAndRun(true);
    machine->setPosition(300, 600);
    addChild(machine, 1);
    m_machine = SlotMachine::create(machine);
    if (m_machine)
    {
        m_machine->retain();
    }

    Director::getInstance()->getScheduler()->scheduleUpdate(this, 1, false);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void HelloWorld::update(float dt)
{
    m_machine->update(dt);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 localPoint = m_machine->getArm()->convertTouchToNodeSpace(touch);
    Rect r = m_machine->getArm()->getBoundingBoxForCurrentFrame();
    return r.containsPoint(localPoint);
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    m_machine->start();
}
