/*******************************************************************************
 * Linked Hash Table implementation.
 *
 * Author: Adrian Bueno (adrian.buenoj@gmail.com)
 * License: MIT
 * Github: github.com/adrian-bueno/hashtable
 * Date: 1 July 2016
 ******************************************************************************/

#include <stdlib.h>


/**** FUNCTION POINTERS *******************************************************/

/*
 * Pointer to function that compare two keys.
 * Return: (= 0) key1 = key2, (< 0) key1 < key2, (> 0) key1 > key2.
 */
typedef int (*fp_compare_keys)(void *key1, void *key2);

/*
 * Pointer to function that calculates the hash value of a key.
 * Return: Hash value of key.
 */
typedef unsigned long (*fp_hashvalue)(void *key);

/*
 * Pointer to function that frees allocated memory of a key or value.
 * You don't need it if you don't allocate memory.
 */
typedef void (*fp_delete)(void *key_or_value);


/**** STRUCTURES **************************************************************/

struct hashnode_s {
    void *key;
    void *value;
    struct hashnode_s *next;
};

struct hashtable_s {
    unsigned long size;
    struct hashnode_s **table;
    fp_compare_keys compare;
    fp_hashvalue hashvalue;
    fp_delete key_delete;
    fp_delete value_delete;
};

typedef struct hashnode_s hashnode_t;
typedef struct hashtable_s hashtable_t;


/**** HASHNODE FUNCTIONS ******************************************************/

/*
 * Creates a new key-value node.
 * Parameter "key" cannot be NULL.
 * Return: Node created or NULL if an error ocurred.
 */
hashnode_t *hashnode_create(void *key, void *value, hashnode_t *next) {

    hashnode_t *node = NULL;

    if (key == NULL)
        return NULL;

    node = (hashnode_t *) malloc (sizeof(hashnode_t));
    if (node == NULL)
        return NULL;

    node->key = key;
    node->value = value;
    node->next = next;

    return node;
}

/*
 * Free allocated memory from one node. You should free key and value, if
 * needed, before calling this function.
 */
void hashnode_delete(hashnode_t *node) {

    if (node == NULL)
        return;

    free(node);
    return;
}


/**** HASHTABLE FUNCTIONS *****************************************************/

/*
 * Creates a new hash table.
 * Parameter "size" must be greater than 0.
 * Parameter "compare_function" cannot be NULL, is needed to compare keys.
 * Parameter "hashvalue_function" cannot be NULL, is needed to calculate key
 * position in the hash table.
 * Parameter "key_delete_function" is used to free allocated memory for a key,
 * this paremeter can be NULL if no memory was allocated with malloc or similar.
 * Parameter "value_delete_function" is used to free allocated memory for a
 * value, this paremeter can be NULL if no memory was allocated with malloc or
 * similar.
 * Return: NULL if error, pointer to hashtable on success.
 */
 hashtable_t *hashtable_create(unsigned long size,
                               fp_compare_keys compare_function,
                               fp_hashvalue hashvalue_function,
                               fp_delete key_delete_function,
                               fp_delete value_delete_function) {

    hashtable_t *hashtable = NULL;
    int i;

    if (size < 1 || compare_function == NULL || hashvalue_function == NULL)
        return NULL;

    hashtable = (hashtable_t *) malloc (sizeof(hashtable_t));
    if (hashtable == NULL)
        return NULL;

    hashtable->size = size;
    hashtable->table = (hashnode_t **) malloc (size * sizeof(hashnode_t*));
    if (hashtable->table == NULL)
        return NULL;

    for (i = 0; i < size; i++)
        hashtable->table[i] = NULL;

    hashtable->compare = compare_function;
    hashtable->hashvalue = hashvalue_function;
    hashtable->key_delete = key_delete_function;
    hashtable->value_delete = value_delete_function;

    return hashtable;
}


/*
 * Calculates in which position of the hash table the key goes.
 * Return: Calculated position.
 */
unsigned long hashtable_calculate_key_position(hashtable_t *hashtable, void *key){

    return hashtable->hashvalue(key) % hashtable->size;
}

