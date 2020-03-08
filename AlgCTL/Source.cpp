#include<iostream>
#include<fstream>
#include<cstring>

#define MAX_FORMULA_LEN 100
#define MAX_LABELS_COUNT 20
#define MAX_WORLDS_COUNT 100

/*
- sa adaug prioritate ca parametru
*/

using namespace std;
struct kripkeStructure {
	int worldsCount;
	char worldsNames[100][10];
	bool worldsRelations[100][100];
	char wordlsLabels[100][20];
	char allLabels[20] = "";
};
void readKripreStructure(kripkeStructure &ks) {
	char aux[100];
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
			if (ks.wordlsLabels[i][j] != '-' && !strchr(ks.allLabels, ks.wordlsLabels[i][j])) {
				ks.allLabels[strlen(ks.allLabels) + 1] = '\0';
				ks.allLabels[strlen(ks.allLabels)] = ks.wordlsLabels[i][j];
			}
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
	cout << "Worlds count=" << ks.worldsCount << endl;
	for (i = 0; i < ks.worldsCount; i++) {
		cout << ks.worldsNames[i] << " ";
	}
	cout << "Labels:" << endl;
	for (i = 0; i < ks.worldsCount; i++) {
		if (strlen(ks.wordlsLabels[i]) == 0) {
			cout << "-" << endl;
		}
		else {
			cout << ks.wordlsLabels[i] << endl;
		}
	}
	cout << "All labels:" << ks.allLabels << endl;
	cout << "Relations:" << endl;
	for (i = 0; i < ks.worldsCount; i++) {
		for (j = 0; j < ks.worldsCount; j++) {
			cout << ks.worldsRelations[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}
bool isWorldLabel(kripkeStructure ks, char character) {
	return strchr(ks.allLabels, character);
}
void operatorPre(kripkeStructure ks, int currentWorld, bool previousWorlds[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++) {
		if (ks.worldsRelations[i][currentWorld] == true) {
			previousWorlds[i] = true;
		}
		else {
			previousWorlds[i] = false;
		}
	}
}
void operatorPreE(kripkeStructure ks, bool suitableWorlds[100], bool previousWorlds[100]) {
	int i, j;
	for (i = 0; i < ks.worldsCount; i++) {
		previousWorlds[i] = false;
		for (j = 0; j < ks.worldsCount; j++) {
			if (suitableWorlds[j]==true && ks.worldsRelations[i][j] == true) {
				previousWorlds[i] = true;
			}
		}
	}
}
void operatorIntersection(kripkeStructure ks, bool firstWorldsList[100], bool secondWorldList[100],bool resultingWorldsList[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++)
		resultingWorldsList[i] = firstWorldsList[i] * secondWorldList[i];
}
void operatorReunion(kripkeStructure ks, bool firstWorldsList[100], bool secondWorldList[100], bool resultingWorldsList[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++)
		resultingWorldsList[i] = firstWorldsList[i] + secondWorldList[i];
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
	strncpy_s(auxFormula, MAX_FORMULA_LEN, formula, flen - 1);
	strncpy_s(formula, MAX_FORMULA_LEN, auxFormula + 1, flen - 2);
	flen -= 2;
	if (formula[0] != '|' || formula[flen - 1] != '|') {
		cout << "Missing \'||\'";
		return;
	}
	strncpy_s(auxFormula, MAX_FORMULA_LEN, formula, flen - 1);
	strncpy_s(formula, MAX_FORMULA_LEN, auxFormula + 1, flen - 2);
	flen -= 2;
}
void convertWorldsListToString(kripkeStructure ks, bool boolList[100], char resultingString[100]) {
	int i;
	strcpy_s(resultingString, MAX_WORLDS_COUNT, "{");
	for (i = 0; i < ks.worldsCount; i++) {
		if (boolList[i] == true) {
			strcat_s(resultingString, MAX_WORLDS_COUNT, ks.worldsNames[i]);
			strcat_s(resultingString, MAX_WORLDS_COUNT, ",");
		}
	}
	resultingString[strlen(resultingString) - 1] = '\0';
	strcat_s(resultingString, MAX_WORLDS_COUNT, "}");
}
void convertStringToWorldsList(kripkeStructure ks, char stringToConvert[100], bool resultingBoolList[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++) {
		if (strstr(stringToConvert, ks.worldsNames[i])) {
			resultingBoolList[i] = true;
		}
		else {
			resultingBoolList[i] = false;
		}
	}
}
void replaceInFormula(char *formula, int nrOfCharsToReplace, char Source[100]) {
	char auxFormula[100];
	strcpy_s(auxFormula, MAX_FORMULA_LEN, formula + nrOfCharsToReplace);
	strcpy_s(formula, MAX_FORMULA_LEN, Source);
	strcpy_s(formula + strlen(Source)+1, MAX_FORMULA_LEN, auxFormula);
}
void findWorldsThatSatifyLabel(kripkeStructure ks, char label, bool resultingWorlds[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++) {
		if (strchr(ks.wordlsLabels[i], label)) {
			resultingWorlds[i] = true;
		}
		else {
			resultingWorlds[i] = false;
		}
	}
}
void extractListFromString(char *formula,char *resultingString) {
	char *pos;
	if (*formula != '{') {
		cout << "Formula "<<formula<<" doesn't start from \'{\'!"<<endl;
		return;
	}
	pos=strchr(formula, '}');
	strncpy_s(resultingString, MAX_FORMULA_LEN,formula, pos - formula);

}
void invertBoolList(bool listToInvert[100],int length) {
	int i;
	for (i = 0; i < length; i++) {
		if (listToInvert[i] == true) {
			listToInvert[i] = false;
		}
		else {
			listToInvert[i] = true;
		}
	}
}
void processFormula(kripkeStructure ks, char *formula) {
	int i;
	bool resultingWorldsList[100], resultBool[100], resultBoolAux[100];
	char resultingWorldsString[100], resultString[100], resultStringAUX[100];
	cout << formula << endl;
	if (strncmp(formula, "_", 1) == 0) {
		processFormula(ks, formula + 1);
		extractListFromString(formula + 1, resultString);
		convertStringToWorldsList(ks, resultString, resultingWorldsList);
		invertBoolList(resultingWorldsList,ks.worldsCount);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 1 + strlen(resultString), resultingWorldsString);
		
	}else if (strncmp(formula, "EX", 2) == 0) {
		if (formula[2] != '{') {
			processFormula(ks, formula + 2);
			extractListFromString(formula + 2, resultString);
			convertStringToWorldsList(ks, resultString, resultBool);
			
			operatorPreE(ks,resultBool, resultingWorldsList);
			convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
			replaceInFormula(formula, 2+strlen(resultString), resultingWorldsString);
		}

	}
	else if (isWorldLabel(ks, *formula)) {
		if (formula[1] == '^') {
			processFormula(ks, formula + 2);
			extractListFromString(formula + 2, resultString);
			convertStringToWorldsList(ks, resultString, resultBool);

			findWorldsThatSatifyLabel(ks, *formula, resultBoolAux);
			operatorIntersection(ks, resultBool, resultBoolAux, resultingWorldsList);
			convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
			replaceInFormula(formula, 2+strlen(resultString), resultingWorldsString);
		}
		else {
			findWorldsThatSatifyLabel(ks, *formula, resultingWorldsList);
			convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
			replaceInFormula(formula, 1, resultingWorldsString);
		}
	}
	else {
		cout << "Encountered error on " << formula<<"!"<<endl;
	}
	cout << formula<<endl;
}
int main() {
	kripkeStructure ks;
	char formula[100];
	readKripreStructure(ks);
	printKripkeStructure(ks);
	readFormula(formula);
	cout << formula << endl;
	checkFormula(formula);
	processFormula(ks, formula);
	cout <<"\nResult: "<< formula << endl;



	int x;
	cin >> x;
}