/**********************************************************************
 * ass5.cpp - CSCI124 - Ass5 - Implementation file for displaying text file word statistics
 * <NAME> <STUDENT No.> <DATE>
 **********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "list.h"
#include "btree.h"
using namespace std;

// ******** Global Data ********

BinaryTree WordTree;

// ******** Function Prototypes ********

void CleanUp();

// ******** Menu Function Definitions ********

void TestContainers()
{
	int i; char *WrdPtr, Word[10];

	cout<<"\n **** TESTING CONTAINERS ****\n";
	cout<<"Testing IntList\n";
	LinkedList IntList;		// declare a list of ints
	cout<<"Inserted:=> ";
	for(i=0;i<10;i++)
	{
		cout<<i<<' ';
		IntList.AddToTail(i);	// insert some ints into it
	}
	cout<<endl;
	IntList.SetIterator();
	cout<<"Extracted=> ";
	while(IntList.More())
		cout<<IntList.Next()<<' ';            // print the ints in the list
	cout<<endl<<endl;

	cout<<"Testing WordTree\n";
	WordCounterPtr P1, P2;                        // declare some WordCounterPtrs
	Word[1]='\0';
	cout<<"Inserted =>\n";
	for(i=0;i<100;i++)
	{
		Word[0]=('A'+rand()%10);                // get a random 1 letter word
		cout << Word << ", ";
		P1 = new WordCounterType;             // allocate new  WordCounterType
		P1->Word = new char[strlen(Word)+1];  // allocate memory for each word
		strcpy(P1->Word,Word);
		P1->Count = 1;                        // set word counter
		int WordPos = i;
		P1->LocList.Insert(WordPos);          // add the word's position to the new WordCounter
		if(WordTree.Locate(P1, P2))           // if word is already in tree then...
		{
			P2->Count++;                     // increment esisting WordCounter
			P2->LocList.AddToTail(WordPos);  // add the word's position to the existing WordCounter
			delete P1->Word;                 // get rid of not needed WordCounter
			delete P1;                       // ditto

		}
		else
			WordTree.Insert(P1);	    // else not in tree! so insert WordCounter
	}
	cout<<endl;
	cout<<"Extracted=> \n";
	WordTree.SetIterator();
	while(WordTree.More())
	{
		P1 = WordTree.Next();
		cout << P1->Word;                        // print each word in the tree
		cout << '\t' << P1->Count<<"\t";
		P1->LocList.SetIterator();
		while(P1->LocList.More())
			 cout<<P1->LocList.Next()<<' ';
		cout << endl;
	}
	cout<<endl;
	CleanUp();                                     // delete tree
	cout<<" **** END OF TESTS ****\n\n";

}


void ReadTxtFile()
{
// ask user to "Enter filename: ";
	char filename[30];
	cout << "Enter filename: " << endl;
	cin >> filename;
// open file
	ifstream fin;
	fin.open(filename);
// if file not found then print error msg and exit
	if (fin.fail())
	{
		cout << "Error: File not found." << endl;
		exit;
	}
// set counter to zero
	int counter = 0;
// read word
	char word[50];

// while not eof
	while (!fin.eof())
	{
//     convert word to lower case and remove any punctuation marks
		fin >> word;
		char newword[50];
		for(int i = 0,j=0; i <= strlen(word) && j <= strlen(word); i++, j++)// convert word to lower case and remove any punctuation marks
                {
                        while (ispunct(word[j]))
				j++;
                        newword[i] = tolower(word[j]);
                }	
		strcpy(word, newword);
		if (isalpha(newword[0]) || isdigit(newword[0]))
		{		
//     increment counter
			counter++;
//     allocate new WordCounterType
			WordCounterPtr p = new WordCounterType, p2;
//     put data in new WordCounter
			p->Word = new char[strlen(word)+1];
			strcpy(p->Word, word);
//     if word is already in tree then
			if(WordTree.Locate(p, p2))           
			{			
//         inc existing WordCounter
				p2->Count++;   
//         add the location (ie counter value) to existing WordCounter                  
				p2->LocList.AddToTail(counter);  
//         free memory 
				delete p->Word;              
				delete p;                
			}
//     else
			else
			{
//         add new wordCounter to tree
				WordTree.Insert(p);
				p->Count = 0;
				p->Count++;
				p->LocList.AddToTail(counter);
			}
		}
	}
//  print how many words were added to tree
	cout << counter << " words were added to the tree." << endl;
//  close file
	fin.close();
}


void DisplayWordStats()
{
// set WordTree's Iterator
	WordCounterPtr p;
	WordTree.SetIterator();
// print heading
	cout << setw(20) << setfill(' ') << "Word";
	cout << "\tCount\tLocations" << endl;
// while more words in tree do
	while (WordTree.More())
	{
//     get next WordCounterPtr
		p = WordTree.Next();
//     print word
		cout << setw(20) << setfill(' ') << p->Word;
//     print word count 
		cout << '\t' << p->Count << '\t';
//     set WordCounter's list iterator
		p->LocList.SetIterator();
//     while more ints in list
		while (p->LocList.More())
//         print next word location
			cout << p->LocList.Next() << ' ';
		cout << endl;
	}
}


void CleanUp()
{
// set WordTree's Iterator
 	WordTree.SetIterator();
        WordCounterPtr p;
// while more words in tree do
        while (WordTree.More())
        {
//     get next WordCounterPtr
                p = WordTree.Next();
//     free data
                delete p->Word;
                delete p;
                p = NULL;
        }
// Kill tree
        WordTree.Kill();
}

