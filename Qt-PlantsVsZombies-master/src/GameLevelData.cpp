#include "GameLevelData.h"
#include "GameScene.h"
#include "ImageManager.h"
#include "Timer.h"

GameLevelData::GameLevelData() : cardKind(0),
                                 dKind(1),
                                 sunNum(50),
                                 backgroundImage("interface/background1.jpg"),
                                 LF{ 0, 1, 1, 1, 1, 1 },
                                 canSelectCard(true),
                                 staticCard(true),
                                 showScroll(true),
                                 produceSun(true),
                                 hasShovel(true),
                                 maxSelectedCards(10),
                                 coord(0),
                                 flagNum(0)
{}

void  GameLevelData::loadAccess(GameScene *gameScene)
{
    gameScene->loadAcessFinished();
}

void GameLevelData::startGame(GameScene *gameScene)
{
    initLawnMower(gameScene);
    gameScene->prepareGrowPlants( [gameScene] {
        gameScene->beginBGM();
        gameScene->beginMonitor();
        gameScene->beginCool();
        gameScene->beginSun(25);
        (new Timer(gameScene, 10000, [gameScene] {
            gameScene->beginZombies();
        }))->start();
    });
}

void GameLevelData::initLawnMower(GameScene *gameScene)
{
    for (int i = 0; i < LF.size(); ++i) {
        if (LF[i] == 1)
            gameScene->customSpecial("oLawnCleaner", -1, i);
        else if (LF[i] == 2)
            gameScene->customSpecial("oPoolCleaner", -1, i);
    }
}

/*
void GameLevelData::endGame(GameScene *gameScene)
{
    gameScene->deleteLater();
}
*/


GameLevelData_1::GameLevelData_1()
{
    backgroundImage = "interface/background1unsodded2.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    LF={ 0, 0, 1, 1, 1, 0 };
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "1";
    cName = tr("Level 1-1");
    pName = { "oPeashooter", "oSnowPea", "oSunflower", "oWallNut"};
    zombieData = { { "oZombie", 2, 1, {} }, { "oZombie2", 2, 1, {} }, { "oZombie3", 2, 1, {} },
                   { "oConeheadZombie", 2, 2, {} }, { "oBucketheadZombie", 1, 3, {} }};
    flagNum = 1;
    largeWaveFlag = { 5, 10};
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 5, 10, 12, 14 }, { 3, 5, 7, 10, 15, 30 });
}

GameLevelData_1::~GameLevelData_1(){

}

GameLevelData_2::GameLevelData_2()
{
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "2";
    cName = tr("Level 1-2");
    pName = { "oPeashooter", "oSnowPea", "oSunflower", "oWallNut", "oPotatoMine", "oCherryBomb", "oRepeater", "oJalapeno"};
    zombieData = { { "oZombie", 2, 1, {} }, { "oZombie2", 2, 1, {} }, { "oZombie3", 2, 1, {} },
                   { "oConeheadZombie", 2, 2, {} }, { "oBucketheadZombie", 1, 3, {} }, { "oScreenDoorZombie", 1, 3, {} },
                   { "oNewspaperZombie", 1, 2, {} }};
    flagNum = 1;
    largeWaveFlag = { 1, 10, 15 };
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 5, 10, 12, 14 }, { 3, 5, 7, 10, 15, 24 });
}

GameLevelData_2::~GameLevelData_2(){

}

GameLevelData_3::GameLevelData_3()
{
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 1000;
    canSelectCard = true;
    showScroll = true;
    eName = "3";
    cName = tr("Level 1-3");
    pName = { "oPeashooter", "oSnowPea", "oSunflower", "oWallNut", "oPotatoMine", "oCherryBomb", "oRepeater", "oTorchwood", "oTallNut", "oPumpkin", "oJalapeno"};
    zombieData = { { "oZombie", 2, 1, {} }, { "oZombie2", 2, 1, {} }, { "oZombie3", 2, 1, {} },
                   { "oConeheadZombie", 2, 2, {} }, { "oBucketheadZombie", 1, 3, {} }, { "oScreenDoorZombie", 1, 3, {} },
                   { "oNewspaperZombie", 1, 2, {} }, { "oPoleVaultingZombie", 1, 3, {} }};
    flagNum = 15;
    largeWaveFlag = { 5, 10, 15 };
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 5, 10, 12, 14 }, { 3, 5, 7, 10, 15, 30 });
}

GameLevelData_3::~GameLevelData_3(){

}

GameLevelData_4::GameLevelData_4()
{
    backgroundImage = "interface/background1.jpg";
    backgroundMusic = "qrc:/audio/UraniwaNi.mp3";
    sunNum = 100;
    canSelectCard = true;
    showScroll = true;
    eName = "4";
    cName = tr("Level 1-4");
    pName = { "oPeashooter", "oSnowPea", "oSunflower", "oWallNut", "oPotatoMine", "oCherryBomb", "oRepeater", "oGatlingPea", "oTorchwood", "oTallNut", "oPumpkin", "oJalapeno", "oSquash"};
    zombieData = { { "oZombie", 2, 1, {} }, { "oZombie2", 2, 1, {} }, { "oZombie3", 2, 1, {} },
                   { "oConeheadZombie", 3, 3, {} }, { "oBucketheadZombie", 2, 7, {} }, { "oScreenDoorZombie", 2, 10, {} },
                   { "oNewspaperZombie", 3, 5, {} }, { "oPoleVaultingZombie", 2, 13, {} }, { "oJackinTheBoxZombie", 1, 15, {} }};
    flagNum = 25;//320;
    largeWaveFlag = { 7, 13, 19, 25};
    flagToSumNum = QPair<QList<int>, QList<int> >({3, 6, 9, 12, 15, 18, 21, 24}, { 1, 2, 4, 7, 11, 16, 22, 29, 46});
}

GameLevelData_4::~GameLevelData_4(){

}

GameLevelData *GameLevelDataFactory(const QString &eName)
{
    //可通过反射自动注册，下次再实现
    if (eName == "1")
        return new GameLevelData_1;
    if (eName == "2")
        return new GameLevelData_2;
    if (eName == "3")
        return new GameLevelData_3;
    if (eName == "4")
        return new GameLevelData_4;
    return nullptr;
}
