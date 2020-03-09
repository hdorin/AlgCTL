#include<iostream>
#include<fstream>
#include<cstring>

#define MAX_FORMULA_LEN 100
#define MAX_LABELS_COUNT 20
#define MAX_WORLDS_COUNT 100

/*
- sa adaug prioritate ca parametru
- cand termin EX sa apelez din nou process
	- daca formula incepe cu {, apelez procecess de la }+1 cu prioritate 2
	- daca gasesc ^, apelez ^+1 cu prioritate 2
		- voi gasit q, il convertesc
		- ma intorc la ^ si aplic operatia
*/

using namespace std;
struct kripkeStructure {
	int worldsCount;
	char worldsNames[100][10];
	bool worldsRelations[100][100];
	char wordlsLabels[100][20];
	char allLabels[20] = "Tt";
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
	cout << endl;
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
void operatorPre(kripkeStructure ks, bool suitableWorlds[100], bool resultingWorldsList[100]) {
	int i, j;
	for (i = 0; i < ks.worldsCount; i++) {
		resultingWorldsList[i] = false;
	}
	for (i = 0; i < ks.worldsCount; i++) {
		for (j = 0; j < ks.worldsCount; j++) {
			if (suitableWorlds[i] == true && ks.worldsRelations[j][i] == true) {
				resultingWorldsList[j] = true;
			}
		}
	}
}
void operatorPreE(kripkeStructure ks, bool suitableWorlds[100], bool resultingWorldsList[100]) {
	int i, j;
	for (i = 0; i < ks.worldsCount; i++) {
		resultingWorldsList[i] = false;
		for (j = 0; j < ks.worldsCount; j++) {
			if (suitableWorlds[j] == true && ks.worldsRelations[i][j] == true) {
				resultingWorldsList[i] = true;
			}
		}
	}
}
//TOTI succesorii unui predecesor trebuie sa fie din suitableWorlds, adica sa satisfaca q
void operatorPreA(kripkeStructure ks, bool suitableWorlds[100], bool resultingWorldsList[100]) {
	int i, j;
	operatorPre(ks, suitableWorlds, resultingWorldsList);
	for (i = 0; i < ks.worldsCount; i++) {
		if (resultingWorldsList[i] == true) {
			for (j = 0; j < ks.worldsCount; j++) {
				if (ks.worldsRelations[i][j] == true && suitableWorlds[j] == false) {
					resultingWorldsList[i] = false;
				}
			}
		}
	}

}
bool worldsListsDiffer(kripkeStructure ks, bool worldsList1[100], bool worldsList2[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++)
		if (worldsList1[i] != worldsList2[i]) {
			return true;
		}
	return false;
}

void operatorIntersection(kripkeStructure ks, bool firstWorldsList[100], bool secondWorldList[100], bool resultingWorldsList[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++)
		resultingWorldsList[i] = firstWorldsList[i] * secondWorldList[i];
}
void operatorReunion(kripkeStructure ks, bool firstWorldsList[100], bool secondWorldList[100], bool resultingWorldsList[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++)
		resultingWorldsList[i] = firstWorldsList[i] + secondWorldList[i];
}
void operatorMC_AF(kripkeStructure ks, bool suitableWorlds[100], bool resultingWorldsList[100]) {
	int i;
	bool Y[100], Z[100], worldsListAux[100];
	for (i = 0; i < ks.worldsCount; i++) {
		Z[i] = suitableWorlds[i];
		Y[i] = true;
	}
	while (worldsListsDiffer(ks, Y, Z)) {
		for (i = 0; i < ks.worldsCount; i++) {
			Y[i] = Z[i];
		}
		operatorPreA(ks, Z, worldsListAux);
		operatorReunion(ks, Z, worldsListAux, Z);
	}
	for (i = 0; i < ks.worldsCount; i++) {
		resultingWorldsList[i] = Y[i];
	}
}
bool worldsListNotIncludedIn(kripkeStructure ks, bool worldsList1[100], bool worldsList2[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++)
		if (worldsList1[i] == true && worldsList2[i] == false) {
			return true;
		}
	return false;
}
void operatorMC_EU(kripkeStructure ks, bool worldsList1[100], bool worldsList2[100], bool resultingWorldsList[100]) {
	int i, j;
	bool Y[100], Z[100], worldsListAux[100];
	for (i = 0; i < ks.worldsCount; i++) {
		Z[i] = worldsList2[i];
		Y[i] = false;
	}
	while (worldsListNotIncludedIn(ks, Z, Y)) {
		operatorReunion(ks, Y, Z, Y);
		operatorPre(ks, Y, worldsListAux);
		operatorIntersection(ks, worldsListAux, worldsList1, Z);
	}
	for (i = 0; i < ks.worldsCount; i++) {
		resultingWorldsList[i] = Y[i];
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
	bool is_empty = true;
	strcpy_s(resultingString, MAX_WORLDS_COUNT, "{");
	for (i = 0; i < ks.worldsCount; i++) {
		if (boolList[i] == true) {
			strcat_s(resultingString, MAX_WORLDS_COUNT, ks.worldsNames[i]);
			strcat_s(resultingString, MAX_WORLDS_COUNT, ",");
			is_empty = false;
		}
	}
	if (is_empty == false) {
		resultingString[strlen(resultingString) - 1] = '\0';
	}
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
	strcpy_s(formula + strlen(Source), MAX_FORMULA_LEN, auxFormula);
}
void findWorldsThatSatifyLabel(kripkeStructure ks, char label, bool resultingWorlds[100]) {
	int i;
	for (i = 0; i < ks.worldsCount; i++) {
		if (label != 't' && (label == 'T' || strchr(ks.wordlsLabels[i], label))) {
			resultingWorlds[i] = true;
		}
		else {
			resultingWorlds[i] = false;
		}
	}
}
void extractListFromString(char *formula, char *resultingString) {
	char *pos;
	if (*formula != '{') {
		cout << "Formula " << formula << " doesn't start from \'{\'!" << endl;
		return;
	}
	pos = strchr(formula, '}');
	strncpy_s(resultingString, MAX_FORMULA_LEN, formula, pos - formula);

}
void invertBoolList(bool listToInvert[100], int length) {
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
void processFormula(kripkeStructure ks, char *formula, int priority) {
	int i;
	bool resultingWorldsList[100], resultBool[100], resultBoolAux[100];
	char resultingWorldsString[100], resultString[100], resultStringAux[100];
	cout << formula << endl;

	if (strncmp(formula, "(", 1) == 0) {
		char formulaAux[100];
		cout << "  (: ";
		processFormula(ks, formula + 1, 2);
		replaceInFormula(formula, strlen(formula)-1, formula + 1);
		//strcpy_s(formulaAux, MAX_FORMULA_LEN, strchr(formula, ')') + 1);
		replaceInFormula(strchr(formula, ')'), strlen(strchr(formula, ')')) - 1, strchr(formula, ')') + 1);

		processFormula(ks, formula, priority);

	}
	else if (priority >= 0 && strncmp(formula, "_", 1) == 0) {
		cout << "  _: ";
		processFormula(ks, formula + 1, 0);
		extractListFromString(formula + 1, resultString);
		convertStringToWorldsList(ks, resultString, resultingWorldsList);
		invertBoolList(resultingWorldsList, ks.worldsCount);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 2 + strlen(resultString), resultingWorldsString);

		processFormula(ks, formula, priority);
	}
	else if (priority >= 0 && strncmp(formula, "EX", 2) == 0) {
		cout << "  EX:";
		processFormula(ks, formula + 2, 0);
		extractListFromString(formula + 2, resultString);
		convertStringToWorldsList(ks, resultString, resultBool);

		operatorPreE(ks, resultBool, resultingWorldsList);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 3 + strlen(resultString), resultingWorldsString);

		processFormula(ks, formula, priority);
	}
	else if (priority >= 0 && strncmp(formula, "AF", 2) == 0) {
		cout << "  AF:";
		processFormula(ks, formula + 2, 0);
		extractListFromString(formula + 2, resultString);
		convertStringToWorldsList(ks, resultString, resultBool);

		operatorMC_AF(ks, resultBool, resultingWorldsList);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 3 + strlen(resultString), resultingWorldsString);

		processFormula(ks, formula, priority);
	}
	else if (priority >= 1 && strncmp(formula, "E", 1) == 0) {
		cout << "  EU:";
		processFormula(ks, formula + 1, 1);
		processFormula(ks, strchr(formula, 'U') + 1, 1);
		extractListFromString(formula + 1, resultString);
		extractListFromString(strchr(formula, 'U') + 1, resultStringAux);
		convertStringToWorldsList(ks, resultString, resultBool);
		convertStringToWorldsList(ks, resultStringAux, resultBoolAux);

		operatorMC_EU(ks, resultBool, resultBoolAux, resultingWorldsList);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 4 + strlen(resultString) + strlen(resultStringAux), resultingWorldsString);

