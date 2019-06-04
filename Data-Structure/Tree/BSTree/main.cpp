#include <iostream>
#include <iomanip>
#include <cmath>

#include "BSTree.h"

#include "Times.h"

using namespace std;
using namespace Viclib;

int main(int argc, char* argv[])
{
	uint16_t layer = 8;	// 结点最小层数

	if ( argc == 2 && atoi(argv[1])>0 )
		layer = static_cast<uint8_t>(atoi(argv[1]));
	else {
		cout << "请输入结点最小层数，注意内存大小" << log(RAND_MAX*RAND_MAX+1+RAND_MAX*2)/log(2) << endl;
		cin >> layer;
	}

	timingStart();							// 启动计时

	cout << endl;

	uint64_t const count = (1ull<<layer)-1;	// 结点数
	uint16_t speed;							// 记录插入和删除的进度百分比（0～100）

	BSTree<uint64_t> *tree = new BSTree<uint64_t>();

	speed = 0;
	srand(static_cast<unsigned int>(time(nullptr)));
	while ( tree->getCount() < count )
	{
		tree->insert(static_cast<size_t>(rand()*rand()+1+RAND_MAX*2));	// 使用随机值插入；如果是顺序值，会退化成链表模式，且插入变慢

		if ( (tree->getCount()*100/count > speed) || (tree->getCount() == count) )	// 进度值出现变化或完成操作时才能输出
		{
			speed = static_cast<uint16_t>(tree->getCount()*100/count);
			cout << "\r已添加：" << setw(3) << speed << '%' << flush;
		}
	}
	cout << endl;

	cout << "\n前序遍历: ";
	tree->preOrder();
	cout << endl;

	cout << "\n中序遍历: ";
	tree->inOrder();
	cout << endl;

	cout << "\n后续遍历: ";
	tree->postOrder();
	cout << endl;

	cout << "\n广度遍历：";
	tree->levelOrder();
	cout << endl;

	cout << "\n最小值: " << tree->minimum();
	cout << "\n最大值: " << tree->maximum();
	cout << "\nheight = " << tree->height();
	cout << "\n结点数= " << tree->getCount();
	cout << endl;

	cout << "\n输出树形信息：" << endl;
	tree->printTree();
	cout << endl;

	speed = 0;
	srand(static_cast<unsigned int>(time(nullptr)));
	while ( tree->getCount() )
	{
		uint64_t node;
//		if ( tree->getCount()*100/count < 20 )			// 剩余量太少时，随机值命中有效结点的概率太低，从而导致删除缓慢
			node = tree->getRootKey();
//		else
//			node = static_cast<size_t>(rand()*rand()+1+RAND_MAX*2);

		tree->remove(node);

		if ( ((count-tree->getCount())*100/count > speed) || (tree->getCount() == count) )
		{
			speed = static_cast<uint16_t>((count-tree->getCount())*100/count);
			cout << "\r已删除：" << setw(3) << speed << '%' << flush;
		}
	}
	cout << endl;

	tree->destroy();
	delete tree;
	tree = nullptr;

	cout << endl;
	timingEnd();

	return 0;
}
