#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function,
 * here is probably a good spot.
 **/

/*checks if is numeric*/
bool check_numeric(char* buffer)
{
  char * end;
  strtol(buffer, &end, 0);
  if(*end)
  {
      printf("Error: data entered was not numeric.\n");
      end = NULL;
      return false;
  }
  return true;
}

void remove_newline(char* buffer)
{
  if(buffer[strlen(buffer)-1]=='\n')
  {
    buffer[strlen(buffer)-1]='\0';
  }
}

void remove_return(char* buffer)
{
  if(buffer[strlen(buffer)-1]=='\r')
  {
    buffer[strlen(buffer)-1]='\0';
  }
}

 void check_buffer_overflow(char* buffer)
{
  if(buffer[strlen(buffer)-1]!='\n')
  {
      printf("Error: buffer overflow\n\n");
      read_rest_of_line();
  }
}

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

bool system_init(struct ppd_system * system)
{
  memset(system,'\0',sizeof(*system));
  return false;
}

bool load_stock(struct ppd_system * system, const char * filename)
{
  struct ppd_stock temp_stock;
  char price[PRICE_STR_SIZE];
  char buffer[LINELEN + EXTRACHARS];
  FILE *fileptr;


  /*open file*/
  fileptr = fopen(filename,"r"); 
  if(fileptr == NULL)
  {
      printf("Unable to open file %s",filename);
      return false;
  }

  system->item_list = init_list();

  while(fgets(buffer,LINELEN + EXTRACHARS,fileptr) != NULL)
  {
      strcpy(temp_stock.id,strtok(buffer,STOCK_DELIM));
      strcpy(temp_stock.name,strtok(NULL,STOCK_DELIM));
      strcpy(temp_stock.desc,strtok(NULL,STOCK_DELIM));
      strcpy(price,strtok(NULL,STOCK_DELIM));
      temp_stock.on_hand = atoi(strtok(NULL,STOCK_DELIM));

      temp_stock.price.dollars = atoi(strtok(price,PRICE_DELIM));
      temp_stock.price.cents = atoi(strtok(NULL,PRICE_DELIM));

      insert_stock(system->item_list,&temp_stock);
  }
  fclose(fileptr);
  return true;
}

bool load_coins(struct ppd_system * system, const char * filename)
{
  char buffer[LINELEN + EXTRACHARS];
  FILE *fileptr;
  int i;

  /*open file*/
  fileptr = fopen(filename,"r"); 
  if(fileptr == NULL)
  {
      printf("Unable to open file %s",filename);
      return false;
  }

  i = 0;
  while(fgets(buffer,LINELEN + EXTRACHARS,fileptr) != NULL)
  {
      system->cash_register[i].denom = atoi(strtok(buffer,COIN_DELIM));
      system->cash_register[i].count = atoi(strtok(NULL,COIN_DELIM));
      i++;
  }
  fclose(fileptr);
  return true;
}

bool is_valid_coins_file(struct ppd_system * system, const char * filename)
{
  int line_counter;
  FILE *fileptr;
  char* temp;
  char buffer[LINELEN + EXTRACHARS];
  char field[LINELEN + EXTRACHARS];

  /*open file*/
  fileptr = fopen(filename,"r"); 
  if(fileptr == NULL)
  {
      printf("Unable to open file %s",filename);
      return false;
  }

  line_counter = 0;
  while(fgets(buffer,LINELEN + EXTRACHARS,fileptr) != NULL)
  {
    line_counter++;
    if((temp = strtok(buffer,COIN_DELIM)) == NULL)
    {
      print_invalid_coins_file_message();
      return false;
    }
    strcpy(field,temp);
    if (!is_valid_denom(field))
    {
      print_invalid_coins_file_message();
      return false;
    }

    if((temp = strtok(NULL,COIN_DELIM)) == NULL)
    {
      print_invalid_coins_file_message();
      return false;
    }
    strcpy(field,temp);
    remove_newline(field);
    remove_return(field);
    if (!is_valid_coin_count(field))
    {
      print_invalid_coins_file_message();
      return false;
    }

    /*too many fields*/
    if(strtok(NULL,COIN_DELIM) != NULL)
    {
      print_invalid_coins_file_message();
      return false;
    }
  }

  if(line_counter != NUM_DENOMS)
  {
    print_invalid_coins_file_message();
    return false;
  }

  fclose(fileptr); 
  return true;
}

bool is_valid_denom(char* denom)
{
  if (!check_numeric(denom))
  {
    return false;
  }

  return true;
}

bool is_valid_coin_count(char* count)
{
  int count_int;
  if (!check_numeric(count))
  {
    return false;
  }

  count_int = atoi(count);

  if (count_int < 0 || count_int > MAX_COIN_COUNT)
  {
    return false;
  }

  return true;
}

