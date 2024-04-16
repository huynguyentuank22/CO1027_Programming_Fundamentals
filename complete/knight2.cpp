#include "knight2.h"

bool ArmyKnights::WinHades = false;
bool ArmyKnights::WinOmegaWeapon = false;
// check Paladin
bool isPrime(int x)
{
    if (x <= 1)
        return 0;
    for (int i = 2; i * i <= x; ++i)
    {
        if (x % i == 0)
            return 0;
    }
    return 1;
}

// check Lancelot
bool is888(int x)
{
    if (x == 888)
        return 1;
    else
        return 0;
}

// check Hiep si rong
bool isDragon(int x)
{
    int Pytago[6] = {345, 354, 435, 453, 534, 543};
    for (int i = 0; i < 6; i++)
    {
        if (x == Pytago[i])
            return 1;
    }
    return 0;
}

/* * * BEGIN implementation of class BaseBag * * */
BaseBag::BaseBag(BaseKnight *knight, int antidote, int phoenixDownI, int sizemax)
{
    this->knight=knight;
    this->size=0;
    this->sizemax=sizemax;
    this->head=nullptr;
    for(int i=0;i<phoenixDownI;i++) {this->insertFirst(new PhoenixDownI_Item());}
    for(int i=0;i<antidote;i++) {this->insertFirst(new Antidote_Item());}
}
bool BaseBag::insertFirst(BaseItem *item) //check co chen duoc vao nua ko
{
    if((size==sizemax && knight -> getType() != PALADIN) || (item->type()==Antidote && knight->getType()==DRAGON)) return 0;
    else
    {
        node tmp = makeNode(item);
        tmp->next=head;
        head=tmp;
        size++;
        return 1;
    }
}
void BaseBag::deleteFirst()
{
    if(size==0) return;
    head=head->next;
    size--;
}
BaseItem *BaseBag::get(ItemType itemType)
{
    if(size==0) return nullptr;
    node tmp=head;
    while(tmp!=nullptr && tmp->item->type()!=itemType) tmp=tmp->next;
    if(tmp!=nullptr)
    {
        swap(head->item,tmp->item);
        return head->item;
    }
    return nullptr;
}
string BaseBag::toString() const
{
    if(size==0) return "Bag[count=0;]";
    string str="Bag[count=" + to_string(size) + ";";
    node tmp=head;
    while(tmp != nullptr){
        if(tmp->item->type()==Antidote) str+="Antidote,";
        else if(tmp->item->type()==PhoenixDownI) str+="PhoenixI,";
        else if(tmp->item->type()==PhoenixDownII) str+="PhoenixII,";
        else if(tmp->item->type()==PhoenixDownIII) str+="PhoenixIII,";
        else if(tmp->item->type()==PhoenixDownIV) str+="PhoenixIV,";
        tmp=tmp->next;
    }
    str[str.length()-1]=']';
    return str;
}

bool BaseBag::heal(ItemType itemType)
{
    node tmp=head;
    BaseItem *ITEM=nullptr;
    if(itemType==Antidote) ITEM=get(itemType);
    else
    {
        if(size==0) return 0;
        while(tmp!=nullptr && !tmp->item->canUse(knight)) tmp=tmp->next;
        if(tmp!=nullptr)
        {
            swap(head->item,tmp->item);
            ITEM=head->item;
        }
    }
    if(ITEM==nullptr)
    {
        return 0;
    }
    else 
    {
        ITEM->use(knight);
        deleteFirst();
        return 1;
    }
}
BagPaladin::BagPaladin(BaseKnight* knight,int antidote, int phoenixDownI):BaseBag(knight, antidote, phoenixDownI, -1) {this->sizemax = -1;}
BagLancelot::BagLancelot(BaseKnight* knight,int antidote, int phoenixDownI):BaseBag(knight, antidote, phoenixDownI, 16) {this->sizemax = 16;}
BagNormal::BagNormal(BaseKnight* knight,int antidote, int phoenixDownI):BaseBag(knight, antidote, phoenixDownI, 19) {this->sizemax = 19;}
BagDragon::BagDragon(BaseKnight* knight,int antidote, int phoenixDownI):BaseBag(knight, antidote, phoenixDownI, 14) {this->sizemax = 14;}

