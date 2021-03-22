#include "Zombie.h"
#include "GameScene.h"
#include "GameLevelData.h"
#include "ImageManager.h"
#include "MouseEventPixmapItem.h"
#include "Plant.h"
#include "Timer.h"

int max(int a, int b){
    return a>b?a:b;
}

Zombie::Zombie()
    : hp(270), level(1), speed(1.5),
      aKind(0), attack(100),
      canSelect(true), canDisplay(true),
      beAttackedPointL(82), beAttackedPointR(156),
      breakPoint(90), sunNum(0), coolTime(0)
{}

bool Zombie::canPass(int row) const//检测该通道是否开启
{
    return scene->getGameLevelData()->LF[row] == 1;
}

void Zombie::update()
{
    QPixmap pic = gImageCache->load(staticGif);
    width = pic.width();
    height = pic.height();
}

Zombie1::Zombie1()
{
    eName = "oZombie";
    cName = tr("Zombie");
    cardGif = "Card/Zombies/Zombie.png";
    QString path = "Zombies/Zombie/";
    staticGif = path + "0.gif";
    normalGif = path + "Zombie.gif";
    attackGif = path + "ZombieAttack.gif";
    lostHeadGif = path + "ZombieLostHead.gif";
    lostHeadAttackGif = path + "ZombieLostHeadAttack.gif";
    headGif = path + "ZombieHead.gif";
    dieGif = path + "ZombieDie.gif";
    boomDieGif = path + "BoomDie.gif";
    standGif = path + "1.gif";
}

Zombie2::Zombie2()
{
    eName = "oZombie2";
    normalGif = "Zombies/Zombie/Zombie2.gif";
    standGif = "Zombies/Zombie/2.gif";
}

Zombie3::Zombie3()
{
    eName = "oZombie3";
    normalGif = "Zombies/Zombie/Zombie3.gif";
    standGif = "Zombies/Zombie/3.gif";
}

FlagZombie::FlagZombie()
{
    eName = "oFlagZombie";
    cName = tr("Flag Zombie");
    speed = 2.2;
    beAttackedPointR = 101;
    QString path = "Zombies/FlagZombie/";
    cardGif = "Card/Zombies/FlagZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "FlagZombie.gif";
    attackGif = path + "FlagZombieAttack.gif";
    lostHeadGif = path + "FlagZombieLostHead.gif";
    lostHeadAttackGif = path + "FlagZombieLostHeadAttack.gif";
    standGif = path + "1.gif";
}

ZombieInstance::ZombieInstance(const Zombie *zombie)
    : zombieProtoType(zombie), picture(new MoviePixmapItem),
      attackMusic(new QMediaPlayer(picture)),
      hitMusic(new QMediaPlayer(picture)),
      frozenMusic(new QMediaPlayer(picture)),
      dieMusic(new QMediaPlayer(picture)),
      DecelerationLayor(0)
{
    uuid = QUuid::createUuid();
    hp = zombieProtoType->hp;
    speed = zombie->speed;
    posyOffset = 10;
    posxOffset = 0;
    altitude = 1;
    beAttacked = true;
    beSlowed = false;
    isAttacking = false;
    goingDie = false;
    canBlock = false;
    canAttack = true;
    normalGif = zombie->normalGif;
    attackGif = zombie->attackGif;
}

void ZombieInstance::birth(int row)
{
    ZX = attackedLX = zombieProtoType->scene->getCoordinate().getX(11);
    X = attackedLX - zombieProtoType->beAttackedPointL;
    attackedRX = X + zombieProtoType->beAttackedPointR;
    this->row = row;

    Coordinate &coordinate = zombieProtoType->scene->getCoordinate();
    picture->setMovie(normalGif);
    picture->setPos(X - posxOffset, coordinate.getY(row) - zombieProtoType->height - posyOffset);
    picture->setZValue(3 * row + 1);
    shadowPNG = new QGraphicsPixmapItem(gImageCache->load("interface/plantShadow.png"));
    shadowPNG->setPos(zombieProtoType->beAttackedPointL - 10 + posxOffset, zombieProtoType->height - 32 + posyOffset);
    shadowPNG->setFlag(QGraphicsItem::ItemStacksBehindParent);
    shadowPNG->setParentItem(picture);
    picture->start();
    zombieProtoType->scene->addToGame(picture);
}

