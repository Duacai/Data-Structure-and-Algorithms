#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

namespace Viclib
{

enum RBTColor { RED, BLACK };

template <typename T>
class RBTNode
{
public:
	RBTColor mColor;
	T mKey;
	RBTNode<T>* mLeft;
	RBTNode<T>* mRight;
	RBTNode<T>* mParent;

	RBTNode(RBTColor color, T key, RBTNode<T>* left, RBTNode<T>* right, RBTNode<T>* parent) :
		mColor(color), mKey(key), mLeft(left), mRight(right), mParent(parent) {}
};

template <typename T>
class RBTree
{
private:
	RBTNode<T>* mRoot;
	uint64_t mCount;
	uint16_t mHeight;

	void preOrder(RBTNode<T>* tree) const;
	void inOrder(RBTNode<T>* tree) const;
	void postOrder(RBTNode<T>* tree) const;

	void levelOrder(RBTNode<T>* tree) const;

	RBTNode<T>* search(RBTNode<T>* tree, T key) const;
	RBTNode<T>* iterativeSearch(RBTNode<T>* tree, T key) const;

	RBTNode<T>* minimum(RBTNode<T>* tree) const;
	RBTNode<T>* maximum(RBTNode<T>* tree) const;

	void lRotate(RBTNode<T>* &tree, RBTNode<T>* node) const;
	void rRotate(RBTNode<T>* &tree, RBTNode<T>* node) const;

	void insert(RBTNode<T>* &tree, RBTNode<T>* node);
	void insertFixUp(RBTNode<T>* &tree, RBTNode<T>* node);	// 插入修正红黑树

	void remove(RBTNode<T>* &tree, RBTNode<T> *del);
	void removeFixUp(RBTNode<T>* &tree, RBTNode<T>* del, RBTNode<T>* parent);	// 删除修正红黑树(被删除的是黑色)

	void printTree(RBTNode<T> const* const tree, bool firstNode) const;

	void destroy(RBTNode<T>* &tree);
	uint16_t max(uint16_t left, uint16_t right) const;
	uint16_t updateHeight(RBTNode<T> *node);

public:
	RBTree();
	~RBTree();

	void preOrder() const;
	void inOrder() const;
	void postOrder() const;

	void levelOrder() const;

	RBTNode<T>* search(T key) const;
	RBTNode<T>* iterativeSearch(T key) const;

	T const* minimum() const;
	T const* maximum() const;

	RBTNode<T>* successor(RBTNode<T>* node) const;
	RBTNode<T>* predecessor(RBTNode<T>* node) const;

	void insert(T key);
	bool remove(T key);

	void printTree() const;

