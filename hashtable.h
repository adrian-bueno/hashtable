/*******************************************************************************
 * Linked Hash Table implementation.
 *
 * Author: Adrian Bueno (adrian.buenoj@gmail.com)
 * License: MIT
 * Github: github.com/adrian-bueno/hashtable
 * Date: 1 July 2016
 ******************************************************************************/

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

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
 * Pointer to function that frees allocated memory of a key/value.
 * You don't need it if you don't allocate memory to create key/value.
 */
typedef void (*fp_delete)(void *key_or_value);

/*
 * Hash table type.
 */
typedef struct hashtable_s hashtable_t;

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
                               fp_delete value_delete_function);

/*
* If the key doesn't exist in the hash table a new key-value pair is introduced
* into the hash table, if it exist, it replaces the value with the one passed
* as parameter.
* Parameter "value" can be NULL.
* Return: 0 on success, -1 on error.
*/
int hashtable_set(hashtable_t *hashtable, void *key, void *value);

/*
 * Gets the value associated to a key.
 * Return: NULL on error, value on success.
 */
void *hashtable_get(hashtable_t *hashtable, void *key);

/*
 * Deletes a key and its associated value from a hash table.
 * Return: -1 on error, 0 on success.
 */
 int hashtable_delete_key(hashtable_t *hashtable, void *key);

 /*
  * Frees all allocated memory in a hash table.
  */
 void hashtable_delete(hashtable_t *hashtable);

 /*
  * Calculates the hash value of a string using djb2 algorithm by Dan Bernstein.
  * Return: Hash value of string, if parameter "string" is NULL it returns 0.
  */
unsigned long string_hash_value(void *string);

#endif
