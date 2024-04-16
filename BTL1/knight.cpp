#include "knight.h"

// some constants
const int maxLevel = 10;
const int maxItem = 99;
const int maxEvent = (int)2e5 + 7;
int fib[40];
string l3files0;
string l3files1;
string l3files2;

// some functions

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

void fibo()
{
    fib[0] = 1;
    fib[1] = 1;
    for (int i = 2; i < 40; ++i)
    {
        fib[i] = fib[i - 1] + fib[i - 2];
    }
}

int strtoi(string s)
{
    int ans = 0;
    for (int i = 0; i < (int)s.size(); ++i)
    {
        ans = ans * 10 + (s[i] - '0');
    }
    return ans;
}

struct L3files
{
    string fileno[3];
    void addstring(int index, string t)
    {
        fileno[index] = t;
    }

    void Input(string filename)
    {
        int n = 0;
        string tmp = "";
        filename += ',';
        for (int i = 0; i < (int)filename.size(); ++i)
        {
            if (filename[i] != ',')
            {
                tmp += filename[i];
            }
            else
            {
                addstring(n++, tmp);
                tmp = "";
            }
        }
    }
} line3;

struct Knights
{
    int HP, Level, Remedy, MaidenKiss, PhoenixDown;
    int Rescue = -1;
    int prevLevel;
    int maxHP = 999;
    int ghostno = 1;

    bool metAsclepius = 0;
    bool metMerlin = 0;
    bool isArthur = 0;
    bool isLancelot = 0;

    void Input(string s)
    {
        stringstream ss(s);
        ss >> HP >> Level >> Remedy >> MaidenKiss >> PhoenixDown;
        maxHP = HP;

        if (HP == 999)
            isArthur = 1;
        if (isPrime(HP))
            isLancelot = 1;
    }

    void healthCheck()
    {
        if (HP <= 0 && PhoenixDown == 0)
            Rescue = 0;
        else if (HP <= 0 && PhoenixDown > 0)
        {
            --PhoenixDown;
            HP = maxHP;
        }
    }

    void display()
    {
        cout << "HP=" << HP
             << ", level=" << Level
             << ", remedy=" << Remedy
             << ", maidenkiss=" << MaidenKiss
             << ", phoenixdown=" << PhoenixDown
             << ", rescue=" << Rescue << endl;
    }

    struct Tiny
    {
        bool isTiny = 0;
        int eventCount;
    } tiny;

    void turnTiny()
    {
        tiny.isTiny = 1;
        tiny.eventCount = 4;
        HP = max(1, HP / 5);
    }

    void returnTiny()
    {
        tiny.isTiny = 0;
        tiny.eventCount = 0;
        HP = min(maxHP, HP * 5);
    }

    void tinyExecute()
    {
        if (tiny.isTiny)
        {
            if (Remedy)
            {
                --Remedy;
                returnTiny();
            }
            else
            {
                if (tiny.eventCount - 1 <= 0)
                    returnTiny();
                else
                    tiny.eventCount--;
            }
        }
    }

    struct frog
    {
        bool isFrog = 0;
        int eventCount = 0;
    } frog;

    void turnFrog()
    {
        frog.isFrog = 1;
        frog.eventCount = 4;
        prevLevel = Level;
        Level = 1;
    }

    void returnFrog()
    {
        frog.isFrog = 1;
        frog.eventCount = 0;
        Level = prevLevel;
    }

    void frogExecute()
    {
        if (frog.isFrog)
        {
            if (MaidenKiss)
            {
                --MaidenKiss;
                returnFrog();
            }
            else
            {
                if (frog.eventCount - 1 <= 0)
                    returnFrog();
                else
                    frog.eventCount--;
            }
        }
    }

    void checkStatus()
    {
        tinyExecute();
        frogExecute();
    }

} knight;

struct Events
{
    int ListOfEvents[maxEvent];
    int numEvents = 0;
    string GhostMush[maxEvent];
    int numGmEvents = 0;
    void Input(string s)
    {
        stringstream ss(s);
        string evet;

        while (ss >> evet)
        {

            if ((int)evet.size() >= 2 && evet[0] == '1' && evet[1] == '3')
            {
                ListOfEvents[++numEvents] = 13;
                GhostMush[++numGmEvents] = evet;
            }
            else
                ListOfEvents[++numEvents] = strtoi(evet);
        }
    }

    // id = 0
    void success(Knights &knight)
    {
        knight.Rescue = 1;
    }

    int levelCalculate(int index)
    {
        int b = index % 10;
        return (index > 6 ? (b > 5 ? b : 5) : b);
    }

    // id = 1 -> 5
    void meetNormal(Knights &knight, int mons, int index)
    {
        int level0 = levelCalculate(index);

        if (knight.Level > level0 || knight.isArthur || knight.isLancelot)
        {
            knight.Level = min(maxLevel, knight.Level + 1);
            return;
        }
        if (knight.Level < level0)
        {
            int baseDamage[6] = {0, 10, 15, 45, 75, 95};
            int damage = level0 * baseDamage[mons];
            knight.HP -= damage;
            knight.healthCheck();
            return;
        }
    }

