#include "main.h"

static int best;

typedef struct{
    int p_hp;
    int p_mana;
    int p_armor;

    int boss_hp;
    int boss_damage;

    int st;
    int pt;
    int rt;

    int cost;
} State;

enum { Magic_Missile, Drain, Shield, Poison, Recharge, spell_count};
enum { LOSS, WIN, CONTINUE };

static char* spell_str(int spell){
    switch(spell){
    case Magic_Missile: return "Magic Missile";
    case Drain:         return "Drain";
    case Shield:        return "Shield";
    case Poison:        return "Poison";
    case Recharge:      return "Recharge";
    default:            return "Unknown";
    }
}

static int cast_spell(State *s, int spell){
    int cost;
    switch(spell){
        case Magic_Missile:
            cost = 53;
            s->boss_hp -= 4;
            break;

        case Drain:
            cost = 73;
            s->boss_hp -= 2;
            s->p_hp += 2;
            break;

        case Shield:
            if (!s->st){
                cost = 113;
                s->st = 6;
            } else {
                printf("Shield timer still active\n");
                return 0;
            }
            break;

        case Poison:
            if (!s->pt){
                cost = 173;
                s->pt = 6;
            } else {
                printf("Poison timer still active\n");
                return 0;
            }
            break;

        case Recharge:
            if (!s->rt){
                cost = 229;
                s->rt = 5;
            } else {
                printf("Recharge timer still active\n");
                return 0;
            }
            break;
    }
    if (cost >= s->p_mana) return 0;
    s->cost += cost;
    s->p_mana -= cost;

    return cost;
}

static void attack(State *s) {
    int result = s->boss_damage - s->p_armor;
    if (result <= 0) result = 1;
    s->p_hp -= result;
}

static void apply_effect(State *s){
    if(s->st){
        printf("Shield active, armor is 7\n");
        s->p_armor = 7;
        s->st--;
    } else s->p_armor = 0;

    if(s->pt) {
        printf("Poison active, reducing boss by 3\n");
        s->boss_hp -= 3;
        s->pt--;
    }
    if(s->rt){
        printf("Recharge active, adding 101 mana\n");
        s->p_mana += 101;
        s->rt--;
    }
}

static int try_round(State *s, int spell)
{
    // Player turn
    apply_effect(s);
    if (s->boss_hp <= 0)        return WIN;
    if (!cast_spell(s, spell))  return LOSS;
    if (s->boss_hp <= 0)        return WIN;

    // boss turn
    apply_effect(s);
    if (s->boss_hp <= 0)        return WIN;
    attack(s);
    if (s->p_hp <= 0)           return LOSS;

    return CONTINUE;
}

static void search(State s)
{
    if (s.cost >= best) return;

    for (int spell = 0; spell < spell_count; spell++)
    {
        State next = s;
        int result = try_round(&next, spell);

        switch(result){
        case LOSS: continue;
        case WIN:
            if (next.cost < best)
                best = next.cost;
            break;
        case CONTINUE: search(next);
        }
    }
}

int pt1(void)
{
    best = 999999;
    State start = {
        .p_hp = 50,
        .p_mana = 500,
        .boss_hp = 55,
        .boss_damage = 8,
    };
    search(start);

    return best;
}
