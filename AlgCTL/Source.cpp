#include<iostream>
#include<fstream>
#include<cstring>

#define FORMULA_MAX_LEN 100

/*
EXp
- caut lumile care au p (sau au p^q sau _p^q etc...)
- la PreE dau ca parametru aceste lumi in loc de label
- functie convertBoolToCharList
- functie convertCharListToBool
*/


using namespace std;
struct kripkeStructure {
	int worldsCount;
	char worldsNames[100][10];
	bool worldsRelations[100][100];
	char wordlsLabels[100][20];
};
void readKripreStructure(kripkeStructure &ks) {
	char aux[100], *auxp;
	int i, j;
	ifstream f("kripke_structure.txt");
	f.getline(aux, 100);//skipping files comments

	f >> ks.worldsCount;

	f.get();
	f.getline(aux, 100);//skipping files comments

	for (i = 0; i < ks.worldsCount; i++) {
		f >> ks.worldsNames[i];
	}

	f.get();
	f.getline(aux, 100);//skipping files comments

	for (i = 0; i < ks.worldsCount; i++) {
		j = -1;
		do {
			j++;
			f >> ks.wordlsLabels[i][j];
		} while (ks.wordlsLabels[i][j] != '-');
		ks.wordlsLabels[i][j] = '\0';
	}

	f.get();
	f.getline(aux, 100);//skipping files comments

	char worldName[10];
	for (i = 0; i < ks.worldsCount; i++) {
		for (j = 0; j < ks.worldsCount; j++) {
			ks.worldsRelations[i][j] = 0;
		}
	}
	for (i = 0; i < ks.worldsCount; i++) {
		j = -1;
		while (true) {
			f >> worldName;
			if (strcmp(worldName, "-") == 0) {
				break;
			}
			for (j = 0; j < ks.worldsCount; j++) {
				if (strcmp(worldName, ks.worldsNames[j]) == 0) {
					ks.worldsRelations[i][j] = true;
				}
			}
		}
	}
	f.close();

}
void printKripkeStructure(kripkeStructure ks) {
	int i, j;
	cout << ks.worldsCount << endl;
	for (i = 0; i < ks.worldsCount; i++) {
		cout << ks.worldsNames[i] << " ";
	}
	cout << endl;
	for (i = 0; i < ks.worldsCount; i++) {
		if (strlen(ks.wordlsLabels[i]) == 0) {
			cout << "-" << endl;
		}
		else {
			cout << ks.wordlsLabels[i] << endl;
		}
	}
	for (i = 0; i < ks.worldsCount; i++) {
		for (j = 0; j < ks.worldsCount; j++) {
			cout << ks.worldsRelations[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
void operatorPre(kripkeStructure ks, int currentWorld, bool previousWorlds[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++) {
		previousWorlds[i] = false;
		if (ks.worldsRelations[i][currentWorld] == true) {
			previousWorlds[i] = true;
		}
	}
}
void operatorPreE(kripkeStructure ks, char label, bool previousWorlds[100]) {
	int i,j;
	for (i = 0; i < ks.worldsCount; i++) {
		previousWorlds[i] = false;
		for (j = 0; j < ks.worldsCount; j++) {
			if (strchr(ks.wordlsLabels[j],label)&&ks.worldsRelations[i][j] == true) {
				previousWorlds[i] = true;
			}
		}
	}
}
void operatorPreENot(kripkeStructure ks, char label, bool previousWorlds[100]) {
	int i, j;
	for (i = 0; i < ks.worldsCount; i++) {
		previousWorlds[i] = false;
		for (j = 0; j < ks.worldsCount; j++) {
			if (!strchr(ks.wordlsLabels[j], label) && ks.worldsRelations[i][j] == true) {
				previousWorlds[i] = true;
			}
		}
	}
}
void readFormula(char formula[100]) {
	ifstream f("formula.txt");
	f.get(formula, 100);
	f.close();
}
void checkFormula(char formula[100]) {
	int flen = strlen(formula);
	char auxFormula[100];
	if (formula[0] != '[' || formula[flen - 1] != ']') {
		cout << "Missing \'[]\'";
		return;
	}
	strncpy_s(auxFormula, FORMULA_MAX_LEN, formula, flen - 1);
	strncpy_s(formula, FORMULA_MAX_LEN, auxFormula + 1, flen - 2);
	flen -= 2;
	if (formula[0] != '|' || formula[flen - 1] != '|') {
		cout << "Missing \'||\'";
		return;
	}
	strncpy_s(auxFormula, FORMULA_MAX_LEN, formula, flen - 1);
	strncpy_s(formula, FORMULA_MAX_LEN, auxFormula + 1, flen - 2);
	flen -= 2;
}
void processFormula(kripkeStructure ks, char *formula) {
	int i;
	if (strncmp(formula, "EX", 2) == 0) {
		if (formula[2] != '{') {
			bool previousWorlds[100];
			char auxResult[100] = "{";
			char auxFormula[100];
			operatorPreE(ks, *(formula + 2), previousWorlds);
			for (int i = 0; i < ks.worldsCount; i++) {
				if (previousWorlds[i] == true) {
					strcat_s(auxResult, FORMULA_MAX_LEN, ks.worldsNames[i]);
					strcat_s(auxResult, FORMULA_MAX_LEN, ",");
				}
			}
			auxResult[strlen(auxResult) - 1] = '\0';
			strcat_s(auxResult, FORMULA_MAX_LEN, "}");
			strcpy_s(auxFormula, FORMULA_MAX_LEN, formula + 3);
			strcpy_s(formula, FORMULA_MAX_LEN, auxResult);
			strcpy_s(formula + strlen(auxResult), FORMULA_MAX_LEN, auxFormula);
		}
	}

}
int main() {
	kripkeStructure ks;
	char formula[100];
	readKripreStructure(ks);
	printKripkeStructure(ks);
	readFormula(formula);
	cout << formula << endl;
	checkFormula(formula);
	cout << formula << endl;
	processFormula(ks,formula);
	cout << formula << endl;



	int x;
	cin >> x;
}