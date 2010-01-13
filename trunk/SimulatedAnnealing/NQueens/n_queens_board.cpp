#include "n_queens_board.h"

NQueensBoard::NQueensBoard(int n):N(n){
	board = new bool*[N];
	for(int h=0; h<N; h++){
		board[h] = new bool[N];
		for(int w=0; w<N; w++){
			board[h][w] = (w == h);
		}
	}
	nQueens = N;
	nErrorsCache = 0;
	cacheCorrect = false;
	calcErrors();
}

NQueensBoard::NQueensBoard(const NQueensBoard& orig):N(orig.N){ //Copy constructor
	board = new bool*[N];
	for(int h=0; h<N; h++){
		board[h] = new bool[N];
		for(int w=0; w<N; w++){
			board[h][w] = orig.board[h][w];
		}
	}
	nQueens = orig.nQueens;
	nErrorsCache = orig.nErrorsCache;
	cacheCorrect = orig.cacheCorrect;
}


NQueensBoard::~NQueensBoard(){
	for (int h = 0; h < N; h++){
		delete [] board[h];
	}
	delete [] board;
}

void NQueensBoard::print() const{
	//upper line
	for(int i=0; i< (2*N+3); i++) std::cout << "*";
	std::cout << std::endl;

	//first separator
	std::cout << "*";
	for(int i=0; i< (2*N+1); i++) std::cout << "-";
	std::cout << "*" << std::endl;


	for(int h=0; h<N; h++){
		std::cout << "*|";
		for(int w=0; w<N; w++){
			std::cout << "" << (board[h][w]?"Q":" ") << "|";
		}
		std::cout << "*";

		//loop separator
		std::cout <<"\n*";
		for(int i=0; i< (2*N+1); i++) std::cout << "-";
		std::cout << "*" << std::endl;
	}

	//lower line
	for(int i=0; i< (2*N+3); i++) std::cout << "*";
	std::cout << "\n\n\n" << std::endl;
}

void NQueensBoard::setQueen(int h, int w){
	checkCoords(h-1, w-1);
	if(!board[h-1][w-1]){
		board[h-1][w-1] = true;
		nQueens++;
		cacheCorrect = false;
	}

}

void NQueensBoard::unsetQueen(int h, int w){
	checkCoords(h-1, w-1);
	if(board[h-1][w-1]){
		board[h-1][w-1] = false;
		nQueens--;
		cacheCorrect = false;
	}
}

bool NQueensBoard::checkCoords(int h, int w){
	assert(h >= 0);
	assert(h < N);
	assert(w >= 0);
	assert(w < N);
	return true;
}

NQueensBoard* NQueensBoard::returnRandomNeighbour() const{	
	NQueensBoard* boardcopy = new NQueensBoard(*this);
	int row1 = rand()%N;
	int row2 = rand()%N;
	int col1 = rand()%N;
	int col2 = rand()%N;
	if( row1 != row2 ){
		//std::cout << "Swap row "<< row1+1 << " and " << row2+1 << std::endl;
		bool* hulp = boardcopy->board[row1];
		boardcopy->board[row1] = boardcopy->board[row2];
		boardcopy->board[row2] = hulp;
		boardcopy->cacheCorrect=false;
	}

	if( col1 != col2){
		//std::cout << "Swap col " << col1+1 << " and " << col2+1 << std::endl;
		for(int i=0; i<N; i++){
			bool hulp = boardcopy->board[i][col1];
			boardcopy->board[i][col1] = boardcopy->board[i][col2];
			boardcopy->board[i][col2] = hulp;
		}
		boardcopy->cacheCorrect=false;
	}

	boardcopy->calcErrors();

	return boardcopy;
}

void NQueensBoard::calcErrors(){
	if(!cacheCorrect){
		nErrorsCache = 0;
		for(int i=0; i<N; i++){
			nErrorsCache += nErrors(nQueensInRow(i));
			nErrorsCache += nErrors(nQueensInColumn(i));
		}
		for(int i=0; i<N+N-2; i++){
			nErrorsCache += nErrors(nQueensInDiagUp(i));
			nErrorsCache += nErrors(nQueensInDiagDown(i));
		}
	}
	cacheCorrect = true;
}

int NQueensBoard::nQueensInRow(int i){
	checkCoords(i, 0);
	int currentNQueens = 0;	for(int w=0; w<N; w++){
		if(board[i][w]){
			currentNQueens++;
		}
	}
	return currentNQueens;
}

int NQueensBoard::nQueensInColumn(int i){
	checkCoords(0, i);
	int currentNQueens = 0;
	for(int h=0; h<N; h++){
		if(board[h][i]){
			currentNQueens++;
		}
	}
	return currentNQueens;
}
int NQueensBoard::nQueensInDiagDown(int i){
	int currentNQueens = 0;
	int w;
	int h;
	if(i < N){
		w=0;
		h=N-1-i;
	}else{
		h=0;
		w=i-(N-1);
	}
	while(w<N && h<N){
		if(board[h][w]){
			currentNQueens++;
		}
		w++;
		h++;
	}
	return currentNQueens;
}
int NQueensBoard::nQueensInDiagUp(int i){
	int currentNQueens = 0;
	int w;
	int h;
	if(i < N){
		w=0;
		h=i;
	}else{
		h=N-1;
		w=i-(N-1);
	}
	while(w<N && h>=0){
		if(board[h][w]){
			currentNQueens++;
		}
		w++;
		h--;
	}
	return currentNQueens;
}

int NQueensBoard::nErrors(int nQueens){
	int nErrors = nQueens-1;
	if(nErrors>=0)
		return nErrors;
	else
		return 0;
}

int NQueensBoard::getErrors() const{
	assert(cacheCorrect);
	return nErrorsCache;
}

std::ostream& operator<<(std::ostream& output, const NQueensBoard& nqb){
	std::cout << "Errors: " << nqb.getErrors() << std::endl;
	nqb.print();
	return output;
}

int NQueensBoard::objects;

//int main(){
//	srand(time(0));
//	
//	std::cout << "Eerste bord wordt aangemaakt" << std::endl;
//
//	NQueensBoard* bord = new NQueensBoard(7);
//
//	bord->print();
//	//bord.setQueen(7,1);
//	//bord.setQueen(6,2);
//	//bord.setQueen(7,3);
//	//bord.print();
//
//	for(int i=0; i<2; i++){
//		std::cout << i << "e bord wordt aangemaakt" << std::endl;
//
//		NQueensBoard* kopie = bord->returnRandomNeighbour();
//		kopie->print();
//
//		std::cout << i << "e bord wordt gedeleted" << std::endl;
//		delete kopie;
//	}
//		
//	std::cout << "Eerste bord wordt gedeleted" << std::endl;
//	delete bord;
//
//	std::cout << "Aantal nog bestaande objecten: " << NQueensBoard.objects << std::endl;
//
//	_CrtDumpMemoryLeaks();
//
//	//std::cout << "Errors: " << bord.getErrors() << std::endl;
//	std::cin.get();
//}