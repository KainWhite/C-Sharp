#include "pch.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <string>
#include <stack>

using namespace std;

bool isDelimeter(char x)
{
	return (x < 'a' || x > 'z') && (x < 'A' || x > 'Z') && (x < '0' || x > '9') && x != '_' && x != '.';
}

bool isLetter(char x)
{
	return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || x == '_';
}

bool isNumber(char x)
{
	return x >= '0' && x <= '9';
}

void replaceWithSpace(string &str, int from, int to)
{
	for (int i = from; i <= to; i++)
		str[i] = ' ';
}

bool areForwardSpacesAndX(string &str, int from, char X)
{
	int i = from;
	while (str[i] == ' ' && str[i] != X)
		i++;
	return str[i] == X;
}

bool readFile(vector<string> &code, string filePath)
{
	ifstream fin(filePath);
	if (!fin.is_open())
	{
		cout << "File was not opened somehow." << endl;
		return false;
	}
	code.push_back("");
	while (getline(fin, code.back()))
		code.push_back("");
	fin.close();
	return true;
}

void deleteTrash(vector<string> &v, map<string, int> &operandCount, set<string> &operandSet)
{
	for (int i = 0; i < size(v); i++)
	{
		int j;
		int quotes = 0;
		bool deleting = false;
		string tmpStr = "";
		for (j = 0; j < v[i].length(); j++)
		{
			if (v[i][j] == '"')
			{	
				quotes++;
				if (j > 0 && v[i][j - 1] == '\\')
				{
					quotes++;
					v[i][j] = ' ';
					v[i][j - 1] = ' ';
				}
				if (quotes % (1 << 1) == 1)
				{
					tmpStr = "";
					deleting = true;
				}
				else
				{	
					tmpStr += "\"";
					if (tmpStr != "\"")
					{
						operandCount[tmpStr]++;
						operandSet.insert(tmpStr);
					}
					v[i][j] = ' ';
					deleting = false;
					continue;
				}
			}
			if (deleting)
			{
				tmpStr += v[i][j];
				if (j + 1 < v[i].size() && v[i][j] == '\\' && v[i][j + 1] == '"')
				{
					v[i][j + 1] = ' ';
					tmpStr += "\"";
				}
				v[i][j] = ' ';
				continue;
			}
			if (v[i][j] == '\'')
			{
				string tempStr = "\'";
				v[i][j] = ' ';
				if (j + 1 < v[i].size())
				{
					tempStr += v[i][j + 1];
					v[i][j + 1] = ' ';
				}
				if (tempStr.length() > 1 && tempStr[1] != '\'')
				{
					tempStr += "\'";
					if (j + 2 < v[i].size())
						v[i][j + 2] = ' ';
				}	
				operandCount[tempStr]++;
				operandSet.insert(tempStr);
			}
			if ((j + 1 < v[i].size()) && (v[i][j] == '/') && (v[i][j + 1] == '/') && ((quotes % (1 << 1)) == !1))
				break;
		}
		if (j < v[i].length())
			v[i] = v[i].substr(0, j);
	}
}

void deleteComments(vector<string> &v)
{
	bool deleting = false;
	for (int i = 0; i < size(v); i++) 
	{
		for (int j = 0; j < v[i].length(); j++)
		{
			if (deleting)
			{	
				if (j + 1 < v[i].length() && v[i][j] == '*' && v[i][j + 1] == '/')
				{
					deleting = false;
					v[i][j] = ' ';
					v[i][j + 1] = ' ';
				}
				else
					v[i][j] = ' ';
			}
			else
			{
				if (j + 1 < v[i].length() && v[i][j] == '/' && v[i][j + 1] == '*')
				{
					deleting = true;
					v[i][j] = ' ';
					v[i][j + 1] = ' ';
				}
			}
		}
	}
}

void detectTripleSignedOperators(vector<string> &code, map<string, int> &statementCount, set<string> &statementSet)
{
	set<string> operators;
	operators.insert(">>>");
	operators.insert("<<=");
	operators.insert(">>=");
	for (int i = 0; i < code.size(); i++)
	{
		for (int j = 2; j < code[i].size(); j++)
		{
			if (operators.find(code[i].substr(j - 2, 3)) != operators.end())
			{
				statementCount[code[i].substr(j - 2, 3)]++;
				statementSet.insert(code[i].substr(j - 2, 3));
				code[i][j - 2] = ' ';
				code[i][j - 1] = ' ';
				code[i][j] = ' ';
			}
		}
	}
}

