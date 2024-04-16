#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG

class BaseKnight;
class BaseItem;
class Events;
class BaseOpponent;
class BaseBag;
class ArmyKnights;
class KnightAdventure;

enum ItemType {Antidote = 0, PhoenixDownI = 1, PhoenixDownII = 2, PhoenixDownIII = 3, PhoenixDownIV = 4};
enum KnightType { PALADIN = 0, LANCELOT = 1, DRAGON = 2, NORMAL = 3};

class BaseBag
{
public:
    class Node
    {
    public:
        BaseItem *item;
        Node *next;
    };
typedef class Node* node;
node makeNode(BaseItem *item)
{
    node tmp=new Node();
    tmp->item=item;
    tmp->next=NULL;
    return tmp;
}
public:
    BaseKnight *knight;
    node head=nullptr;
    int size=0, sizemax=0;
    BaseBag(BaseKnight *knight, int antidote, int phoenixDownI, int sizemax);
    virtual bool insertFirst(BaseItem * item);
    virtual void deleteFirst();
    virtual BaseItem * get(ItemType itemType);
    virtual string toString() const;
    virtual bool heal(ItemType itemType);
};
class BagPaladin : public BaseBag{
public:
    BagPaladin(BaseKnight* knight, int antidote, int phoenixDownI);
};
class BagLancelot : public BaseBag{
public:
    BagLancelot(BaseKnight* knight, int antidote, int phoenixDownI);
};
class BagNormal : public BaseBag{
public:
    BagNormal(BaseKnight* knight, int antidote, int phoenixDownI);
};
class BagDragon : public BaseBag{
public:
    BagDragon(BaseKnight* knight, int antidote, int phoenixDownI);
};

class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag * bag;
    KnightType knightType;
    bool toxic;
public:
    int getHP();
    int getMaxHP();
    int getLV();
    int getGil();
    bool getToxic();
    BaseBag * getBag();
    void setHP(int hp);
    void setLV(int level);
    void setGil(int gil);
    void setToxic(int toxic);
    bool setBag(BaseItem *item);
    void heal1();
public:
    BaseKnight(int id, int maxhp, int level, int gil);
    static BaseKnight * create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;
    KnightType getType() {return knightType;}
    virtual bool fight(BaseOpponent * opponent, int);
    virtual int knightBaseDamage()=0;
};
class PaladinKnight : public BaseKnight{
public:
    PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    int knightBaseDamage() {return this->hp * this->level * 0.06;}
};
class LancelotKnight : public BaseKnight{
public:
    LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    int knightBaseDamage() {return this->hp * this->level * 0.05;}
};
class DragonKnight : public BaseKnight{
public:
    DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    int knightBaseDamage() {return this->hp * this->level *0.075;}
};
class NormalKnight : public BaseKnight{
public:
    NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    int knightBaseDamage() {return 0;}
};

class ArmyKnights {
public:
    static bool WinOmegaWeapon;
    static bool WinHades;
    BaseKnight **knights;
    int count_knights, i=0;
    bool PaladinShield, LancelotSpear, GuinevereHair, ExcaliburSword;
public:
    ArmyKnights (const string & file_armyknights);
    ~ArmyKnights();
    bool fight(BaseOpponent * opponent);
    bool adventure (Events * events);
    int count() const {return this->count_knights;}
    BaseKnight * lastKnight() const {return this->knights[count_knights-1];}

    bool hasPaladinShield() const {return PaladinShield;}
    bool hasLancelotSpear() const {return LancelotSpear;}
    bool hasGuinevereHair() const {return GuinevereHair;}
    bool hasExcaliburSword() const {return ExcaliburSword;}

    void printInfo() const;
    void printResult(bool win) const;
};

class BaseItem { //done
public:
    ItemType itemtype;
    virtual bool canUse ( BaseKnight * knight ) = 0;
    virtual void use ( BaseKnight * knight ) = 0;
    virtual ItemType type(){return itemtype;}
};
class Antidote_Item: public BaseItem{
public:
    Antidote_Item() {itemtype=Antidote;}
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownI_Item: public BaseItem{
public:
    PhoenixDownI_Item() {itemtype=PhoenixDownI;}
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownII_Item: public BaseItem{
public:
    PhoenixDownII_Item() {itemtype=PhoenixDownII;}
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownIII_Item: public BaseItem{
public:
    PhoenixDownIII_Item() {itemtype=PhoenixDownIII;}
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class PhoenixDownIV_Item: public BaseItem{
public:
    PhoenixDownIV_Item() {itemtype=PhoenixDownIV;}
    bool canUse(BaseKnight* knight);
    void use(BaseKnight* knight);
};
class Events { //done
public:
    int *sukien;
    int count_sukien;
    Events(const string &file_events);
    int count() const;
    int get(int i) const;
    ~Events();
};

class BaseOpponent{
public:
    int id_event,gil_bonus,damage;
};
class MadBear_Op: public BaseOpponent{
public:
    MadBear_Op() {id_event=1;gil_bonus=100;damage=10;}
};
class Bandit_Op: public BaseOpponent{
public:
    Bandit_Op() {id_event=2;gil_bonus=150;damage=15;}
};
class LordLupin_Op: public BaseOpponent{
public:
    LordLupin_Op() {id_event=3;gil_bonus=450;damage=45;}
};
class Elf_Op: public BaseOpponent{
public:
    Elf_Op() {id_event=4;gil_bonus=750;damage=75;}
};
class Troll_Op: public BaseOpponent{
public:
    Troll_Op() {id_event=5;gil_bonus=800;damage=95;}
};
class Tornbery_Op: public BaseOpponent{
public:
    Tornbery_Op() {id_event=6;}
};
class QueenOfCards_Op: public BaseOpponent{
public:
    QueenOfCards_Op() {id_event=7;}
};
class NinaDeRings_Op: public BaseOpponent{
public:
    NinaDeRings_Op() {id_event=8;}
};
class DurianGarden_Op: public BaseOpponent{
public:
    DurianGarden_Op() {id_event=9;}

};
class OmegaWeapon_Op: public BaseOpponent{
public:
    OmegaWeapon_Op() {id_event=10;}
};
class Hades_Op: public BaseOpponent{
public:
    Hades_Op() {id_event=11;}
};

class KnightAdventure {
private:
    ArmyKnights * armyKnights;
    Events * events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string &);
    void loadEvents(const string &);
    void run();
};

#endif // __KNIGHT2_H__