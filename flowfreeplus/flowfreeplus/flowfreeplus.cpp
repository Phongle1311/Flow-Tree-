#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <utility>
#include <vector>
#include <map>
#include <fstream>
#include <string>
using namespace std;
struct toado {
	int x, y;
};
struct dt {
	toado a, b;
};

int iter = 1000;
int n, m, a[1005][1005], tmp;
dt color[500005];
toado trace[1005][1005];
toado trong, cam;
int arr[4] = { 1, 2, 3, 4 };
int s = 4;
fstream f1, f2;

const int numTest = 20;
typedef pair<int, int> cell;

bool AreNeighboring(cell a, cell b, int n) { // Kiem tra 2 o co ke nhau
	return (a.first == b.first && ((a.second + 1) % n == b.second || (a.second + n - 1) % n == b.second)) ||
		(a.second == b.second && ((a.first + 1) % n == b.first || (a.first + n - 1) % n == b.first));
}

double SingleTestGrader(string inputFile, string outputFile) { // Ham tinh diem cua 1 test
	fstream f;
	f.open(inputFile, std::fstream::in);
	if (f.is_open() == false)
		return 0;
	int n, m;
	f >> n >> m;
	map<cell, int> col;
	map<cell, bool> visited;
	for (int i = 1; i <= 2 * m; ++i) {
		int c, x, y;
		f >> c >> x >> y;
		x -= 1; y -= 1;
		col[{x, y}] = c;
	}
	f.close();
	f.open(outputFile, std::fstream::in);
	if (f.is_open() == false)
		return 0;
	vector<cell> path;
	int numCell = 0, numPair = 0, len;
	while (f >> len) {
		path.clear();
		numCell += len;
		++numPair;
		for (int i = 0; i < len; ++i) {
			int x, y;
			f >> x >> y;
			x -= 1; y -= 1;
			path.push_back({ x, y });
			if (visited[{x, y}])
				return 0;
			else
				visited[{x, y}] = true;
		}
		if (len < 2)
			return 0;
		else if (col[path[0]] != col[path.back()] || col[path[0]] == 0)
			return 0;
		for (int i = 1; i < path.size(); ++i) {
			if (!AreNeighboring(path[i], path[i - 1], n))
				return 0;
		}
	}
	f.close();
	return 1.0 * (1.0 * numCell / (n * n) + 1) * numPair / m;
}

double ContestantGrader() {
	double res = 0;
	for (int i = 1; i <= numTest; ++i) {
		res += SingleTestGrader("D://Flow Free++/data/input" + to_string(i) + ".txt", "D://Flow Free++/data/output" + to_string(i) + ".txt");
	}
	return res;
}


bool operator == (toado a, toado b) {
	return a.x == b.x && a.y == b.y;
}

void Swap(int* number_1, int* number_2)
{
	int temp = *number_1;
	*number_1 = *number_2;
	*number_2 = temp;
}

void ShuffleArray(int* arr, int n)
{
	srand(time(NULL));

	int minPosition;
	int maxPosition = n - 1;
	int swapPosition;

	int i = 0;
	while (i < n - 1)
	{
		minPosition = i + 1;
		swapPosition = rand() % (maxPosition - minPosition + 1) + minPosition;

		Swap(&arr[i], &arr[swapPosition]);
		i++;
	}
}

void enter() {
	f1.open("D://Flow Free++/data/input1.txt", ios::in);
	trong.x = 0; trong.y = 0;
	cam.x = -1; cam.y = -1;
	f1 >> n >> m;
	//for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) trace[i][j] = trong;
	int x, y, z;
	for (int i = 1; i <= 2 * m; i++) {
		f1 >> z >> x >> y;
		a[x][y] = z;
		trace[x][y] = cam;
		if (color[z].a == trong) color[z].a.x = x, color[z].a.y = y;
		else color[z].b.x = x, color[z].b.y = y;
	}
	f1.close();
}