void ZombieInstance::checkActs()//策略
{
    if (hp < 1) return;
    if (beAttacked && !isAttacking) {
        judgeAttack();
    }
    if (!isAttacking) {
        attackedRX -= beSlowed ? speed/2 : speed;
        ZX = attackedLX -= beSlowed ? speed/2 : speed;
        X -= beSlowed ? speed/2 : speed;
        picture->setX(X - posxOffset);
        if (attackedRX < -50) {
            zombieProtoType->scene->zombieDie(this);
        }
        else if (attackedRX < 100) {
            zombieProtoType->scene->gameStatus = true;
            zombieProtoType->scene->loseGame();
        }
    }
}

void ZombieInstance::judgeAttack()
{
    bool tempIsAttacking = false;
    PlantInstance *plant = nullptr;
    int col = zombieProtoType->scene->getCoordinate().getCol(ZX);
    if (col >= 1 && col <= 9) {
        auto plants = zombieProtoType->scene->getPlant(col, row);
        QList<int> keys = plants.keys();
        qSort(keys.begin(), keys.end(), [](int a, int b) { return b < a; });
        for (auto key: keys) {
            plant = plants[key];
            if (plant->plantProtoType->canEat && plant->attackedRX >= ZX && plant->attackedLX <= ZX) {
                tempIsAttacking = true;
                break;
            }
        }
    }
    if (tempIsAttacking != isAttacking) {
        isAttacking = tempIsAttacking;
        if (isAttacking) {
            picture->setMovie(attackGif);
        }
        else
            picture->setMovie(normalGif);
        picture->start();
    }
    if (isAttacking)
        normalAttack(plant);
}

void ZombieInstance::normalAttack(PlantInstance *plantInstance)
{
    if (!canAttack) return;
    if (qrand() % 2)
        attackMusic->setMedia(QUrl("qrc:/audio/chomp.mp3"));
    else
        attackMusic->setMedia(QUrl("qrc:/audio/chompsoft.mp3"));
    attackMusic->play();

    (new Timer(this->picture, beSlowed ? 1000 : 500, [this] {
        if (qrand() % 2)
            attackMusic->setMedia(QUrl("qrc:/audio/chomp.mp3"));
        else
            attackMusic->setMedia(QUrl("qrc:/audio/chompsoft.mp3"));
        attackMusic->play();
    }))->start();

    QUuid plantUuid = plantInstance->uuid;
    (new Timer(this->picture, beSlowed ? 2000 : 1000, [this, plantUuid] {
        if (beAttacked) {
            PlantInstance *plant = zombieProtoType->scene->getPlant(plantUuid);
            if (plant)
                plant->getHurt(this, zombieProtoType->aKind, zombieProtoType->attack);
            judgeAttack();
        }
    }))->start();
}

ZombieInstance::~ZombieInstance()
{
    picture->deleteLater();
}

void ZombieInstance::crushDie()
{
    if (goingDie)
        return;
    goingDie =  true;
    hp = 0;
    MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
    goingDieHead->setPos(zombieProtoType->beAttackedPointL - posxOffset, -20);
    goingDieHead->setParentItem(picture);
    goingDieHead->start();
    switch (qrand() % 13) {
        case 0: dieMusic->setMedia(QUrl("qrc:/audio/Laugh1.mp3")); break;
        case 1: dieMusic->setMedia(QUrl("qrc:/audio/Laugh2.mp3")); break;
        case 2: dieMusic->setMedia(QUrl("qrc:/audio/Laugh3.mp3")); break;
        case 3: dieMusic->setMedia(QUrl("qrc:/audio/Laugh4.mp3")); break;
        case 4: dieMusic->setMedia(QUrl("qrc:/audio/Laugh5.mp3")); break;
        case 5: dieMusic->setMedia(QUrl("qrc:/audio/Laugh6.mp3")); break;
        case 6: dieMusic->setMedia(QUrl("qrc:/audio/Laugh7.mp3")); break;
        case 7: dieMusic->setMedia(QUrl("qrc:/audio/Laugh8.mp3")); break;
        case 8: dieMusic->setMedia(QUrl("qrc:/audio/Laugh9.mp3")); break;
        case 9: dieMusic->setMedia(QUrl("qrc:/audio/Laugh10.mp3")); break;
        case 10: dieMusic->setMedia(QUrl("qrc:/audio/Laugh11.mp3")); break;
        case 11: dieMusic->setMedia(QUrl("qrc:/audio/Laugh12.mp3")); break;
        default: dieMusic->setMedia(QUrl("qrc:/audio/Laugh13.mp3")); break;
    }
    dieMusic->play();
    shadowPNG->setPixmap(QPixmap());
    picture->stop();
    picture->setPixmap(QPixmap());
    (new Timer(picture, 2000, [this] {
        // TODO: Pole Vaulting Zombie
        zombieProtoType->scene->zombieDie(this);
    }))->start();
}

