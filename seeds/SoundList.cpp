#include <allegro.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <math.h>
#include <vector>
#include "mappyal.h"
#include "main.h"
#include "BitmapList.h"
#include "SoundList.h"
#include "Speechbubble.h"
#include "Enemy.h"
#include "Level.h"
#include "Player.h"
#include "App.h"

SoundList::SoundList()
{
	// initialize data
	head = NULL;
	tail = NULL;
}

SoundList::~SoundList()
{
	// go through the list and delete all SoundNodes (which will cause the SoundNodes to also delete their sample datas)
	SoundNode * current = head;
	SoundNode * next = NULL;

	while (current != NULL)
	{
		next = current->next;
		delete current;
		current = next;
	}
}

void SoundList::AddNode(SoundNode *node)
{
	if (head == NULL) // if list is empty
	{
		head = node;
		tail = node;
	}
	else
	{ // list has at least one node in it already
		tail->next = node; // make the old/current tail point to the new node as its "next"
		node->next = NULL; // make sure the new node's "next" is NULL
		tail = node; // the new node is the new tail of the list
	}
}

SAMPLE * SoundList::LoadSound(std::string filename)
{
	// first, see if a node with a matching filename exists by traversing the linked list.  if it exists, return it, otherwise load a new one and return that instead.
	SoundNode * current_node = head;
	SoundNode * previous_node = NULL;
	while (current_node != NULL)
	{
		// if a match has been found on this current node
		if (current_node->strFilename == filename)
			return current_node->wavSound;
		current_node = current_node->next;
	}

	// no existing image was found, so let's load the sample and add it to this list for future usage.
	// load sample
	SAMPLE * wavNewSound = load_sample(filename.c_str());
	// add sample to new node in this list
	SoundNode * bnNode;
	bnNode = new SoundNode();
	bnNode->wavSound = wavNewSound; // point sample
	bnNode->strFilename = filename; // copy filename
	// add the new node to the list
	AddNode(bnNode);
	// return the newly loaded sample
	return wavNewSound;
}

void SoundList::RemoveNode(std::string filename)
{
	// finds a SoundNode in the list with the matching filename and deletes it
	
	// first, find the node with the matching filename by traversing the linked list
	SoundNode * current_node = head;
	SoundNode * previous_node = NULL;
	while (current_node != NULL)
	{
		// if a match has been found on this current node
		if (current_node->strFilename == filename)
		{
			SoundNode * next_node = current_node->next; // make a note of the next node so that the previous node can point to it
			delete current_node; // delete the node's data
			
			if (previous_node == NULL) // if the previous node is NULL (meaning this is the "head" node in the list)
			{
				if (head == tail) // if the head is the only node in the list
				{
					head = NULL; // set the head to NULL
					tail = NULL; // set the tail to NULL
				}
				else
					head = next_node; // the deleted node's "next" node is now the new head of the list
			}
			else
			{ // there was a real monster before this one (meaning the deleted node was not "head")
				previous_node->next = next_node; // the deleted node's "next" is now located directly after the deleted node's "previous",
													// cutting the deleted node out of the list

				// set the tail to the previous node if the deleted node was the old tail
				if (next_node == NULL)
					tail = previous_node;
			}

			break; // break out of this loop because all work is complete in the removal & deletion of the monster
		}
		
		// move to next node in the list
		previous_node = current_node;
		current_node = current_node->next;
	}
}

SoundNode::SoundNode()
{
	// initialize data
	wavSound = NULL;
	next = NULL;
	strFilename = "";
}

SoundNode::~SoundNode()
{
	// delete sample data
	destroy_sample(wavSound);
}