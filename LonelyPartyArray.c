
//Aadil Habibi - 4296487
//Dr. Szumlanski
//Due Date - 09/23/18


#include <stdio.h>
#include <stdlib.h>
#include "LonelyPartyArray.h"

double difficultyRating(void)
{
  return 4.0;
}

double hoursSpent(void)
{
  return 46.0;
}

//Creating an array of fragments by fragment length
LonelyPartyArray *createLonelyPartyArray(int num_fragments, int fragment_length)
{
  int i;

  //If you have nothing passed in, you immediately exit
  if(num_fragments <= 0 || fragment_length <= 0)
  {
    return NULL;
  }
    //Creating the name of the Arrays
  LonelyPartyArray *party = NULL;

  //Initializing LPA and variables inside
  party =  malloc(sizeof(LonelyPartyArray));
  party->num_fragments = num_fragments;
  party->fragment_length = fragment_length;
  party->size = 0;
  party->fragments = malloc(sizeof(int*) * party->num_fragments);
  party->fragment_sizes = malloc(sizeof(int*) * num_fragments);
  party->num_active_fragments = 0;

  //If call for malloc fails for Party Array
  if(party == NULL)
  {
    return NULL;
  }

  //If calls for fragment or their sizes fail, free all memory and exit program
  if(party->fragments == NULL)
  {
    free(party);
    return NULL;
  }

  //If malloc for fragment sizes fails
  if(party->fragment_sizes == NULL)
  {
    //Free all allocated memory
    free(party->fragments);
    free(party);
    return NULL;
  }

  //If malloc is correct
  for(i = 0; i < num_fragments; i++)
  {
    party->fragments[i] = NULL;
    party->fragment_sizes[i] = 0;
  }

    printf("-> A new LonelyPartyArray has emerged from the void. (capacity: %d, fragments: %d)\n", num_fragments*fragment_length, party->num_fragments);
    return  party;
}

//Freeing all of the memory allocated, and putting party to NULL
LonelyPartyArray *destroyLonelyPartyArray(LonelyPartyArray *party)
{
  int i, j;

  //if party is already NULL - exit
  if(party == NULL)
  {
    return NULL;
  }

  for(i = 0; i < party->num_fragments; i++)
  {
    if(party->fragments[i] != NULL)
    {
      free(party->fragments[i]);
    }
  }

  //Free allocated memory in order, the party being last
  free(party->fragment_sizes);
  free(party->fragments);
  free(party);

  printf("-> The LonelyPartyArray has returned to the void.\n");

  return NULL;
}

int set(LonelyPartyArray *party, int index, int key)
{
  int location, location_index, i;
  int lower, upper;

  //If Party is NULL exit program
  if(party == NULL)
  {
      printf("-> Bloop! NULL pointer detected in set().\n");
      return LPA_FAILURE;
  }

  //Finding fragment and the location inside fragment
  location = index / party->fragment_length;
  location_index = index % party->fragment_length;
  lower = ((index / party->fragment_length) * party->fragment_length);
  upper = lower + (party->fragment_length - 1);


  if(index < 0 || index > (party->num_fragments * party->fragment_length - 1))
  {
      printf("-> Bloop! Invalid access in set(). (index: %d, fragment: %d, offset: %d)\n", index, location, location_index);
      return LPA_FAILURE;
  }

  //Checking if fragment is NULL if so malloc space
  if(party->fragments[location] == NULL)
  {
      //Creating space for the length per fragment
      party->fragments[location] = malloc(sizeof(int) * party->fragment_length);
      //If call for malloc fails
      if (party->fragments[location] == NULL)
      {
        return LPA_FAILURE;
      }

      party->num_active_fragments++;

      //Setting each cell in Fragment to UNUSED
      for(i = 0; i < party->fragment_length; i++)
      {
          party->fragments[location][i] = UNUSED;
      }
      printf("-> Spawned fragment %d. (capacity: %d, indices: %d..%d)\n", location, party->fragment_length, lower, upper);
  }

  //Updating count for the sizes
  if(party->fragments[location][location_index] == UNUSED)
  {
    party->size++;
    party->fragment_sizes[location]++;
  }

  //Store key at index
  party->fragments[location][location_index] = key;
  return LPA_SUCCESS;
}

int get(LonelyPartyArray *party, int index)
{

    int location, location_index;

    if(party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in get().\n");
        return LPA_FAILURE;
    }

    location = index / party->fragment_length;
    location_index = index - (party->fragment_length * location);

    //If location being needed is invalid, return FAILURE
    if(index < 0 || index > (party->num_fragments * party->fragment_length - 1))
    {
        printf("-> Bloop! Invalid access in get(). (index: %d, fragment: %d, offset: %d)\n", index, location, location_index);
        return LPA_FAILURE;
    }

    //Checking if fragment being called has been intialized
    if ( party->fragments[location] != NULL)
    {
      return party->fragments[location][location_index];
    }

    return UNUSED;
}

