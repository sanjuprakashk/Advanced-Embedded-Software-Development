/**
 * @\file	k_animal_sort.c
 * @\author	Sanju Prakash Kannioth
 * @\brief	This program uses kernel linked lists to perform sorting and storing
 * @\date	02/17/2019 
 * References : https://www.tldp.org/LDP/lkmpg/2.6/html/x323.html ,
				https://gist.github.com/atduskgreg/3cf8ef48cb0d29cf151bedad81553a54,
				http://www.roman10.net/2011/07/28/linux-kernel-programminglinked-list/,
				https://www.geeksforgeeks.org/linked-list-set-2-inserting-a-node/ 
*/


#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sort.h>
#include <linux/list.h>
#include <uapi/linux/string.h>

#include <linux/slab.h>

static int filter_count = 0;	//Count filter given from command line
static char *filter = "DEFAULT";//Animal name filter given from command line

/* Static array with animal names */
static char user_array_seed[][50] = {"spoonbill","squid","squirrel","starfish","mole","zebra", \
"stingray","stoat", "moth", "goat", "mouse", "hornet", "gull", "hookworm","porpoise",\
"tortoise","warbler", "moth", "goat", "starfish", "lemming", "lemur", "leonberger", \
"leopard","mare", "zebra", "marmoset", "zebra", "zebra", "ant", "pufferfish",\
"puffin", "pug", "pullet", "puma",  "nubiangoat", "nudibranch", "numbat", "nurseshark", \
"nutcracker", "nuthatch", "moth", "zebra", "starfish", "lemming","lemur", "zebra", \
"squid", "puffin", "zebra" };

static int animals_count_arr[50]; // Array to store count of each array element 

static char sorted_animals_array[20][50]; //Array to store sorted elements


