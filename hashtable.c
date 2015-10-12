// Linked Hash Table implementation.
// v1.0

// The MIT License (MIT)
//
// Copyright (c) 2015 Adrian Bueno Jimenez  (adrian.buenoj@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct hashnode_s{
	char *key;
	char *value;
	struct hashnode_s *next;
};

struct hashtable_s{
	int size;
	struct hashnode_s **table;
};

typedef struct hashnode_s hashnode_t;
typedef struct hashtable_s hashtable_t;

/*
 *
 */
hashnode_t *hashnode_create(char *key, char *value, hashnode_t *next){

	hashnode_t *node = NULL;

	node = (hashnode_t *) malloc (sizeof(hashnode_t));
	if(node == NULL) return NULL;

	node->key = strdup(key);
	if(node->key == NULL){
		free(node);
		return NULL;
	}
	node->value = strdup(value);
	if(node->value == NULL){
		free(node->key);
		free(node);
		return NULL;
	}

	node->next = next;

	return node;
}

/*
 *
 */
char *hashnode_get_key(hashnode_t *node){
	if (node == NULL) return NULL;
	return node->key;
}

/*
 *
 */
char *hashnode_get_value(hashnode_t *node){
	if (node == NULL) return NULL;
	return node->value;
}

/*
 *
 */
hashnode_t *hashnode_get_next(hashnode_t *node){
	if (node == NULL) return NULL;
	return node->next;
}

/*
 * Free allocated memory from one node
 */
void hashnode_delete(hashnode_t *node){

	if(node == NULL) return;

	free(node->key);
	free(node->value);
	free(node);
	return;
}

/*
 * Free allocated memory from one node and the next ones in the list.
 */
void hashnode_delete_all(hashnode_t *node){

	if(node == NULL)
		return;

	hashnode_delete_all(node->next);
	node->next = NULL;

	hashnode_delete(node);

	return;
}

/*
 * CHange the value of a node.
 */
int hashnode_set_value(hashnode_t *node, char *value){

	if(node == NULL) return -1;

	free(node->value);

	if(value != NULL){
		node->value = strdup(value);
		if(node->value == NULL)
			return -1;
	}
	return 0;
}

/*
 * Searh in the list of nodes and returns the value if the node with that key exits.
 */
char *hashnode_search_and_get(hashnode_t *node, char *key){

	if(node == NULL || key == NULL) return NULL;

	if(strcmp(node->key, key) == 0)
		return node->value;

	return hashnode_search_and_get(node->next, key);
}

/*
 * Search if the key exits, if not, it creates one node with that key and value.
 */
hashnode_t *hashnode_search_and_set(hashnode_t *node, char *key, char *value){

	if(key == NULL) return NULL;

	if(node == NULL)
		return hashnode_create(key, value, NULL);

	if(strcmp(node->key, key) == 0){
		hashnode_set_value(node, value);
		return node;
	}

	node->next = hashnode_search_and_set(node->next, key, value);
	return node;
}

/*
 *
 */
hashtable_t *hashtable_create(int size){

    hashtable_t *hashtable = NULL;
    int i;

    if(size < 1) return NULL;

    hashtable = (hashtable_t *) malloc (sizeof(hashtable_t));
    if(hashtable == NULL) return NULL;

    hashtable->size = size;
    hashtable->table = (hashnode_t **) malloc (size * sizeof(hashnode_t*));
    if(hashtable->table == NULL) return NULL;

    for(i = 0; i < size; i++)
        hashtable->table[i] = NULL;

    return hashtable;
}

/*
 * Using djb2 algorithm by Dan Bernstein
 */
unsigned long get_string_hash_value(char *str){

	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

/*
 *
 */
unsigned long hashtable_calculate_key_position(hashtable_t *hashtable, char *key){

    return get_string_hash_value(key) % hashtable->size;
}

/*
 *
 */
int hashtable_set_key_value(hashtable_t *hashtable, char *key, char *value){

    unsigned long key_pos = 0;

    if(hashtable == NULL || key == NULL) return -1;

    key_pos = hashtable_calculate_key_position(hashtable, key);

	hashtable->table[key_pos] = hashnode_search_and_set(hashtable->table[key_pos], key, value);
	if(hashtable->table[key_pos] == NULL)
		return -1;

	return 0;
}

/*
 *
 */
char *hashtable_get_value(hashtable_t *hashtable, char *key){

    unsigned long key_pos = 0;

	if(hashtable == NULL || key == NULL) return NULL;

	key_pos = hashtable_calculate_key_position(hashtable, key);

	return hashnode_search_and_get(hashtable->table[key_pos], key);
}

/*
 *
 */
void hashtable_delete(hashtable_t *hashtable){

    int i;

    if(hashtable == NULL) return;

	//delete all possible lists
    for(i = 0; i < hashtable->size; i++)
		hashnode_delete_all(hashtable->table[i]);

	free(hashtable->table);
	free(hashtable);

    return;
}
