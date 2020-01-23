#include <bits/stdc++.h>
#include <sstream>

using namespace std;

struct chapin
{
    int times;
    string type;
    int nesting;
    int functionNumber;
    //bool initialized;
    bool input;
    bool changed;
    bool controlled;
    bool output;
    bool used;
    chapin()
    {
        times = 0;
        type = "";
        nesting = 0;
        functionNumber = 0;
        //initialized = 0;
        input = 0;
        changed = 0;
        controlled = 0;
        output = 0;
        used = 0;
    }
};

string intToString(int a)
{
    stringstream ss;
    string s;
    ss << a;
    ss >> s;
    return s;
}

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

bool isAssignOperator(string &str, const int i)
{
    if(i < str.size() && str[i] == '=' ||
       i < str.size() - 1 && str[i+1] == '=' && (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '&' || str[i] == '|' || str[i] == '%' || str[i] == '^') ||
       i < str.size() - 2 && str[i+2] == '=' && (str[i+1] == '>' && str[i] == '>' || str[i+1] == '<' && str[i] == '<'))
        return true;
    return false;
}

bool isReading(string &str)
{
    return (str == "Scanner.in.nextLine" || str == "Scanner.in.nextInt" || str == "Scanner.in.nextChar" || str == "Scanner.in.nextFloat");
}

bool isWriting(string &str)
{
    return (str == "System.out.println");
}

bool isControlling(string &str)
{
    return (str == "if" || str == "switch" || str == "for" || str == "while");
}

void replaceWithSpace(string &str, int from, int to)
{
	for (int i = from; i <= to; i++)
		str[i] = ' ';
}

bool areForwardSpacesAndX(const vector<string> &vstr, int fromI, int fromJ, char X)
{
	for(int i = fromI; i < vstr.size(); i++)
        for(int j = (i == fromI? fromJ : 0); j < vstr[i].size(); j++)
            if (vstr[i][j] != ' ' && vstr[i][j] != '\n')
                return vstr[i][j] == X;
    return 0;
}

void jmpForwardUntilAnyOfX(const vector<string> &vstr, int &fromI, int &fromJ, string X)
{
    for(int i = fromI; i < vstr.size(); i++)
        for(int j = (i == fromI? fromJ : 0); j < vstr[i].size(); j++)
            for(int k = 0; k < X.size(); k++)
                if(vstr[i][j] == X[k])
                {
                    fromI = i;
                    fromJ = j;
                    return;
                }
}

void jmpToNextValidPosition(const vector<string> &vstr, int &fromI, int &fromJ)
{
    for(int i = fromI; i < vstr.size(); i++)
        for(int j = (fromI == i ? fromJ : 0); j < vstr[i].size(); j ++)
            if(vstr[i][j] && vstr[i][j] != ' ')
            {
                fromI = i;
                fromJ = j;
                return;
            }
}

int countCasesUntilX(vector<string> vstr, int fromI, int fromJ, char X)
{
    int ans = 0;
    for(int i = fromI; i < vstr.size(); i++)
    {
        int firstPosMet = (i == fromI? fromJ : 0);
        for(int j = (i == fromI? fromJ : 0); j <= vstr[i].size(); j++)
        {
            if (j == vstr[i].size() || isDelimeter(vstr[i][j]))
			{
			    string currentStatement = vstr[i].substr(firstPosMet, max(j - firstPosMet, 0));
				firstPosMet = j+1;
				if(currentStatement == "case" || currentStatement == "default")
                    ans++;
                if(vstr[i][j] == X)
                    return ans;
			    if(vstr[i][j] == '{')
                {
                    int cnt = 1;
                    while(cnt)
                    {
                        jmpToNextValidPosition(vstr, i, j);
                        jmpForwardUntilAnyOfX(vstr, i, j, "{}");
                        if(vstr[i][j] == '{')
                            cnt++;
                        if(vstr[i][j] == '}')
                            cnt--;
                    }
                    firstPosMet = j;
                }
			}
        }
    }
    return ans;
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

void deleteTrash(vector<string> &v, vector< vector< map<string, chapin> > > &operandCount, vector< vector< set<string> > > &operandSet )
{
	for (int i = 0; i < v.size(); i++)
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
					/*if (tmpStr != "\"")
					{
						operandCount[tmpStr]++;
						operandSet.insert(tmpStr);
					}*/
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
				//operandCount[tempStr]++;
				//operandSet.insert(tempStr);
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
	for (int i = 0; i < v.size(); i++)
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
	//operators.insert("<<=");
	//operators.insert(">>=");
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
	/*operators.insert("+=");
	operators.insert("-=");
	operators.insert("*=");
	operators.insert("/=");
	operators.insert("%=");
	operators.insert("&=");
	operators.insert("|=");
	operators.insert("^=");*/
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
	//operators.insert("=");
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

void parseFunctionalBrackets(string code, int &j, vector< map<string, pair<string, int> > > &variables, vector< vector< map<string, chapin> > > &operandCount, const char mode)
{
    int firstPosMet = 0;
    string currentStatement;
    for(; j <= code.size(); j++)
    {
        if (j == code.size() || isDelimeter(code[j]))
        {
            currentStatement = code.substr(firstPosMet, j - firstPosMet);
            firstPosMet = j + 1;
            for(int i = 99; i >= 0; i--)
            {
                if(variables[i][currentStatement].first != "")
                {
                    switch(mode)
                    {
                    case 'r':
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].times ++;
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].input = true;
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].changed = false;
                        break;
                    case 'w':
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].times ++;
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].output = true;
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].used = true;
                        break;
                    case 'i':
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].times ++;
                        operandCount[i] [variables[i][currentStatement].second] [currentStatement].controlled = true;
                        break;
                    }
                }
            }
        }
        if(j < code.size() && code[j] == ')')
            break;
    }
}

