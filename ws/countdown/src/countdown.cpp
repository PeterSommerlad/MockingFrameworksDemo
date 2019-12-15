#include <iostream>

struct Fract{
	int num;
	int den;

	Fract(int n,int d ):num{n},den{den}{}
};

std::ostream& operator<<(std::ostream&out,Fract const &f){
	return out << "(" << f.num << "/" << f.den << ")";
}
Fract operator*(Fract l, Fract r){
	return {l.num*r.num,l.den*r.den};
}

int main(){
	Fract f{2,3};
	Fract g{3,4};
	std::cout << f << " * " << g << " == " << f * g;
}
