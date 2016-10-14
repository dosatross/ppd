#include "ppd_stock.h"
struct ppd_list * init_list()
{
	struct ppd_list * ppd_list = malloc(sizeof(*ppd_list));
    if(ppd_list != NULL)
    {
        ppd_list->head = NULL;
        ppd_list->count = 0;
    }
    return ppd_list;
}


bool insert_stock(struct ppd_list * ppd_list,struct ppd_stock * stock_item)
{
	struct ppd_node * current, * previous;

	/*dynamically allocate memory for new node*/
	struct ppd_node * new_node = malloc(sizeof(*new_node));
	if(new_node == NULL)
	{
		return false;
	}
	/*dynamically allocate memory for new stock item*/
	new_node->data = malloc(sizeof(*new_node->data));
	if(new_node->data == NULL)
	{
		free(new_node);
		return false;
	}

	/*copy stock to new node*/
	*new_node->data = *stock_item;

	/*remove next node pointer*/
	new_node->next = NULL;

	/*current (list pointer) point to head*/
	previous = NULL;
	current = ppd_list->head; 

	/*traverse until alphabetically ordered position is found*/
	while(current != NULL)
	{
		if(strcmp(stock_item->name,current->data->name) < 0)
		{
			/*break if new item is alphabetically less than current item*/
			break;
		}
		previous = current;
		current = current->next;
	}

	/*insert node*/
	if(ppd_list->head == NULL)
	{
		/*if list is empty*/
		ppd_list->head = new_node;
	}
	else if(previous == NULL)
	{
		/*insert at head*/
		new_node->next = ppd_list->head;
		ppd_list->head = new_node;
	}
	else
	{
		previous->next = new_node;
		new_node->next = current;
	}
	ppd_list->count++; /*increment list size*/

	return true;
}


bool remove_stock(struct ppd_list * ppd_list,struct ppd_stock * stock_item)
{
	struct ppd_node * current, * previous;


	/*current (list pointer) point to head*/
	previous = NULL;
	current = ppd_list->head; 

	while(current != NULL)
	{
		if(strcmp(stock_item->name,current->data->name) == 0)
		{
			break;
		}
		previous = current;
		current = current->next;
	}

	if(current == NULL)
	{
		/*return if item doesnt exist in list*/
		return false;
	}

	if(previous == NULL)
	{
		/*remove at head*/
		ppd_list->head = current->next;
	}
	else
	{
		previous->next = current->next;
	}
	ppd_list->count--; /*decrement list size*/


	/*free removed item from heap*/
	free(current->data);
    free(current);

	return true;
}


struct ppd_stock * search_stock_by_id(struct ppd_system * system,char * id)
{
	struct ppd_node * current, * previous;

	/*current (list pointer) point to head*/
	previous = NULL;
	current = system->item_list->head; 

	/*traverse until alphabetically ordered position is found*/
	while(current != NULL)
	{
		if(strcmp(id,current->data->id) == 0)
		{
			return current->data;
		}
		previous = current;
		current = current->next;
	}
	return NULL;
}

bool free_list(struct ppd_list * ppd_list)
{
	struct ppd_node * current;

	while((current = ppd_list->head) != NULL)
	{
		ppd_list->head = ppd_list->head->next;
		free(current->data);
		free(current);
	}
	free(ppd_list);
	return true;
}

/*will return the largest id + 1, skips ids less than the largest that do not exist*/
bool get_next_id(struct ppd_list * ppd_list,char* id)
{
	struct ppd_node * current, * previous;
	char id_number[IDLEN]; 
	int id_number_int;

	/*current (list pointer) point to head*/
	previous = NULL;
	current = ppd_list->head;
	strcpy(id,"I0001");

	while(current != NULL)
	{
		if(strcmp(id,current->data->id) < 0)
		{
			strcpy(id,current->data->id);
		}
		previous = current;
		current = current->next;
	}

	memcpy(id_number,&id[1],IDLEN);
	id_number_int = atoi(id_number);
	id_number_int++;
	if(id_number_int > LIST_SIZE)
	{
		/*if list is full*/
		return false;
	}
	sprintf(id,"I%04d",id_number_int);

	return true;
}