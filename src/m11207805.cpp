#include "../include/multilevel.h"
using namespace std;

int main(int argc, char* argv[]) {
	multilevel test;
	// test.setMode(argv[0]);
	
	test.read_blif_file(argv[1]);
	cout << "Original literal count: " << test.cal_literal() << endl;
	test.synthesis();
	test.write_blif_file();
	cout << "Optimized literal count: " << test.cal_literal() << endl;
	return 0;
}