bool is_valid_stock_file(struct ppd_system * system, const char * filename)
{
  char price[PRICE_STR_SIZE];
  int line_counter;
  char buffer[LINELEN + EXTRACHARS];
  char field[LINELEN + EXTRACHARS];
  char* temp;
  FILE *fileptr;


  /*open file*/
  fileptr = fopen(filename,"r"); 
  if(fileptr == NULL)
  {
      printf("Unable to open file %s",filename);
      return false;
  }

  line_counter = 0;
  while(fgets(buffer,LINELEN + EXTRACHARS,fileptr) != NULL)
  {
    line_counter++;

    if((temp = strtok(buffer,STOCK_DELIM)) == NULL)
    {
      print_invalid_stock_file_message();
      return false;
    }
    strcpy(field,temp);
    if (!is_valid_id(field))
    {
      print_invalid_stock_file_message();
      return false;
    }

    if((temp = strtok(NULL,STOCK_DELIM)) == NULL)
    {
      print_invalid_stock_file_message();
      return false;
    }
    strcpy(field,temp);
    if (!is_valid_name(field))
    {
      print_invalid_stock_file_message();
      return false;
    }

    if((temp = strtok(NULL,STOCK_DELIM)) == NULL)
    {
      print_invalid_stock_file_message();
      return false;
    }
    strcpy(field,temp);
    if (!is_valid_desc(field))
    {
      print_invalid_stock_file_message();
      return false;
    }

    if((temp = strtok(NULL,STOCK_DELIM)) == NULL)
    {
      print_invalid_stock_file_message();
      return false;
    }
    strcpy(price,temp);

    if((temp = strtok(NULL,STOCK_DELIM)) == NULL)
    {
      print_invalid_stock_file_message();
      return false;
    }
    strcpy(field,temp);
    remove_newline(field);
    remove_return(field);
    if (!is_valid_on_hand(field))
    {
      print_invalid_stock_file_message();
      return false;
    }

    /*too many fields*/
    if(strtok(NULL,STOCK_DELIM) != NULL)
    {
      print_invalid_stock_file_message();
      return false;
    }

    if (!is_valid_price(price))
    {
      print_invalid_stock_file_message();
      return false;
    }
    
  }
  fclose(fileptr);

  if(line_counter == 0 || line_counter > LIST_SIZE)
  {
    /*if stock file is empty or has greater than max amount of items*/
    print_invalid_stock_file_message();
    return false;
  }

  return true;
}

bool is_valid_id(char* id)
{
  char sub_buffer[LINELEN + EXTRACHARS];

  if(strlen(id) != IDLEN)
  {
    return false;
  }

  if(*id != 'I')
  {
    return false;
  }
  
  memcpy(sub_buffer,&id[1],IDLEN);

  if(!check_numeric(sub_buffer))
  {
    return false;
  }
  return true;
}

bool is_valid_name(char* name)
{
  if(strlen(name) > NAMELEN)
  {
    return false;
  }
  return true;
}

bool is_valid_desc(char* desc)
{
  if(strlen(desc) > DESCLEN)
  {
    return false;
  }
  return true;
}

bool is_valid_price(char* price)
{
  int dollars;
  int cents;
  char* temp;

  if ((temp = strtok(price,PRICE_DELIM)) == NULL)
  {
    return false;
  }
  if(!check_numeric(temp))
  {
    return false;
  }
  dollars = atoi(temp);

  if ((temp = strtok(NULL,PRICE_DELIM)) == NULL)
  {
    return false;
  }
  if(!check_numeric(temp))
  {
    return false;
  }
  if(strlen(temp) > 2)
  {
    return false;
  }
  cents = atoi(temp);

  /*too many tokens*/
  if(strtok(NULL,STOCK_DELIM) != NULL)
  {
    return false;
  }

  
  if (dollars == MAX_DOLLAR_PRICE && cents > MAX_CENTS_PRICE)
  {
    return false;
  }

  if (dollars > MAX_DOLLAR_PRICE)
  {
    return false;
  }

  if(cents%5 != 0)
  {

    return false;
  }

  if (dollars < 0 || cents < 0)
  {
    return false;
  }

  return true;
}

bool is_valid_on_hand(char* on_hand)
{
  int on_hand_int;
  if(!check_numeric(on_hand))
  {
    return false;
  }

  on_hand_int = atoi(on_hand);

  if(on_hand_int > MAX_ON_HAND || on_hand_int < 0)
  {
    return false;
  }
  return true;
}

void print_invalid_stock_file_message()
{
  printf("Invalid Stock File");
}

void print_invalid_coins_file_message()
{
  printf("Invalid Coins File");
}

void system_free(struct ppd_system * system)
{
  free_list(system->item_list);
}