#include "SlotScene.h"
#include "GAF.h"
#include "SlotMachine.h"

USING_NS_CC;
USING_NS_GAF;

Scene* SlotScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SlotScene::create();
    scene->addChild(layer);
    return scene;
}

bool SlotScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto switchMachineType = MenuItemImage::create(
        "btn_swap.png",
        "btn_swap_down.png",
        CC_CALLBACK_1(SlotScene::switchMachineCallback, this));
    
    switchMachineType->setPosition(Vec2(origin.x + visibleSize.width - switchMachineType->getContentSize().width,
        origin.y + switchMachineType->getContentSize().height));
    
    auto menu = Menu::create(switchMachineType, NULL);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 2);

    auto asset = GAFAsset::create("slot_machine/slot_machine.gaf", nullptr);
    auto machine = asset->createObjectAndRun(true);
    machine->setPosition(0, 1920); // TODO: hardcode
    addChild(machine, 1);
    m_machine = SlotMachine::create(machine);
    if (m_machine)
    {
        m_machine->retain();
    }

    Director::getInstance()->getScheduler()->scheduleUpdate(this, 1, false);

    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(SlotScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(SlotScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

void SlotScene::update(float dt)
{
    m_machine->update(dt);
}

bool SlotScene::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 localPoint = m_machine->getArm()->convertTouchToNodeSpace(touch);
    localPoint = cocos2d::PointApplyTransform(localPoint, m_machine->getArm()->getNodeToParentTransform());
    Rect r = m_machine->getArm()->getBoundingBoxForCurrentFrame();
    return r.containsPoint(localPoint);
}

void SlotScene::onTouchEnded(Touch* touch, Event* event)
{
    m_machine->start();
}

void SlotScene::switchMachineCallback(Ref* pSender)
{
    m_machine->switchType();
}
