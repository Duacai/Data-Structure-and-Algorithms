#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <queue>
#include <iomanip>

using namespace std;

namespace Viclib
{

template <typename T>
class AVLTreeNode
{
public:
	T key;
	uint16_t height;
	AVLTreeNode* left;
	AVLTreeNode* right;

	AVLTreeNode(T v, AVLTreeNode* l, AVLTreeNode* r) :
		key(v), height(0), left(l), right(r) {}
};

template <typename T>
class AVLTree
{
private:
	AVLTreeNode<T>* mRoot;
	uint64_t mCount;

	void preOrder(AVLTreeNode<T>* tree) const;
	void inOrder(AVLTreeNode<T>* tree) const;
	void postOrder(AVLTreeNode<T>* tree) const;

	void levelOrder(AVLTreeNode<T>* tree) const;

	AVLTreeNode<T>* search(AVLTreeNode<T>* tree, T key) const;
	AVLTreeNode<T>* iterativeSearch(AVLTreeNode<T>* tree, T key) const;

	AVLTreeNode<T>* minimum(AVLTreeNode<T>* tree) const;
	AVLTreeNode<T>* maximum(AVLTreeNode<T>* tree) const;

	AVLTreeNode<T>* llRotation(AVLTreeNode<T>* tree) const;
	AVLTreeNode<T>* rrRotation(AVLTreeNode<T>* tree) const;
	AVLTreeNode<T>* lrRotation(AVLTreeNode<T>* tree) const;
	AVLTreeNode<T>* rlRotation(AVLTreeNode<T>* tree) const;

	AVLTreeNode<T>* insert(AVLTreeNode<T>* &tree, T key);
	AVLTreeNode<T>* remove(AVLTreeNode<T>* &tree, AVLTreeNode<T>* del);

	void printGraph(const void* mRoot, uint16_t m_keyStrLen) const;
	void printTree(AVLTreeNode<T> const* const tree, bool firstNode) const;

	void destroy(AVLTreeNode<T>* &tree) const;
	uint16_t height(AVLTreeNode<T>* tree) const;

public:
	AVLTree();
	virtual ~AVLTree();

	void preOrder() const;
	void inOrder() const;
	void postOrder() const;

	void levelOrder() const;

	AVLTreeNode<T>* search(T key) const;			// 递归版
	AVLTreeNode<T>* iterativeSearch(T key) const;	// 非递归版

	T minimum() const;
	T maximum() const;

	void insert(T key);
	bool remove(T key);

	void print() const;								// 打印结点关系，谁是谁的结点
	void printGraph(uint16_t keyStrLen) const;		// 以图形树方式打印关系
	void printTree() const;

	void destroy();
	uint16_t height() const;

	uint64_t getCount() const;
	bool rootIsNullptr() const;

