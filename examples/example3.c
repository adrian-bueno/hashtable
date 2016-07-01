/*******************************************************************************
 * Example 3
 * Key = struct (_hero)
 * Value = struct (_car)
 * Memory allocated when key and value are created.
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../hashtable.h"

#define HERO(h) ((Hero*)h)
#define CAR(c)  ((Car*)c)


/**** STRUCTURES **************************************************************/

typedef struct _hero {
    int id;
    char *name;
    /* ... */
} Hero;

typedef struct _car {
    float id;
    char *color;
    /* ... */
} Car;


/**** HERO FUNCTIONS **********************************************************/

Hero *hero_create(int id, char *name) {

    Hero *hero = (Hero*) malloc (sizeof(Hero));
    if (hero == NULL)
        return NULL;

    hero->id = id;
    if (name != NULL)
        hero->name = strdup(name);
    else
        hero->name = NULL;

    return hero;
}

void hero_delete(void *hero) {
    if (hero == NULL)
        return;

    if (HERO(hero)->name != NULL)
        free(HERO(hero)->name);
    free(hero);
}

int hero_compare(void *hero1, void *hero2) {
    if (HERO(hero1)->id > HERO(hero2)->id)
        return 1;
    else if (HERO(hero1)->id < HERO(hero2)->id)
        return -1;
    else
        return 0;
}

unsigned long hero_hash_value(void *hero) {
    return (unsigned long)HERO(hero)->id;
}


/**** CAR FUNCTIONS ***********************************************************/

Car *car_create(int id, char *color) {

    Car *car = (Car*) malloc (sizeof(Car));
    if (car == NULL)
        return NULL;

    car->id = id;
    if (color != NULL)
        car->color = strdup(color);
    else
        car->color = NULL;

    return car;
}

void car_delete(void *car) {
    if (car == NULL)
        return;

    if (CAR(car)->color != NULL)
        free(CAR(car)->color);
    free(car);
}


/**** MAIN ********************************************************************/

int main() {

    hashtable_t *h = NULL;
    char *heros[] = {"Batman", "Spiderman", "Wonder Woman", "Hulk", "Wolverine", "Goku"};
    char *colors[] = {"Black", "Red", "Gold", "Green", "Yellow", "Orange", "Purple", "Pink"};
    Hero *key = NULL;
    Car *value = NULL;
    int i;

    h = hashtable_create(100, hero_compare, hero_hash_value, hero_delete, car_delete);

    // Introduce key-value
    for (i = 0; i < 6; i++) {
        key = hero_create(i, heros[i]);
        value = car_create(i, colors[i]);
        hashtable_set(h, key, value);
    }

    // Get values
    for (i = 0; i < 6; i++) {
        key = hero_create(i, heros[i]);
        value = hashtable_get(h, key);
        printf("%s - %s\n", key->name, (char*) value->color);
        hero_delete(key);
    }

    printf("\n");

    // Replace values
    key = hero_create(0, NULL); // Batman
    value = car_create(123, colors[6]);
    hashtable_set(h, key, value);
    hero_delete(key);

    key = hero_create(4, NULL); // Wolverine
    value = car_create(45, colors[7]);
    hashtable_set(h, key, value);
    hero_delete(key);

    // Delete key-value
    key = hero_create(3, NULL); // Hulk
    i = hashtable_delete_key(h, key);
    hero_delete(key);

    if(i == 0)
        printf("Ok: Hulk deleted correctly.\n\n");
    else
        printf("Error: Hulk was not deleted.\n\n");

    // Get values
    for (i = 0; i < 6; i++) {
        key = hero_create(i, heros[i]);
        value = hashtable_get(h, key);
        if (value != NULL)
            printf("%s - %s\n", key->name, (char*) value->color);
        hero_delete(key);
    }

    hashtable_delete(h);

    return 0;
}