void ZombieInstance::getPea(int attack, int direction, int type)
{
    if(!canBlock&&type==1) playFireballAudio();
    else playNormalballAudio();
    if(!canBlock&&type==-1) getSlowed(10);
    getHit(attack);
}

void ZombieInstance::getHit(int attack)
{
    if (!beAttacked || goingDie)
        return;
    hp -= attack;
    if (hp < zombieProtoType->breakPoint) {
        //hp = 0;
        if (isAttacking)
            picture->setMovie(zombieProtoType->lostHeadAttackGif);
        else
            picture->setMovie(zombieProtoType->lostHeadGif);
        picture->start();
        MoviePixmapItem *goingDieHead = new MoviePixmapItem(zombieProtoType->headGif);
        goingDieHead->setPos(attackedLX - posxOffset, picture->y() - 20);
        goingDieHead->setZValue(picture->zValue());
        zombieProtoType->scene->addToGame(goingDieHead);
        goingDieHead->start();
        (new Timer(zombieProtoType->scene, 2000, [goingDieHead] {
            goingDieHead->deleteLater();
        }))->start();
        beAttacked = 0;
        autoReduceHp();
    }
    else {
        picture->setOpacity(0.5);
        (new Timer(picture, 100, [this] {
            picture->setOpacity(1);//消隐100毫秒
        }))->start();
    }
}

void ZombieInstance::getBoom(int attack){
    hp -= attack;
    if(hp <= 0){
        MoviePixmapItem *boomDie = new MoviePixmapItem(zombieProtoType->boomDieGif);
        boomDie->setPos(picture->pos());
        zombieProtoType->scene->addToGame(boomDie);
        boomDie->start();
        (new Timer(zombieProtoType->scene, 3000, [boomDie] {
            boomDie->deleteLater();
        }))->start();
        zombieProtoType->scene->zombieDie(this);
    }
}

void ZombieInstance::getSlowed(const int time)//缓速
{
    if(!beSlowed){
        frozenMusic->stop();
        frozenMusic->setMedia(QUrl("qrc:/audio/frozen.mp3"));
        frozenMusic->play();
        picture->setRGB(0,0,255);
        beSlowed = true;
    }
    DecelerationLayor++;
    (new Timer(picture, time*1000, [this] {
        DecelerationLayor--;
        if (DecelerationLayor <= 0){
            beSlowed = false;
            picture->setRGB(255,255,255);
        }
    }))->start();
}

void ZombieInstance::autoReduceHp()//掉头就走：D
{
    switch (qrand() % 13) {
        case 0: dieMusic->setMedia(QUrl("qrc:/audio/Laugh1.mp3")); break;
        case 1: dieMusic->setMedia(QUrl("qrc:/audio/Laugh2.mp3")); break;
        case 2: dieMusic->setMedia(QUrl("qrc:/audio/Laugh3.mp3")); break;
        case 3: dieMusic->setMedia(QUrl("qrc:/audio/Laugh4.mp3")); break;
        case 4: dieMusic->setMedia(QUrl("qrc:/audio/Laugh5.mp3")); break;
        case 5: dieMusic->setMedia(QUrl("qrc:/audio/Laugh6.mp3")); break;
        case 6: dieMusic->setMedia(QUrl("qrc:/audio/Laugh7.mp3")); break;
        case 7: dieMusic->setMedia(QUrl("qrc:/audio/Laugh8.mp3")); break;
        case 8: dieMusic->setMedia(QUrl("qrc:/audio/Laugh9.mp3")); break;
        case 9: dieMusic->setMedia(QUrl("qrc:/audio/Laugh10.mp3")); break;
        case 10: dieMusic->setMedia(QUrl("qrc:/audio/Laugh11.mp3")); break;
        case 11: dieMusic->setMedia(QUrl("qrc:/audio/Laugh12.mp3")); break;
        default: dieMusic->setMedia(QUrl("qrc:/audio/Laugh13.mp3")); break;
    }
    dieMusic->play();
    (new Timer(picture, 1000, [this] {
        hp-= 60;
        if (hp < 1)
            normalDie();
        else
            autoReduceHp();
    }))->start();//Timer匿名递归循环
}

