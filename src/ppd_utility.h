#include <stdio.h>
#include "ppd_main.h"
#ifndef PPD_UTILITY
#define PPD_UTILITY
/* the default line length for input / output */
#define LINELEN 320
/* the last two characters required in a string as returned from fgets */
#define EXTRACHARS 2

/**
 * @file ppd_utility.h defines some helper functions for the rest of your
 * program. In particular, it declares three of the most important
 * functions for your application: @ref load_data which loads the data
 * in from files, @ref system_init which initialises the system to a
 * known safe state and @ref system_free which frees all memory when you
 * are going to quit the application.
 **/

void read_rest_of_line(void);
bool check_numeric(char*);
void remove_newline(char*);
void remove_return(char*);
void check_buffer_overflow(char* );

bool load_stock(struct ppd_system *, const char *);
bool load_coins(struct ppd_system *, const char *);

bool system_init(struct ppd_system *);
void system_free(struct ppd_system *);

struct ppd_list * init_list();

bool is_valid_stock_file(struct ppd_system *, const char *);
bool is_valid_id(char*);
bool is_valid_name(char*);
bool is_valid_desc(char*);
bool is_valid_price(char*);
bool is_valid_on_hand(char*);
void print_invalid_stock_file_message();

bool is_valid_coins_file(struct ppd_system *, const char *);
bool is_valid_denom(char* denom);
bool is_valid_coin_count(char* count);
void print_invalid_coins_file_message();
#endif