void detectAll(vector<string> &code, vector< vector< map<string, chapin> > > &operandCount, map<string, int> &statementCount,
                                     vector< vector< set<string> > >         &operandSet,   set<string>      &statementSet,
                                     vector< map<string, pair<string, int> > > &variables,
                                     int functionNumber, int nesting)
{
    //cout<<code[0]<<endl;
	set<string> statements;
	statements.insert("for");
	statements.insert("do");
	statements.insert("while");
	statements.insert("continue");
	statements.insert("break");
	statements.insert("return");
	statements.insert("switch");
	statements.insert("if");
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
	bool isDefining = false;
	bool isChanging = false;
	int curFunctionNumber = functionNumber;
	int curNesting = nesting;
	string currentStatement, lastLetterStatement, currentType;
	int i = 0;
	while(i < code.size())
	{
		int firstPosMet = 0;
		isDefining = false;
		int j = 0;
		while(j <= code[i].size())
		{
		    isChanging = false;
			if (j == code[i].size() || isDelimeter(code[i][j]))
			{
			    currentStatement = code[i].substr(firstPosMet, j - firstPosMet);
			    //string curStat = currentStatement + "(" + intToString(functionNumber) + "." + intToString(curNesting) + ")";
			    //cout<<code[i] <<" "<< currentStatement<<code[i][j] <<endl;
			    jmpToNextValidPosition(code, i, j);
			    if(isAssignOperator(code[i], j))
                    isChanging = true;
				firstPosMet = j + (j == code[i].size()? 0:(isDelimeter(code[i][j])));
				if(isReading(currentStatement))
                {
                    parseFunctionalBrackets(code[i], j, variables, operandCount, 'r');
                    firstPosMet = j+1;
                    continue;
                }
                if(isWriting(currentStatement))
                {
                    parseFunctionalBrackets(code[i], j, variables, operandCount, 'w');
                    firstPosMet = j+1;
                    continue;
                }
                if(isControlling(currentStatement))
                {
                    parseFunctionalBrackets(code[i], j, variables, operandCount, 'i');
                    firstPosMet = j+1;
                    continue;
                }
				if (isDefining)
				{
				    if(j < code.size() && code[i][j] == '(')
                    {
                        int opened = 1;
                        j++;
                        if(j >= code[i].size())
                        {
                            i++;
                            j = 0;
                        }
                        vector<string> subcode(1);
                        while(i < code.size() && opened)
                        {
                            opened += (code[i][j] == '(');
                            opened -= (code[i][j] == ')');
                            subcode[0].push_back(code[i][j]);
                            j++;
                            if(j >= code[i].size())
                            {
                                i++;
                                j = 0;
                            }
                        }
                        firstPosMet = j;
                        //cout<<curFunctionNumber<<endl;
                        detectAll(subcode, operandCount, statementCount, operandSet, statementSet, variables, curFunctionNumber + 1, curNesting + 1);
                        continue;
                    }
					if (isLetter(currentStatement[0]) && types.find(currentStatement) == types.end() &&
														 statements.find(currentStatement) == statements.end())
					{
					    int realFunctionNumber = (curNesting == 0? 0:curFunctionNumber);
					    //cout<<"def: "<<i<<" "<<j<<" : "<<realFunctionNumber << " " <<curNesting <<" "<<currentStatement <<endl;
                        variables[curNesting][currentStatement] = make_pair(currentType, realFunctionNumber);
                        operandCount[curNesting][realFunctionNumber][currentStatement].type = currentType;
                        operandCount[curNesting][realFunctionNumber][currentStatement].nesting = curNesting;
                        operandCount[curNesting][realFunctionNumber][currentStatement].functionNumber = realFunctionNumber;
                        operandSet[curNesting][realFunctionNumber].insert(currentStatement);
                        if(isChanging)
                        {
                            operandCount[curNesting] [realFunctionNumber] [currentStatement].changed = true;
                            operandCount[curNesting] [realFunctionNumber] [currentStatement].input = false;
                        }

						replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
					}
				}
				else
				{
					/*if (statements.find(currentStatement) != statements.end())
					{
						statementSet.insert(currentStatement);
						statementCount[currentStatement]++;
						replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
					}*/
					//cout<<"not def: "<<currentStatement<<endl;
					for(int k = curNesting; k >= 0; k--)
                        if (variables[k][currentStatement].first != "")
                        {
                            //string curStat = currentStatement+"("+intToString(curFunctionNumber)+"."+intToString(k)+")";

                            operandCount[k] [variables[k][currentStatement].second] [currentStatement].times++;
                            if(isChanging)
                            {
                                operandCount[k] [variables[k][currentStatement].second] [currentStatement].changed = true;
                                operandCount[k] [variables[k][currentStatement].second] [currentStatement].input = false;
                            }
                            else
                                operandCount[k] [variables[k][currentStatement].second] [currentStatement].used = true;
                            replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
                            break;
                        }
				}
				/*if (isNumber(currentStatement[0]))
				{
					operandSet.insert(currentStatement);
					operandCount[currentStatement];
					replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
				}*/
				if (types.find(currentStatement) != types.end() && !areForwardSpacesAndX(code, i, j, ')'))
				{
					isDefining = true;
					currentType = currentStatement;
					replaceWithSpace(code[i], j - currentStatement.size(), j - 1);
				}
				if (j < code[i].size() && (code[i][j] == ';' || code[i][j] == '{'))
                {
                    isDefining = false;
                    currentType = "";
                }
                if(currentStatement.size() > 0)
                    lastLetterStatement = currentStatement;

                if(j < code[i].size() && code[i][j] == '{')
                {
                    curNesting++;
                    //cout<<(curNesting == 1)<<curFunctionNumber;
                    if(curNesting == 1)
                        curFunctionNumber++;
                    //cout<<curFunctionNumber<<endl;
                }
                if(j < code[i].size() && code[i][j] == '}')
                {
                    variables[curNesting].clear();
                    curNesting--;
                }
			}
			j++;
		}
		i++;
	}
}

