#include"Sudoku.h"
Sudoku::Sudoku(){setAns();}
void Sudoku::setAns(){
	int orig_ans[SudokuSize]={7,3,5,1,6,4,9,2,8,4,2,6,9,7,8,3,1,5,1,9,8,5,3,2,6,7,4,2,4,9,3,8,1,7,5,6,3,8,7,2,5,6,1,4,9,5,6,1,7,4,9,8,3,2,8,5,2,6,1,7,4,9,3,9,1,4,8,2,3,5,6,7,6,7,3,4,9,5,2,8,1};
	int hard_ans[SudokuSize]={0,0,6,0,0,3,0,0,0,0,0,0,8,9,0,0,0,0,4,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,2,0,0,0,1,0,0,0,0,9,0,0,0,0,0,0,6,0,3,0,0,0,0,8,0,0,0,0,1,0,0,0,2,0,0,0,0,0,0,4,5,0,0,0};
	int i;
	for(i=0;i<SudokuSize;i++){
		sudoku_ans[i]=hard_ans[i]; //could switch to hard_ans or orig_ans
	}
}
void Sudoku::printSudoku(){
	int i;
	for(i=0;i<SudokuSize;i++){
		printf("%d%c",sudoku_ans[i],(i+1)%9==0?'\n':' ');
	}
}
void Sudoku::giveQuestion(){
	setAns();
	srand(time(NULL));
	changeNum(rand()%RowNum+1,rand()%RowNum+1);
	changeNum(rand()%RowNum+1,rand()%RowNum+1);
	changeRow(rand()%3,rand()%3);
	changeCol(rand()%3,rand()%3);
	rotate(rand()%101);
	flip(rand()%2);
	//setSpace(); //if it's orig_ans then setSpace
	printSudoku();
}
void Sudoku::setSpace(){
	int i, j;
	srand(time(NULL));
	for(i=0;i<(rand()%6+15);i++){
		j=rand()%81;
		while(sudoku_ans[j]==0){
			j=rand()%81;
		}
		sudoku_ans[j]=0;
		sudoku_ans[80-j]=0;
	}
}
void Sudoku::readIn(){
	printf("Please enter your sudoku:\n");
	int i;
	for(i=0;i<SudokuSize;i++){
		cin >> sudoku_ans[i];
	}
}
void Sudoku::solve(){
	int cell, i, j, startRow, startCol, startSec;
	int prog, firstblank, firstblankpossi, nopossiflag, ansnum=0, noblank=0;
	//check wrong question
	if(ifNoAns()==1){
		printf("%d\n",ansnum);
		return;
	}
	int temp[SudokuSize];
	vector<int> possi(RowNum,1);
	vector<int> isblank(SudokuSize,0);
	//ensure the blank cell in the sudoku whose isblank are marked 1
	for(i=0;i<SudokuSize;i++){
		if(sudoku_ans[i]==0){
			isblank[i]=1;
			noblank++;
		}
	}
	//check if there is no blank(for transform test)
	if(noblank==0){
		printf("1\n");
		printSudoku();
		return;
	}
	else if(noblank>64){
		printf("2\n");
		return;
	}
	//find the first blank
	for(i=0;i<SudokuSize;i++){
		if(sudoku_ans[i]==0){
			firstblank=i;
			break;
		}
	}
	//for every cell, if it's not a fixed number, then fill the smallest possi as an answer. if no possi then go back to the previous cell and change its value to the next possi of it
	for(cell=0;cell<SudokuSize;cell++){
		if(isblank[cell]==1){
			//initialize the possi of the recent cell
			for(i=0;i<RowNum;i++) possi[i]=1;
			//checkRow
			startRow=cell/9*9;
			for(i=startRow;i<startRow+RowNum;i++){
				if(i!=cell && sudoku_ans[i]!=0) possi[sudoku_ans[i]-1]=0;
			}
			//checkCol
			startCol=cell%9;
			for(i=startCol;i<SudokuSize;i=i+9){
				if(i!=cell && sudoku_ans[i]!=0) possi[sudoku_ans[i]-1]=0;
			}
			//checkSec
			startSec=(startRow/27*27)+(startCol/3*3);
			for(i=startSec;i<startSec+3;i++){
				for(j=i;j<i+ColNum*3;j=j+ColNum){
					if(j!=cell && sudoku_ans[j]!=0) possi[sudoku_ans[j]-1]=0;
				}
			}
			//select the largest possi of the first blank
			if(cell==firstblank){
				for(i=RowNum-1;i>-1;i--){
					if(possi[i]==1){
						firstblankpossi=i+1;
						break;
					}
				}
			}
			//fill the smallest possi in the cell and check if no possible answer can be filled
			nopossiflag=0;
			for(i=sudoku_ans[cell]+1;i<10;i++){
				if(possi[i-1]==1){
					sudoku_ans[cell]=i;
					nopossiflag=1;
					break; // to the next cell
				}
			}
			//there's no num to fill in this blank so go to the previous cell
			if(nopossiflag==0){
				// check for no answer
				if(ansnum==0 && cell==firstblank && sudoku_ans[cell]==firstblankpossi){
					printf("%d\n",ansnum);
					return;
				}
				// check for unique answer
				if(ansnum==1 && cell==firstblank && sudoku_ans[cell]==firstblankpossi){
					printf("%d\n",ansnum);
					for(i=0;i<SudokuSize;i++){
						printf("%d%c",temp[i],(i%9==8)?'\n':' ');
					}
					return;
				}
				prog=cell;
				cell=prog-2;
				while(isblank[cell+1]==0){
					cell--;
				}
				for(i=prog;i>cell+1;i--){
					if(isblank[i]==1) sudoku_ans[i]=0;
				}
			}
		}
		// check for more answer(has found an answer)
		if(cell==80){
			ansnum++;
			if(ansnum>1){
				printf("%d\n",ansnum);
				return;
			}
			else if(ansnum==1){
				for(i=0;i<SudokuSize;i++){
					temp[i]=sudoku_ans[i];
				}
				prog=cell;
				cell=prog-2;
				while(isblank[cell+1]==0){
					cell--;
				}
				for(i=prog;i>cell+1;i--){
					if(isblank[i]==1) sudoku_ans[i]=0;
				}
			}
		}
	}
}
int Sudoku::ifNoAns(){
	int i, j, k, l;
	vector<int> check(RowNum,0);
	//checkRow
	for(i=0;i<RowNum;i++){
		for(j=i*RowNum;j<i*RowNum+RowNum;j++){
			if(sudoku_ans[j]!=0) check[sudoku_ans[j]-1]++;
		}
		for(j=0;j<RowNum;j++){
			if(check[j]>1) return 1;
			else check[j]=0;
		}
	}
	//checkCol
	for(i=0;i<RowNum;i++){
		for(j=i;j<SudokuSize;j=j+ColNum){
			if(sudoku_ans[j]!=0) check[sudoku_ans[j]-1]++;
		}
		for(j=0;j<RowNum;j++){
			if(check[j]>1) return 1;
			else check[j]=0;
		}
	}
	//checkSec
	for(i=0;i<55;i=i+27){
		for(j=i;j<i+7;j=j+3){
			for(k=j;k<j+19;k=k+RowNum){
				for(l=k;l<k+3;l++){
					if(sudoku_ans[l]!=0) check[sudoku_ans[l]-1]++;
				}
			}
			for(k=0;k<RowNum;k++){
				if(check[k]>1) return 1;
				else check[k]=0;
			}
		}
	}
	return 0;
}
void Sudoku::changeNum(int a, int b){
	int i;
	for(i=0;i<SudokuSize;i++){
		if(sudoku_ans[i]==a)sudoku_ans[i]=b;
		else if(sudoku_ans[i]==b)sudoku_ans[i]=a;
	}
}
void Sudoku::changeRow(int a, int b){
	int temp[RowNum];
	int i, j;
	for(j=0;j<3;j++){
		for(i=0;i<RowNum;i++){
			temp[i]=sudoku_ans[(a*3+j)*RowNum+i];
			sudoku_ans[(a*3+j)*RowNum+i]=sudoku_ans[(b*3+j)*RowNum+i];
			sudoku_ans[(b*3+j)*RowNum+i]=temp[i];
		}
	}
}
void Sudoku::changeCol(int a, int b){
	int temp[ColNum];
	int i, j;
	for(j=0;j<3;j++){
		for(i=0;i<ColNum;i++){
			temp[i]=sudoku_ans[a*3+j+i*9];
			sudoku_ans[a*3+j+i*9]=sudoku_ans[b*3+j+i*9];
			sudoku_ans[b*3+j+i*9]=temp[i];
		}
	}
}
void Sudoku::rotate(int n){
	n=n%4;
	if(n!=0){
		int temp[SudokuSize];
		int i, j, k;
		for(k=0;k<n;k++){
			for(i=0;i<ColNum;i++){
				for(j=0;j<ColNum;j++){
					temp[ColNum-1-i+j*9]=sudoku_ans[i*ColNum+j];
				}
			}
			for(i=0;i<SudokuSize;i++){
				sudoku_ans[i]=temp[i];
			}
		}
	}
}
void Sudoku::flip(int n){
	int temp[SudokuSize];
	int i, j;
	if(n==0){
		for(i=RowNum-1;i>=0;i--){
			for(j=0;j<RowNum;j++){
				temp[(RowNum-1-i)*RowNum+j]=sudoku_ans[i*9+j];
			}
		}
		for(i=0;i<SudokuSize;i++){
			sudoku_ans[i]=temp[i];
		}
	}
	else{
		for(i=0;i<RowNum;i++){
			for(j=0;j<RowNum;j++){
				temp[i*9+j]=sudoku_ans[i*9+(RowNum-1-j)];
			}
		}
		for(i=0;i<SudokuSize;i++){
			sudoku_ans[i]=temp[i];
		}
	}
}
void Sudoku::transform(){
	srand(time(NULL));
	changeNum(rand()%RowNum+1,rand()%RowNum+1);
	changeNum(rand()%RowNum+1,rand()%RowNum+1);
	changeRow(rand()%3,rand()%3);
	changeCol(rand()%3,rand()%3);
	rotate(rand()%101);
	flip(rand()%2);
	printf("\n\n");
	printSudoku();
}
