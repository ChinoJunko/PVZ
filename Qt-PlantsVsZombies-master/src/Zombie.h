#ifndef PLANTS_VS_ZOMBIES_ZOMBIE_H
#define PLANTS_VS_ZOMBIES_ZOMBIE_H


#include <QtCore>
#include <QtWidgets>
#include <QtMultimedia>

class MoviePixmapItem;
class GameScene;
class PlantInstance;

//僵尸基类
class Zombie
{
    Q_DECLARE_TR_FUNCTIONS(Zombie)
public:
    Zombie();
    virtual ~Zombie() {}

    QString eName, cName;

    int width, height;

    int hp, level;
    qreal speed;
    int aKind, attack;
    bool canSelect, canDisplay;

    QString cardGif, staticGif, normalGif, attackGif, lostHeadGif,
            lostHeadAttackGif, headGif, dieGif, boomDieGif, standGif;

    int beAttackedPointL, beAttackedPointR;
    int breakPoint, sunNum;
    qreal coolTime;

    virtual bool canPass(int row) const;

    void update();

    GameScene *scene;
};

//普通僵尸1
class Zombie1: public Zombie
{
    Q_DECLARE_TR_FUNCTIONS(Zombie1)
public:
    Zombie1();
};

//普通僵尸2
class Zombie2: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(Zombie2)
public:
    Zombie2();
};

//普通僵尸3
class Zombie3: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(Zombie3)
public:
    Zombie3();
};

//旗帜僵尸
class FlagZombie: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(FlagZombie)
public:
    FlagZombie();
};

//僵尸实例
class ZombieInstance
{
public:
    ZombieInstance(const Zombie *zombie);
    virtual ~ZombieInstance();

    virtual void birth(int row);
    virtual void checkActs();
    virtual void judgeAttack();
    virtual void normalAttack(PlantInstance *plant);
    virtual void crushDie();
    virtual void getPea(int attack, int direction, int type);
    virtual void getHit(int attack);
    virtual void getBoom(int attack);
    virtual void getSlowed(const int time);
    virtual void autoReduceHp();
    virtual void normalDie();
    virtual void playNormalballAudio();
    virtual void playFireballAudio();

    QUuid uuid;
    int hp;
    qreal speed;
    int altitude;
    int posyOffset,posxOffset;
    bool beAttacked, isAttacking, goingDie, beSlowed, canBlock, canAttack;
    int DecelerationLayor;

    qreal X, ZX;
    qreal attackedLX, attackedRX;
    int row;
    const Zombie *zombieProtoType;

    QString normalGif, attackGif;//常态gif,攻击gif

    QGraphicsPixmapItem *shadowPNG;
    MoviePixmapItem *picture;
    QMediaPlayer *attackMusic, *hitMusic, *frozenMusic, *dieMusic;
};

//障碍僵尸
class OrnZombie1: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(OrnZombie1)
public:
    int ornHp;
    QString ornLostNormalGif, ornLostAttackGif;
};

class OrnZombieInstance1: public ZombieInstance
{
public:
    OrnZombieInstance1(const Zombie *zombie);
    const OrnZombie1 *getZombieProtoType();
    virtual void getHit(int attack);

    int ornHp;
    bool hasOrnaments;
};

//路障僵尸
class ConeheadZombie: public OrnZombie1
{
    Q_DECLARE_TR_FUNCTIONS(ConeheadZombie)
public:
    ConeheadZombie();
};

class ConeheadZombieInstance: public OrnZombieInstance1
{
public:
    ConeheadZombieInstance(const Zombie *zombie);
    virtual void playNormalballAudio();

};

//铁桶僵尸
class BucketheadZombie: public ConeheadZombie
{
    Q_DECLARE_TR_FUNCTIONS(BucketheadZombie)
public:
    BucketheadZombie();
};

class BucketheadZombieInstance: public OrnZombieInstance1
{
public:
    BucketheadZombieInstance(const Zombie *zombie);
    virtual void playNormalballAudio();

};

//铁栅门僵尸
class ScreenDoorZombie: public OrnZombie1
{
    Q_DECLARE_TR_FUNCTIONS(ScreenDoorZombie)
public:
    ScreenDoorZombie();
};

class ScreenDoorZombieInstance: public OrnZombieInstance1
{
public:
    ScreenDoorZombieInstance(const Zombie *zombie);
    virtual void playNormalballAudio();

};

//报纸僵尸
class NewspaperZombie: public OrnZombie1
{
    Q_DECLARE_TR_FUNCTIONS(NewspaperZombie)
public:
    NewspaperZombie();
};

class NewspaperZombieInstance: public OrnZombieInstance1
{
public:
    NewspaperZombieInstance(const Zombie *zombie);
    virtual void getHit(int attack);
};

//起飞僵尸
class PoleVaultingZombie: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(PoleVaultingZombie)
public:
    PoleVaultingZombie();
    QString jumpGif1,jumpGif2;
};

class PoleVaultingZombieInstance: public ZombieInstance
{
public:
    bool hasPole;
    PoleVaultingZombieInstance(const Zombie *zombie);
    virtual void judgeAttack();
};

//沙口僵尸
class JackInTheBoxZombie: public Zombie1
{
    Q_DECLARE_TR_FUNCTIONS(JackInTheBoxZombie)
public:
    JackInTheBoxZombie();
    QString goOutGif,boomGif;
};

class JackInTheBoxZombieInstance: public ZombieInstance
{
public:
    JackInTheBoxZombieInstance(const Zombie *zombie);
    virtual void birth(int row);
    virtual void boom();

    QMediaPlayer *BoxMusic;
};

Zombie *ZombieFactory(GameScene *scene, const QString &ename);
ZombieInstance *ZombieInstanceFactory(const Zombie *zombie);

#endif //PLANTS_VS_ZOMBIES_ZOMBIE_H