bool check(int u, int v) {
	return trace[u][v] == trong;
}
void DFS(int x, int y) {
	ShuffleArray(arr, s);
	//if (a[x][y] == tmp && !(trace[x][y] == trong)) return;
	for (int i = 1; i <= s; i++) {
		if (arr[i] == 1) {
			//do ben duoi
			if (x + 1 <= n) {
				if (check(x + 1, y)) trace[x + 1][y].x = x, trace[x + 1][y].y = y, DFS(x + 1, y);
			}
			else if (check(1, y)) trace[1][y].x = x, trace[1][y].y = y, DFS(1, y);
		}
		if (arr[i] == 2) {
			//do ben tren
			if (x - 1 >= 1) {
				if (check(x - 1, y)) trace[x - 1][y].x = x, trace[x - 1][y].y = y, DFS(x - 1, y);
			}
			else if (check(n, y)) trace[n][y].x = x, trace[n][y].y = y, DFS(n, y);
		}
		if (arr[i] == 3) {
			//do ben trai
			if (y - 1 >= 1) {
				if (check(x, y - 1)) trace[x][y - 1].x = x, trace[x][y - 1].y = y, DFS(x, y - 1);
			}
			else if (check(x, n)) trace[x][n].x = x, trace[x][n].y = y, DFS(x, n);
		}
		if (arr[i] == 4) {
			//do ben phai
			if (y + 1 <= n) {
				if (check(x, y + 1)) trace[x][y + 1].x = x, trace[x][y + 1].y = y, DFS(x, y + 1);
			}
			else if (check(x, 1)) trace[x][1].x = x, trace[x][1].y = y, DFS(x, 1);
		}
	}
}
void Trace(toado a) {

	if (!(trace[a.x][a.y] == trong)) {
		toado tmp1 = a, tmp2;
		int cnt = 1;
		//dem so o
		while (!(trace[tmp1.x][tmp1.y] == cam)) {
			tmp1 = trace[tmp1.x][tmp1.y];
			cnt++;
		}
		f2 << endl << cnt << endl;
		//truy vet duong di
		tmp1 = a;
		f2 << a.x << " " << a.y << " ";
		while (!(trace[tmp1.x][tmp1.y] == cam)) {
			f2 << trace[tmp1.x][tmp1.y].x << " " << trace[tmp1.x][tmp1.y].y << " ";
			tmp2 = tmp1;
			tmp1 = trace[tmp1.x][tmp1.y];
			trace[tmp2.x][tmp2.y] = cam;
		}
	}
	else trace[color[tmp].a.x][color[tmp].a.y] = trong;
	//tai tao lai arr trace
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			if (!(trace[i][j] == cam)) trace[i][j] = trong;
}
/*
void search_flow() {
	f2.open("D://Flow Free++/data/output1.txt", ios::out);
	//for (int i=m;i;i--){
	int arr_color[500005];
	for (int i = 1; i <= m; i++) arr_color[i] = i;
	ShuffleArray(arr_color, m);
	int phu;
	for (int i = 1; i <= m; i++) {
		//ShuffleArray(arr, s);
		phu = arr_color[i];
		trace[color[phu].b.x][color[phu].b.y] = trong;
		tmp = a[color[phu].a.x][color[phu].a.y];
		DFS(color[phu].a.x, color[phu].a.y);
		Trace(color[phu].b);
	}
	f2.close();
}
*/
void search_flow() {
	f2.open("D://Flow Free++/data/output1.txt", ios::out);
	for (int i = m; i; i--) {
		//for (int i = 1; i <= m; i++) {
		trace[color[i].b.x][color[i].b.y] = trong;
		tmp = a[color[i].a.x][color[i].a.y];
		DFS(color[i].a.x, color[i].a.y);
		Trace(color[i].b);
	}
	f2.close();
}

void create_ouput() {
	enter();
	search_flow();
}

void copy_good_res() {
	fstream f3, f4;
	f3.open("D://Flow Free++/data/output1.txt", ios::in);
	f4.open("D://Flow Free++/data/goodres.txt", ios::out);
	string data;
	string line;
	while (!f3.eof()) {
		getline(f3, line);
		data += line;
		data += "\n";
	}
	f3.close();
	f4 << data;
	f4.close();
}

int main() {
	double point = 0, res;
	while (iter--) {
		create_ouput();
		res = ContestantGrader();
		if (res > point) point = res, copy_good_res();
	}
	cout << point << "\n";
	return 0;
}