int delete(LonelyPartyArray *party, int index)
{
    int location, location_index;
    int lower, upper;

    if(party == NULL)
    {
      printf("-> Bloop! NULL pointer detected in delete().\n");
      return LPA_FAILURE;
    }

    location = index / party->fragment_length;
    location_index = index % party->fragment_length;
    lower = (index / party->fragment_length) * party->fragment_length;
    upper = lower + (party->fragment_length - 1);

    //If index is given at invalid point
    if(index < 0 || index > (party->num_fragments * party->fragment_length-1))
    {
        printf("-> Bloop! Invalid access in delete(). (index: %d, fragment: %d, offset: %d)\n",index, location, location_index);
        return LPA_FAILURE;
    }

  //While Fragments are not NULL, go through LPA and decrement size and free
  if(party->fragments[location] != NULL)
  {
    if(party->fragments[location][location_index] == UNUSED)
    {
        return LPA_FAILURE;
    }
        //Set value to unused and decrease sizes
        party->fragments[location][location_index] = UNUSED;
        party->size--;
        party->fragment_sizes[location]--;

        if(party->fragment_sizes[location] == 0)
        {
            //Free memory since it's not used
            free(party->fragments[location]);
            party->fragments[location] = NULL;
            party->num_active_fragments--;
            printf("-> Deallocated fragment %d. (capacity: %d, indices: %d..%d)\n", location, party->fragment_length, lower, upper);
        }
      return LPA_SUCCESS;
  }
        return LPA_FAILURE;
}

int constainsKey(LonelyPartyArray *party, int key)
{
  int i, j;

  //If Party is set to NULL
  if (party == NULL)
  {
    return 0;
  }

  //Looping through entire LPA
  for(i = 0; i < party->num_fragments; i++)
  {
    for(j = 0; j < party->fragment_length; j++)
    {
      //If key is found return positive
      if(party->fragments[i][j] == key)
      {
        return 1;
      }
    }
  }
}

int isSet(LonelyPartyArray *party, int index)
{
  int location = 0;
  int location_index = 0;

  //If party is set to NULL
  if(party == NULL)
  {
    return 0;
  }

  if(index < 0 || index > (party->num_fragments * party->fragment_length - 1))
  {
    return 0;
  }

  location = index / party->fragment_length;
  location_index = index - (party->fragment_length * location);

  //Checking if the fragment and fragment index is UNUSED, if so then Set failed
  if(party->fragments[location] == NULL || party->fragments[location][location_index] == UNUSED)
  {
    return 0;
  }

  return 1;
}

int printIfValid(LonelyPartyArray *party, int index)
{
    int location, location_index;

    if(party == NULL)
    {
        return LPA_FAILURE;
    }

    location = index / party->fragment_length;
    location_index = index - (party->fragment_length * location);

    if(index < 0 || index > (party->num_fragments * party->fragment_length-1))
    {
        return LPA_FAILURE;
    }

    if(party->fragments[location] != NULL)
    {
      if(party->fragments[location][location_index] == UNUSED)
      {
        return LPA_FAILURE;
      }
      //If values are valid, print out the values
      printf("%d\n",party->fragments[location][location_index]);
      return LPA_SUCCESS;
    }
    //Else Party failed with valid numbers
    return LPA_FAILURE;
}

LonelyPartyArray *resetLonelyPartyArray(LonelyPartyArray *party)
{
    int i, j;

    if(party == NULL)
    {
        printf("-> Bloop! NULL pointer detected in resetLonelyPartyArray().\n");
        return party;
    }

    for(i = 0; i < party->num_fragments; i++)
    {
        //freeing all values in the fragments
        //Then setting them to NULL when freed
        if(party->fragments[i] != NULL)
        {
          free(party->fragments[i]);
          party->fragments[i] = NULL;
        }
    }

    for(j = 0; j < party->num_fragments; j++)
    {
      //Setting all the sizes for fragments to 0
      //Since they're not being used
      if(party-> fragment_sizes[j] != 0)
      {
        party->fragment_sizes[j] = 0;
      }
    }

    //Setting party and active fragment sizes to 0
    party->size = 0;
    party->num_active_fragments = 0;

    printf("-> The LonelyPartyArray has returned to its nascent state. (capacity: %d, fragments: %d)\n",party->num_fragments*party->fragment_length,party->num_fragments);

    return party;
}

int getSize(LonelyPartyArray *party)
{
    if(party == NULL)
    {
      return -1;
    }

    //Returning size for party array
    return party->size;
}

int getCapacity(LonelyPartyArray *party)
{
    if(party == NULL)
    {
      return -1;
    }

    //Returning the number of fragments by fragment fragment_length
    //To return overall numbers LPA can hold
    return party->num_fragments * party->fragment_length;
}

int getAllocatedCellCount(LonelyPartyArray *party)
{
    int count;

    if (party == NULL)
        return -1;

    //Only multiply active fragments by frag length
    //So that you have only frag. that are used
    count = party->num_active_fragments * party->fragment_length;

    return count;
}

long long unsigned int getArraySizeInBytes(LonelyPartyArray *party)
{
    long long unsigned int arraySize;
    if(party == NULL)
    {
      return 0;
    }

    //Multipling the frag. length by num fragment_sizes
    //Multiplied by size of int to get overall byte size
    arraySize = (party->fragment_length * party->num_fragments) * sizeof(int);
    return (long long unsigned int)(party->num_fragments * party->fragment_length) * sizeof(int);
}

long long unsigned int getCurrentSizeInBytes(LonelyPartyArray *party)
{

    if (party == NULL)
        return 0;

    return (long long unsigned int)(sizeof(party) + sizeof(LPA) + (sizeof(int*) * party->num_fragments) + (sizeof(int)*party->num_fragments) + ((party->num_active_fragments*party->fragment_length)*sizeof(int)));
}
