#include "RBTree.h"

#include "Times.h"

using namespace std;
using namespace Viclib;

typedef uint64_t templateType;
typedef uint64_t sizeType;

static bool checkTree(RBTree<templateType>* tree);

int main(int argc, char* argv[])
{
	// msys2终端1920*2宽424个英文字符
	uint16_t layer = 16;

	if ( argc == 2 && atoi(argv[1])>=0 )
		layer = static_cast<uint16_t>(atoi(argv[1]));
	else {
		cout << "请输入结点层数，注意内存大小" << endl;
		cin >> layer;
	}

	timingStart();
	cout << endl;

	uint64_t const count = (1ull<<layer)-1ull;

	cout << "您设定的最大层数上限：" << layer << endl;
	cout << "您设定的最大结点数上限：" << count << endl;

	templateType *t = nullptr, tmp = 0;
	RBTree<templateType>* tree = new RBTree<templateType>();

	cout << endl << "添加元素：\n\tkey\tcount\tlayer" << endl;
	srand(static_cast<uint32_t>(time(nullptr)));
	while ( tree->getCount() < count )
	{
		do
		{
			tmp = static_cast<templateType>(
				static_cast<sizeType>(rand())
				* static_cast<sizeType>(rand())
				* static_cast<sizeType>(rand())
				% (count*2));
		} while(tree->iterativeSearch(tmp));
		//tmp = setArr(count);
		tree->insert(tmp);
		//cout << "插入：\t" << tmp << "\t" << tree->getCount() << "\t" << tree->getHeight(true) << endl;
		
		if ( (tree->getCount()*100%count) == 0 || tree->getCount() == count )
			cout << "\r已添加：" << setw(2) << tree->getCount()*100.0/count << '%' << flush;
	}
	cout << endl;

	cout << "\n红黑树平衡校验结果：";
	if ( checkTree(tree) )
		cout << "成功\n" << endl;
	else
	{
		cout << "节点的路径与左边第一个节点路径黑色数量不同\n" << endl;

		cout << "输出目录树模式关系图：" << endl;
		tree->printTree();
		cout << endl;

		exit(1);
	}

	cout << "前序遍历: ";
	tree->preOrder();
	cout << "\n中序遍历: ";
	tree->inOrder();
	cout << "\n后序遍历: ";
	tree->postOrder();
	cout << "\n广度优先: ";
	tree->levelOrder();
	cout << endl;

	if ( (tree!=nullptr) && ((t = const_cast<templateType*>(tree->minimum())) != nullptr) )
		cout << "最小结点：" << *t << endl;
	if ( (tree!=nullptr) && ((t = const_cast<templateType*>(tree->maximum())) != nullptr) )
		cout << "最大结点：" << *t << endl;
	cout << "树的结点数：" << tree->getCount() << endl;
	cout << "树的高度（不含最底层叶节点）：" << tree->getHeight(true) << endl;

//	cout << "输出树形关系图：" << endl;
//	tree->printGraph();
//	cout << endl;

	cout << "输出目录树模式关系图：" << endl;
	tree->printTree();
	cout << endl;

	cout << "开始删除：\n\tkey\tcount\tlayer" << endl;
	srand(static_cast<uint32_t>(time(nullptr)));
	while ( !tree->rootIsNullptr() )		// 随机数删除
	{
		do
		{
			tmp = static_cast<templateType>(
				static_cast<sizeType>(rand())
				* static_cast<sizeType>(rand())
				* static_cast<sizeType>(rand())
				% (count*2));
		} while(!tree->iterativeSearch(tmp));
		if ( tree->remove(tree->getRootKey()) )
		{
			//cout << "删除：\t" << tmp << "\t" << tree->getCount() << "\t" << tree->getHeight(true) << endl;

			if ( (tree->getCount()*100%count) == 0 || tree->getCount() == count )
				cout << "\r已删除：" << setw(2) << (count-tree->getCount())*100.0/count << '%' << flush;
		}
	}
	cout << endl;

	tree->destroy();
	tree = nullptr;

	cout << endl;
	timingEnd();

	return 0;
}

static bool checkTree(RBTree<templateType>* tree)
{
	if ( tree == nullptr )
		return true;

	queue<RBTNode<templateType>*> tmp;
	tmp.push(tree->search(tree->getRootKey()));
	queue<RBTNode<templateType>*> leaf;
	RBTNode<templateType>* t;
	uint8_t i = 0;
	uint8_t j = 0;

	while( tmp.size() > 0 )
	{
		t = tmp.front();

		if ( t->mLeft != nullptr )
			tmp.push(t->mLeft);
		else
			leaf.push(t);

		if ( t->mRight != nullptr )
			tmp.push(t->mRight);
		else
			leaf.push(t);

		tmp.pop();
	}

	t = leaf.front();
	leaf.pop();
	while ( t != nullptr )
	{
		if ( t->mColor == BLACK ) ++i;
		t = t->mParent;
	}

	while ( !leaf.empty() )
	{
		t = leaf.front();

		j = 0;
		if ( t->mColor == BLACK ) ++j;
		while ( t->mParent != nullptr )
		{
			t = t->mParent;
			if ( t->mColor == BLACK ) ++j;
		}

		if ( i != j )
		{
			cout << leaf.front()->mKey;
			return false;
		}

		leaf.pop();
	}

	return true;
}