	void destroy();
	uint64_t getCount() const;
	uint16_t getHeight(bool update = true);
	bool rootIsNullptr() const;
	T getRootKey() const;
	uint8_t setKeyStrLen();
};

template <typename T>
RBTree<T>::RBTree() : mRoot(nullptr), mCount(0ull), mHeight(0)
{
}

template <typename T>
RBTree<T>::~RBTree()
{
	destroy();
}

template <typename T>
void RBTree<T>::preOrder(RBTNode<T>* tree) const
{
	if ( tree != nullptr )
	{
		cout << tree->mKey << " " << flush;
		preOrder(tree->mLeft);
		preOrder(tree->mRight);
	}
}

template <typename T>
void RBTree<T>::preOrder() const
{
	preOrder(mRoot);
	cout << endl;
}

template <typename T>
void RBTree<T>::inOrder(RBTNode<T>* tree) const
{
	if ( tree != nullptr )
	{
		inOrder(tree->mLeft);
		cout << tree->mKey << " " << flush;
		inOrder(tree->mRight);
	}
}

template <typename T>
void RBTree<T>::inOrder() const
{
	inOrder(mRoot);
	cout << endl;
}

template <typename T>
void RBTree<T>::postOrder(RBTNode<T>* tree) const
{
	if ( tree != nullptr )
	{
		postOrder(tree->mLeft);
		postOrder(tree->mRight);
		cout << tree->mKey << " " << flush;
	}
}

template <typename T>
void RBTree<T>::postOrder() const
{
	postOrder(mRoot);
	cout << endl;
}

template <typename T>
void RBTree<T>::levelOrder(RBTNode<T>* tree) const
{
	if ( tree != nullptr )
	{
		queue<RBTNode<T>*> tmp;
		tmp.push(tree);

		while( tmp.size() > 0 )
		{
			RBTNode<T>* t = tmp.front();

			if ( t->mLeft != nullptr )
				tmp.push(t->mLeft);

			if ( t->mRight != nullptr )
				tmp.push(t->mRight);

			tmp.pop();

			cout << t->mKey << " " << flush;
		}
	}
}

template <typename T>
void RBTree<T>::levelOrder() const
{
	levelOrder(mRoot);
	cout << endl;
}

template <typename T>
RBTNode<T>* RBTree<T>::search(RBTNode<T>* tree, T key) const
{
	if ( tree==nullptr || key==tree->mKey )
		return tree;

	if ( key < tree->mKey )
		return search(tree->mLeft, key);
	else
		return search(tree->mRight, key);
}

template <typename T>
RBTNode<T>* RBTree<T>::search(T key) const
{
	return search(mRoot, key);
}

template <typename T>
RBTNode<T>* RBTree<T>::iterativeSearch(RBTNode<T>* tree, T key) const
{
	while ( tree!=nullptr && key!=tree->mKey )
	{
		if ( key < tree->mKey )
			tree = tree->mLeft;
		else
			tree = tree->mRight;
	}

	return tree;
}

template <typename T>
RBTNode<T>* RBTree<T>::iterativeSearch(T key) const
{
	return iterativeSearch(mRoot, key);
}

template <typename T>
RBTNode<T>* RBTree<T>::minimum(RBTNode<T>* tree) const
{
	if ( tree == nullptr )
		return nullptr;

	while ( tree->mLeft != nullptr )
		tree = tree->mLeft;

	return tree;
}

template <typename T>
T const* RBTree<T>::minimum() const
{
	RBTNode<T>* ret = minimum(mRoot);
	if ( ret != nullptr )
		return &ret->mKey;

	return nullptr;
}

template <typename T>
RBTNode<T>* RBTree<T>::maximum(RBTNode<T>* tree) const
{
	if ( tree == nullptr )
		return nullptr;

	while ( tree->mRight != nullptr )
		tree = tree->mRight;

	return tree;
}

template <typename T>
T const* RBTree<T>::maximum() const
{
	RBTNode<T>* ret = maximum(mRoot);
	if ( ret != nullptr )
		return &ret->mKey;

	return nullptr;
}

template <typename T>
RBTNode<T>* RBTree<T>::successor(RBTNode<T>* tree) const	// 查找tree的后继，比tree大
{
	if ( tree->right != nullptr )			// 在右节点查找最小结点
		return minimum(tree->right);

	RBTNode<T>* p = tree->parent;
	while ( p!=nullptr && tree==p->right )	// 父节点非空且自己是右节点就继续寻找，直至自己是左结点或父节点为空
	{
		tree = p;
		p = p->parent;
	}

	return p;
}

template <typename T>
RBTNode<T>* RBTree<T>::predecessor(RBTNode<T>* tree) const	// 查找tree的前任，比tree小
{
	if ( tree->left != nullptr )			// 在左结点查找最大结点
		return maximum(tree->left);

	RBTNode<T>* p = tree->parent;
	while ( p!=nullptr && tree==p->left )	// 父节点非空且自己是左结点就继续寻找，直至自己是右节点或父节点为空
	{
		tree = p;
		p = p->parent;
	}

	return p;
}

/*	左旋
 *    p                     p
 *    |                     |
 *   old                   new
 *   / \     --(左旋)-->    / \
 *  a  new                old c
 *     / \                / \
 *    B   c              a   B
 */
template <typename T>
void RBTree<T>::lRotate(RBTNode<T>* &tree, RBTNode<T>* node) const	// 将右边重的结点旋转至左边重
{																	// 当前结点成为右孩子的左孩子，右孩子的左孩子成为自己的右孩子，右孩子则替换自己位置
	RBTNode<T>* r = node->mRight;			// 新结点指向右节点

	node->mRight = r->mLeft;					// 更新 【当前结点（旧结点）】 与 【右节点（新结点）的左孩子】 之间的关系
	if ( r->mLeft != nullptr )
		r->mLeft->mParent = node;

	r->mParent = node->mParent;				// 更新 父节点 和 新孩子 之间的关系
	if ( node->mParent == nullptr )
		tree = r;
	else
	{
		if ( node == node->mParent->mLeft )		// 判断并更新父节点的新孩子
			node->mParent->mLeft = r;
		else
			node->mParent->mRight = r;
	}

	r->mLeft = node;							// 更新 新旧结点 之间的关系
	node->mParent = r;
}

/*	右旋
 *      p                  p
 *      |                  |
 *     old                new
 *     / \   --(右旋)-->   / \
 *   new  c              a  old
 *   / \                    / \
 *  a   B                  B   c
 */
template <typename T>
void RBTree<T>::rRotate(RBTNode<T>* &tree, RBTNode<T>* node) const
{
	RBTNode<T>* l = node->mLeft;

	node->mLeft = l->mRight;
	if ( l->mRight != nullptr )
		l->mRight->mParent = node;

	l->mParent = node->mParent;
	if ( node->mParent == nullptr )
		tree = l;
	else
	{
		if ( node == node->mParent->mLeft )
			node->mParent->mLeft = l;
		else
			node->mParent->mRight = l;
	}

	l->mRight = node;
	node->mParent = l;
}

template <typename T>
void RBTree<T>::insertFixUp(RBTNode<T>* &tree, RBTNode<T>* node)	// 插入修正红黑树
{
	RBTNode<T> *parent, *gparent;	// 父结点，爷爷结点

	// node有父结点且父亲是红色(R红色、B黑色、@插入结点)
	while ( (parent = node->mParent) && (parent->mColor==RED) )
	{
		gparent = parent->mParent;

		if ( parent == gparent->mLeft )	// 父亲是左孩子，叔叔是右孩子
		{
			{	// 叔叔有效且是红色，while保证父亲也是红色
				RBTNode<T>* uncle = gparent->mRight;
				if ( uncle && uncle->mColor==RED )	// 父亲是红色，自己默认又是红色，所以需要变色
				{									// 将父亲和叔叔设为黑结点，爷爷设为红节点；
					uncle->mColor = BLACK;	//   B		      R
					parent->mColor = BLACK;	// R   R		B   B
					gparent->mColor = RED;	// R(@)			R(@)	// 不区分自己是左孩子还是右孩子
					node = gparent;					// node指向爷爷后向上再判断其它结点是否需要平衡
					continue;
				}
			}
			// 父亲为红色时如果叔叔不是红色，则叔叔必是黑色叶子，且父亲的子女也全是叶子；因为父亲必须有一个叶子子结点才能插入，如果叔叔不是叶子或父亲的儿子不全是叶子则无法平衡
			{	// 叔叔为空，自己是红色父亲的右孩子，旋转成左孩子（父子身份也交换，且父子仍为红色）
				if ( parent->mRight == node )// 红节点的子结点如有叶子则全是叶子，否则不平衡；父亲之前没有子结点则父亲无兄弟
				{
					RBTNode<T>* tmp;
					lRotate(tree, parent);	// 左旋后node替换父亲，父亲则成为自己的左孩子，变成左左模式，左左都是红色
					tmp = parent;			// 旋转后修正父子指针位置，父子互换
					parent = node;			//	 B  	  B  		B
					node = tmp;				//	R   	 R(@)	   R
				}							//	 R(@)	R    	  R(@)
			}

			{	// 叔叔为空，自己是红色父亲的左孩子
				parent->mColor = BLACK;		//	 B  	  R 		 B
				gparent->mColor = RED;		//	R   	 B   	 R(@)   R
				rRotate(tree, gparent);		// R(@)		R(@)
			}
		}
		else						// 父亲是右孩子，伯父是左孩子
		{
			{	// 伯父有效且是红色，while保证父亲也是红色
				RBTNode<T>* uncle = gparent->mLeft;
				if ( uncle && uncle->mColor==RED )
				{
					uncle->mColor = BLACK;	//	 B  		  R
					parent->mColor = BLACK;	// R   R 		B   B
					gparent->mColor = RED;	//	   R(@)			R(@)
					node = gparent;
					continue;
				}
			}

			{	// 伯父为空或为黑色，自己是红色父亲的左孩子，旋转成右孩子（父子身份也交换，且父子仍为红色）
				if ( parent->mLeft == node )
				{
					RBTNode<T>* tmp;
					rRotate(tree, parent);
					tmp = parent;			// B 		B   	B
					parent = node;			//  R   	 R(@)	 R
					node = tmp;				// R(@)		  R 	  R(@)
				}
			}

			{	// 伯父为空或为黑色，自己是红色父亲的右孩子
				parent->mColor = BLACK;		// B 		R   		 B
				gparent->mColor = RED;		//	R   	 #  	 R       R(@)
				lRotate(tree, gparent);		//	 R(@)	  R(@)
			}
		}
	}

	tree->mColor = BLACK;	// 如果没有父节点则当前结点就是根节点；父节点为黑则这条语句无意义
}

template <typename T>
void RBTree<T>::insert(RBTNode<T>* &tree, RBTNode<T>* node)
{
	RBTNode<T>* parent = nullptr;	// 插入点的父节点
	RBTNode<T>* root = tree;		// 辅助寻找parent

	while ( root != nullptr )		// 寻找插入点
	{
		parent = root;
		if ( node->mKey < root->mKey )
			root = root->mLeft;
		else
			root = root->mRight;
	}

	node->mParent = parent;			// 设置node结点的父节点
	if ( parent != nullptr )		// 有父节点则插入为子结点
		if ( node->mKey < parent->mKey )
			parent->mLeft = node;
		else
			parent->mRight = node;
	else							// 父节点为空则设为根节点
		tree = node;

	node->mColor = RED;				// 设为红色
	++mCount;

	insertFixUp(tree, node);		// 只有父节点是红色才需要平衡，但是要注意根节点没有父亲且默认插入的是红色
}

template <typename T>
void RBTree<T>::insert(T key)
{
	RBTNode<T>* node = new RBTNode<T>(RED, key, nullptr, nullptr, nullptr);	// 颜色在重载版本改为红色，此处可任意填写

	insert(mRoot, node);
}

template <typename T>
void RBTree<T>::removeFixUp(RBTNode<T>* &tree, RBTNode<T>* del_child, RBTNode<T>* del_parent)	// 删除修正红黑树(被删除的是黑色)
{
	RBTNode<T>* other;	// child的兄弟（原来的叔伯）

	// del_child为假或del_child为黑结点，且del_child不是根节点(del_child如果不是根节点就绝对是nullptr)
	while ( (!del_child || del_child->mColor==BLACK) && del_child!=tree )	// B黑，R红，p=parent，c=child，o=other，ol=other->left，or=other->right
	{
		if ( del_parent->mLeft == del_child )		// 如果del_child是左结点；注意替换者已经离开了，所以child和parent是父子关系
		{											// 父亲绝对有两个儿子，因为del_child原先是黑色孙子，所以绝对有一个叔伯（现在是兄弟）
			other = del_parent->mRight;
			if ( other->mColor == RED )								// del_child的兄弟是红节点，它的子结点必定全是黑色
			{
				other->mColor = BLACK;
				del_parent->mColor = RED;
				lRotate(tree, del_parent);
				other = del_parent->mRight;
			}

			if ( (!other->mLeft || other->mLeft->mColor==BLACK) &&		// del_child兄弟的左结点为假或者为黑，且右结点也为假或者为黑
				 (!other->mRight || other->mRight->mColor==BLACK) )	// 上面if保证del_child的兄弟也是黑色
			{
				other->mColor = RED;
				del_child = del_parent;
				del_parent = del_child->mParent;
			}
			else
			{
				if ( !other->mRight || other->mRight->mColor==BLACK )		// del_child兄弟是黑色，且该兄弟孩子不全为黑
				{
					other->mLeft->mColor = BLACK;
					other->mColor = RED;
					rRotate(tree, other);
					other = del_parent->mRight;
				}

				other->mColor = del_parent->mColor;
				del_parent->mColor = BLACK;
				other->mRight->mColor = BLACK;
				lRotate(tree, del_parent);
				del_child = tree;
				break;
			}
		}
		else										// 如果del_child是右结点
		{
			other = del_parent->mLeft;
			if ( other->mColor == RED )
			{
				other->mColor = BLACK;
				del_parent->mColor = RED;
				rRotate(tree, del_parent);
				other = del_parent->mLeft;
			}

			if ( (!other->mLeft || other->mLeft->mColor==BLACK) &&
				 (!other->mRight || other->mRight->mColor==BLACK) )
			{
				other->mColor = RED;
				del_child = del_parent;
				del_parent = del_child->mParent;
			}
			else
			{
				if ( !other->mLeft || other->mLeft->mColor==BLACK )
				{
					other->mRight->mColor = BLACK;
					other->mColor = RED;
					lRotate(tree, other);
					other = del_parent->mLeft;
				}

				other->mColor = del_parent->mColor;
				del_parent->mColor = BLACK;
				other->mLeft->mColor = BLACK;
				rRotate(tree, del_parent);
				del_child = tree;					// 也可以改成 tree->color = BLACK;
				break;
			}
		}
	}

	if ( del_child != nullptr )					// del_child如果存在且是红色，或者是根节点
		del_child->mColor = BLACK;
}

template <typename T>
void RBTree<T>::remove(RBTNode<T>* &tree, RBTNode<T>* del)
{
	RBTNode<T> *child, *parent;
	RBTColor color;

	if ( del->mLeft!=nullptr && del->mRight!=nullptr )		// 如果删除结点有两个孩子，需要找一个替换者
	{
		RBTNode<T>* replace = del->mRight;			// 替换者指向右结点最小者；也可以指向左结点的最大者
		while ( replace->mLeft != nullptr )
			replace = replace->mLeft;

		if ( del->mParent != nullptr )				// 更新父结点指向替换者
		{
			if ( del->mParent->mLeft == del )
				del->mParent->mLeft = replace;
			else
				del->mParent->mRight = replace;
		}
		else
			tree = replace;

		child = replace->mRight;						// 保存替换者的子结点、父结点、颜色
		parent = replace->mParent;
		color = replace->mColor;

		if ( del == parent )						// 删除的是替换者的父结点（这时替换者就是del的右结点，因为替换者没有左结点，所以del的右结点最小）
			parent = replace;
		else
		{
			if ( child != nullptr )
				child->mParent = parent;
			parent->mLeft = child;					// 替换者的父亲接管替换者的儿子（此时替换者只有右儿子，因为自己是右子树的最左下者）

			replace->mRight = del->mRight;			// 更新替换者和被删除者右儿子的关系（因为替换者位于右子树）
			del->mRight->mParent = replace;
		}

		replace->mParent = del->mParent;				// 更新替换者的父亲、颜色、以及与被删除者左结点的关系
		replace->mColor = del->mColor;
		replace->mLeft = del->mLeft;
		del->mLeft->mParent = replace;
	}
	else													// 删除结点孩子不足两个，独子或者叶节点就是替换者
	{
		if ( del->mLeft != nullptr )					// 保存替换者的子结点、父结点、颜色
			child = del->mLeft;
		else
			child = del->mRight;
		parent = del->mParent;
		color = del->mColor;

		if ( child != nullptr )						// 更新 '被删除结点的父节点' 和 '被删除结点的子结点' 的关系
			child->mParent = parent;					// 父亲（也就是被删除结点）被删除，所以爷爷直接和唯一一个孙子互相更新关系即可
		if ( parent != nullptr )
		{
			if ( parent->mLeft == del )
				parent->mLeft = child;
			else
				parent->mRight = child;
		}
		else
			tree = child;
	}

	--mCount;									// 结点计数减一

	if ( color == BLACK )						// 如果替换者或被删除者是黑色需要重新平衡（被删除者有两个儿子则是替换者），因为删除了一个黑结点
		removeFixUp(tree, child, parent);		// child如果不是根节点或红色节点，那它绝对是nullptr指针（替换者至多有一个红色儿子，且该儿子没有后代）

	delete del;									// 删除节点并返回
	del = nullptr;
}

template <typename T>
bool RBTree<T>::remove(T key)
{
	bool ret = false;
	RBTNode<T>* node = search(mRoot, key);

	if ( node != nullptr )
	{
		remove(mRoot, node);
		ret = true;
	}

	return ret;
}

template <typename T>
void RBTree<T>::printTree(RBTNode<T> const* const tree, bool firstNode) const
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
		cout << "+-------" << flush;
	}
	cout << tree->mKey << ' ' << (tree->mColor==BLACK ? 'B' : 'R') << endl;

	for ( i=2-1; i>0; --i)		// 从右往左输出结点，即先打印最右边结点，其次次右边的结点；此循环不输出最左边的结点
	{
		if ( (tree->mLeft+i) != nullptr )	// 注意树的子结点指针必须是从左往右依次排列，中间不能有其它变量（left_1,left_2,left_3...left_n）
		{									// 如果你的子结点数量不定，一定要把后面的首个指针设为nullptr
			outTag[layer] = !firstNode;
			printTree(tree->mRight, false);
		}
	}
	if ( tree->mLeft != nullptr )			// 输出最左边的结点
	{
		printTree(tree->mLeft, true);
		outTag[layer] = firstNode;
	}

	--layer;
}