/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseItem * * */
bool Antidote_Item::canUse(BaseKnight *knight)
{
    if(knight->getToxic()) return 1;
    return 0;
}
void Antidote_Item::use(BaseKnight *knight)
{
    knight->setToxic(false);
}
bool PhoenixDownI_Item::canUse(BaseKnight *knight)
{
    if(knight->getHP()<=0) return 1;
    return 0;
}
void PhoenixDownI_Item::use(BaseKnight *knight)
{
    knight->setHP(knight->getMaxHP());
}
bool PhoenixDownII_Item::canUse(BaseKnight *knight)
{
    if(knight->getHP() < knight->getMaxHP()/4) return 1;
    return 0;
}
void PhoenixDownII_Item::use(BaseKnight *knight)
{
    knight->setHP(knight->getMaxHP());
}
bool PhoenixDownIII_Item::canUse(BaseKnight *knight)
{
    if(knight->getHP() < knight->getMaxHP()/3) return 1;
    return 0;
}
void PhoenixDownIII_Item::use(BaseKnight *knight)
{
    if(knight->getHP()<=0) knight->setHP(knight->getMaxHP()/3);
    else knight->setHP(knight->getHP() + knight->getMaxHP()/4);
}
bool PhoenixDownIV_Item::canUse(BaseKnight *knight)
{
    if(knight->getHP() < knight->getMaxHP()/2) return 1;
    return 0;
}
void PhoenixDownIV_Item::use(BaseKnight *knight)
{
    if(knight->getHP()<=0) knight->setHP(knight->getMaxHP()/2);
    else knight->setHP(knight->getHP() + knight->getMaxHP()/5);
}
/* * * END implementation of class BaseItem * * */

/* * * BEGIN implementation of class BaseKnight * * */
int BaseKnight::getHP() {return hp;}
int BaseKnight::getMaxHP() {return maxhp;}
int BaseKnight::getLV() {return level;}
int BaseKnight::getGil() {return gil;}
bool BaseKnight::getToxic() {return toxic;}
BaseBag *BaseKnight::getBag() {return bag;}
void BaseKnight::setHP(int hp)
{
    if(hp > this->maxhp)
        this->hp = this->maxhp;
    else this->hp=hp;
}
void BaseKnight::setLV(int level)
{
    if(this->level>10)
        this->level=10;
    else this->level=level;
}
void BaseKnight::setGil(int gil)
{
    this->gil=gil;
}
void BaseKnight::setToxic(int toxic)
{
    this->toxic=toxic;
}
bool BaseKnight::setBag(BaseItem *item) {return bag->insertFirst(item);}

BaseKnight *BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI)
{
    BaseKnight *knight;
    if(isPrime(maxhp))
        knight = new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else if(isDragon(maxhp))
        knight = new DragonKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else if(is888(maxhp))
        knight = new LancelotKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    else knight = new NormalKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    return knight;
}
string BaseKnight::toString() const {
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same
    string s("");
    s += "[Knight:id:" + to_string(id)
        + ",hp:" + to_string(hp)
        + ",maxhp:" + to_string(maxhp)
        + ",level:" + to_string(level)
        + ",gil:" + to_string(gil)
        + "," + bag->toString()
        + ",knight_type:" + typeString[knightType]
        + "]";
    return s;
}

