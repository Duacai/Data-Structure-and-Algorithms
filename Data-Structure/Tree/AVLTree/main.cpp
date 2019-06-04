#include <iostream>

#include "AVLTree.h"

#include "Times.h"

using namespace std;
using namespace Viclib;

typedef uint64_t templateType;
typedef uint64_t sizeType;

int main(int argc, char* argv[])
{
	sizeType i, len = 5;
	//uint16_t keyStrLen = 3;	// 打印结点占用的字符宽度，printGraph(node, keyStrLen)

	if ( argc == 2 )
		len = static_cast<sizeType>(atoi(argv[1]));
	else {
		cout << "请输入结点层数，注意内存大小" << endl;
		cin >> len;
	}

	timingStart();

	templateType tmp = 0;
	AVLTree<templateType>* tree = new AVLTree<templateType>();

	cout << "添加元素：\nkey\tcount\tlayer" << endl;
	srand(static_cast<uint32_t>(time(nullptr)));
	i = 0;
	while ( tree->getCount() < (1ull<<len)-1 )
	{
		do
		{
			tmp = static_cast<templateType>(
				static_cast<sizeType>(rand())
				* static_cast<sizeType>(rand())
				* static_cast<sizeType>(rand())
				);
		} while(tree->iterativeSearch(tmp));
		tree->insert(tmp);
		cout << tmp << "\t" << ++i << "\t" << tree->height() << endl;
		if ( tree->height() >= static_cast<int>(len) )	// 限制树的高度
			break;
	}
	cout << endl;

	cout << "先序遍历：" << endl;
	tree->preOrder();
	cout << endl;
	cout << "中序遍历：" << endl;
	tree->inOrder();
	cout << endl;
	cout << "后序遍历：" << endl;
	tree->postOrder();
	cout << endl;
	cout << "广度优先：" << endl;
	tree->levelOrder();
	cout << endl;

	// 输出树形描述的关系图
//	tree->printGraph(keyStrLen);
//	cout << endl;

	tree->printTree();
	cout << endl;

	cout << "最小结点：" << tree->minimum() << endl;
	cout << "最大结点：" << tree->maximum() << endl;
	cout << "树的高度：" << tree->height() << endl;
	cout << "树的结点数：" << tree->getCount() << endl;

	cout << "\n开始删除！！！\nkey\tlayer" << endl;
	while ( !tree->rootIsNullptr() )		// 随机数删除
	{
//		tmp = static_cast<templateType>(
//			static_cast<sizeType>(rand())
//			* static_cast<sizeType>(rand())
//			* static_cast<sizeType>(rand())
//			% ( (1ull<<len)-1) );
		if ( tree->remove(tree->getRootKey()) )
			cout << tmp << "\t" << tree->height() << "\t" << endl;
	}
	cout << endl;

	cout << "删除后输出===" << endl;
	cout << "树的高度：" << tree->height() << endl;
	cout << "树的结点数：" << tree->getCount() << endl;
	cout << "中序遍历：" << endl;
	tree->inOrder();

	cout << "\n销毁AVL树。" << endl;
	tree->destroy();
	cout << "\n树的高度：" << tree->height() << endl;
	cout << "树的结点数：" << tree->getCount() << endl;
	delete tree;
	tree = nullptr;

	cout << endl;
	timingEnd();
	cout << endl;

	return 0;
}
