//********************************************
// File: parser.h
// Description: 
//  define the parse class
//********************************************

#ifndef _PARSE_H_
#define _PARSE_H_

class Parser {
public:
	Parser (arguments);
	virtual ~Parser ();
	//void genCircle(double center, double r, double c, double l);
	//void genRect(double bottomLeft, double width, double height, double c, double l);
void Parser::parse(string filename, 
		double* &c, double* &l, 
		double &Rs, double &Rl, double &t);
//void Parser::parse(string filename, 
//		int &Nx, int &Ny, double* &c, double* &l, 
//		double &deltax, double &deltay, double &deltat );
private:
	bool hasinitial;
	double xsize, ysize;
};

//********************************************
// Function: openfile
// Description: prepare FILE* fd
//********************************************
void Parser::openfile(FILE* &fd, string filename) {
	fd = fopen(filename.c_str(), "r");
	if (fd == NULL){
		cerr << "Cannot open file!" << endl;
		exit(EXIT_FAILURE);
	}
}

#endif /* end of include guard: _PARSE_H_ */
