#include<iostream>
#include<fstream>
#include<cstring>
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
		while(true) {
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
	int i,j;
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
			cout << ks.worldsRelations[i][j]<<" ";
		}
		cout << endl;
	}
	cout << endl;
}
void operatorPre(kripkeStructure ks,int currentWorld,bool previousWorlds[100]) {
	int i, j;
	for (i = 0; i < ks.worldsCount; i++) {
		if (ks.worldsRelations[i][currentWorld] == true) {
			previousWorlds[i] = true;
		}
	}
}
int main() {
	kripkeStructure ks;
	readKripreStructure(ks);
	printKripkeStructure(ks);
	

	bool previousWrolds[100] = {false,false,false,false};
	operatorPre(ks, 1, previousWrolds);
	for (int i = 0; i < ks.worldsCount; i++) {
		cout << previousWrolds[i]<<" ";
	}
	
	int x;
	cin >> x;
}