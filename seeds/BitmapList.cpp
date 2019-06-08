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

using namespace std;

BitmapList::BitmapList()
{
	// initialize data
	head = NULL;
	tail = NULL;
}

BitmapList::~BitmapList()
{
	// go through the list and delete all BitmapNodes (which will cause the BitmapNodes to also delete their bitmap datas)
	BitmapNode * current = head;
	BitmapNode * next = NULL;

	while (current != NULL)
	{
		next = current->next;
		delete current;
		current = next;
	}
}

void BitmapList::AddNode(BitmapNode *node)
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

BITMAP * BitmapList::LoadBitmap(std::string filename)
{
	// first, see if a node with a matching filename exists by traversing the linked list.  if it exists, return it, otherwise load a new one and return that instead.
	BitmapNode * current_node = head;
	BitmapNode * previous_node = NULL;
	while (current_node != NULL)
	{
		// if a match has been found on this current node
		if (current_node->strFilename == filename)
			return current_node->bmpImage;
		current_node = current_node->next;
	}

	// no existing image was found, so let's load the bitmap and add it to this list for future usage.
	// load bitmap
	BITMAP * bmpNewImage = load_bitmap(filename.c_str(), NULL);
	// add bitmap to new node in this list
	BitmapNode * bnNode;
	bnNode = new BitmapNode();
	bnNode->bmpImage = bmpNewImage; // point bitmap
	bnNode->strFilename = filename; // copy filename
	// add the new node to the list
	AddNode(bnNode);
	// return the newly loaded bitmap
	return bmpNewImage;
}

void BitmapList::RemoveNode(std::string filename)
{
	// finds a BitmapNode in the list with the matching filename and deletes it
	
	// first, find the node with the matching filename by traversing the linked list
	BitmapNode * current_node = head;
	BitmapNode * previous_node = NULL;
	while (current_node != NULL)
	{
		// if a match has been found on this current node
		if (current_node->strFilename == filename)
		{
			BitmapNode * next_node = current_node->next; // make a note of the next node so that the previous node can point to it
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

BitmapNode::BitmapNode()
{
	// initialize data
	bmpImage = NULL;
	next = NULL;
	strFilename = "";
}

BitmapNode::~BitmapNode()
{
	// delete bitmap data
	destroy_bitmap(bmpImage);
}