#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<vector>
using namespace std;
const int SudokuSize=81;
const int RowNum=9;
const int ColNum=9;
class Sudoku{
	public:
		Sudoku();
		void setAns();
		void printSudoku();
		void giveQuestion();
		void setSpace();
		void readIn();
		void solve();
		int ifNoAns();
		void changeNum(int a, int b);
		void changeRow(int a, int b);
		void changeCol(int a, int b);
		void rotate(int n);
		void flip(int n);
		void transform();
	private:
		int sudoku_ans[SudokuSize];
};
