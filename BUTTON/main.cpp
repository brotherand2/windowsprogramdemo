#include<iostream>
#include<memory>
#include<windows.h>
#include<cassert>
using namespace std;
int main()
{
	int *p = new int[10]{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	{
		shared_ptr<int>p1(new int[3]);
		cout << "p1:" << p1.use_count() << endl;
		assert(p1.use_count() ==1);
		{
			shared_ptr<int>p2(new int[8], [](int *p){cout << "release" << endl; });
			cout << "p1:" << p1.use_count() << ",p2:" << p2.use_count() << endl;
			p2 = p1;
			cout << "p2:" << p2.use_count() << endl;
			shared_ptr<int>p3(new int[8], [](int *p){cout << "p3 release" << endl; });


		}
		cout << "p1:" << p1.use_count() << endl;

	}
	shared_ptr<int>temp;
	{
		//if (p)
		//	cout << "p is not null\n";
		//else
		//	cout << "p is  null\n";
		//shared_ptr<int>sp1(new int[10], [](int *p){cout << "\np4 delete\n"; });
		//shared_ptr<int>p3(new int[8], [](int *p){cout << "p5 release" << endl; });

		shared_ptr<int>sp(p, [](int *&pq){
			for (int i = 0; i < 10; i++)
				cout << pq[i] << "\t";

			delete[]pq; pq = NULL; cout << "\np6  delete\n";  });
		//for (int i = 0; i < 10; i++)
		//	cout << *(sp.get() + i)<<"\t";
		//sp.reset();
		//delete[]p;
			temp = sp;
			cout << "sp:" << sp.use_count() << ",temp:" << temp.use_count() << endl;

	}
	cout << "\ntemp:" << temp.use_count() << endl;

	if (p!=NULL)
		cout << "\np is not null\n";
	else
		cout << "p is  null\n";
	cout << endl;
	for (int i = 0; i < 10; i++)
		cout << p[i] << "\t";
	cin.get();
}