void BaseKnight::heal1()
{
    if(hp<= 0 && gil >= 100)
        {
            hp=maxhp / 2;
            gil-=100;
        }
}
bool BaseKnight::fight(BaseOpponent * opponent, int i)
{
    if(1<=opponent->id_event && opponent->id_event<=5)
    {
        int levelO=(i+opponent->id_event)%10+1;
        if(level>=levelO || knightType==PALADIN || knightType==LANCELOT)
        {    
            gil+=opponent->gil_bonus;
        }
        else
        {
            hp=hp-opponent->damage*(levelO-level);
            this->bag->heal(PhoenixDownI);
        }
    }
    else if(opponent->id_event==6)
    {
        int levelO=(i+opponent->id_event)%10+1;
        if(level>=levelO) {level = min(10, level + 1);}
        else if(knightType != DRAGON)
        {
            this->toxic=true;
            this->bag->heal(Antidote);
            if(this->toxic)
            {
                this->toxic=false;
                this->bag->deleteFirst();
                this->bag->deleteFirst();
                this->bag->deleteFirst();
                this->hp-=10;
                this->bag->heal(PhoenixDownI);
            }
        }
    }
    else if(opponent->id_event==7)
    {
        int levelO=(i+opponent->id_event)%10+1;
        if(level>=levelO) {gil*=2;}
        else {if(knightType != PALADIN) gil/=2;}
    }
    else if(opponent->id_event==8 && hp<maxhp/3 && knightType == PALADIN) {hp+=maxhp/5;}
    else if(opponent->id_event == 8 && hp< maxhp/3 && gil >=50)
    {
        gil-=50;
        hp+=maxhp/5;
    }
    else if(opponent->id_event == 9) {hp=maxhp;}
    else if(opponent->id_event == 10 && !ArmyKnights::WinOmegaWeapon)
    {
        if(level==10 && hp==maxhp || knightType == DRAGON)
        {
            level=10;
            gil=999;
            ArmyKnights::WinOmegaWeapon=true;
        }
        else
        {
            hp=0;
            this->bag->heal(PhoenixDownI);
        }
    }
    else if(opponent->id_event==11 && !ArmyKnights::WinHades)
    {
        if(level==10 || knightType==PALADIN && level>=8)
        {
            ArmyKnights::WinHades=true;
        }
        else
        {
            hp=0;
            this->bag->heal(PhoenixDownI);
        }
    }
    heal1();
    return hp>0;
}

BaseKnight::BaseKnight(int id, int maxhp, int level, int gil)
{
    this->id = id;
    this->maxhp = this->hp = maxhp;
    this->level = level;
    this->gil = gil;
}
PaladinKnight::PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI):BaseKnight(id, maxhp, level, gil)
{
    this->knightType=PALADIN;
    this->bag=new BagPaladin(this, antidote, phoenixdownI);
}
LancelotKnight::LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI):BaseKnight(id, maxhp, level, gil)
{
    this->knightType = LANCELOT;
    this->bag = new BagLancelot(this,antidote,phoenixdownI);
}
DragonKnight::DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI):BaseKnight(id, maxhp, level, gil)
{
    this->knightType = DRAGON;
    this->bag = new BagDragon(this,antidote,phoenixdownI);
}
NormalKnight::NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI):BaseKnight(id, maxhp, level, gil)
{
    this->knightType = NORMAL;
    this->bag = new BagNormal(this,antidote,phoenixdownI);
}
/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */
ArmyKnights::ArmyKnights(const string &file_armyknights)
{
    PaladinShield = LancelotSpear = GuinevereHair = ExcaliburSword = false;
    int id = 1;
    ifstream fi(file_armyknights);
    fi >> count_knights;
    knights = new BaseKnight*[count_knights];
    for (int j = 0; j < count_knights; j++)
    {
        int maxhp, level, phoenixdownI, gil, antidote;
        fi >> maxhp >> level >> phoenixdownI >> gil >> antidote;
        knights[j] = BaseKnight::create(id++, maxhp, level, gil, antidote, phoenixdownI);
    }
}
ArmyKnights::~ArmyKnights()
{
    delete[] knights;
}

bool ArmyKnights::fight(BaseOpponent *opponent)
{
    while(count_knights)
    {
        BaseKnight *lknight = lastKnight();
        if(lknight->fight(opponent,i))
        {
            if(ArmyKnights::WinHades) this->PaladinShield=true;
            for(int i=count_knights-1;i>=0;i--)
            {
                if(knights[i]->getGil()-999<=0) break;
                knights[i]->setGil(999);
                if(i>0) knights[i-1]->setGil(knights[i-1]->getGil() + knights[i]->getGil()-999);
            }
            break;
        }
        count_knights--;
        delete lknight;
        break;
    }
    delete opponent;
    return count_knights;
}

