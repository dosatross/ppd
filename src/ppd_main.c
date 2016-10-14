#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

int main(int argc, char **argv)
{
  /* represents the data structures to manage the system */
  struct ppd_system system;
  struct menu_item menu[NUM_MENU_ITEMS];
  menu_function menu_function;

  system_init(&system); /*initialise system*/

  /* validate command line arguments */
  if(argc != 2 && argc != 3)
  {
    printf("Invalid arguments");
    return EXIT_FAILURE;
  }

  if(!is_valid_stock_file(&system,argv[1]))
  {
    return EXIT_FAILURE;
  }

  system.stock_file_name = argv[1];

  if (load_stock(&system,argv[1]) == false)
  {
    return EXIT_FAILURE;
  }

  if(argc == 3)
  {
    system.coin_from_file = true;
    if(!is_valid_coins_file(&system,argv[2]))
    {
      return EXIT_FAILURE;
    }

    system.coin_file_name = argv[2];

    if (load_coins(&system,argv[2]) == false)
    {
      return EXIT_FAILURE;
    }
  }

  init_menu(menu);

  while(true)
  {
    print_menu(menu);

    if ((menu_function = get_menu_choice(menu)) == NULL)
    {
      continue;
    }
    
    if (menu_function(&system) == false)
    {
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}