#ifndef BSTREE_H
#define BSTREE_H

#include <iostream>
#include <queue>

using namespace std;

namespace Viclib
{

template < typename T >
class BSTNode
{
public:
	T mKey;
	BSTNode<T>	*mLeft;
	BSTNode<T>	*mRight;
	BSTNode<T>	*mParent;

	BSTNode() = delete;
	BSTNode(const T& key, BSTNode<T> *left, BSTNode<T> *right, BSTNode<T> *parent);
	BSTNode(const BSTNode<T>& tree) = delete;
	BSTNode(BSTNode<T> && tree);
	BSTNode<T>& operator = (const BSTNode<T>& tree) = delete;
	BSTNode<T>& operator = (BSTNode<T> && tree);
	virtual ~BSTNode();
};

template < typename T >
BSTNode<T>::BSTNode(const T& key, BSTNode<T> *left, BSTNode<T> *right, BSTNode<T> *parent) :
	mKey(key), mLeft(left), mRight(right), mParent(parent)
{
}

template < typename T >
BSTNode<T>::BSTNode(BSTNode<T> && tree)
{
	mLeft = tree.mLeft;
	mRight = tree.mRight;
	mParent = tree.mParent;

	tree->mLeft = nullptr;
	tree->mRight = nullptr;
	tree->mParent = nullptr;
}

template < typename T >
BSTNode<T>& BSTNode<T>::operator = (BSTNode<T> && tree)
{
	if ( this == &tree )
		return *this;
	
	delete mLeft;
	delete mRight;
	delete mParent;
	
	mLeft = tree->mLeft;
	mRight = tree->mRight;
	mParent = tree->mParent;

	return *this;
}

template < typename T >
BSTNode<T>::~BSTNode()
{
}

template < typename T >
class BSTree
{
protected:
	BSTNode<T>* mRoot;
	size_t mCount;

	virtual void preOrder(BSTNode<T> *tree) const;								// 深度优先的前、中、后序遍历
	virtual void inOrder(BSTNode<T> *tree) const;
	virtual void postOrder(BSTNode<T> *tree) const;

	virtual void levelOrder(BSTNode<T> *tree) const;							// 广度优先

	virtual BSTNode<T>* search(BSTNode<T> *tree, const T& key) const;			// 递归版搜索
	virtual BSTNode<T>* iterativeSearch(BSTNode<T> *tree, const T& key) const;	// 非递归版搜索

	virtual BSTNode<T>* minimum(BSTNode<T> *tree) const;						// 获取最小最大值
	virtual BSTNode<T>* maximum(BSTNode<T> *tree) const;

	virtual BSTNode<T>* successor(BSTNode<T> *node) const;						// 查找后继，即大于又最接近node的节点
	virtual BSTNode<T>* predecessor(BSTNode<T> *node) const;					// 查找前继

	virtual void insert(BSTNode<T> *node);
	virtual BSTNode<T>* remove(BSTNode<T> *node);								// 返回值用于删除，返回值实际是替换结点
	virtual void destroy(BSTNode<T> *tree);

	void printTree(BSTNode<T> *tree, bool firstNode) const;						// 打印树，类似linux的tree命令

	virtual int height(BSTNode<T> *tree) const;									// 树的高度
	virtual int degree(BSTNode<T> *tree) const;									// 结点的度数

public:
	BSTree();
	BSTree(const BSTree<T>&) = default;
	BSTree(BSTree<T> &&) = default;
	BSTree<T>& operator = (const BSTree<T>&) = default;
	BSTree<T>& operator = (BSTree<T> &&) = default;
	virtual ~BSTree();

	virtual void preOrder() const;
	virtual void inOrder() const;
	virtual void postOrder() const;

	virtual void levelOrder() const;

	virtual BSTNode<T>* search(const T& key) const;
	virtual BSTNode<T>* iterativeSearch(const T& key) const;

	virtual const T& minimum() const;
	virtual const T& maximum() const;

	virtual void insert(const T& key);
	virtual void remove(const T& key);
	virtual void destroy();

	virtual int height() const;
	virtual int degree() const;

	virtual size_t getCount() const;