void detectProcs(vector<string> &code, map<string, int> &statementCount, set<string> &statementSet, int &fcount)
{
	for (int i = 0; i < code.size(); i++)
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
	map<string, int> statementCount;
	set<string> statementSet;
	vector< vector< map<string, chapin> > > operandCount(100,  vector< map<string, chapin> >(100));
	vector< vector< set<string> > > operandSet(100, vector< set<string> >(100));
	vector< map<string, pair<string, int>> > variables(100);
	if (!readFile(code, "code1.txt"))
		return 0;
	deleteTrash(code, operandCount, operandSet);
	deleteComments(code);
	detectTripleSignedOperators(code, statementCount, statementSet);
	detectDoubleSignedOperators(code, statementCount, statementSet);
	detectSingleSignedOperators(code, statementCount, statementSet);
	detectAll(code, operandCount, statementCount, operandSet, statementSet, variables, 0, 0);
	/*detectProcs(code, statementCount, statementSet, functionCount);
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
	cout << "Operand dictionary(total count " << operandSet.size() << " ): " << endl;*/
	cout << "Spen: " << endl;
	int totalSpen = 0;
	for(int i = 0; i < 100; i++)
        for(int j = 0; j < 100; j++)
            for (set<string>::iterator it = operandSet[i][j].begin(); it != operandSet[i][j].end(); it++)
            {
                string fullName = *it + "(" + intToString(operandCount[i][j][*it].functionNumber) + "." + intToString(operandCount[i][j][*it].nesting) + ")";
                cout << "    " << fullName << " : " << operandCount[i] [j] [*it].times << endl;
                totalSpen += operandCount[i] [j] [*it].times;
            }
    cout << "  total : " << totalSpen << endl << endl;
    set<string> p, m, c, t;
    set<string> iop, iom, ioc, iot;
    for(int i = 0; i < 100; i++)
        for(int j = 0; j < 100; j++)
            for (set<string>::iterator it = operandSet[i][j].begin(); it != operandSet[i][j].end(); it++)
            {
                string fullName = *it + "(" + intToString(j) + "." + intToString(i) + ")";
                if(operandCount[i][j][*it].input && !operandCount[i][j][*it].changed && !operandCount[i][j][*it].controlled && operandCount[i][j][*it].used)
                    p.insert(fullName);
                if(!operandCount[i][j][*it].input && !operandCount[i][j][*it].controlled && operandCount[i][j][*it].used)
                    m.insert(fullName);
                if(operandCount[i][j][*it].controlled)
                    c.insert(fullName);
                if(!operandCount[i][j][*it].controlled && !operandCount[i][j][*it].output && !operandCount[i][j][*it].used)
                    t.insert(fullName);

                if(operandCount[i][j][*it].input && !operandCount[i][j][*it].changed && !operandCount[i][j][*it].controlled && operandCount[i][j][*it].used)
                    iop.insert(fullName);
                if(!operandCount[i][j][*it].input && !operandCount[i][j][*it].controlled && operandCount[i][j][*it].output && operandCount[i][j][*it].used)
                    iom.insert(fullName);
                if((operandCount[i][j][*it].input || operandCount[i][j][*it].output) && operandCount[i][j][*it].controlled)
                    ioc.insert(fullName);
                if(operandCount[i][j][*it].input && !operandCount[i][j][*it].controlled && !operandCount[i][j][*it].output && !operandCount[i][j][*it].used)
                    iot.insert(fullName);

                /*cout << fullName << " : " << endl
                        << "      times: " << operandCount[i] [j] [*it].times << ";" << endl
                        << "      type: " << operandCount[i] [j] [*it].type << ";" << endl
                        << "      nesting: " << operandCount[i] [j] [*it].nesting << ";" << endl
                        << "      functionNumber: " << operandCount[i] [j] [*it].functionNumber << ";" << endl
                        << "      input: " << operandCount[i] [j] [*it].input << ";" << endl
                        << "      changed: " << operandCount[i] [j] [*it].changed << ";" << endl
                        << "      controlled: " << operandCount[i] [j] [*it].controlled << ";" << endl
                        << "      output: " << operandCount[i] [j] [*it].output << ";" << endl
                        << "      used: " << operandCount[i] [j] [*it].used << ";" << endl;*/
            }

    cout << "Full Chapin's metrics: " << endl;

    cout << "    P : " << endl;
    cout << "        ";
    for(set<string>::iterator it = p.begin(); it != p.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << p.size() << endl;

    cout << "    M : " << endl;
    cout << "        ";
    for(set<string>::iterator it = m.begin(); it != m.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << m.size() << endl;

    cout << "    C : " << endl;
    cout << "        ";
    for(set<string>::iterator it = c.begin(); it != c.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << c.size() << endl;

    cout << "    T : " << endl;
    cout << "        ";
    for(set<string>::iterator it = t.begin(); it != t.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << t.size() << endl;

    cout << "  Q = " << p.size() << " + 2*" << m.size() << " + 3*" << c.size() << " + 0.5*" << t.size() << " = ";
    cout << p.size() + 2*m.size() + 3*c.size() + 0.5*t.size() << endl << endl;


    cout << "IO Chapin's metrics:" << endl;

    cout << "    P : " << endl;
    cout << "        ";
    for(set<string>::iterator it = iop.begin(); it != iop.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << iop.size() << endl;

    cout << "    M : " << endl;
    cout << "        ";
    for(set<string>::iterator it = iom.begin(); it != iom.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << iom.size() << endl;

    cout << "    C : " << endl;
    cout << "        ";
    for(set<string>::iterator it = ioc.begin(); it != ioc.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << ioc.size() << endl;

    cout << "    T : " << endl;
    cout << "        ";
    for(set<string>::iterator it = iot.begin(); it != iot.end(); it++)
        cout << *it << " ";
    cout << endl;
    cout << "      total: " << iot.size() << endl;

    cout << "  Q = " << iop.size() << " + 2*" << iom.size() << " + 3*" << ioc.size() << " + 0.5*" << iot.size() << " = ";
    cout << iop.size() + 2*iom.size() + 3*ioc.size() + 0.5*iot.size() << endl;
	/*cout << "Total operands : " << mapOperandTotalCount << endl << endl;
	cout << "Program dictionary : " << statementSet.size() + operandSet.size() << endl;
	cout << "Program length : " << mapOperandTotalCount + mapStatementTotalCount << endl;
	cout << "Program volume : " << (mapOperandTotalCount + mapStatementTotalCount) * log2(statementSet.size() + operandSet.size()) << endl;*/

	//writeFile(code, "output.txt");
}
