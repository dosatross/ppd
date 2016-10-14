#include "ppd_menu.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
  menu->function = &display_items;
  strcpy((menu++)->name,"Display Items");
  menu->function = &purchase_item;
  strcpy((menu++)->name,"Purchase Items");
  menu->function = &save_and_exit_system;
  strcpy((menu++)->name,"Save and Exit");
  menu->function = &add_item;
  strcpy((menu++)->name,"Add Item");
  menu->function = &remove_item;
  strcpy((menu++)->name,"Remove Item");
  menu->function = &display_coins;
  strcpy((menu++)->name,"Display Coins");
  menu->function = &reset_stock;
  strcpy((menu++)->name,"Reset Stock");
  menu->function = &reset_coins;
  strcpy((menu++)->name,"Reset Coins");
  menu->function = &abort_program;
  strcpy((menu)->name,"Abort Program");
}

/**
 * @return a menu_function that defines how to perform the user's
 * selection
 **/
menu_function get_menu_choice(struct menu_item * menu)
{
  char buffer[BUFFER_SIZE + FGETS_EXTRA_SPACES];
  int option;
  int i;

  fgets(buffer,BUFFER_SIZE + FGETS_EXTRA_SPACES,stdin);
  check_buffer_overflow(buffer);
  remove_newline(buffer); /*remove newline created by fgets*/
  
  if (check_numeric(buffer) == false)
  {
      return NULL;
  }
  option = strtol(buffer,NULL,0);
  if (is_valid_option(option) == true)
  {
    for(i = 1; i < option; i++)
    {
      menu++;
    }
    return menu->function;
  }
  printf("Error: Invalid Option\n");
  return NULL;
}


void print_menu(struct menu_item * menu)
{
  int i;
  puts("Main Menu:");
  for(i = 1;i <= 3;i++)
  {
    printf("%d.%s\n",i,(menu++)->name);
  }
  puts("Administrator-Only Menu:");
  for(i = 4;i <= 9;i++)
  {
    printf("%d.%s\n",i,(menu++)->name);
  }
  printf("Select your option (1-9): ");
}

bool is_valid_option(int option)
{
  if(option > 0 && option <= NUM_MENU_ITEMS)
  {
    return true;
  }
  return false;
}