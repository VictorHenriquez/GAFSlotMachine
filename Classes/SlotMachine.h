#pragma once

#include "cocos2d.h"
#include "GAF.h"

class SlotMachine : public cocos2d::Ref
{
    friend class SlotBar;

    enum class EMachineState : uint16_t
    {
        Initial = 0,
        ArmTouched,
        Spin,
        SpinEnd,
        Win,
        End,

        COUNT
    };

    enum class EPrize : uint16_t
    {
        None = 0,
        C1k,
        C500k,
        C1000k,

        COUNT
    };

    typedef std::vector<int> PrizeBar_t;
    typedef std::vector<PrizeBar_t> PrizeMatrix_t;

public:
    static SlotMachine* create(gaf::GAFObject* mainObject);

    SlotMachine();
    ~SlotMachine();

    bool init(gaf::GAFObject* mainObject);
    void update(float dt);

    void start();

    void onFinishSequence(gaf::GAFObject* object);

private:
    gaf::GAFObject* m_arm;
    gaf::GAFObject* m_whiteBG;
    gaf::GAFObject* m_rewardText;
    gaf::GAFObject* m_bottomCoins;
    gaf::GAFObject* m_centralCoins[3];
    gaf::GAFObject* m_winFrame;
    SlotBar* m_bars[3];

    EMachineState m_state;
    std::string m_rewardType;
    
    void defaultPlacing();
    void nextState();
    void showPrize(EPrize prize);

    std::string getTextByPrize(EPrize prize);
    EPrize m_prize;
    EPrize generatePrize();
    PrizeMatrix_t generateSpinResult(EPrize prize);

    float m_countdown;

    const static std::string s_rewardCoins;
    const static std::string s_rewardChips;
    const static int s_fruitCount;
    const static float s_barTimeout;
};