#ifndef HASHTABLE_HPP
# define HASHTABLE_HPP

# include "ConfigParser.hpp"
# include <vector>

/*
 * A hash table is a data structure that allows for fast access to key-value pairs.
 * It uses a hash function to map keys to unique indexes within a fixed-size array.
 * This enables constant time (average) lookup for retrieving values based on their keys.
 * Hash tables are particularly useful for scenarios
 * where frequent insertions, deletions, and lookups are required.
 * 
 * The HashTable class implements a hash table
 * with separate chaining for collision handling.
 * 
 * It stores key-value pairs in HashNode objects.
 * 
 * Each HashNode has:
 *     - a key
 *     - a vector of value strings (to store multiple values for a key)
 *     - a next pointer for linked list management in case of collisions
 * 
 * The HashTable maintains an array of HashNode pointers (table) with a size defined by TABLE_SIZE
*/

# define TABLE_SIZE 1024

class HashNode {
    public:
        std::string                 key;
        std::vector<std::string>    value;
        HashNode*                   next;

        HashNode(std::string k, std::string v) : key(k), next(NULL) {
            value.push_back(v);
        }

        void add_value(std::string v) {
            value.push_back(v);
        }
};


class HashTable {
    private:
        HashNode*   table[TABLE_SIZE];
        
    public:
        HashTable() {
            for (int i = 0; i < TABLE_SIZE; ++i) {
                table[i] = NULL;
            }
        }

        unsigned int hash(const std::string& key) {
            /*
             * This function implements the DJB (Daniel J. Bernstein) hash function (improved version) 
             * to calculate a hash index for a given key.
            */
            unsigned int hash = 5381;
            for (size_t i = 0; i < key.length(); i++) {
                char c = key[i];
                hash = ((hash << 5) + hash) + c;
            }
            return hash % TABLE_SIZE;
        }
        

        void insert(std::string key, std::string value) {
            /*
             * This function inserts a key-value pair into the hash table.
             *     Calculates the hash index using the hash function.
             *     It checks for existing nodes with the same key (collision handling).
             *         If a node exists, it adds new_node to the existing vector using add_value.
             *         If no node exists, a new node is created from key-value pair
             *         and inserted at the head of the linked list at the calculated index
            */

            unsigned int    index;
            HashNode*       new_node;

            new_node = NULL;
            index = hash(key);
            if (table[index] == NULL) {
                new_node = new HashNode(key, value);
                new_node->next = table[index];
                table[index] = new_node;
            } else {
                new_node = table[index];
                while (new_node != NULL) {
                    if (new_node->key == key) {
                        new_node->add_value(value);
                        return ;
                    }
                    new_node = new_node->next;
                }
            }
        }

        std::vector<std::string>    findVectorValue(std::string key) {
            /*
             * The function calculates hash index and iterates
             * through the linked list at that index, similar to the insert function.
             * Key Match:
             *     If node finds matching key (current->key == key):
             *     function returns vector
            */

            unsigned int index = hash(key);
            HashNode* current = table[index];

            while (current != NULL) {
                if (current->key == key) {
                    return current->value;
                }
                current = current->next;
            }
            return std::vector<std::string>(); 
        }

        ~HashTable() {}
};

#endif
