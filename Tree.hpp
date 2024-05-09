#pragma once

#include <vector>

template<class T>
struct Node
{
	T data;
	Node<T>* parent;
	std::vector<Node<T>*> children;
	bool root = false;

	Node(Node* dad, T info)
	{
		data = info;
		parent = dad;
		if (parent == nullptr)
			root = true;
	}
};

template<class T>
struct Tree
{
	std::vector<Node<T>> nodes;
	Node<T>* root;

	Tree()
	{
	}

	void addNode(Node<T>* parent, T data)
	{
		nodes.push_back(Node(parent, data));
	}
};