void detectDoubleSignedOperators(vector<string> &code, map<string, int> &statementCount, set<string> &statementSet)
{
	set<string> operators;
	operators.insert("++");
	operators.insert("--");
	operators.insert("==");
	operators.insert("!=");
	operators.insert(">=");
	operators.insert("<=");
	operators.insert(">>");
	operators.insert("<<");
	operators.insert("&&");
	operators.insert("||");
	operators.insert("+=");
	operators.insert("-=");
	operators.insert("*=");
	operators.insert("/=");
	operators.insert("%=");
	operators.insert("&=");
	operators.insert("|=");
	operators.insert("^=");
	for (int i = 0; i < code.size(); i++)
	{
		for (int j = 1; j < code[i].size(); j++)
		{
			if (operators.find(code[i].substr(j - 1, 2)) != operators.end())
			{
				statementCount[code[i].substr(j - 1, 2)]++;
				statementSet.insert(code[i].substr(j - 1, 2));
				code[i][j-1] = ' ';
				code[i][j] = ' ';
			}
		}
	}
}

void detectSingleSignedOperators(vector<string> &code, map<string, int> &statementCount, set<string> &statementSet)
{
	set<string> operators;
	operators.insert("+");
	operators.insert("-");
	operators.insert("=");
	operators.insert("!");
	operators.insert(">");
	operators.insert("<");
	operators.insert("&");
	operators.insert("|");
	operators.insert("*");
	operators.insert("/");
	operators.insert("%");
	operators.insert("^");
	operators.insert("~");
	for (int i = 0; i < code.size(); i++)
	{
		for (int j = 0; j < code[i].size(); j++)
		{
			if (operators.find(code[i].substr(j, 1)) != operators.end())
			{
				statementCount[code[i].substr(j, 1)]++;
				statementSet.insert(code[i].substr(j, 1));
				code[i][j] = ' ';
			}
		}
	}
}

void detectAll(vector<string> &code, map<string, int> &operandCount, map<string, int> &statementCount, set<string> &operandSet, set<string> &statementSet, int &functionCount)
{
	set<string> statements;
	statements.insert("for");
	statements.insert("do");
	statements.insert("while");
	statements.insert("continue");
	statements.insert("break");
	statements.insert("return");
	statements.insert("switch");
	statements.insert("if");
	set<string> variables;
	set<string> types;
	types.insert("int");
	types.insert("short");
	types.insert("byte");
	types.insert("long");
	types.insert("float");
	types.insert("double");
	types.insert("char");
	types.insert("boolean");
	types.insert("void");
	types.insert("String");
	types.insert("Integer");
	types.insert("Double");
	types.insert("Character");
	types.insert("Long");
	types.insert("Float");
	types.insert("Boolean");
	for (int i = 0; i < code.size(); i++)
	{
		bool isDefining = false;
		int firstPosMet = 0;
		for (int j = 0; j <= code[i].size(); j++)
		{
			if (isDelimeter(code[i][j]) || j == code[i].size())
			{
				string currentStatement = code[i].substr(firstPosMet, j - firstPosMet);
				firstPosMet = j+1;
				if (isDefining)
				{
					if (isLetter(currentStatement[0]) && types.find(currentStatement) == types.end() &&
														 statements.find(currentStatement) == statements.end())
					{
						variables.insert(currentStatement);
						replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
					}
				}
				else
				{
					if (statements.find(currentStatement) != statements.end())
					{
						statementSet.insert(currentStatement);
						statementCount[currentStatement]++;
						replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
					}
					if (variables.find(currentStatement) != variables.end())
					{
						operandSet.insert(currentStatement);
						operandCount[currentStatement]++;
						replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
					}
				}
				if (isNumber(currentStatement[0]))
				{
					operandSet.insert(currentStatement);
					operandCount[currentStatement]++;
					replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
				}
				if (types.find(currentStatement) != types.end() && !areForwardSpacesAndX(code[i], j, ')'))
				{
					if (!isDefining && code[i].find(';') == code[i].npos)
						functionCount++;
					isDefining = true;
					replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
				}
				if (j < code[i].size() && code[i][j] == ';')
					isDefining = false;
			}
		}
	}
}

