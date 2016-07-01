/*******************************************************************************
 * Example 2
 * Key = int
 * Value = String
 * No memory allocated when key and value are created.
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../hashtable.h"


int int_compare(void *int1, void *int2) {
    if (int1 > int2)
        return 1;
    else if (int1 < int2)
        return -1;
    else
        return 0;
}

unsigned long int_hash_value(void *integer) {
    return *(unsigned long *)integer;
}


int main() {

    int key1 = 1;
    int key2 = 2;
    int key3 = 3;
    int key4 = 4;
    int key5 = 5;
    int key6 = 6;

    char value1[] = "Dog";
    char value2[] = "Cat";
    char value3[] = "Dolphin";
    char value4[] = "Spider";
    char value5[] = "Mouse";
    char value6[] = "Snake";
    char value7[] = "Shark";
    char value8[] = "Elephant";

    hashtable_t *h;

    h = hashtable_create(100, int_compare, int_hash_value, NULL, NULL);
    if (h == NULL)
        return -1;

    hashtable_set(h, &key1, value1);
    hashtable_set(h, &key2, value2);
    hashtable_set(h, &key3, value3);
    hashtable_set(h, &key4, value4);
    hashtable_set(h, &key5, value5);
    hashtable_set(h, &key6, value6);

    printf("%d - %s\n", key1, (char*)hashtable_get(h, &key1));
    printf("%d - %s\n", key2, (char*)hashtable_get(h, &key2));
    printf("%d - %s\n", key3, (char*)hashtable_get(h, &key3));
    printf("%d - %s\n", key4, (char*)hashtable_get(h, &key4));
    printf("%d - %s\n", key5, (char*)hashtable_get(h, &key5));
    printf("%d - %s\n", key6, (char*)hashtable_get(h, &key6));
    printf("\n");

    hashtable_set(h, &key1, value7);
    hashtable_set(h, &key4, value8);

    printf("%d - %s\n", key1, (char*)hashtable_get(h, &key1));
    printf("%d - %s\n", key2, (char*)hashtable_get(h, &key2));
    printf("%d - %s\n", key3, (char*)hashtable_get(h, &key3));
    printf("%d - %s\n", key4, (char*)hashtable_get(h, &key4));
    printf("%d - %s\n", key5, (char*)hashtable_get(h, &key5));
    printf("%d - %s\n", key6, (char*)hashtable_get(h, &key6));
    printf("\n");

    hashtable_delete_key(h, &key3);
    hashtable_delete_key(h, &key1);
    hashtable_delete_key(h, &key2);

    if (hashtable_get(h, &key3) == NULL)
        printf("%d - not exist\n", key3);
    else
        printf("%d - exist\n", key3);

    if (hashtable_get(h, &key1) == NULL)
        printf("%d - not exist\n", key1);
    else
        printf("%d - exist\n", key1);

    if (hashtable_get(h, &key2) == NULL)
        printf("%d - not exist\n", key2);
    else
        printf("%d - exist\n", key2);

    printf("\n");

    hashtable_set(h, &key1, value3);
    hashtable_set(h, &key2, value7);
    hashtable_set(h, &key3, value5);

    printf("%d - %s\n", key1, (char*)hashtable_get(h, &key1));
    printf("%d - %s\n", key2, (char*)hashtable_get(h, &key2));
    printf("%d - %s\n", key3, (char*)hashtable_get(h, &key3));
    printf("%d - %s\n", key4, (char*)hashtable_get(h, &key4));
    printf("%d - %s\n", key5, (char*)hashtable_get(h, &key5));
    printf("%d - %s\n", key6, (char*)hashtable_get(h, &key6));

    hashtable_delete(h);

    return 0;
}