bool ArmyKnights::adventure(Events *events)
{
    ArmyKnights::WinHades = ArmyKnights::WinOmegaWeapon = false;
    for(this->i=0;i<events->count();i++)
    {
        if(events->get(i) == 1 ) this->fight(new MadBear_Op());
        else if(events->get(i) == 2) this->fight(new Bandit_Op());
        else if(events->get(i) == 3) this->fight(new LordLupin_Op());
        else if(events->get(i) == 4) this->fight(new Elf_Op());
        else if(events->get(i) == 5) this->fight(new Troll_Op());
        else if(events->get(i) == 6) this->fight(new Tornbery_Op());
        else if(events->get(i) == 7) this->fight(new QueenOfCards_Op());
        else if(events->get(i) == 8) this->fight(new NinaDeRings_Op());
        else if(events->get(i) == 9) this->fight(new DurianGarden_Op());
        else if(events->get(i) == 10) this->fight(new OmegaWeapon_Op());
        else if(events->get(i) == 11) this->fight(new Hades_Op());
        else if(events->get(i) == 112 || events->get(i) == 113 || events->get(i) == 114)
        {
            BaseItem *item;
            if(events->get(i) == 112) item=new PhoenixDownII_Item();
            else if(events->get(i) == 113) item =new PhoenixDownIII_Item();
            else if(events->get(i) == 114) item =new PhoenixDownIV_Item();
            for(int j=count_knights-1;j>=0;j--)
            {
                if(knights[j]->setBag(item)) break;
                else delete item;
            }
        }
        else if(events->get(i) == 95) this->PaladinShield=true;
        else if(events->get(i) == 96) this->LancelotSpear=true;
        else if(events->get(i) == 97) this->GuinevereHair=true;
        else if(events->get(i) == 98 && this->PaladinShield && this->LancelotSpear && this->GuinevereHair||this->ExcaliburSword) this->ExcaliburSword=true;
        else if(events->get(i) == 99)
        {
            if(this->ExcaliburSword)
            {
                this->printInfo();
                return 1;
            }
            else if(!PaladinShield || !LancelotSpear || !GuinevereHair)
            {
                delete[] knights;
                count_knights=0;
                this->printInfo();
                return 0;
            }
            else
            {
                int HP_Ultimecia = 5000;
                while(count_knights)
                {
                    BaseKnight *lknight= lastKnight();
                    HP_Ultimecia -= lknight->knightBaseDamage();
                    if(HP_Ultimecia>0)
                    {
                        count_knights--;
                        delete lknight;
                    }
                    else
                    {
                        this->printInfo();
                        return 1;
                    }
                }
                if(!count_knights)
                {
                    this->printInfo();
                    return 0;
                }
            }    
        }
        this->printInfo();
        if(count_knights == 0) return 0;
    }
    return 1;
}
void ArmyKnights::printInfo() const {
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight * lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure() {
    armyKnights = nullptr;
    events = nullptr;
}

void KnightAdventure::loadArmyKnights(const string &file)
{
    armyKnights= new ArmyKnights(file);
}
void KnightAdventure::loadEvents(const string & file)
{
    events = new Events(file);
}
void KnightAdventure::run()
{
    armyKnights->printResult(armyKnights->adventure(events));
}
KnightAdventure::~KnightAdventure()
{
        delete armyKnights;
        delete events;
}
/* * * END implementation of class KnightAdventure * * */

/* * * BEGIN implementation of class Events * * */
Events::Events(const string &file_events)
{
    ifstream fi(file_events);
    fi >> count_sukien;
    sukien = new int[count_sukien];
    for (int i = 0; i < count_sukien; i++)
    {
        fi >> sukien[i];
    }
}
int Events::count() const
{
    return count_sukien;
}
int Events::get(int i) const
{
    return sukien[i];
}
Events::~Events()
{
    delete[] sukien;
}
/* * * END implementation of class Events * * */