void ZombieInstance::normalDie()
{
    if (goingDie)
        return;
    goingDie =  true;
    hp = 0;
    picture->setMovie(zombieProtoType->dieGif);
    picture->start();
    (new Timer(picture, 2500, [this] {
        zombieProtoType->scene->zombieDie(this);
    }))->start();
}

void ZombieInstance::playNormalballAudio()
{
    hitMusic->stop();
    switch (qrand() % 3) {
        case 0: hitMusic->setMedia(QUrl("qrc:/audio/splat1.mp3")); break;
        case 1: hitMusic->setMedia(QUrl("qrc:/audio/splat2.mp3")); break;
        default: hitMusic->setMedia(QUrl("qrc:/audio/splat3.mp3")); break;
    }
    hitMusic->play();
}

void ZombieInstance::playFireballAudio()
{
    hitMusic->stop();
    hitMusic->setMedia(QUrl("qrc:/audio/firepea.mp3"));
    hitMusic->play();
}



OrnZombieInstance1::OrnZombieInstance1(const Zombie *zombie)
    : ZombieInstance(zombie)
{
    ornHp = getZombieProtoType()->ornHp;
    hasOrnaments = true;
}

const OrnZombie1 *OrnZombieInstance1::getZombieProtoType()
{
    return static_cast<const OrnZombie1 *>(zombieProtoType);
}

void OrnZombieInstance1::getHit(int attack)
{
    if (hasOrnaments) {
        ornHp -= attack;
        if (ornHp < 1) {
            hp += ornHp;
            hasOrnaments = false;
            canBlock = false;
            normalGif = getZombieProtoType()->ornLostNormalGif;
            attackGif = getZombieProtoType()->ornLostAttackGif;
            picture->setMovie(isAttacking ? attackGif : normalGif);
            picture->start();
        }
        picture->setOpacity(0.5);
        (new Timer(picture, 100, [this] {
            picture->setOpacity(1);
        }))->start();
    }
    else
        ZombieInstance::getHit(attack);
}

ConeheadZombie::ConeheadZombie()
{
    eName = "oConeheadZombie";
    cName = tr("Conehead Zombie");
    ornHp = 370;
    level = 2;
    sunNum = 75;
    QString path = "Zombies/ConeheadZombie/";
    cardGif = "Card/Zombies/ConeheadZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "ConeheadZombie.gif";
    attackGif = path + "ConeheadZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie.gif";
    ornLostAttackGif = "Zombies/Zombie/ZombieAttack.gif";
    standGif = path + "1.gif";
}

ConeheadZombieInstance::ConeheadZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{}