template <typename T>
void RBTree<T>::printTree() const
{
	printTree(mRoot, true);	// 右边参数此时无意义
}

template <typename T>
void RBTree<T>::destroy(RBTNode<T>* &tree)
{
	if ( tree == nullptr )
		return;

	if ( tree->mLeft != nullptr )
		destroy(tree->mLeft);
	if ( tree->mRight != nullptr )
		destroy(tree->mRight);

	delete tree;
}

template <typename T>
void RBTree<T>::destroy()
{
	destroy(mRoot);

	mRoot = nullptr;
	mCount = 0ull;
	mHeight = 0;
}

template <typename T>
uint64_t RBTree<T>::getCount() const
{
	return mCount;
}

template <typename T>
uint16_t RBTree<T>::updateHeight(RBTNode<T> *node)
{
	if ( node == nullptr )
		return 0;

	return max(updateHeight(node->mLeft), updateHeight(node->mRight))+1;
}

template <typename T>
uint16_t RBTree<T>::getHeight(bool update)
{
	if ( update == true )
		mHeight = updateHeight(mRoot);

	return mHeight;
}

template <typename T>
uint16_t RBTree<T>::max(uint16_t left, uint16_t right) const
{
	return (left > right) ? left : right;
}

template <typename T>
bool RBTree<T>::rootIsNullptr() const
{
	return mRoot==nullptr;
}

template <typename T>
T RBTree<T>::getRootKey() const
{
	return (rootIsNullptr()) ? ~0ull : mRoot->mKey;
}

}

#endif // RBTREE_H
