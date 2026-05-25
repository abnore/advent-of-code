#include "main.h"
#include <blackbox.h>

typedef struct{
    int hit_points;
    int damage;
    int armor;
} Player;

typedef struct {
    int cost;
    int damage;
    int armor;
} Item;

// Weapons:    Cost  Damage  Armor
static Item weapons[] = { // 5 weapons
    { 8, 4, 0}, // Dagger
    {10, 5, 0}, // Shortsword
    {25, 6, 0}, // Warhammer
    {40, 7, 0}, // Longsword
    {74, 8, 0} // Greataxe
};
//Armor:      Cost  Damage  Armor
static Item armor[] = { // 6 armors
    {0, 0, 0}, // no armor
    {13, 0, 1}, //Leather
    {31, 0, 2}, //Chainmail
    {53, 0, 3}, //Splintmail
    {75, 0, 4}, //Bandedmail
    {102, 0, 5} //Platemail
};

//Rings:      Cost  Damage  Armor
static Item rings[] = { // 8 rings
    {0,0,0}, // no ring
    {0,0,0}, // no second rings
    {25, 1, 0}, //Damage +1
    {50, 2, 0}, //Damage +2
    {100, 3, 0}, //Damage +3
    {20, 0, 1}, //Defense +1
    {40, 0, 2}, //Defense +2
    {80, 0, 3} //Defense +3
};


static int attack(Player *one, Player *two)
{
    int result = one->damage - two->armor;
    if (result <= 0) result = 1;

    two->hit_points -= result;
    return two->hit_points;
}

static int turn(Player *me, Player *boss)
{
    // first me
    int first = attack(me, boss);
    if (first <= 0) return 1; // if i win, return 1

    // then boss
    int second = attack(boss, me);
    if (second<= 0) return 2; // if i lose, return 2

    return 0; // no victor yet
}

static int fight(Player me, Player boss)
{
    int result;
    do {
        result = turn(&me, &boss);
    } while (!result);

    return result;
}
/*
You must buy exactly one weapon; no dual-wielding. Armor is optional, but you
can't use more than one. You can buy 0-2 rings (at most one for each hand). You
must use any items you buy. The shop only has one of each item, so you can't
buy, for example, two rings of Damage +3.

*/


int pt2(void)
{
    int best = 0;
    Player boss = { .hit_points = 104, .damage = 8, .armor = 1 };
    Player me = { .hit_points = 100 };

    for (int w = 0; w < 5; w++) {
        for (int a = 0; a < 6; a++) {
            for (int r1 = 0; r1 < 8; r1++) {
                for (int r2 = r1 + 1; r2 < 8; r2++)
                {
                    int cost = weapons[w].cost + armor[a].cost +
                               rings[r1].cost + rings[r2].cost;

                    me.damage = weapons[w].damage + armor[a].damage +
                                rings[r1].damage + rings[r2].damage;

                    me.armor = weapons[w].armor + armor[a].armor +
                               rings[r1].armor + rings[r2].armor;

                    if (fight(me, boss) == 2 && cost > best)
                        best = cost;
                }
            }
        }
    }

    return best;
}
