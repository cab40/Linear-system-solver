
#include <stdio.h>
#include <stdlib.h>

#define MAX_SYSTEMS 100
#define MAX_CHARS 2000

typedef struct{
    int num;
    int den;
}FractionType;

typedef struct{
    FractionType F1;
    FractionType F2;
    FractionType F3;
    FractionType F4;
    FractionType F5;
    FractionType F6;
    char var1;
    char var2;
    char o1;
    char o2;
    FractionType Ans1;
    FractionType Ans2;
    int AnsType;
}linearSystemType;

int convert (char s, int mult) {
    return (s - '0') * mult;
}

void reduce(long int * x, long int * y) {
	int neg = 1; //for negative
	if (*x < 0){
		neg = neg * -1; 
		*x = *x * -1;
	}
	if (*y < 0){
		neg = neg * -1;
		*y = *y * -1;
	}
	int temp = *y; //might be a better method
	while(temp > 1) {
		if (*x % temp == 0 && *y % temp == 0) { //if both are divisable
			*x = *x / temp;
			*y = *y / temp;
			temp = *y; //sets temp to the new y value
		}
		else temp--;
	}
	*x = *x * neg; //if negative
}

void computeX(linearSystemType * ls){
    long int Ynum = ls->F2.num*ls->Ans2.num;
    long int Yden = ls->F2.den*ls->Ans2.den;
    reduce (&Ynum, &Yden);
    long int Num = ls->F1.den * (ls->F3.num*Yden - ls->F3.den*Ynum);
    long int Den = ls->F1.num*ls->F3.den*Yden;
    reduce(&Num, &Den);
    ls->Ans1.num = Num;
    ls->Ans1.den = Den;
}

void computeY(linearSystemType * ls){
    if(ls->o1== '-') ls->F2.num *= -1;
    if(ls->o2== '-') ls->F5.num *= -1;
    
    long int topNum = ls->F4.num * ls->F3.num;
    long int topDen = ls->F3.den*ls->F4.den;
    reduce(&topNum, &topDen);
    long int topNumTwo = ls->F1.num * ls->F6.num;
    long int topDenTwo = ls->F1.den*ls->F6.den;
    reduce(&topNumTwo, &topDenTwo);
    topNum = topNum * topDenTwo - topNumTwo * topDen;
    topDen *= topDenTwo;
    reduce(&topNum, &topDen);
    long int botNum = ls->F2.num * ls->F4.num;
    long int botDen = ls->F2.den * ls->F4.den;
    reduce(&botNum, &botDen);
    long int botNumTwo = ls->F1.num * ls->F5.num;
    long int botDenTwo = ls->F1.den * ls->F5.den;
    reduce(&botNumTwo, &botDenTwo);
    botNum = botNum * botDenTwo - botNumTwo * botDen;
    botDen *= botDenTwo;
    reduce(&botNum, &botDen);
    topNum = topNum * botDen;
    topDen = topDen * botNum;
    reduce(&topNum, &topDen);
    ls->Ans2.num = topNum;
    ls->Ans2.den = topDen;
    computeX(ls);
    if(ls->o1== '-') ls->F2.num *= -1;
    if(ls->o2== '-') ls->F5.num *= -1;
}

int parseFraction (char* s, int first, int last, FractionType * f, int i) {
    //returns 0 if invalid, 
    int n = first; //passed the first element of the fraction
    f->num = 0;
    f->den = 0;
    //set the base values of the fraction to zero
    if (s[n] == '0' && i == 1) return 0;//i is which fraction in the line, one for first and last, two for middle 
    //if there is a leading zero, it is invalid (return 0)
    if (s[n] >= '0' && s[n] <= '9') {
        if (s[n+1] >= '0' && s[n+1] <= '9') {
            f->num += convert (s[n], 10);
            n++;
        }
        f->num += convert (s[n], 1);
        n++;
    }else return 0;
    if (n > last) {
        f->den = 1;
        return 1;
    }
    if(s[n]!='/') return 0;
    n++;
    if (s[n] == '0') return 0;
    if (s[n] >= '0' && s[n] <= '9') {
        if (s[n+1] >= '0' && s[n+1] <= '9') {
            f->den += convert (s[n], 10);
            n++;
        }
        f->den += convert (s[n], 1);
    }else return 0;
    if ((last-first)>4) return 0;
    return 1;
}

