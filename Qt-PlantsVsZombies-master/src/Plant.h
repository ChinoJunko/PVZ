#ifndef PLANTS_VS_ZOMBIES_PLANT_H
#define PLANTS_VS_ZOMBIES_PLANT_H

#include <QtCore>
#include <QtWidgets>
#include <QtMultimedia>

class MoviePixmapItem;
class GameScene;
class ZombieInstance;
class Trigger;

class Plant
{
    Q_DECLARE_TR_FUNCTIONS(Plant)

public:
    Plant();
    virtual  ~Plant() {}

    QString eName, cName;
    int width, height;
    int hp, pKind, bKind;//优先级
    int beAttackedPointL, beAttackedPointR;
    int zIndex;
    QString cardGif, staticGif, normalGif;
    bool canEat, canSelect, night;
    double coolTime;
    int stature, sleep;
    int sunNum;
    QString toolTip;

    virtual double getDX() const;
    virtual double getDY(int x, int y) const;
    virtual bool canGrow(int x, int y) const;

    GameScene *scene;
    void update();
};

class PlantInstance
{
public:
    PlantInstance(const Plant *plant);
    virtual ~PlantInstance();

    virtual void birth(int c, int r);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack(ZombieInstance *zombieInstance);
    virtual void getHurt(ZombieInstance *zombie, int aKind, int attack);

    bool contains(const QPointF &pos);

    const Plant *plantProtoType;

    QUuid uuid;
    int row, col;
    int hp;
    bool canTrigger,canVaulted;
    qreal attackedLX, attackedRX;
    QMap<int, QList<Trigger *> > triggers;

    QGraphicsPixmapItem *shadowPNG;
    MoviePixmapItem *picture;
    QMediaPlayer *attackMusic, *dieMusic;
};

//豌豆射手
class Peashooter: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(Peashooter)
public:
    Peashooter();
};

class PeashooterInstance: public PlantInstance
{
public:
    PeashooterInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
};

//双枪射手
class Repeater: public Peashooter
{
    Q_DECLARE_TR_FUNCTIONS(Repeater)
public:
    Repeater();
};

class RepeaterInstance: public PeashooterInstance
{
public:
    RepeaterInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
};

//加特林
class GatlingPea: public Peashooter
{
    Q_DECLARE_TR_FUNCTIONS(GatlingPea)
public:
    GatlingPea();
};

class GatlingPeaInstance: public PeashooterInstance
{
public:
    GatlingPeaInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
};

//寒冰射手
class SnowPea: public Peashooter
{
    Q_DECLARE_TR_FUNCTIONS(SnowPea)
public:
    SnowPea();
};

class SnowPeaInstance: public PeashooterInstance
{
    Q_DECLARE_TR_FUNCTIONS(SnowPea)
public:
    SnowPeaInstance(const Plant *plant);
    virtual void normalAttack(ZombieInstance *zombieInstance);
};

//向日葵
class SunFlower: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(SunFlower)
public:
    SunFlower();
};

class SunFlowerInstance: public PlantInstance
{
public:
    SunFlowerInstance(const Plant *plant);
    virtual void initTrigger();
private:
    QString lightedGif;
};

//芜香蛋(坚果）
class WallNut: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(WallNut)
public:
    WallNut();
    virtual bool canGrow(int x, int y) const;
};

class WallNutInstance: public PlantInstance
{
public:
    WallNutInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void getHurt(ZombieInstance *zombie, int aKind, int attack);
protected:
    int hurtStatus,hurtPoint1,hurtPoint2;
    QString crackedGif1, crackedGif2;
    QMediaPlayer *hurtMusic;
};

//高坚果
class TallNut: public WallNut
{
    Q_DECLARE_TR_FUNCTIONS(TallNut)
public:
    TallNut();
    virtual bool canGrow(int x, int y) const;
};

class TallNutInstance: public WallNutInstance
{
public:
    TallNutInstance(const Plant *plant);
};

//南瓜
class Pumpkin: public WallNut
{
    Q_DECLARE_TR_FUNCTIONS(Pumpkin)
public:
    Pumpkin();
    virtual bool canGrow(int x, int y) const;
};

class PumpkinInstance: public WallNutInstance
{
public:
    PumpkinInstance(const Plant *plant);

    ~PumpkinInstance();

    virtual void birth(int c, int r);

    MoviePixmapItem *backPicture;
};

//火炬
class Torchwood: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(Torchwood)
public:
    Torchwood();
};

class TorchwoodInstance: public PlantInstance
{
public:
    TorchwoodInstance(const Plant *plant);
    virtual void initTrigger();
};

//马老师绝对不用的土豆雷
class PotatoMine: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(PotatoMine)
public:
    PotatoMine();
};

class PotatoMineInstance: public PlantInstance
{
public:
    PotatoMineInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack();
private:
    QString explosionSpudowGif, mashedGif;
};

//瓜皮
class Squash: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(Squash)
public:
    Squash();
};

class SquashInstance: public PlantInstance
{
public:
    SquashInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack();
private:
    QString attackGif;
    qreal target;
};

//樱桃炸弹
class CherryBomb: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(CherryBomb)
public:
    CherryBomb();
};

class CherryBombInstance: public PlantInstance
{
public:
    CherryBombInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack();
private:
    QString BoomGif;
};

//火爆辣椒
class Jalapeno: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(Jalapeno)
public:
    Jalapeno();
};

class JalapenoInstance: public PlantInstance
{
public:
    JalapenoInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack();
private:
    QString BoomGif;
};

class LawnCleaner: public Plant
{
    Q_DECLARE_TR_FUNCTIONS(LawnCleaner)
public:
    LawnCleaner();
};

class LawnCleanerInstance: public PlantInstance
{
public:
    LawnCleanerInstance(const Plant *plant);
    virtual void initTrigger();
    virtual void triggerCheck(ZombieInstance *zombieInstance, Trigger *trigger);
    virtual void normalAttack(ZombieInstance *zombieInstance);
};

class PoolCleaner: public LawnCleaner
{
    Q_DECLARE_TR_FUNCTIONS(PoolCleaner)
public:
    PoolCleaner();
};

class Bullet
{
public:
    Bullet(GameScene *scene, int type, int row, qreal from, qreal x, qreal y, qreal zvalue,  int direction);
    ~Bullet();
    void start();
private:
    void move();

    GameScene *scene;
    int count, type, row, direction;
    bool beFired;
    qreal from;
    QGraphicsPixmapItem *picture;
};

Plant *PlantFactory(GameScene *scene, const QString &eName);
PlantInstance *PlantInstanceFactory(const Plant *plant);

#endif //PLANTS_VS_ZOMBIES_PLANT_H
