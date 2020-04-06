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
};

#endif 