int parseLinearSystem(char * s, linearSystemType * ls, int system){
    int n = 0;
    //n is the starting location of the string
    int first;
    //first will be used as the starting place of fractions
    int negative = 1;
    if (s[n] == '-') {
        negative = -1;
        n++;
    }
    first = n;
    //set the start of the first fraction to the current index
    while (s[n] < 'a' || s[n] > 'z') {
        n++;
    }
    //find the index of the variable
    if (system == 1){ //first line in the system
        if(!parseFraction(s, first, n-1, &(ls->F1), 1)) return 0;
    }
    if (system == 2){ //second line in the system
        if(!parseFraction(s, first, n-1, &(ls->F4), 1)) return 0;
    }
    //parse the fraction - pass through the start of the fraction and the end (variable-1)
    //if invalid parsefraction returns 0
    if (system == 1)ls->F1.num *= negative;
    if (system == 2)ls->F4.num *= negative;
    if (system == 1) {
        if (s[n] >= 'a' && s[n] <= 'z') ls->var1 = s[n];
        else return 0;
    }
    if (system == 2){
        if (s[n]!=ls->var1) return 0;
    }
    n++;
    if(s[n]!=' ') return 0;
    n++;
    if (s[n] == '+' || s[n] == '-'){
        if(system == 1) ls->o1 = s[n];
        else ls->o2 = s[n];
    }
    else return 0;
    n++;
    if(s[n]!=' ') return 0;
    n++;
    first = n;
    //set the start of the first fraction to the current index
    while (s[n] < 'a' || s[n] > 'z') {
        n++;
    }
    //find the index of the variable
    if (system == 1){
        if(!parseFraction(s, first, n-1, &(ls->F2), 2)) return 0;
    }
    if (system == 2){
        if(!parseFraction(s, first, n-1, &(ls->F5), 2)) return 0;
    }
    //parse the fraction - pass through the start of the fraction and the end (variable-1)
    if (system == 1){
        if (s[n] >= 'a' && s[n] <= 'z') ls->var2 = s[n];
        else return 0;
    }
    if (system == 2){
        if (s[n] != ls->var2) return 0;
    }
    n++;
    if(s[n]!=' ') return 0;
    n++;
    if(s[n]!='=') return 0;
    n++;
    if(s[n]!=' ') return 0;
    n++;
    if (s[n] == '-'){ 
        negative = -1;
        n++;
    }
    else negative = 1;
    first = n;
    //set the start of the first fraction to the current index
    while ((s[n+1] >= '0' && s[n+1] <= '9') || s[n+1] == '/') {
        n++;
    }
    //find the index of the variable
    if (system == 1){
        if(!parseFraction(s, first, n, &(ls->F3), 1)) return 0;
        ls->F3.num *= negative;
    }
    if (system == 2) {
        if(!parseFraction(s, first, n, &(ls->F6), 1)) return 0;
        ls->F6.num *= negative;
    }
    //parse the fraction - pass through the start of the fraction and the end (variable-1)
    if ((ls->var1)==(ls->var2)) return 0;
    return 1;
}

