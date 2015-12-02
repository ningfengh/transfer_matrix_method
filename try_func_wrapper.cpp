#include <iostream>
using namespace std;

class myclass {
	int a;
	int b;
    public:
	double get_number(void*);
	myclass(int, int);
};

myclass::myclass(int a,int b){
	this->a = a;
	this->b = b;
}

double myclass::get_number(void*){
	return a+b;
}

double wrapper(void *o) {
	return reinterpret_cast<myclass*>(o)->get_number(o);
}

int main(void)
{
	myclass my_o(3,5);
	cout<<wrapper(&my_o)<<endl;
}
