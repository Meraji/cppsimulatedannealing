#include <iostream>
#include <assert.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

class NQueensBoard{
	friend std::ostream& operator<<(std::ostream& output, const NQueensBoard& nqb);

public:
	NQueensBoard(int n);
	~NQueensBoard();
	NQueensBoard(const NQueensBoard& board);
	//NQueensBoard& operator=(const NQueensBoard &rhs);

	void print() const;
	void setQueen(int h, int w);
	void unsetQueen(int h, int w);
	NQueensBoard* returnRandomNeighbour() const;

	int getErrors() const;

	static int objects;

private:
	const int N;
	int nQueens;
	int nErrorsCache;
	bool** board;
	bool cacheCorrect;

	bool checkCoords(int h, int w);


	int nQueensInRow(int i);
	int nQueensInColumn(int i);
	int nQueensInDiagDown(int i);
	int nQueensInDiagUp(int i);

	int nErrors(int nQueens);

	void calcErrors();

};