void output (linearSystemType * system, int i, FILE * file){
    fprintf (file, "%i", system[i].F1.num);
    if (system[i].F1.den != 1) fprintf (file, "/%i", system[i].F1.den);
    fprintf (file, "%c %c %i", system[i].var1, system[i].o1, system[i].F2.num);
    if (system[i].F2.den != 1) fprintf (file, "/%i", system[i].F2.den);
    fprintf (file, "%c = %i", system[i].var2, system[i].F3.num);
    if (system[i].F3.den != 1) fprintf (file, "/%i", system[i].F3.den);
    fprintf (file, "\n%i", system[i].F4.num);
    if (system[i].F4.den != 1) fprintf (file, "/%i", system[i].F4.den);
    fprintf (file, "%c %c %i", system[i].var1, system[i].o2, system[i].F5.num);
    if (system[i].F5.den != 1) fprintf (file, "/%i", system[i].F5.den);
    fprintf (file, "%c = %i", system[i].var2, system[i].F6.num);
    if (system[i].F6.den != 1) fprintf (file, "/%i", system[i].F6.den);
    fprintf (file, "\n");
    if(system[i].AnsType == 0){
        fprintf (file, "(%c, %c) = (%i", system[i].var1, system[i].var2, system[i].Ans1.num);
        if (system[i].Ans1.den != 1) fprintf (file, "/%i", system[i].Ans1.den);
        fprintf(file, ", %i", system[i].Ans2.num);
        if (system[i].Ans2.den != 1) fprintf (file, "/%i", system[i].Ans2.den);
        fprintf(file, ")\n\n");
    }
    else if(system[i].AnsType == 1) fprintf(file, "The lines are parallel\n\n");
    else fprintf(file, "The lines are the same\n\n");
}

int comparesFractions(FractionType * one, FractionType * two){
    double a = (double) one->num * two->den;
    double b = (double) two->num * one->den;
    if(a > b) return 1;
    //don't need to include epsilons since the values will be whole numbers
    if(a == b) return 0;
    return -1;
}

void swapSystems(linearSystemType * one, linearSystemType * two){
    linearSystemType temp = *one;
    *one = *two;
    *two = temp;
}

void systemType(linearSystemType * ls){
    ls->AnsType = 0;
    // int mNum1 = -1 * (ls->F1.num * ls->F2.den);
    // int mDen1 = ls->F2.num * ls->F1.den;
    // int mNum2 = -1 * (ls->F4.num * ls->F5.den);
    // int mDen2 = ls->F5.num * ls->F4.den;
    if (ls->Ans1.den == 0 && ls->Ans2.den == 0){
        ls->AnsType = 1;
        if (ls->Ans1.num == 0 && ls->Ans2.num == 0) ls->AnsType = 2;
    }

}

int main(){
    char systemList[200];
    linearSystemType systems[MAX_SYSTEMS];

    FILE* getFile;
    FILE* outFile;
    FILE* sortFile;

    getFile = fopen("linearSystems.txt", "r");
    outFile = fopen("solvedLinearSystems.txt", "w");

    //starting of code for getting systems
    int i = 1;
    int j = 0;
    printf ("Please wait while the program processes the file...\n\n");
    while((fgets(systemList, MAX_CHARS, getFile) != NULL)&&j<MAX_SYSTEMS){
        if(i%3!=0){
            if(!parseLinearSystem(systemList, &systems[j], i%3)){
                while (i%3 != 0){
                    i++;
                }
            }
        }
        else {
            if (systemList[0]=='\n'){
                computeY(&systems[j]);
                systemType(&systems[j]);
                output (systems, j, outFile);
                j++;
            }
        }
        i++;
    }
    printf ("The correct linear systems have been outputted to solvedLinearSystems.txt.\n Please wait while the answers are sorted...\n\n");
    for(int k = 0; k<j-1; k++){
        for(int l = 0; l<j-1;l++){
            if(comparesFractions(&systems[l].Ans1, &systems[l+1].Ans1) == 1) swapSystems(&systems[l], &systems[l+1]);
            else if(comparesFractions(&systems[l].Ans1, &systems[l+1].Ans1) == 0) if(comparesFractions(&systems[l].Ans2, &systems[l+1].Ans2) == 1) swapSystems(&systems[l], &systems[l+1]);
        }
    }
    fclose(outFile);
    sortFile = fopen("sortedSolvedLinearSystems.txt", "w");
    for(int m = 0; m<j; m++) {
        output (systems, m, sortFile);
    }
    printf ("The linear systems have been sorted and exported to sortedSolvedLinearSystems.txt.");
    fclose(getFile);
    fclose(sortFile);
}
