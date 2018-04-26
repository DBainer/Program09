//Dalton Bainer
#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <sstream>


using namespace std;

class Memory {
	string memory[1000]; //Array of 1000 strings
public:
	Memory() {
		for (int i = 0; i < 1000; i++) //Initializes memory to nop
		{
			memory[i] = "nop";
		}
	}
	void setmem(int a, string b);
	string getmem(int a) { return memory[a]; }

};

int PC;
string IR;
int MAR;
string MBR;
int MQ = 0;
int AC = 0;
int halt;
match_results<std::string::const_iterator> result;
Memory memory;

regex pattern1("(\\d+) (.*)"); //Regex patterns to match when reading file.
regex pattern2("load M\\((\\d+)\\)");
regex pattern3("stor M\\((\\d+)\\)");
regex pattern4("load MQ");
regex pattern5("load -M\\((\\d+)\\)");
regex pattern6("load \\|M\\((\\d+)\\)\\|");
regex pattern7("load -\\|M\\((\\d+)\\)\\|");
regex pattern8("load MQ,M\\((\\d+)\\)");
regex pattern9("halt");
regex pattern10("begin");
regex pattern11("nop");
regex pattern12("(.*)(\\.)(.*)");



void Memory::setmem(int a, string b)
{
	memory[a] = b;
}

int GetIntVal2(string strConvert) //Converts strings to ints
{
	int intReturn = 0;
	intReturn = stoi(strConvert);
	return(intReturn);
}

string GetStrVal(int intConvert) //Converts ints to strings
{

	ostringstream cstr;
	cstr << intConvert;
	return cstr.str();
}

void execute(int a, string b)
{
	int c = GetIntVal2(b); //Converts string to int
	if (a == 1)
	{
		AC = GetIntVal2(memory.getmem(c)); //transfer M(x) to AC
	}
	else if (a == 2)
	{
		memory.setmem(c, GetStrVal(AC)); //store AC to memory location x
	}
	else if (a == 3)
	{
		AC = MQ; // Transfer MQ into AC
	}
	else if (a == 4) 
	{
		AC = -1 * GetIntVal2(memory.getmem(c));//Transfer -M(x) to AC
	}
	else if (a == 5)
	{
		AC = abs(GetIntVal2(memory.getmem(c)));// Trasnfer absolute value of M(x) to AC
	}
	else if (a == 6)
	{
		AC = -1 * abs(GetIntVal2(memory.getmem(c))); //transfer -|M(x)| to AC
	}
	else if (a == 7)
	{
		MQ = GetIntVal2(memory.getmem(c)); //transfer contents of Memory location to register MQ
	}
}
void decode()
{
	if (regex_match(IR, result, pattern2)) //match load M(x)
	{
		execute(1, result[1]);
	}
	else if (regex_match(IR, result, pattern3))//Match stor M(x)
	{
		execute(2, result[1]);
	}
	else if (regex_match(IR, result, pattern8))//Match load MQ, M(x)
	{
		execute(7, result[1]);
	}
	else if (regex_match(IR, result, pattern4)) //Match load MQ
	{
		execute(3,"1");
	}
	else if (regex_match(IR, result, pattern5)) // Match load -M(x)
	{
		execute(4, result[1]);
	}
	else if (regex_match(IR, result, pattern6)) //Match load |M(x)|
	{
		execute(5, result[1]);
	}
	else if (regex_match(IR, result, pattern7))//Match load -|M(x)|
	{
		execute(6, result[1]);
	}
	
	else if (regex_match(IR, result, pattern9)) //match halt
	{
		halt = PC; //halt used to memory print loop variable.
	}
	else if (regex_match(IR, result, pattern10))//Match begin
	{
		//do Nothing
	}
	else if (regex_match(IR, result, pattern11)) //match nop
	{
		//Do nothing
	}
	else if (regex_match(IR, result, pattern12)) //Match comments
	{
		//Do Nothing
	}
	else
	{
		//no match
		abort();
	}

}


int main()
{
	ifstream file;
	string line;
	string inFile;

	cout << "Please enter filename: " << endl;

	cin >> inFile;
	file.open(inFile);
	while (!file.eof()) //while we aren't at end of the file
	{
		getline(file, line);
		if (regex_match(line, result, pattern1))
		{
			if (result[2] == "begin")
			{
				PC = GetIntVal2(result[1]); //Set PC to line where we read begin
			}
			memory.setmem(GetIntVal2(result[1]), result[2]); //store info after the line number into the memory at that line numbers location
			
		}

	}

	cout << "Execution of Program" << endl;
	while (memory.getmem(PC-1) != "halt")
	{
		MAR = PC; //Fetch execute cycle
		MBR = memory.getmem(PC);
		cout << "PC = " << PC;
		PC++;
		IR = MBR;
		cout << "	IR = " << IR << endl;
		decode();
		cout << "PC = " << PC << "	AC = " << AC << "	MQ = " << MQ << endl;
		cout << "--------------------------------------------------------" << endl;
	}
	//Print out memory
	cout << "Memory" << endl; 

	for (int i = 0; i < halt; i++)
	{
		cout << i << " " << memory.getmem(i) << endl;
	}
	system("pause");
}