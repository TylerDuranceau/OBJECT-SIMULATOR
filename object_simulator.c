/*
 * Object Simulator Assignment
 * Computer Science XII - Computer Systems
 * 
 * This program simulates object-oriented programming using structs and
 * function pointers to understand how Python's class system works at the
 * implementation level.
 *
 * You'll build an RPG character system with inheritance and polymorphism.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// PART 1: BASE CHARACTER STRUCT
// =============================================================================

typedef struct Character {
    char name[50];
    int health;
    int level;
    void (*attack)(struct Character* self);
    void (*take_damage)(struct Character* self, int damage);
} Character;

// =============================================================================
// PART 2: CHARACTER METHODS
// =============================================================================

void character_attack(Character* self) {
    printf("%s performs a basic attack!\n", self->name);
}

void character_take_damage(Character* self, int damage) {
    self->health -= damage;
    if (self->health < 0) self->health = 0;
    printf("%s takes %d damage! Health: %d\n", self->name, damage, self->health);
}

void character_init(Character* c, const char* name, int health, int level) {
    strncpy(c->name, name, 49);
    c->name[49] = '\0';
    c->health = health;
    c->level = level;
    c->attack = character_attack;
    c->take_damage = character_take_damage;
}

// =============================================================================
// PART 3: DERIVED CHARACTER TYPES
// =============================================================================

typedef struct Warrior {
    Character base;
    int strength;
} Warrior;

typedef struct Mage {
    Character base;
    int mana;
} Mage;

// =============================================================================
// PART 4: POLYMORPHIC METHODS
// =============================================================================

void warrior_attack(Character* self) {
    Warrior* w = (Warrior*)self;
    printf("%s swings sword with %d strength!\n", w->base.name, w->strength);
}

void mage_attack(Character* self) {
    Mage* m = (Mage*)self;
    if (m->mana >= 10) {
        m->mana -= 10;
        printf("%s casts fireball using %d mana!\n", m->base.name, m->mana);
    } else {
        printf("%s is out of mana!\n", m->base.name);
    }
}

void warrior_init(Warrior* w, const char* name, int health, int level, int strength) {
    character_init(&w->base, name, health, level);
    w->strength = strength;
    w->base.attack = warrior_attack;
}

void mage_init(Mage* m, const char* name, int health, int level, int mana) {
    character_init(&m->base, name, health, level);
    m->mana = mana;
    m->base.attack = mage_attack;
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    printf("=============================================================\n");
    printf("         OBJECT SIMULATOR: RPG Character System\n");
    printf("=============================================================\n");

    // Create basic Character
    Character hero;
    character_init(&hero, "Arin", 100, 1);
    hero.attack(&hero);
    hero.take_damage(&hero, 20);

    // Create Warrior
    Warrior thor;
    warrior_init(&thor, "Thorin", 120, 2, 50);
    thor.base.attack((Character*)&thor);
    thor.base.take_damage((Character*)&thor, 30);

    // Create Mage
    Mage gandalf;
    mage_init(&gandalf, "Gandalf", 80, 5, 30);
    gandalf.base.attack((Character*)&gandalf);
    gandalf.base.attack((Character*)&gandalf);
    gandalf.base.attack((Character*)&gandalf); // Demonstrate out-of-mana

    // Polymorphism: store different types in Character* array
    Character* party[] = {
        &hero,
        (Character*)&thor,
        (Character*)&gandalf
    };
    printf("\n-- Party Attacks --\n");
    for (int i = 0; i < 3; i++) {
        party[i]->attack(party[i]);
    }

    printf("\n=============================================================\n");
    printf("Object simulation complete!\n");
    printf("=============================================================\n");

    return 0;
}