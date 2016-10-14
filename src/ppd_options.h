#include "ppd_main.h"
#include "ppd_utility.h"
#define DOLLARS_MULTIPLE 100
#define CENTS_MULTIPLE 1

/**
 * @file ppd_options.h declares all functions to implement the menu
 * options that can be called from the main menu. 
 **/

bool display_items(struct ppd_system *);

bool purchase_item(struct ppd_system *);

bool save_and_exit_system(struct ppd_system *);

bool add_item(struct ppd_system *);

bool remove_item(struct ppd_system *);

bool reset_stock(struct ppd_system *);

bool reset_coins(struct ppd_system *);

bool display_coins(struct ppd_system *);

bool abort_program(struct ppd_system *);

int get_length_longest_name(struct ppd_system *);

bool set_padding(char *,int);
