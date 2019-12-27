#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <ctime> 

using namespace std;

class Taboo {
private:
	ifstream file;
	string fname;
	string line;
	vector < vector<string> > rows;
	unique_ptr<char[]> buffer;
	string_view svBuffer;

	int orig_size;
	void getcsvLine();
	void getRows();
	int getNum();
public:
	Taboo(string fname) : fname(fname) {};
	void readFile();
	void playGame();
	void processFile();
};

int Taboo::getNum() { return (rand() % rows.size());}

void Taboo::getcsvLine() {
	auto lineend = svBuffer.find("\n");
	if (lineend == string_view::npos) {
		line = static_cast<string>(svBuffer.substr(0, string_view::npos));
		svBuffer = "";
		return;
	}
	line = static_cast<string>(svBuffer.substr(0, lineend));
	svBuffer.remove_prefix(lineend + 1);
	return;
}

void Taboo::getRows() {
	while (!svBuffer.empty()) {
		string row = "";
		size_t start = 0;
		getcsvLine();
		vector<string> vec;
		while (true) {
			string row = "";
			auto end = line.find(",", start);
			if (end != string::npos) {
				row += line.substr(start, end - start);
				vec.emplace_back(row);
				start = end + 1;;
			}
			else {
				row += line.substr(start, end);
				vec.emplace_back(row);
				rows.emplace_back(vec);
				break;
			}
		}
	}
	orig_size = rows.size();
	return;
}

void Taboo::readFile() {
	file.open(fname);
	if (!file.is_open()) {
		cout << "Error opening file";
		exit(1);
	}
	file.seekg(0, file.end);
	const auto length = static_cast<size_t>(file.tellg()) + 1;
	file.seekg(0, file.beg);
	buffer = make_unique<char[]>(length);
	memset(buffer.get(), 0, length);
	file.read(buffer.get(), length);
	svBuffer = buffer.get();
	file.close();
	return;
}

void Taboo::processFile() {
	getRows();
	return;
}

void Taboo::playGame() {
	int num;
	cout << endl << endl;
	for (int i = 0; i < orig_size; ++i) {
		num = getNum();
		system("cls");
		cout << "Word is: " << rows[num][0] << endl << endl;
		cout << "Taboo Words are: ";
		for (unsigned int j = 1; j < rows[num].size(); ++j)
			cout << endl << rows[num][j];
		cout << endl << endl;
		system("pause");
		rows.erase(rows.begin() + num);
	}	
	return;
}

int main(int argc, char* argv[]) {
	srand((unsigned)time(0));
	if (argc == 2) {
		string f = argv[1];
		Taboo game(f);
		char ch = ' ';
		cout << "Press enter to play game! ";

		while (ch != '\n')
			ch = getchar();

		game.readFile();
		game.processFile();
		game.playGame();
	}

	else {
		cout << "Incorrect Use! " << endl;
		cout << "Usage: " << "wordsFilePath" << endl;
		return -1;
	}
	return 0;
}