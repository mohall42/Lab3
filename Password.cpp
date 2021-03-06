#include "Password.h"
#include "String.h"
#include "ListArray.h"
#include "ListArrayIterator.h"

#include <iostream>
using namespace std;
using namespace CSC2110;

using CSC2110::ListArrayIterator;
using CSC2110::String;
using CSC2110::ListArray;


Password::Password()
{
	ListArray<String>* viable_words = new ListArray<String>();
	
	ListArray<String>* all_words = new ListArray<String>();
	
    len = 0;
}

Password::~Password()
{
	ListArrayIterator<String>* viable_iter = viable_words->iterator();
	while (viable_iter->hasNext())
	{
		String* viable_word = viable_iter->next();
		delete viable_word;
		delete viable_iter;
	}
	ListArrayIterator<String>* all_iter = all_words->iterator();
	while (all_iter->hasNext())
	{
		String* all_words = all_iter->next();
		delete all_words;
		delete all_iter;
	}
}

int Password::getNumMatches(String* curr_word, String* word_guess)
{
	int num_matches = 0;
	for (int i = 0; i < len; i++)
	{		
		char curr = curr_word->charAt(i);
		char guess = word_guess->charAt(i);
		if(curr == guess)
		{
		num_matches ++;
		}
	}
	return num_matches;
}
void Password::addWord(String* word)
{
	if (len == 0)
	{
		len = word->length();
	}
	
	if(len == word->length())
	{
		all_words->add(word);
		viable_words->add(word);
	}
}

void Password::guess (int try_password, int num_matches)
{
	int matches = 0;
	int i = 1;
	String* password = getOriginalWord(try_password);
	ListArrayIterator<String>* iter = viable_words->iterator();
	//int stringSize = password->length();
	while ( iter->hasNext())
	{	
		
		 matches = getNumMatches(password, viable_words[i]);
		 if (matches < num_matches)
			 viable_word->remove(i);
		 
		i++;	//origninalIndex = iter->next();
	}	
	/*ListArrayIterator<String>* allIter = all_words->iterator();
	while (allIter->hasNext())
	{
		//if (getNumMatches(password, iter->next) < num_matches)
	//		viable_words->remove(iter->next);
	}*/
}

int Password::getNumberOfPasswordsLeft() //returns the number of possible passwords remaining
{
	int size;
	size = viable_words->size();
	return size;
	//return size of viable_words
}	

void Password::displayViableWords()
{
	ListArrayIterator<String>* viable_iter = viable_words->iterator();
	{
		while(viable_iter->hasNext())
		{
			String* viable_words = viable_iter->next();
			cout<<viable_words;
		}
	}
	delete viable_iter;
}

String* Password::getOriginalWord(int index)
{
	 all_words->get(index);
}

int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}