module_param(filter, charp, 0000);
MODULE_PARM_DESC(filter, "A character string");
module_param(filter_count, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(filter_count, "An integer");

/*module_param_array(user_array_seed, charp, &count3, 0000);
MODULE_PARM_DESC(user_array_seed, "An array of chars");
*/

struct sorted_animals{
	char name[20];
	struct list_head list;
	int count;
};

struct filtered_animals{
	char name[20];
	struct list_head list;
	int count;
};

struct Node 
{ 
	char name[20];
	int count; 
	struct Node *next; 
};

struct sorted_animals *temp_node; // Temporary array of type sorted_animals

struct filtered_animals filtered; 

struct filtered_animals *temp_node1; // Temporary array of type filtered_animals

struct Node *node1; 

struct sorted_animals animalList;

/* Function to populate set2 list with elements */
void populateList(void)
{
	if(node1 != NULL)
	{
		temp_node1->count = node1->count;
		strcpy(temp_node1->name,node1->name);
		node1 = node1->next;
	}
}

/* Function to add elements to intermediate list used to store elements */
void add_to_list(struct Node** reference_head, int count, char *name) 
{ 
	struct Node* new_node = (struct Node*) kmalloc(sizeof(struct Node), GFP_KERNEL); 

	struct Node *last = *reference_head;  

	new_node->count  = count; 

	strcpy(new_node->name, name); 

	new_node->next = NULL; 

	if (*reference_head == NULL) 
	{ 
		*reference_head = new_node; 
		return; 
	} 

	while (last->next != NULL) 
	{
		last = last->next; 
	}

	last->next = new_node; 
	return; 
} 

/* Function to perform comapre logic to be used by kernel sort function */ 
int compare(const void *arr1, const void *arr2)
{
	int compare = 0;
	char *arr1_str = (char *)arr1;
	char *arr2_str = (char *)arr2;

	compare = strcmp(arr1_str,arr2_str);
	if(compare >= 0)
	{
		return 1;
	}
	else
	{
		return -1;
	}

}


static int init_func(void)
{
	struct Node* head = NULL; 
	int no_elements = 0;
	int i = 0, j = 0, element = 0, exists = 0, no_of_nodes_set1 = 1, no_of_nodes_set2 = 1;


	int animals_count = (sizeof(user_array_seed))/(sizeof(user_array_seed[0]));

	INIT_LIST_HEAD(&animalList.list);

	sort(user_array_seed, animals_count, sizeof(user_array_seed[0]), compare, NULL); // Sort function present in sort.h

	for(i=0;i<animals_count;i++)
	{
		for(j=0;j<animals_count;j++)
		{
			if(strcmp(user_array_seed[i],user_array_seed[j]) == 0)
			{
				animals_count_arr[i]++;
			}

			if(strcmp(user_array_seed[i], sorted_animals_array[j]) == 0)
			{
				exists = 1;	
			}
		}

		if(exists != 1)
		{
			strcpy(sorted_animals_array[element],user_array_seed[i]);
			

			temp_node= (struct sorted_animals *)kmalloc(sizeof(struct sorted_animals), GFP_KERNEL);

			strcpy(temp_node->name,user_array_seed[i]);
			temp_node->count = animals_count_arr[i];

			list_add_tail(&(temp_node->list), &(animalList.list));
			element++;
			no_of_nodes_set1++;

		}
		exists = 0;
	}
 	
 	printk("Amount of dynamic memory allocated for first set1 = %d", no_of_nodes_set1*sizeof(struct sorted_animals));
	
	INIT_LIST_HEAD(&filtered.list);


	printk("Printing contents of Set 1\n");
	list_for_each_entry(temp_node, &animalList.list, list)
	{
		printk("Name = %s Count= %d\n", temp_node->name, temp_node->count);
		if(filter_count != 0 && !strcmp(filter, "DEFAULT")) // If user specifies only filter count
		{
			if(temp_node->count >= filter_count)
			{
				add_to_list(&head, temp_node->count, temp_node->name);
				no_elements++;
			}
		}
		else if(filter_count != 0 && !strcmp(filter,temp_node->name)) // If user specifies animal name and filter count
		{
			if(temp_node->count >= filter_count)
			{
				add_to_list(&head, temp_node->count, temp_node->name);
				no_elements++;
			}
		}
		else if(filter_count == 0 && (!strcmp(filter,temp_node->name))) // If user specifies only animal name
		{
			add_to_list(&head, temp_node->count, temp_node->name);
			no_elements++;
		}
		else if(filter_count == 0 && (!strcmp(filter,"DEFAULT"))) // If user does not specify any values
		{
			add_to_list(&head, temp_node->count, temp_node->name);
			no_elements = element;
		}	
	}
	
	printk("Number of nodes created for Set 1 = %d", no_of_nodes_set1);

	node1 = head;

	for(i=0; i<no_elements; ++i){
		temp_node1= (struct filtered_animals *)kmalloc(sizeof(struct filtered_animals), GFP_KERNEL);
		
		populateList(); // Populate set 2 with filtered array
		no_of_nodes_set2++;
		
		list_add_tail(&(temp_node1->list), &(filtered.list));
	}

	printk("Printing Contents of set 2\n");
	list_for_each_entry(temp_node1, &filtered.list, list)
	{
		printk("Name = %s Count= %d\n", temp_node1->name, temp_node1->count);
	}

	printk("Amount of dynamic memory allocated for first set2 = %d", no_of_nodes_set2*sizeof(struct filtered_animals));
	printk("Number of nodes created for Set 2 = %d", no_of_nodes_set2);
	printk("\n");
	return 0;
	
	
}

static void exit_func(void)
{
	int count_set1 = 1, count_set2 = 1;
	printk(KERN_ALERT "Exiting!\n");
	list_for_each_entry(temp_node,&animalList.list,list)
	{
		count_set1++;
		kfree(temp_node);
	}
	printk("Amount of memory freed for Set 1 = %d", count_set1*sizeof(struct sorted_animals));

	printk("Sorted list set freed");

	list_for_each_entry(temp_node1,&filtered.list,list)
	{
		count_set2++;
		kfree(temp_node1);
	}

	printk("Amount of memory freed for Set 2 = %d", count_set2*sizeof(struct filtered_animals));

	printk("Total memory freed = %d", (count_set1*sizeof(struct sorted_animals))+(count_set2*sizeof(struct filtered_animals)));
	printk("Filtered list set freed");
	printk("\n");
}


module_init(init_func);
module_exit(exit_func);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sanju Prakash Kannioth");
MODULE_DESCRIPTION("Kernel Animal Sorting Module");
MODULE_VERSION("1.0");