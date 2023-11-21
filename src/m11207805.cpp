#include "../include/multilevel.h"
using namespace std;

int main(int argc, char* argv[]) {
	multilevel test;
	test.setMode(argv[1]);
	
	test.read_blif_file(argv[2]);
	cout << "Original literal count: " << test.cal_literal() << endl;
	test.test_decompose();
	cout << "Optimized literal count: " << test.cal_literal() << endl;
	test.write_blif_file();
	return 0;
}