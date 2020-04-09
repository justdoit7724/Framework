#pragma once

#ifndef _CUSTOMSTL
#define _CUSTOMSTL

namespace CustomSTL
{
	class BinaryTree
	{
	public:
		BinaryTree(int value) 
			:val(value), left(nullptr), right(nullptr){}

		int val;
		BinaryTree* left;
		BinaryTree* right;
	};

	class BST
	{
	public:
		BST():root(nullptr) {}

		void Insert(int key) {

			if (Contain(key))
				return;

			BinaryTree** parent = &root;
			BinaryTree* curNode = root;
			while (curNode)
			{
				if (key < curNode->val)
				{
					parent = &(curNode->left);
					curNode = curNode->left;
				}
				else
				{
					parent = &(curNode->right);
					curNode = curNode->right;
				}
			}

			*parent = new BinaryTree(key);
		}
		void Delete(int key) {

			if (!Contain(key))
				return;

			BinaryTree** parent = &root;
			BinaryTree* curNode = root;
			while (true)
			{
				if (curNode->val == key)
				{
					BinaryTree* switchNode = nullptr;
					if (curNode->left)
					{
						switchNode=TakeOffGreatest(curNode->left, curNode);
						switchNode->left = curNode->left;
						switchNode->right = curNode->right;
					}
					else if (curNode->right)
					{
						switchNode = TakeOffLowest(curNode->right, curNode);
						switchNode->left = curNode->left;
						switchNode->right = curNode->right;
					}

					*parent = switchNode;
					delete curNode;
					break;
				}
				else if (key < curNode->val) {
					parent = &curNode->left;
					curNode = curNode->left;
				}
				else {
					parent = &curNode->right;
					curNode = curNode->right;
				}
			}
		}
		bool Contain(int key) {

			BinaryTree* curNode = root;
			while (curNode) {
				if (curNode->val == key)
					return true;
				else if (key < curNode->val)
					curNode = curNode->left;
				else
					curNode = curNode->right;
			}

			return false;
		}
	private:
		BinaryTree* root;

		BinaryTree* TakeOffGreatest(BinaryTree* node, BinaryTree* parent) {
			if (!node)
				return nullptr;

			if (node->right)
				return TakeOffGreatest(node->right, node);
			else {
				parent->right = node->left;
				return node;
			}
		}
		BinaryTree* TakeOffLowest(BinaryTree* node, BinaryTree* parent) {
			if (!node)
				return nullptr;

			if (node->left)
				return TakeOffLowest(node->left, node);
			else {
				parent->left = node->right;
				return node;
			}
		}
	};

	template<class T>
	class SLNode
	{
	public:
		SLNode(T data) :data(data), next(nullptr) {}

		T data;
		SLNode* next;
	};
	template<class T>
	class DLNode
	{
	public:
		DLNode(T data) :data(data), prev(nullptr), next(nullptr) {}

		T data;
		DLNode* prev = nullptr;
		DLNode* next = nullptr;
	};

	template<class T>
	class SLList
	{
	public:
		SLList() {}

		void Insert(T item)
		{
			if (!head)
			{
				tail = head = new SLNode<T>(item);
			}
			else
			{
				SLNode<T>* newNode = new SLNode<T>(item);
				tail->next = newNode;
				tail = newNode;
			}

			size++;
		}
		void DeleteIdx(int idx)
		{
			assert(size > idx);

			if (size == 1)
			{
				delete head;
				head = nullptr;
				tail = nullptr;
			}
			else if(idx==0)
			{
				SLNode<T>* delNode = head;
				head = head->next;
				delete delNode;
			}
			else if (idx == size - 1)
			{
				delete tail;
				tail = head;
				for (int i=0; i<idx-1; ++i)
				{
					tail = tail->next;
				}

				tail->next = nullptr;
			}
			else
			{
				SLNode<T>* prevNode = nullptr;
				SLNode<T>* curNode = head;

				for (int i = 0; i < idx; ++i)
				{
					prevNode = curNode;
					curNode = curNode->next;
				}

				prevNode->next = curNode->next;

				delete curNode;
			}

			

			size--;
		}
		void Delete(T item)
		{
			int idx = 0;

			SLNode<T>* curNode = head;

			while (true)
			{
				if (curNode->data == item)
				{
					break;
				}

				curNode = curNode->next;
				idx++;
			}

			DeleteIdx(idx);
		}


	private:
		SLNode<T>* head=nullptr;
		SLNode<T>* tail=nullptr;
		int size = 0;
	};
};

#endif 

