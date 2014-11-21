#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class SlotMachine;

class SlotScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();  

    virtual void update(float dt);

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void switchMachineCallback(Ref* pSender);
    
    CREATE_FUNC(SlotScene);

private:
    SlotMachine* m_machine;
};

#endif // __HELLOWORLD_SCENE_H__