    // id = 6
    void meetShaman(Knights &knight, int index)
    {
        if (knight.tiny.isTiny || knight.frog.isFrog)
            return;
        int level0 = levelCalculate(index);

        if (knight.Level > level0)
        {
            knight.Level = min(maxLevel, knight.Level + 2);
            return;
        }
        if (knight.Level < level0)
            knight.turnTiny();
    }

    // id = 7
    void meetSirenVajsh(Knights &knight, int index)
    {
        if (knight.tiny.isTiny || knight.frog.isFrog)
            return;
        int level0 = levelCalculate(index);

        if (knight.Level > level0 || knight.isArthur || knight.isLancelot)
        {
            knight.Level = min(maxLevel, knight.Level + 2);
            return;
        }
        if (knight.Level < level0)
            knight.turnFrog();
    }

    // id = 11
    void pickMariomush(Knights &knight)
    {
        int n1 = ((knight.Level + knight.PhoenixDown) % 5 + 1) * 3;
        int s1 = 0;
        for (int i = 99; n1 <= 0; --n1, i -= 2)
            s1 += i;
        knight.HP += knight.HP + (s1 % 100);
        while (!isPrime(knight.HP))
            knight.HP++;
        knight.HP = max(knight.maxHP, knight.HP);
    }

    // id = 12, fibonacci
    void pickFibmush(Knights &knight)
    {
        int x = fib[1];
        for (int i = 1; i <= 40; ++i)
        {
            if (fib[i] <= knight.HP)
                x = fib[i];
            else
                break;
        }
        knight.HP = x;
    }

    // id = 13
    struct ghostMushroom
    {
        int array[102], arrayTrans[102];
        int n;

        void Input(string file_mush_ghost)
        {
            ifstream mg(file_mush_ghost);
            mg >> n;
            string s;
            getline(mg, s);
            getline(mg, s);
            s = s + ',';
            int idx = 0;
            bool negative = 0;
            for (int i = 0; i < (int)s.size(); ++i)
            {
                if (s[i] != ',')
                {
                    if (s[i] == '-')
                    {
                        negative = 1;
                    }
                    else
                        array[idx] = array[idx] * 10 + (s[i] - '0');
                }
                else
                {
                    if (negative)
                        array[idx] *= -1;
                    negative = 0;
                    ++idx;
                }
            }
            for (int i = 0; i < n; ++i)
            {
                arrayTrans[i] = abs(array[i]);
                arrayTrans[i] = (17 * arrayTrans[i] + 9) % 257;
            }

            mg.close();
        }

        // ms = 1
        void pickMush1(int &x, int &y)
        {
            int mx = -2147483648;
            int mn = 2147483647;
            for (int i = 0; i < n; ++i)
            {
                if (array[i] >= mx)
                {
                    x = i;
                    mx = array[i];
                }
                if (array[i] <= mn)
                {
                    y = i;
                    mn = array[i];
                }
            }
        }

        // ms = 2
        void pickMush2(int &val, int &pos)
        {
            val = -2;
            pos = -3;
            int l = 0, r = n - 1;

            while (l != n - 1 && array[l] < array[l + 1])
            {
                ++l;
            }

            while (r != 1 && array[r] < array[r - 1])
            {
                --r;
            }

            if (l != r)
                return;
            val = array[l], pos = l;
        }

        // ms = 3
        void pickMush3(int &x, int &y)
        {
            int mx = -2147483648;
            int mn = 2147483647;
            for (int i = 0; i < n; ++i)
            {
                if (arrayTrans[i] > mx)
                {
                    x = i;
                    mx = arrayTrans[i];
                }
                if (arrayTrans[i] < mn)
                {
                    y = i;
                    mn = arrayTrans[i];
                }
            }
        }

        // ms = 4
        void pickMush4(int &val, int &pos)
        {
            val = -5;
            pos = -7;
            int mx = max(max(arrayTrans[0], arrayTrans[1]), arrayTrans[2]);
            int mn = min(min(arrayTrans[0], arrayTrans[1]), arrayTrans[2]);
            if (mx == mn)
                return;
            if (arrayTrans[0] == arrayTrans[1] || arrayTrans[0] == arrayTrans[2] || arrayTrans[1] == arrayTrans[2])
            {
                for (int i = 0; i < 3; ++i)
                {
                    if (arrayTrans[i] < mx && arrayTrans[i] >= mn)
                    {
                        val = arrayTrans[i];
                        pos = i;

                        return;
                    }
                }
            }
            else
            {
                for (int i = 0; i < 3; ++i)
                {
                    if (arrayTrans[i] < mx && arrayTrans[i] > mn)
                    {
                        val = arrayTrans[i];
                        pos = i;

                        return;
                    }
                }
            }
        }
    } ghostmush;

