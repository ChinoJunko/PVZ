#ifndef PLANTS_VS_ZOMBIES_GAMELEVELDATA_H
#define PLANTS_VS_ZOMBIES_GAMELEVELDATA_H

#include <QtCore>

class GameScene;

class ZombieData
{
public:
    QString eName;
    int num;
    int firstFlag;//出现波数
    QList<int> flagList;//先决权
};

class GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData)
public:
    GameLevelData();
    virtual ~GameLevelData() {}

    QString eName, cName;

    QList<QString> pName, zName;
    int cardKind;           // 0: plant     1: zombies
    int dKind;
    int sunNum;

    QString backgroundImage;
    QList<int> LF; // 地板
    bool canSelectCard, staticCard, showScroll, produceSun, hasShovel;
    int maxSelectedCards;
    int coord;

    int flagNum;
    QList<int> largeWaveFlag;
    QPair<QList<int>, QList<int> > flagToSumNum;
    QMap<int, std::function<void(GameScene *)> > flagToMonitor;

    QList<ZombieData> zombieData;

    QString backgroundMusic;

    virtual void loadAccess(GameScene *gameScene);
    virtual void initLawnMower(GameScene *gameScene);
    virtual void startGame(GameScene *gameScene);
    //virtual void endGame(GameScene *gameScene);
};

class GameLevelData_1: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_1)
public:
    GameLevelData_1();
    ~GameLevelData_1();
};

class GameLevelData_2: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_2)
public:
    GameLevelData_2();
    ~GameLevelData_2();
};


class GameLevelData_3: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_3)
public:
    GameLevelData_3();
    ~GameLevelData_3();
};

class GameLevelData_4: public GameLevelData
{
    Q_DECLARE_TR_FUNCTIONS(GameLevelData_4)
public:
    GameLevelData_4();
    ~GameLevelData_4();
};

GameLevelData * GameLevelDataFactory(const QString &eName);

#endif //PLANTS_VS_ZOMBIES_GAMELEVEL_H
