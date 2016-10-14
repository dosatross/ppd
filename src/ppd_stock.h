#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "ppd_coin.h"
#include "ppd_shared.h"
#ifndef PPD_STOCK
#define PPD_STOCK
#define STOCK_DELIM "|"
#define PRICE_DELIM "."
#define LIST_SIZE 9999
#define PRICE_STR_SIZE 7

/**
 * @file ppd_stock.h this file defines the data structures required to 
 * manage the stock list.
 **/

/**
 * The length of the id string not counting the nul terminator
 **/
#define IDLEN 5

/**
 * The maximum length of a product name not counting the nul terminator
 **/
#define NAMELEN 40

/**
 * The maximum length of a product description not counting the nul
 * terminator.
 **/
#define DESCLEN 255

/**
 * The default coin level to reset the coins to on request
 **/
#define DEFAULT_COIN_COUNT 20

/**
 * The default stock level that all new stock should start at and that 
 * we should reset to on restock
 **/
#define DEFAULT_STOCK_LEVEL 20

/**
 * The number of denominations of currency available in the system 
 **/
#define NUM_DENOMS 8

/**
 * Maximum number of coins that system will give as change
 **/
#define MAX_NUM_COINS_CHANGE 20

/**
 * Maximum amount money an item can cost
 **/
#define MAX_DOLLAR_PRICE 99
#define MAX_CENTS_PRICE 99

/**
 * Maximum count for a given item
 **/
#define MAX_ON_HAND 99

/**
 * Maximum count for a given denomination
 **/
#define MAX_COIN_COUNT 99

struct price
{
    unsigned dollars;       
    unsigned cents;
};

/**
 * data structure to represent a stock item within the system
 **/
struct ppd_stock
{
    /**
     * the unique id for this item
     **/
    char id[IDLEN + 1];
    /**
     * the name of this item
     **/
    char name[NAMELEN + 1];
    /**
     * the description of this item
     **/
    char desc[DESCLEN + 1];
    /**
     * the price of this item
     **/
    struct price price;
    /**
     * how many of this item do we have on hand? 
     **/
    unsigned on_hand;
    /**
     * a pointer to the next node in the list
     **/
};

/**
 * the node that holds the data about an item stored in memory
 **/
struct ppd_node
{
    /* pointer to the data held for the node */
    struct ppd_stock * data;
    /* pointer to the next node in the list */
    struct ppd_node * next;
};

/**
 * the list of products - each link is the list is a @ref ppd_node
 **/
struct ppd_list
{
    /**
     * the beginning of the list
     **/
    struct ppd_node * head;
    /**
     * how many nodes are there in the list?
     **/
    unsigned count;
};

/**
 * this is the header structure for all the datatypes that is 
 * passed around and manipulated
 **/
struct ppd_system
{
    /**
     * the container for all the money manipulated by the system
     **/
    struct coin cash_register[NUM_DENOMS];

    /**
     * the linked list - note that this is a pointer
     **/
    struct ppd_list * item_list;

    /**
     * the name of the coin file
     **/
    const char * coin_file_name;
    /**
     * the name of the stock file
     **/
    const char * stock_file_name;

    /* are the coins loaded in from a file ? */
    bool coin_from_file;
};



bool insert_stock(struct ppd_list*,struct ppd_stock *);
bool remove_stock(struct ppd_list *,struct ppd_stock *);
bool count_stock(struct ppd_list*);
struct ppd_stock * search_stock_by_id(struct ppd_system *,char *);
bool free_list(struct ppd_list *);
bool get_next_id(struct ppd_list *,char*);
#endif