/*
 * If the key doesn't exist in the hash table a new key-value pair is introduced
 * into the hash table, if it exist, it replaces the value with the one passed
 * as parameter.
 * Parameter "value" can be NULL.
 * Return: 0 on success, -1 on error.
 */
int hashtable_set(hashtable_t *hashtable, void *key, void *value) {

    unsigned long key_pos = 0;
    hashnode_t *node = NULL;

    if (hashtable == NULL || key == NULL)
        return -1;

    key_pos = hashtable_calculate_key_position(hashtable, key);

    node = hashtable->table[key_pos];

    // First position of list != NULL
    while (node != NULL) {
        if (hashtable->compare(key, node->key) == 0) {

            if(hashtable->value_delete != NULL)
                hashtable->value_delete(node->value);

            node->value = value;
            return 0;
        }
        else if (node->next == NULL) {
            node->next = hashnode_create(key, value, NULL);
            if(node->next == NULL)
                return -1;
            return 0;
        }
        else {
            node = node->next;
        }
    }

    //First position of list == NULL
    node = hashnode_create(key, value, NULL);
    if(node == NULL)
        return -1;
    hashtable->table[key_pos] = node;
    return 0;
}

/*
 * Gets the value associated to a key.
 * Return: NULL on error, value on success.
 */
void *hashtable_get(hashtable_t *hashtable, void *key){

    unsigned long key_pos = 0;
    hashnode_t *node = NULL;

    if (hashtable == NULL || key == NULL)
        return NULL;

    key_pos = hashtable_calculate_key_position(hashtable, key);

    node = hashtable->table[key_pos];

    while (node != NULL) {
        if (hashtable->compare(key, node->key) == 0) {
            return node->value;
        }
        else {
            node = node->next;
        }
    }

    return NULL;
}

/*
 * Deletes a key and its associated value from a hash table.
 * Return: -1 on error, 0 on success.
 */
int hashtable_delete_key(hashtable_t *hashtable, void *key) {

    unsigned long key_pos = 0;
    hashnode_t *node = NULL;
    hashnode_t *node_aux = NULL;

    if (hashtable == NULL || key == NULL)
        return -1;

    key_pos = hashtable_calculate_key_position(hashtable, key);

    node = hashtable->table[key_pos];

    // First position in list
    if (node != NULL && hashtable->compare(key, node->key) == 0) {
        hashtable->table[key_pos] = node->next;

		if (hashtable->key_delete != NULL)
			hashtable->key_delete(node->key);
		if (hashtable->value_delete != NULL)
			hashtable->value_delete(node->value);

        hashnode_delete(node);
        return 0;
    }

    // Other position in list
    while (node != NULL) {

        node_aux = node;
        node = node->next;

        if (hashtable->compare(key, node->key) == 0) {
            node_aux->next = node->next->next;

            if (hashtable->key_delete != NULL)
    			hashtable->key_delete(node->key);
    		if (hashtable->value_delete != NULL)
    			hashtable->value_delete(node->value);

            hashnode_delete(node);
            return 0;
        }
    }

    return 0;
}

/*
 * Frees all allocated memory in a hash table.
 */
void hashtable_delete(hashtable_t *hashtable) {

    hashnode_t *node = NULL;
    hashnode_t *node_aux = NULL;
    int i;

    if (hashtable == NULL)
        return;

    for (i = 0; i < hashtable->size; i++) {
        node = hashtable->table[i];

        //delete all possible lists
        while (node != NULL) {
            node_aux = node;
            node = node->next;

            if (hashtable->key_delete != NULL)
    			hashtable->key_delete(node_aux->key);
    		if (hashtable->value_delete != NULL)
    			hashtable->value_delete(node_aux->value);

            hashnode_delete(node_aux);
			node_aux = NULL;
        }
    }

    free(hashtable->table);
    free(hashtable);

    return;
}


/**** CALCULATE HASH VALUE FUNCTIONS ******************************************/

/*
 * Calculates the hash value of a string using djb2 algorithm by Dan Bernstein.
 * Return: Hash value of string, if parameter "string" is NULL it returns 0.
 */
unsigned long string_hash_value(void *string) {

    unsigned long hash = 5381;
    int c;

    if (string == NULL)
        return 0;

    while ((c = *((char*)string++)))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