void ConeheadZombieInstance::playNormalballAudio()
{
    if (hasOrnaments) {
        hitMusic->stop();
        hitMusic->setMedia(QUrl("qrc:/audio/plastichit.mp3"));
        hitMusic->play();
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}

BucketheadZombieInstance::BucketheadZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{}

void BucketheadZombieInstance::playNormalballAudio()
{
    if (hasOrnaments) {
        hitMusic->stop();
        if (qrand() % 2)
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit.mp3"));
        else
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit2.mp3"));
        hitMusic->play();
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}

BucketheadZombie::BucketheadZombie()
{
    eName = "oBucketheadZombie";
    cName = tr("Buckethead Zombie");
    ornHp = 1100;
    level = 3;
    sunNum = 125;
    QString path = "Zombies/BucketheadZombie/";
    cardGif = "Card/Zombies/BucketheadZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "BucketheadZombie.gif";
    attackGif = path + "BucketheadZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie2.gif";
    standGif = path + "1.gif";
}

ScreenDoorZombie::ScreenDoorZombie()
{
    eName = "oScreenDoorZombie";
    cName = tr("Screen Door Zombie");
    ornHp = 1100;
    level = 3;
    sunNum = 150;
    QString path = "Zombies/ScreenDoorZombie/";
    cardGif = "Card/Zombies/ScreenDoorZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "ScreenDoorZombie.gif";
    attackGif = path + "ScreenDoorZombieAttack.gif";
    ornLostNormalGif =  "Zombies/Zombie/Zombie.gif";
    ornLostAttackGif = "Zombies/Zombie/ZombieAttack.gif";
    standGif = path + "1.gif";
}

ScreenDoorZombieInstance::ScreenDoorZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{
    canBlock = true;
    posyOffset = 0;
}

void ScreenDoorZombieInstance::playNormalballAudio()
{
    if (hasOrnaments) {
        hitMusic->stop();
        if (qrand() % 2)
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit.mp3"));
        else
            hitMusic->setMedia(QUrl("qrc:/audio/shieldhit2.mp3"));
        hitMusic->play();
    }
    else
        OrnZombieInstance1::playNormalballAudio();
}

NewspaperZombie::NewspaperZombie()
{
    eName = "oNewspaperZombie";
    cName = tr("Newspaper Zombie");
    ornHp = 150;
    level = 2;
    sunNum = 100;
    QString path = "Zombies/NewspaperZombie/";
    lostHeadGif = path + "LostHeadWalk0.gif";
    lostHeadAttackGif = path + "ZombieLostHeadAttack0.gif";
    headGif = path + "Head.gif";
    dieGif = path + "Die.gif";
    cardGif = "Card/Zombies/NewspaperZombie.png";
    staticGif = path + "0.gif";
    normalGif = path + "HeadWalk1.gif";
    attackGif = path + "HeadAttack1.gif";
    ornLostNormalGif =  "Zombies/NewspaperZombie/HeadWalk0.gif";
    ornLostAttackGif = "Zombies/NewspaperZombie/HeadAttack0.gif";
    standGif = path + "1.gif";
}

NewspaperZombieInstance::NewspaperZombieInstance(const Zombie *zombie)
    : OrnZombieInstance1(zombie)
{
    canBlock = true;
    posyOffset = 0;
}

void NewspaperZombieInstance::getHit(int attack)
{
    if (hasOrnaments) {
        ornHp -= attack;
        if (ornHp < 1) {
            hp += ornHp;
            hasOrnaments = false;
            canBlock = false;
            normalGif = getZombieProtoType()->ornLostNormalGif;
            attackGif = getZombieProtoType()->ornLostAttackGif;
            picture->setMovie("Zombies/NewspaperZombie/LostNewspaper.gif");
            picture->start();
            canAttack = false;
            speed = 0;
            QMediaPlayer *angryMusic = new QMediaPlayer(picture);
            angryMusic->setMedia(QUrl("qrc:/audio/zhemelao.mp3"));
            angryMusic->play();
            (new Timer(picture, 1600,[this, angryMusic]{
                angryMusic->deleteLater();
                picture->setMovie(isAttacking?attackGif:normalGif);
                picture->start();
                canAttack = true;
                speed = 5.3;
            }))->start();
        }
        picture->setOpacity(0.5);
        (new Timer(picture, 100, [this] {
            picture->setOpacity(1);
        }))->start();
    }
    else
        ZombieInstance::getHit(attack);
}

PoleVaultingZombie::PoleVaultingZombie()
{
    eName = "oPoleVaultingZombie";
    cName = tr("Pole Vaulting Zombie");
    hp = 500;
    speed = 3.2;//3.2
    level = 2;
    sunNum = 75;
    cardGif = "Card/Zombies/PoleVaultingZombie.png";
    QString path = "Zombies/PoleVaultingZombie/";
    staticGif = path + "0.gif";
    normalGif = path + "PoleVaultingZombie.gif";
    attackGif = path + "PoleVaultingZombieAttack.gif";
    lostHeadGif = path + "PoleVaultingZombieLostHead.gif";
    lostHeadAttackGif = path + "PoleVaultingZombieLostHeadAttack.gif";
    headGif = path + "PoleVaultingZombieHead.gif";
    dieGif = path + "PoleVaultingZombieDie.gif";
    boomDieGif = path + "BoomDie.gif";
    standGif = path + "1.gif";
    jumpGif1 = path + "PoleVaultingZombieJump.gif";
    jumpGif2 = path + "PoleVaultingZombieJump2.gif";
}

PoleVaultingZombieInstance::PoleVaultingZombieInstance(const Zombie *zombie)
    : ZombieInstance(zombie),
      hasPole(true)
{
    posyOffset = -2;
    posxOffset = 132;
}

void PoleVaultingZombieInstance::judgeAttack()
{
    bool tempIsAttacking = false;
    PlantInstance *plant = nullptr;
    int col = zombieProtoType->scene->getCoordinate().getCol(ZX);
    if (col >= 1 && col <= 9) {
        auto plants = zombieProtoType->scene->getPlant(col, row);
        QList<int> keys = plants.keys();
        qSort(keys.begin(), keys.end(), [](int a, int b) { return b < a; });
        for (auto key: keys) {
            plant = plants[key];
            if (plant->plantProtoType->canEat && plant->attackedRX >= (hasPole?ZX-40:ZX) && plant->attackedLX <= ZX) {
                tempIsAttacking = true;
                break;
            }
        }
    }
    if(tempIsAttacking&&hasPole){
        speed = 0;
        hasPole = false;
        picture->setMovie(((PoleVaultingZombie *)zombieProtoType)->jumpGif1);
        picture->start();
        QMediaPlayer *jumpMusic = new QMediaPlayer(picture);
        QMediaPlayer *voiceMusic = new QMediaPlayer(picture);
        jumpMusic->setMedia(QUrl("qrc:/audio/polevault.mp3"));
        voiceMusic->setMedia(QUrl("qrc:/audio/qifei.mp3"));
        jumpMusic->play();
        voiceMusic->play();
        (new Timer(picture, 1000, [this, jumpMusic, voiceMusic, plant] {
            picture->setMovie(((PoleVaultingZombie *)zombieProtoType)->jumpGif2);
            picture->start();
            if(plant->canVaulted){
                attackedRX -= 110;
                ZX = attackedLX -= 110;
                X -= 110;
            }
            picture->setX(X - posxOffset);
            (new Timer(picture, 1000, [this, jumpMusic, voiceMusic] {
                speed = 1.5;
                normalGif = "Zombies/PoleVaultingZombie/PoleVaultingZombieWalk.gif";
                picture->setMovie(normalGif);
                //posxOffset = 0;
                picture->start();
                jumpMusic->deleteLater();
                voiceMusic->deleteLater();
            }))->start();
        }))->start();
    }

    if (speed==0)  tempIsAttacking = false;//防止动画期间僵尸攻击

    if (tempIsAttacking != isAttacking) {
        isAttacking = tempIsAttacking;
        if (isAttacking) {
            picture->setMovie(attackGif);
        }
        else
            picture->setMovie(normalGif);
        picture->start();
    }
    if (isAttacking)
        normalAttack(plant);
}

JackInTheBoxZombie::JackInTheBoxZombie()
{
    eName = "oJackinTheBoxZombie";
    cName = tr("Jack-in-the-Box Zombie");
    hp = 500;
    speed = 3;//3.6
    level = 3;
    sunNum = 100;
    beAttackedPointL = 102;
    beAttackedPointR = 176;
    cardGif = "Card/Zombies/JackboxZombie.png";
    QString path = "Zombies/JackinTheBoxZombie/";
    staticGif = path + "0.gif";
    normalGif = path + "Walk.gif";
    attackGif = path + "Attack.gif";
    lostHeadGif = path + "LostHead.gif";
    lostHeadAttackGif = path + "LostHeadAttack.gif";
    dieGif = path + "Die.gif";
    boomDieGif = path + "BoomDie.gif";
    standGif = path + "1.gif";
    goOutGif = path + "GoOut.gif";
    boomGif = path + "Boom.gif";
}

JackInTheBoxZombieInstance::JackInTheBoxZombieInstance(const Zombie *zombie)
    : ZombieInstance(zombie),
      BoxMusic(new QMediaPlayer(picture))
{
    posyOffset = 0;
    posxOffset = 20;
}

void JackInTheBoxZombieInstance::birth(int row){
    (new Timer(picture, 2000, [this] {
        boom();
    }))->start();
    ZombieInstance::birth(row);
    BoxMusic->setMedia(QUrl("qrc:/audio/jackinthebox.mp3"));
    BoxMusic->play();
}

void JackInTheBoxZombieInstance::boom(){
    if((qrand()%max(2,(ZX-200)/4))==0){
        BoxMusic->stop();
        hp = 10000;//无敌
        canAttack = false;
        speed = 0;
        picture->setMovie(((JackInTheBoxZombie *)zombieProtoType)->goOutGif);
        picture->start();
        QMediaPlayer *boomMusic = new QMediaPlayer(zombieProtoType->scene);
        QMediaPlayer *voiceMusic = new QMediaPlayer(picture);
        QMediaPlayer *dieMusic = new QMediaPlayer(zombieProtoType->scene);
        boomMusic->setMedia(QUrl("qrc:/audio/explosion.mp3"));
        voiceMusic->setMedia(QUrl("qrc:/audio/Joker.mp3"));
        dieMusic->setMedia(QUrl("qrc:/audio/JokerDie.mp3"));
        (new Timer(picture, 300, [this, voiceMusic] {
            voiceMusic->play();
        }))->start();
        (new Timer(picture, 1400, [this, boomMusic, dieMusic] {
            boomMusic->play();
            dieMusic->play();
            MoviePixmapItem* boomPixmap = new MoviePixmapItem;
            boomPixmap->setPos(picture->pos()+QPointF(-10,-25));
            boomPixmap->setZValue(picture->zValue());
            boomPixmap->setMovie(((JackInTheBoxZombie *)zombieProtoType)->boomGif);
            boomPixmap->start();
            zombieProtoType->scene->addToGame(boomPixmap);
            (new Timer(zombieProtoType->scene, 1200, [boomPixmap] {
                boomPixmap->deleteLater();
            }))->start();
            Coordinate &coordinate = zombieProtoType->scene->getCoordinate();
            int col = coordinate.getCol(ZX);
            for (int r = row>1?row-1:1; r <= coordinate.rowCount() && r <= row + 1; ++r) {
                for (int c = col>0?col-1:0; c <= coordinate.colCount() && c <= col + 1; ++c) {
                   QMap<int, PlantInstance *> pmap = zombieProtoType->scene->getPlant(c,r);
                   for (auto iter = pmap.begin(); iter != pmap.end();iter++) {
                           (*iter)->getHurt(this,1,1800);
                   }
                }
            }
            getBoom(30000);
        }))->start();
        (new Timer(picture, 5000, [this, boomMusic, voiceMusic, dieMusic] {
            voiceMusic->deleteLater();
            boomMusic->deleteLater();
            dieMusic->deleteLater();
        }))->start();
    }
    else
        (new Timer(picture, 250, [this] {
            boom();
        }))->start();
}

Zombie *ZombieFactory(GameScene *scene, const QString &ename)//僵尸生成工厂
{
    Zombie *zombie = nullptr;
    if (ename == "oZombie")
        zombie = new Zombie1;
    else if (ename == "oZombie2")
        zombie = new Zombie2;
    else if (ename == "oZombie3")
        zombie = new Zombie3;
    else if (ename == "oFlagZombie")
        zombie = new FlagZombie;
    else if (ename == "oConeheadZombie")
        zombie = new ConeheadZombie;
    else if (ename == "oBucketheadZombie")
        zombie = new BucketheadZombie;
    else if (ename == "oScreenDoorZombie")
        zombie = new ScreenDoorZombie;
    else if (ename == "oNewspaperZombie")
        zombie = new NewspaperZombie;
    else if (ename == "oPoleVaultingZombie")
        zombie = new PoleVaultingZombie;
    else if (ename == "oJackinTheBoxZombie")
        zombie = new JackInTheBoxZombie;
    if (zombie) {
        zombie->scene = scene;
        zombie->update();
    }
    return zombie;
}

ZombieInstance *ZombieInstanceFactory(const Zombie *zombie)//实例工厂
{
    if (zombie->eName == "oConeheadZombie")
        return new ConeheadZombieInstance(zombie);
    else if (zombie->eName == "oBucketheadZombie")
        return new BucketheadZombieInstance(zombie);
    else if (zombie->eName == "oScreenDoorZombie")
        return new ScreenDoorZombieInstance(zombie);
    else if (zombie->eName == "oNewspaperZombie")
        return new NewspaperZombieInstance(zombie);
    else if (zombie->eName == "oPoleVaultingZombie")
        return new PoleVaultingZombieInstance(zombie);
    else if (zombie->eName == "oJackinTheBoxZombie")
        return new JackInTheBoxZombieInstance(zombie);
    return new ZombieInstance(zombie);
}