	virtual const T& getRootKey() const;
	virtual void printTree() const;
};

template < typename T >
BSTree<T>::BSTree() : mRoot(nullptr), mCount(0)
{
}

template < typename T >
void BSTree<T>::preOrder(BSTNode<T> *tree) const
{
	if ( tree != nullptr )
	{
		cout << tree->mKey << " " << flush;
		preOrder(tree->mLeft);
		preOrder(tree->mRight);
	}
}

template < typename T >
void BSTree<T>::preOrder() const
{
	preOrder(dynamic_cast<BSTNode<T>*>(this->mRoot));
}

template < typename T >
void BSTree<T>::inOrder(BSTNode<T> *tree) const
{
	if ( tree != nullptr )
	{
		inOrder(tree->mLeft);
		cout << tree->mKey << " " << flush;
		inOrder(tree->mRight);
	}
}

template < typename T >
void BSTree<T>::inOrder() const
{
	inOrder(dynamic_cast<BSTNode<T>*>(this->mRoot));
}

template < typename T >
void BSTree<T>::postOrder(BSTNode<T> *tree) const
{
	if ( tree != nullptr )
	{
		postOrder(tree->mLeft);
		postOrder(tree->mRight);
		cout << tree->mKey << " " << flush;
	}
}

template < typename T >
void BSTree<T>::postOrder() const
{
	postOrder(dynamic_cast<BSTNode<T>*>(this->mRoot));
}

template < typename T >
void BSTree<T>::levelOrder(BSTNode<T> *tree) const
{
	if ( tree != nullptr )
	{
		queue<BSTNode<T>*> tmp;
		tmp.push(tree);

		while( tmp.size() > 0 )
		{
			BSTNode<T>* t = tmp.front();
			tmp.pop();

			if ( t->mLeft != nullptr )
				tmp.push(t->mLeft);

			if ( t->mRight != nullptr )
				tmp.push(t->mRight);

			cout << t->mKey << " " << flush;
		}
	}
}

template < typename T >
void BSTree<T>::levelOrder() const
{
	levelOrder(dynamic_cast<BSTNode<T>*>(this->mRoot));
}

template < typename T >
BSTNode<T>* BSTree<T>::search(BSTNode<T> *node, const T& key) const
{
	if ( node == nullptr || node->mKey == key )
	{
		return node;
	}
	else if ( key < node->mKey )
		return search(dynamic_cast<BSTNode<T>*>(node)->mLeft, key);
	else
		return search(dynamic_cast<BSTNode<T>*>(node)->mRight, key);
}

template < typename T >
BSTNode<T>* BSTree<T>::search(const T& key) const
{
	return search(dynamic_cast<BSTNode<T>*>(this->mRoot), key);
}

template < typename T >
BSTNode<T>* BSTree<T>::iterativeSearch(BSTNode<T>* node, const T& key) const
{
	while ( (node != nullptr) && (node->mKey != key) )
	{
		if ( key < node->mKey )
			node = node->mLeft;
		else
			node = node->mRight;
	}

	return node;
}

template < typename T >
BSTNode<T>* BSTree<T>::iterativeSearch(const T& value) const
{
	return iterativeSearch(dynamic_cast<BSTNode<T>*>(this->mRoot), value);
}

template < typename T >
BSTNode<T>* BSTree<T>::minimum(BSTNode<T>* tree) const
{
	if ( tree == nullptr )
		return nullptr;

	while ( tree->mLeft != nullptr )
		tree = tree->mLeft;

	return tree;
}

template < typename T >
const T& BSTree<T>::minimum() const
{
	BSTNode<T> *p = minimum(dynamic_cast<BSTNode<T>*>(this->mRoot));
//	if ( p == nullptr )
//		THROW_EXCEPTION(EmptyTreeException, "The tree is empty ...");

	return p->mKey;
}

template < typename T >
BSTNode<T>* BSTree<T>::maximum(BSTNode<T>* tree) const
{
	if ( tree == nullptr )
		return nullptr;

	while ( tree->mRight != nullptr )
		tree = tree->mRight;

	return tree;
}

template < typename T >
const T& BSTree<T>::maximum() const
{
	BSTNode<T> *p = maximum(dynamic_cast<BSTNode<T>*>(this->mRoot));
//	if ( p == nullptr )
//		THROW_EXCEPTION(EmptyTreeException, "The tree is empty ...");

	return p->mKey;
}

template < typename T >
BSTNode<T>* BSTree<T>::successor(BSTNode<T> *node) const	// 查找后继
{
	if ( node->mRight != nullptr )							// 如果node有右孩子，则在它的右孩子里面最小的是后继
	{
		return minimum(node->mRight);
	}

	BSTNode<T>* ret = node->mParent;						// 如果node没有右孩子，且它自身是左孩子，则它的父亲是后继
	while ( (ret != nullptr) && (node == ret->mRight) )		// 如果node没有右孩子，且它自身是右孩子，它的最近祖先是左孩子的，则这个祖先的父亲就是后继
	{
		node = ret;
		ret = ret->mParent;
	}

	return ret;
}

template < typename T >
BSTNode<T>* BSTree<T>::predecessor(BSTNode<T>* node) const	// 查找前继
{
	if ( node->mLeft != nullptr )							// 如果node有左孩子，则在它的左孩子里面最大的是前继
		return maximum(node->mLeft);

	BSTNode<T>* ret = node->mParent;						// 如果node没有左孩子，且它自身是右孩子，则它的父亲是后继
	while ( (ret != nullptr) && (node == ret->mLeft) )		// 如果node没有左孩子，且它自身是左孩子，它的最近祖先是右孩子的，则这个祖先的父亲就是后继
	{
		node = ret;
		ret = ret->mLeft;
	}

	return ret;
}

template < typename T >
void BSTree<T>::insert(BSTNode<T>* node)
{
	BSTNode<T>* in = nullptr;				// 插入点
	BSTNode<T>* parent = dynamic_cast<BSTNode<T>*>(this->mRoot);		// 插入点的父节点

	while ( parent != nullptr )				// 查找node的插入点
	{
		in = parent;
		if ( node->mKey < in->mKey )		// 如果node结点小于插入点，则插入到左孩子分支
			parent = parent->mLeft;
		else if ( node->mKey > in->mKey )	// 如果node结点大于插入点，则插入到右孩子分支
			parent = parent->mRight;
		else								// 如果数据等同则跳出
			break;
	}

	node->mParent = in;					// 设置node结点的父亲为插入点
	if ( in == nullptr )				// 如果插入点为空，设node结点为根节点
		this->mRoot = node;
	else if ( node->mKey < in->mKey )	// 如果node结点小于插入点，插入左边
		in->mLeft = node;
	else if ( node->mKey > in->mKey )	// 如果node结点大于插入点，插入右边
		in->mRight = node;
	else								// 如果数据等同则直接替换并释放插入的结点
	{
		in->mKey = node->mKey;
		delete node;
		return;
	}

	++this->mCount;						// 结点统计，注意重复结点插入时不能增加
}

template < typename T >
void BSTree<T>::insert(const T& key)
{
	insert(new BSTNode<T>(key, nullptr, nullptr, nullptr));
}

template < typename T >
BSTNode<T>* BSTree<T>::remove(BSTNode<T> *node)
{
	BSTNode<T>* replace = nullptr;			// 代替结点
	BSTNode<T>* del = nullptr;				// 删除点

	if ( (node->mLeft == nullptr) || (node->mRight == nullptr) )	// 如果孩子不全，node就是删除点，可以直接用仅有的孩子代替
		del = node;
	else															// 否则有两个孩子，后继就是删除点(右子树中最小的成员)
	{
		del = successor(node);										// 查找后继
		node->mKey = del->mKey;										// 直接用后继key替换node的key
	}

	if ( del->mLeft != nullptr )			// 如果有左孩子，则更新左孩子为代替结点（如果if成立就只有左孩子）
		replace = del->mLeft;
	else									// 否则设右孩子为代替结点
		replace = del->mRight;

	if ( replace != nullptr )				// 如果代替结点不是空树，更新代替结点的父亲
		replace->mParent = del->mParent;

	if ( del->mParent == nullptr )			// 如果结点是根节点且孩子不全，那就直接替换
		this->mRoot = replace;
	else if ( del == del->mParent->mLeft )	// 如果删除点为左孩子，则更新父节点的左孩子
		del->mParent->mLeft = replace;
	else									// 否则更新父节点的右孩子
		del->mParent->mRight = replace;

	--this->mCount;

	return del;
}

template < typename T >
void BSTree<T>::remove(const T& key)
{
	BSTNode<T> *del = nullptr, *node;
	del = search(dynamic_cast<BSTNode<T>*>(this->mRoot), key);

	if ( del != nullptr )						// 找到的删除点不为空
		if ( (node = remove(del)) != nullptr )	// 删除node并返回代替结点
			delete node;						// 删除代替结点
}

template < typename T >
void BSTree<T>::destroy(BSTNode<T> *tree)
{
	if ( tree == nullptr )
		return;

	if ( tree->mLeft != nullptr )
		destroy(tree->mLeft);

	if ( tree->mRight != nullptr )
		destroy(tree->mRight);

	delete tree;
	tree = nullptr;
}

template < typename T >
void BSTree<T>::destroy()
{
	destroy(dynamic_cast<BSTNode<T>*>(this->mRoot));
	this->mRoot = nullptr;
	this->mCount = 0;
}

template < typename T >
int BSTree<T>::height(BSTNode<T>* node) const
{
	int ret = 0;

	if( node != nullptr )
	{
		int lh = height(node->mLeft);
		int rh = height(node->mRight);

		ret = ((lh > rh) ? lh : rh) + 1;
	}

	return ret;
}

template < typename T >
int BSTree<T>::height() const
{
	int ret = 0;

	if( this->mRoot != nullptr )
	{
		ret = height(dynamic_cast<BSTNode<T>*>(this->mRoot));
	}

	return ret;
}

template < typename T >
int BSTree<T>::degree(BSTNode<T>* node) const
{
	int ret = 0;

	if( node != nullptr )
	{
		BSTNode<T>* child[] = { node->mLeft, node->mRight };

		ret = (!!node->mLeft + !!node->mRight);		// 统计有效结点数，!!用于转换成bool类型，如果是有效结点则为1，否则为0

		for(int i=0; (i<2) && (ret<2); i++)			// 如果儿子不足2个需要检查
		{
			int d = degree(child[i]);

			if( ret < d )
			{
				ret = d;
			}
		}
	}

	return ret;
}

template < typename T >
int BSTree<T>::degree() const
{
	return degree(dynamic_cast<BSTNode<T>*>(this->mRoot));
}

template < typename T >
size_t BSTree<T>::getCount() const
{
	return this->mCount;
}

template <typename T>
void BSTree<T>::printTree(BSTNode<T> *tree, bool firstNode) const
{
	if ( tree == nullptr )
		return;

	size_t height = this->height();				// 树的高度
	static bool *outTag = new bool[height]();	// 左边是否还有结点的标记，初始化为false
	uint8_t static layer = 0;					// 当前层数，根结点为第一层
	uint8_t i;
	++layer;

	if ( layer > 1 )					// 如果不是根节点需要输出特殊符号
	{
		for (i=1; i<layer-1; ++i )		// 根节点和最后一个结点不需要，所以从1至(layer-1)
			if ( outTag[i] )			// 如果左边还有结点
				cout << "|       ";
			else
				cout << "        ";

		if ( firstNode == true )		// 判断左右结点，非二叉树需要另外处理
			cout << "L-------" << flush;
		else
			cout << "R-------" << flush;
	}
	cout << tree->mKey << endl;

	if ( (tree->mRight) != nullptr )	// 先输出右节点
	{
		if ( tree->mLeft != nullptr )	// 如果左边还有结点需要做标记
			outTag[layer] = true;
		printTree(tree->mRight, false);	// false表示当前是右节点
	}

	if ( tree->mLeft != nullptr )
	{
		outTag[layer] = false;			// 左结点左边不再有结点，恢复默认标记
		printTree(tree->mLeft, true);
	}

	--layer;							// 结点回溯时高度需要减1
}

template <typename T>
void BSTree<T>::printTree() const
{
	printTree(dynamic_cast<BSTNode<T>*>(this->mRoot), false);	// 作为根节点时，右参数无意义
}

template < typename T >
const T& BSTree<T>::getRootKey() const
{
	return this->mRoot->mKey;
}

template < typename T >
BSTree<T>::~BSTree()
{
	destroy();
}

}

#endif // BSTREE_H
