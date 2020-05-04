#pragma once
#include <cstdlib>
#include <assert.h>

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
		void DeepCopy(SLNode<T>** outNode)
		{
			*outNode = new SLNode<T>(data);
			SLNode<T>* curNode = *outNode;
			SLNode<T>* iNode = next;

			while (iNode)
			{
				SLNode<T>* newNode = new SLNode<T>(iNode->data);
				curNode->next = newNode;
				curNode = newNode;

				iNode = iNode->next;
			}
		}

		T data;
		SLNode* next;
	};
	template<class T>
	class DLNode
	{
	public:
		DLNode(T data) :data(data), prev(nullptr), next(nullptr) {}
		void DeepCopy(DLNode<T>** outNode)const
		{
			*outNode= new DLNode<T>(data);
			DLNode<T>* curNode = *outNode;
			DLNode<T>* iNode = next;

			while (iNode)
			{
				DLNode<T>* newNode = new DLNode<T>(iNode->data);
				newNode->prev = curNode;
				curNode->next = newNode;
				curNode = newNode;

				iNode = iNode->next;
			}
		}

		T data;
		DLNode* prev = nullptr;
		DLNode* next = nullptr;
	};

	template<class T>
	class SLList
	{
	public:
		SLList() {}
		~SLList() {
			
			while (head)
			{
				SLNode<T>* delNode = head;
				head = head->next;
				delete delNode;
			}
		}

		void InsertFront(T item)
		{
			SLNode<T>* newNode = new SLNode<T>(item);
			if (!head)
			{
				tail = head = newNode;
			}
			else
			{
				newNode->next = head;
				head = newNode;
			}

			size++;
		}
		void InsertBack(T item)
		{
			SLNode<T>* newNode = new SLNode<T>(item);
			if (!head)
			{
				tail = head = newNode;
			}
			else
			{
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

			while (idx < size)
			{
				if (curNode->data == item)
				{
					DeleteIdx(idx);
					break;
				}

				curNode = curNode->next;
				idx++;
			}
		}

		SLNode<T>* Head() { return head; }
		SLNode<T>* Tail() { return tail; }
		int Size() { return size; }

	private:
		SLNode<T>* head=nullptr;
		SLNode<T>* tail=nullptr;
		int size = 0;
	};

	template<class T>
	class DLList
	{
	public:
		DLList() {}
		DLList(const DLList& list)
		{
			if (head)
			{
				int curSize = size;
				for (int i = 0; i < curSize; ++i)
				{
					DeleteIdx(0);
				}
			}

			const CustomSTL::DLNode<T>* head = list.Head();

			while(head)
			{
				InsertBack(head->data);
				head = head->next;
			}
		}

		void InsertFront(const T item)
		{
			DLNode<T>* newNode = new DLNode<T>(item);

			if (size==0)
			{
				head = newNode;
				tail = newNode;
			}
			else
			{
				newNode->next = head;
				head = newNode;
			}

			size++;
		}
		void InsertBack(const T item)
		{
			DLNode<T>* newNode = new DLNode<T>(item);

			if (size == 0)
			{
				head = newNode;
				tail = newNode;
			}
			else
			{
				tail->next = newNode;
				newNode->prev = tail;
				tail = newNode;
			}

			size++;
		}

		void DeleteIdx(int idx)
		{
			assert(idx < size);

			if (size == 1)
			{
				delete head;

				head = nullptr;
				tail = nullptr;
			}
			else if (idx == 0)
			{
				head = head->next;
				delete head->prev;
				head->prev = nullptr;
			}
			else if (idx == size - 1)
			{
				tail = tail->prev;
				delete tail->next;
				tail->next = nullptr;
			}
			else
			{
				DLNode<T>* checkNode = head;
				for (int i = 0; i < idx; ++i)
				{
					checkNode = checkNode->next;
				}

				DLNode<T>* delNode = checkNode;
				checkNode->prev->next = checkNode->next;
				checkNode->next->prev = checkNode->prev;

				delete delNode;
			}
		}
		void Delete(T item)
		{
			DLNode<T>* checkNode = head;
			int idx = 0;

			while (idx < size)
			{
				if (checkNode->data == item)
				{
					DeleteIdx(idx);
					break;
				}
				checkNode = checkNode->next;
				idx++;
			}
		}

		DLNode<T>* Head()const { return head; }
		DLNode<T>* Tail()const { return tail; }
		int Size()const { return size; }

	private:
		DLNode<T>* head=nullptr;
		DLNode<T>* tail=nullptr;

		int size = 0;
	};
};

