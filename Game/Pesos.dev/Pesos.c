#include <bits/stdc++.h>

int power(double n, int i){
	int j = 0;
	for(j=0;j<i;j++){
		n = n*n;
	}
	return n;
}

int main(void){
	double din = 10, tempo = 50, pts = 3, conh = 3;
	double atdin = 10, attempo, atpts, atconh, sumconh;
	for(int i = 0; i < 66; i++){
		atdin = (i+1) * 3;
		attempo = pow(1.5, i) * tempo;
		if(i%2 == 1){
			atpts = pow(1.2, i) * pts;
			atconh = pow(1.16, i) * conh;
		}else{
			atpts = pow(1.25, i) * pts;
			atconh = pow(1.13, i) * conh;
		}
		sumconh += atconh;
		printf("%.0lf %.0lf %.0lf %.0lf\n", atdin, atpts, atconh, attempo);
	}


printf("\n\n %lf", sumconh);

}



/*
Disciplinas pares
Pontos = 1.15^n
Conhecimento = 1.2^n
Disciplinas ímpares
Pontos = 1.18^n
Conhecimento = 1.16^n

Dificuldade = 1.4^n (tempo)
Dinheiro = 1.05 * n;

dinheiro pontos conhecimento tempo
*/
