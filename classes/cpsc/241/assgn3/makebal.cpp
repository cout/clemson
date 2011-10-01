#include <iostream.h>
#include <fstream.h>
#include <string>
#include <vector>
#include <algorithm>

void output (ostream &out, vector<string> v, int low, int n);

int main(int argc, char *argv[]) {
	if(argc != 3) {
		cerr << "Usage: " << argv[0] << " <infile> <outfile>" << endl;
		exit(1);
	}

	ifstream in(argv[1]);
	if(!in) {
		cerr << "Error: unable to open file " << argv[1] << " for reading." << endl;
		exit(1);
	}

	ofstream out(argv[2]);
	if(!out) {
		cerr << "Error: unable to open file " << argv[2] << " for writing." << endl;
		exit(1);
	}

	vector<string> words;
	string s;
	while(getline(in, s)) words.push_back(s);

	sort(words.begin(), words.end());
	output(out, words, 0, words.size());

	return 0;
}
	
void output (ostream &out, vector<string> v, int low, int n) {
	if(n < 1) return;
	int m = n / 2;
	cout << v[low + m] << endl;
	out << v[low + m] << endl;
	output(out, v, low, m);
	output(out, v, low + m + 1, n - m - 1);
}