    void pickGhostmush(Knights &knight, string s)
    {
        int list[maxEvent], numlist = 0;

        for (int i = 2; i < (int)s.size(); ++i)
        {
            list[++numlist] = s[i] - '0';
        }

        ghostmush.Input(line3.fileno[0]);
        for (int i = 1; i <= numlist; ++i)
        {
            int x, y;
            if (list[i] == 1)
            {
                ghostmush.pickMush1(x, y);
            }
            if (list[i] == 2)
            {
                ghostmush.pickMush2(x, y);
            }
            if (list[i] == 3)
            {
                ghostmush.pickMush3(x, y);
            }
            if (list[i] == 4)
            {
                ghostmush.pickMush4(x, y);
            }
            knight.HP = knight.HP - (x + y);
            knight.healthCheck();

            if (knight.Rescue == 0)
                return;
        }
    }

    // id = 15 -> 17
    void pickItem(Knights &knight, int index)
    {
        if (index == 15)
            knight.Remedy = min(knight.Remedy + 1, maxItem);
        if (index == 16)
            knight.MaidenKiss = min(knight.MaidenKiss + 1, maxItem);
        if (index == 17)
            knight.PhoenixDown = min(knight.PhoenixDown + 1, maxItem);
    }

    int chooseItem(Knights &knight, int index)
    {
        if (index == 16)
            return knight.Remedy;
        if (index == 17)
            return knight.MaidenKiss;
        else
            return knight.PhoenixDown;
    }

    // id = 19, Asclepius
    void meetAsclepius(Knights &knight)
    {
        knight.metAsclepius = 1;
        ifstream asc(line3.fileno[1]);

        int n, m;
        asc >> n >> m;
        for (int i = 1; i <= n; ++i)
        {
            int cnt = 0;
            for (int j = 1; j <= m; ++j)
            {
                int x;
                asc >> x;
                if (cnt != 3 && (16 <= x && x <= 18) && chooseItem(knight, x) < maxItem)
                {
                    ++cnt;
                    pickItem(knight, x - 1);
                }
                if (cnt == 3)
                    break;
            }
        }
    }

    // id = 18, witch
    void meetMerlin(Knights &knight)
    {
        knight.metMerlin = 1;
        ifstream mer(line3.fileno[2]);
        int n;
        mer >> n;
        string Merlin = "Merlin";
        string merlin = "merlin";
        for (int i = 0; i < n; ++i)
        {
            string s;
            mer >> s;
            bool check[256] = {};
            if ((int)s.size() < 6)
                continue;
            for (int j = 0; j < (int)s.size(); ++j)
                check[s[j]] = 1;
            if ((check['m'] || check['M']) &&
                (check['e'] || check['E']) &&
                (check['r'] || check['R']) && // nice
                (check['l'] || check['L']) &&
                (check['i'] || check['I']) &&
                (check['n'] || check['N']))
            {
                knight.HP = min(knight.maxHP, knight.HP + 2);
            }
            for (int i = 0; i < (int)s.size() - 5; ++i)
            {
                string tmp = s.substr(i, 6);
                if (tmp == merlin || tmp == Merlin)
                    knight.HP = min(knight.maxHP, knight.HP + 1);
            }
        }
        mer.close();
    }

    // id = 99
    void meetBowser(Knights &knight)
    {
        if (knight.isArthur || (knight.isLancelot && knight.Level >= 8) || knight.Level == maxLevel)
            knight.Level = maxLevel;
        else
            knight.Rescue = 0;
    }

    void meetEvent(Knights &knight, int index, int eventid)
    {
        if (index == 0)
            success(knight);
        if (1 <= index && index <= 5)
            meetNormal(knight, index, eventid);
        if (index == 6)
            meetShaman(knight, eventid);
        if (index == 7)
            meetSirenVajsh(knight, eventid);
        if (index == 11)
            pickMariomush(knight);
        if (index == 12)
            pickFibmush(knight);
        if (index == 13)
            pickGhostmush(knight, GhostMush[knight.ghostno++]);
        if (15 <= index && index <= 17)
            pickItem(knight, index);
        if (index == 18 && !knight.metMerlin)
            meetMerlin(knight);
        if (index == 19 && !knight.metAsclepius)
            meetAsclepius(knight);
        if (index == 99)
            meetBowser(knight);
    }
} event;

void fileINP(string file_input)
{
    ifstream in(file_input);
    string sinp;
    getline(in, sinp);
    knight.Input(sinp);

    getline(in, sinp);
    event.Input(sinp);

    getline(in, sinp);

    line3.Input(sinp);

    in.close();
}

void adventureToKoopa(string file_input, int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue)
{
    fibo();
    fileINP(file_input);

    int eventid = 1;
    while (knight.Rescue == -1 && eventid <= event.numEvents)
    {

        event.meetEvent(knight, event.ListOfEvents[eventid], eventid);

        if (eventid >= event.numEvents && knight.HP > 0)
        {
            knight.Rescue = 1;
        }
        knight.checkStatus();
        knight.display();
        eventid++;
    }
}