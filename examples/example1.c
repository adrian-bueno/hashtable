/*******************************************************************************
 * Example 1
 * Key = string
 * Value = int
 * No memory allocated when key and value are created.
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../hashtable.h"


int string_compare(void *str1, void *str2) {
    return strcmp((char*)str1, (char*)str2);
}


int main() {

    char key1[] = "Dog";
    char key2[] = "Cat";
    char key3[] = "Dolphin";
    char key4[] = "Spider";
    char key5[] = "Mouse";
    char key6[] = "Snake";

    int value1 = 1;
    int value2 = 2;
    int value3 = 3;
    int value4 = 4;
    int value5 = 5;
    int value6 = 6;
    int value7 = 7;
    int value8 = 8;

    hashtable_t *h;

    h = hashtable_create(100, string_compare, string_hash_value, NULL, NULL);
    if (h == NULL)
        return -1;

    hashtable_set(h, key1, &value1);
    hashtable_set(h, key2, &value2);
    hashtable_set(h, key3, &value3);
    hashtable_set(h, key4, &value4);
    hashtable_set(h, key5, &value5);
    hashtable_set(h, key6, &value6);

    printf("%s - %d\n", key1, *((int*)hashtable_get(h, key1)));
    printf("%s - %d\n", key2, *((int*)hashtable_get(h, key2)));
    printf("%s - %d\n", key3, *((int*)hashtable_get(h, key3)));
    printf("%s - %d\n", key4, *((int*)hashtable_get(h, key4)));
    printf("%s - %d\n", key5, *((int*)hashtable_get(h, key5)));
    printf("%s - %d\n", key6, *((int*)hashtable_get(h, key6)));
    printf("\n");

    hashtable_set(h, key1, &value7);
    hashtable_set(h, key4, &value8);

    printf("%s - %d\n", key1, *((int*)hashtable_get(h, key1)));
    printf("%s - %d\n", key2, *((int*)hashtable_get(h, key2)));
    printf("%s - %d\n", key3, *((int*)hashtable_get(h, key3)));
    printf("%s - %d\n", key4, *((int*)hashtable_get(h, key4)));
    printf("%s - %d\n", key5, *((int*)hashtable_get(h, key5)));
    printf("%s - %d\n", key6, *((int*)hashtable_get(h, key6)));
    printf("\n");

    hashtable_delete_key(h, key3);
    hashtable_delete_key(h, key1);
    hashtable_delete_key(h, key2);

    if (hashtable_get(h, key3) == NULL)
        printf("%s - not exist\n", key3);
    else
        printf("%s - exist\n", key3);

    if (hashtable_get(h, key1) == NULL)
        printf("%s - not exist\n", key1);
    else
        printf("%s - exist\n", key1);

    if (hashtable_get(h, key2) == NULL)
        printf("%s - not exist\n", key2);
    else
        printf("%s - exist\n", key2);

    printf("\n");

    hashtable_set(h, key1, &value3);
    hashtable_set(h, key2, &value7);
    hashtable_set(h, key3, &value5);

    printf("%s - %d\n", key1, *((int*)hashtable_get(h, key1)));
    printf("%s - %d\n", key2, *((int*)hashtable_get(h, key2)));
    printf("%s - %d\n", key3, *((int*)hashtable_get(h, key3)));
    printf("%s - %d\n", key4, *((int*)hashtable_get(h, key4)));
    printf("%s - %d\n", key5, *((int*)hashtable_get(h, key5)));
    printf("%s - %d\n", key6, *((int*)hashtable_get(h, key6)));

    hashtable_delete(h);

    return 0;
}