	T getRootKey() const;
};

template <typename T>
AVLTree<T>::AVLTree() : mRoot(nullptr), mCount(0)
{
}

template <typename T>
void AVLTree<T>::preOrder(AVLTreeNode<T>* tree) const	// 前序遍历
{
	if ( tree != nullptr )
	{
		cout << tree->key << " " << flush;
		preOrder(tree->left);
		preOrder(tree->right);
	}
}

template <typename T>
void AVLTree<T>::preOrder() const
{
	preOrder(mRoot);
	cout << endl;
}

template <typename T>
void AVLTree<T>::inOrder(AVLTreeNode<T>* tree) const	// 中序遍历
{
	if ( tree != nullptr )
	{
		inOrder(tree->left);
		cout << tree->key << " " << flush;
		inOrder(tree->right);
	}
}

template <typename T>
void AVLTree<T>::inOrder() const
{
	inOrder(mRoot);
	cout << endl;
}

template <typename T>
void AVLTree<T>::postOrder(AVLTreeNode<T>* tree) const	// 后续遍历
{
	if ( tree != nullptr )
	{
		postOrder(tree->left);
		postOrder(tree->right);
		cout << tree->key << " " << flush;
	}
}

template <typename T>
void AVLTree<T>::postOrder() const
{
	postOrder(mRoot);
	cout << endl;
}

template <typename T>
void AVLTree<T>::levelOrder(AVLTreeNode<T>* tree) const	// 广度优先
{
	if ( tree != nullptr )
	{
		queue<AVLTreeNode<T>*> tmp;
		tmp.push(tree);

		while( tmp.size() > 0 )
		{
			AVLTreeNode<T>* t = tmp.front();

			if ( t->left != nullptr )
				tmp.push(t->left);

			if ( t->right != nullptr )
				tmp.push(t->right);

			tmp.pop();

			cout << t->key << " " << flush;
		}
	}
}

template <typename T>
void AVLTree<T>::levelOrder() const
{
	levelOrder(mRoot);
	cout << endl;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::search(AVLTreeNode<T>* tree, T key) const	// 递归版搜索
{
	if ( (tree == nullptr) || (tree->key == key) )
		return tree;

	if ( key < tree->key )
		return search(tree->left, key);
	else
		return search(tree->right, key);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::search(T key) const
{
	return search(mRoot, key);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::iterativeSearch(AVLTreeNode<T>* tree, T key) const	// 非递归版搜索
{
	while ( (tree != nullptr) && (tree->key != key) )
	{
		if ( key < tree->key )
			tree = tree->left;
		else
			tree = tree->right;
	}

	return tree;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::iterativeSearch(T key) const
{
	return iterativeSearch(mRoot, key);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::minimum(AVLTreeNode<T>* tree) const
{
	if ( tree == nullptr )
		return nullptr;

	while ( tree->left != nullptr )
		tree = tree->left;

	return tree;
}

template <typename T>
T AVLTree<T>::minimum() const
{
	AVLTreeNode<T> *ret = minimum(mRoot);
//	if ( ret == nullptr )
//		THROW_EXCEPTION(EmptyTreeException, "The tree is empty ...");

	return ret->key;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::maximum(AVLTreeNode<T>* tree) const
{
	if ( tree == nullptr )
		return nullptr;

	while ( tree->right != nullptr )
		tree = tree->right;

	return tree;
}

template <typename T>
T AVLTree<T>::maximum() const
{
	AVLTreeNode<T> *ret = maximum(mRoot);
//	if ( ret == nullptr )
//		THROW_EXCEPTION(EmptyTreeException, "The tree is empty ...");

	return ret->key;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::llRotation(AVLTreeNode<T>* tree) const			// 左单旋，旋转前左孩子有孩子右孩子没有
{
	AVLTreeNode<T>* ret;

	ret = tree->left;
	tree->left = tree->left->right;
	ret->right = tree;

	tree->height = max( height(tree->left), height(tree->right) ) + 1;
	ret->height = max( height(ret->left), height(ret->right) ) + 1;

	return ret;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::rrRotation(AVLTreeNode<T>* tree) const			// 右单旋，旋转前右孩子有孩子左孩子没有
{
	AVLTreeNode<T>* ret;

	ret = tree->right;
	tree->right = tree->right->left;
	ret->left = tree;

	tree->height = max( height(tree->left), height(tree->right) ) + 1;
	ret->height = max ( height(ret->left), height(ret->right) ) + 1;

	return ret;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::lrRotation(AVLTreeNode<T>* tree) const			// 左右双旋，先右旋，后左旋；tree左孩子的右孩子有后代，tree右孩子没有后代
{
	tree->left = rrRotation(tree->left);	// 确保多出的后代在左子树的左子树上

	return llRotation(tree);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::rlRotation(AVLTreeNode<T>* tree) const			// 右左双旋，先左旋，后右旋；tree右孩子的左孩子有后代，tree左孩子没有后代
{
	tree->right = llRotation(tree->right);	// 确保多出的后代在右子树的右子树上

	return rrRotation(tree);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::insert(AVLTreeNode<T>* &tree, T key)
{
	if ( tree == nullptr )									// 创建新结点，下面的两个else if会最终递归进入此if创建新结点
	{
		tree = new AVLTreeNode<T>(key, nullptr, nullptr);
		++mCount;
	}
	else if ( key < tree->key )								// 将新结点交给左子树负责插入
	{
		tree->left = insert(tree->left, key);				// 注意更新左结点，因为它可能旋转或是新创建的结点

		if ( height(tree->left) - height(tree->right) == 2)	// 如果插入的左子树超重
		{
			if ( key < tree->left->key )
				tree = llRotation(tree);					// 如果插入的位置是左孩子的左孩子，左单旋
			else
				tree = lrRotation(tree);					// 否则插入的位置是左孩子的右孩子，右单旋；最外层的else保证不会有重复值
		}
	}
	else if ( key > tree->key )								// 将新结点交给右子树负责插入
	{
		tree->right = insert(tree->right, key);

		if ( height(tree->right) - height(tree->left) == 2 )
		{
			if ( key > tree->right->key )
				tree = rrRotation(tree);
			else
				tree = rlRotation(tree);
		}
	}
	else													// 结点重复
		;//THROW_EXCEPTION(DuplicateDataException, "Can't create duplicate nodes ...");

	tree->height = max(height(tree->left), height(tree->right)) + 1;

	return tree;
}

template <typename T>
void AVLTree<T>::insert(T key)
{
	insert(mRoot, key);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::remove(AVLTreeNode<T>* &tree, AVLTreeNode<T>* del)
{
	if ( (tree == nullptr) || (del == nullptr) )
		return nullptr;

	if ( del->key < tree->key )							// 交给左子树删除，最终进入最外层else
	{
		tree->left = remove(tree->left, del);						// 更新可能旋转的结点

		if ( height(tree->right) - height(tree->left) == 2 )		// 删除左子树结点后如果右子树超重
		{
			if ( height(tree->right->left) > height(tree->right->right) )
				tree = rlRotation(tree);								// 右左重
			else
				tree = rrRotation(tree);								// 右右重
		}
	}
	else if ( del->key > tree->key )					// 交给右子树删除，最终进入最外层else
	{
		tree->right = remove(tree->right, del);

		if ( height(tree->left) - height(tree->right) == 2 )
		{
			if ( height(tree->left->right) - height(tree->left->left) == 2 )
				tree = lrRotation(tree);
			else
				tree = llRotation(tree);
		}
	}
	else												// 找到删除节点
	{
		if ( (tree->left != nullptr) && (tree->right != nullptr) )	// 删除点有两个孩子，在较重的分支找替换者
		{
			if ( height(tree->left) > height(tree->right) )				// 左孩子重
			{
				AVLTreeNode<T>* lmax = maximum(tree->left);					// 查找左边最大者用于替换；如果用右边的最小者，当右边没有孩子就会删除自身导致不平衡
				tree->key = lmax->key;										// 采用值拷贝的方式删除，否则你还要处理子结点
				tree->left = remove(tree->left, lmax);						// 更新可能旋转的结点
			}
			else														// 右孩子重
			{
				AVLTreeNode<T>* rmin = minimum(tree->right);
				tree->key = rmin->key;
				tree->right = remove(tree->right, rmin);
			}
		}
		else														//	删除点孩子不足两个，直接删除并用孩子替换
		{
			AVLTreeNode<T>* tmp = tree;
			tree = (tree->left != nullptr) ? tree->left : tree->right;	// 替换删除结点
			delete tmp;
			--mCount;
		}
	}

	if ( tree != nullptr )
		tree->height = max(height(tree->left), height(tree->right)) + 1;

	return tree;
}

template <typename T>
bool AVLTree<T>::remove(T key)
{
	bool ret = false;
	AVLTreeNode<T>* tmp;

	if ( (tmp = search(mRoot, key)) != nullptr )	// 查找并删除节点
	{
		mRoot = remove(mRoot, tmp);					// remove()的返回值用于不能修改参数的编程语言，函数左边的参数是左值mRoot的引用
		ret = true;
	}

	return ret;
}

template <typename T>
void AVLTree<T>::printGraph(const void* Root, uint16_t m_keyStrLen) const
{
	AVLTreeNode<T>const* node = static_cast<AVLTreeNode<T>const*>(Root);
	if ( node == nullptr )
		return;

	uint16_t height = node->height;			// 要打印的树总高度
	uint16_t layer = 1;						// 当前层，root为第一层
	uint64_t i, index;
	uint64_t keyStrLen = m_keyStrLen;	// i: 循环变量；index: 当前层最大结点数；keyStrLen: 结点输出占用字符宽度
	queue<AVLTreeNode<T>const *> q;		// 记录每层的所有节点，包括nullptr
	q.push(node);
	cout << "输出树形关系图！！！" << endl;
	while ( true )
	{
		cout << layer << "\t" << flush;									// 输出当前层号和当前层满节点数
		AVLTreeNode<T> const* tmp = nullptr;							// 取出结点变量
		index = 1ull<<(layer-1);
		while ( index-- )
		{
			tmp = q.front();											// 取出结点
			q.pop();
			for ( i=0; i<((1ull<<(height-layer))-1ull)*keyStrLen; ++i )	// 结点前的填充
				cout << " ";
			cout << flush;

			if ( tmp != nullptr )										// 打印有效结点
			{
				cout << right << setw(keyStrLen) << setfill('0') << tmp->key << flush;

				if ( tmp->left != nullptr )		// 加入左结点
					q.push(tmp->left);
				else
					q.push(nullptr);

				if ( tmp->right != nullptr )	// 加入右节点
					q.push(tmp->right);
				else
					q.push(nullptr);
			}
			else														// 打印无效结点
			{
				for ( i=0; i<keyStrLen; ++i )
					cout << "#";
				cout << flush;

				q.push(nullptr);				// 如果结点是空的则为其加入两个空子结点
				q.push(nullptr);
			}

			for ( i=0; i<((1ull<<(height-layer))-1ull)*keyStrLen; ++i )	// 结点后的填充
				cout << " ";
			cout << flush;

			if ( q.size() != (1ull<<(layer)) )
				for ( i=0; i<keyStrLen; ++i )								// 两节点间填充，因为父节点位于两节点的中间上面，而不是其中一个的上面
					cout << " ";
			else
				cout << "\t";
			cout << flush;
		}
		cout << layer << endl;													// 输出一层换行

		if ( ++layer > height )	// while循环出口，当前层大于总高度时退出
			break;
	}
}

template <typename T>
void AVLTree<T>::printGraph(uint16_t keyStrLen) const
{
	if ( mRoot == nullptr )
		return;

	printGraph(mRoot, keyStrLen);
}

template <typename T>
void AVLTree<T>::printTree(AVLTreeNode<T> const* const tree, bool firstNode) const
{
	if ( tree==nullptr )
		return;

	bool static outTag[64] = {false};	// size = max layer limit;
	uint8_t static layer = 0;
	uint8_t i;
	++layer;

	if ( layer >= 2 )
	{
		for (i=2; i<layer; ++i )
			if ( outTag[i] )
				cout << "|       ";
			else
				cout << "        ";
		if ( firstNode == true )		// 判断左右结点，非二叉树需要另外处理
			cout << "L-------" << flush;
		else
			cout << "R-------" << flush;
	}
	cout << tree->key << endl;

	for ( i=2-1; i>0; --i)		// 从右往左输出结点，即先打印最右边结点，其次次右边的结点；此循环不输出最左边的结点
	{
		if ( (tree->left+i) != nullptr )	// 注意树的子结点指针必须是从左往右依次排列，中间不能有其它变量（left_1,left_2,left_3...left_n）
		{									// 如果你的子结点数量不定，一定要把后面的首个指针设为nullptr
			outTag[layer] = !firstNode;
			printTree(tree->right, false);
		}
	}
	if ( tree->left != nullptr )			// 输出最左边的结点
	{
		printTree(tree->left, true);
		outTag[layer] = firstNode;
	}

	--layer;
}

template <typename T>
void AVLTree<T>::printTree() const
{
	printTree(mRoot, true);	// 右边参数此时无意义
}

template <typename T>
void AVLTree<T>::destroy(AVLTreeNode<T>* &tree) const
{
	if ( tree == nullptr )
		return;

	if ( tree->left != nullptr )
		destroy(tree->left);
	else
		destroy(tree->right);

	delete tree;
}

template <typename T>
void AVLTree<T>::destroy()
{
	destroy(mRoot);
	mRoot = nullptr;
	mCount = 0;
}

template <typename T>
uint16_t AVLTree<T>::height(AVLTreeNode<T>* tree) const
{
	if ( tree != nullptr )
		return tree->height;

	return 0;
}

template <typename T>
uint16_t AVLTree<T>::height() const
{
	return height(mRoot);
}

template <typename T>
uint64_t AVLTree<T>::getCount() const
{
	return mCount;
}

template <typename T>
bool AVLTree<T>::rootIsNullptr() const
{
	return mRoot == nullptr;
}

template <typename T>
T AVLTree<T>::getRootKey() const
{
//	if ( mRoot == nullptr )
//		THROW_EXCEPTION(EmptyTreeException, "The tree is empty ...");

	return mRoot->key;
}

template <typename T>
AVLTree<T>::~AVLTree()
{
	destroy();
}

}

#endif // AVLTREE_H