		processFormula(ks, formula, priority);
	}
	else if (priority >= 2 && *formula == '{' && *(strchr(formula, '}') + 1) == '^') {
		cout << "  ^: ";
		processFormula(ks, strchr(formula, '}') + 2, 2);
		extractListFromString(formula, resultString);
		convertStringToWorldsList(ks, resultString, resultBool);
		extractListFromString(strchr(formula, '}') + 2, resultStringAux);
		convertStringToWorldsList(ks, resultStringAux, resultBoolAux);

		//findWorldsThatSatifyLabel(ks, *formula, resultBoolAux);
		operatorIntersection(ks, resultBool, resultBoolAux, resultingWorldsList);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 3 + strlen(resultString) + strlen(resultStringAux), resultingWorldsString);
	}
	else if (priority >= 2 && *formula == '{' && *(strchr(formula, '}') + 1) == 'v') {
		cout << "v: ";
		processFormula(ks, strchr(formula, '}') + 2, 2);
		extractListFromString(formula, resultString);
		convertStringToWorldsList(ks, resultString, resultBool);
		extractListFromString(strchr(formula, '}') + 2, resultStringAux);
		convertStringToWorldsList(ks, resultStringAux, resultBoolAux);

		//findWorldsThatSatifyLabel(ks, *formula, resultBoolAux);
		operatorReunion(ks, resultBool, resultBoolAux, resultingWorldsList);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 3 + strlen(resultString) + strlen(resultStringAux), resultingWorldsString);
	}
	else if (strncmp(formula, "AX", 2) == 0) {
		cout << "  AX:";
		strcpy_s(resultString, MAX_FORMULA_LEN, "_EX_");
		replaceInFormula(formula, 2, resultString);
		processFormula(ks, formula, priority);
	}
	else if (*formula != '\0' && isWorldLabel(ks, *formula)) {
		cout << "  label: ";
		//processFormula(ks, formula+1, priority);
		findWorldsThatSatifyLabel(ks, *formula, resultingWorldsList);
		convertWorldsListToString(ks, resultingWorldsList, resultingWorldsString);
		replaceInFormula(formula, 1, resultingWorldsString);
		processFormula(ks, formula, priority);
	}
	else if (*formula != '\0'&&*formula != '{') {
		cout << "Encountered error on " << formula << "!" << endl;
		return;
	}
	if (*formula != '{') {
		cout << formula << endl;
	}
}
int main() {
	kripkeStructure ks;
	char formula[100];
	readKripreStructure(ks);
	printKripkeStructure(ks);
	readFormula(formula);
	cout << formula << endl << endl;
	checkFormula(formula);
	processFormula(ks, formula, 2);
	if (*(strchr(formula, '}') + 1) != '\0') {
		cout << "Encountered error on " << (strchr(formula, '}') + 1) << "!" << endl;
	}

	cout << "\nResult: " << formula << endl;


	int x;
	cin >> x;
}