void detectProcs(vector<string> &code, map<string, int> &statementCount, set<string> &statementSet, int &fcount)
{
	for (int i = 0; i < size(code); i++)
	{
		if (code[i].find(';') != code[i].npos)
			for (int j = 0; j < code[i].length(); j++)
			{
				if (code[i][j] == '(')
				{
					int b = j - 1;
					while (b > 0 && code[i][b] == ' ')
					{
						b -= 1;
					}
					int a = b;
					while (a - 1 > 0 && !isDelimeter(code[i][a - 1]))
					{
						a--;
					}
					if (b)
					{
						bool isValid = true;
						for (int k = a; k <= b; k++)
							if (isDelimeter(code[i][k]))
								isValid = false;
						if (isValid)
						{
							fcount++;
							string tempStr = code[i].substr(a, b - a + 1) + "()";
							statementCount[tempStr]++;
							statementSet.insert(tempStr);
						}

					}
				}
			}
	}
}

void detectAllBrackets(vector<string> &code, map<string, int> &statementCount, set<string> &statementSet)
{
	stack<char> openedBracket;
	for (int i = 0; i < code.size(); i++)
	{
		for (int j = 0; j < code[i].size(); j++)
		{
			if (code[i][j] == '(' || code[i][j] == '[' || code[i][j] == '{')
				openedBracket.push(code[i][j]);
			if (code[i][j] == ')' && openedBracket.top() == '(')
			{
				statementCount["( ... )"]++;
				statementSet.insert("( ... )");
				openedBracket.pop();
			}
			if (code[i][j] == ']' && openedBracket.top() == '[')
			{
				statementCount["[ ... ]"]++;
				statementSet.insert("[ ... ]");
				openedBracket.pop();
			}
			if (code[i][j] == '}' && openedBracket.top() == '{')
			{
				statementCount["{ ... }"]++;
				statementSet.insert("{ ... }");
				openedBracket.pop();
			}
		}
	}
}

void writeFile(vector<string> &code, string path)
{
	ofstream fout(path);
	if (!fout.is_open())
	{
		cout << "File was not opened somehow." << endl;
		return;
	}
	for (int i = 0; i < code.size(); i++)
		fout << code[i] << endl;
	fout.close();
}

int main()
{
	setlocale(LC_ALL, "rus");
	int functionCount = 0;
	vector<string> code;
	map<string, int> statementCount, operandCount;
	set<string> statementSet, operandSet;
	if (!readFile(code, "Code.txt"))
		return 0;
	deleteTrash(code, operandCount, operandSet);
	deleteComments(code);
	detectTripleSignedOperators(code, statementCount, statementSet);
	detectDoubleSignedOperators(code, statementCount, statementSet);
	detectSingleSignedOperators(code, statementCount, statementSet);
	detectAll(code, operandCount, statementCount, operandSet, statementSet, functionCount);
	detectProcs(code, statementCount, statementSet, functionCount);
	detectAllBrackets(code, statementCount, statementSet);
	int i = 0, mapStatementTotalCount = 0, mapOperandTotalCount = 0;
	cout << "Statement dictionary : " << statementSet.size() << endl;
	for (set<string>::iterator it = statementSet.begin(); it != statementSet.end(); it++, i++)
	{
		if (*it == "( ... )")
		{
			cout << i << ": " << *it << " " << statementCount[*it] - functionCount << endl;
			mapStatementTotalCount += statementCount[*it] - functionCount;
		}
		else
		{
			cout << i << ": " << *it << " " << statementCount[*it] << endl;
			mapStatementTotalCount += statementCount[*it];
		}
	}
	cout << "Total statements : " << mapStatementTotalCount << endl;
	i = 0;
	cout << "Operand dictionary(total count " << operandSet.size() << " ): " << endl;
	for (set<string>::iterator it = operandSet.begin(); it != operandSet.end(); it++, i++)
	{
		cout << i << ": " << *it << " " << operandCount[*it] << endl;
		mapOperandTotalCount += operandCount[*it];
	}
	cout << "Total operands : " << mapOperandTotalCount << endl << endl;
	cout << "Program dictionary : " << statementSet.size() + operandSet.size() << endl;
	cout << "Program length : " << mapOperandTotalCount + mapStatementTotalCount << endl;
	cout << "Program volume : " << (mapOperandTotalCount + mapStatementTotalCount) * log2(statementSet.size() + operandSet.size()) << endl;
	writeFile(code, "output.txt");
}
