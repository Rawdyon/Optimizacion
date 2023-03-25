#include <stdio.h>
#include <stdlib.h>


int m,n;

typedef struct _M_{
	double a;
	double b;
}M;
/*
typedef struct _matriz_{
  int n,m;
  M a[m][n];
  int restric[m];
  double capacidades[m]
}MATRIZ;
*/
/*
int leeM(MATRIZ *a,enum tipoCoeficiente tipo,FILE *f);
int escribeM(MATRIZ X, enum tipoCoeficiente tipo,FILE *f);
MATRIZ sumaM(MATRIZ X, MATRIZ Y,void *oper, enum tipoCoeficiente tipo,FILE *f);
MATRIZ creaEspacioMatriz(int m, int n);
int liberaEspacioMatriz(MATRIZ *mat);
*/

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
int compara(M a, M b);
int imprime(M a);

int main(int argc, char *argv[]) {
	int nvar,nrest;
	int opc;
	int i,j, masNega,menosPos,contador =0,contador1 =0,contador2=0,contador3=0,k;
	double aux;
	FILE *archivo,*salida;
	//MATRIZ T;
	//Leer si es de maximización o minimización
	//Si es de Max sea aux=1 y si es de Min sea aux=-1
	//Al leer los coeficientes de costo de la función Z hay que multiplicarlos por aux
	
	archivo = fopen("Entrada.txt","r");
	salida = fopen("Resultado.txt","w");
	
	fscanf(archivo,"%d",&opc); // 0 SI ES MAXIMIZAR 1 SI ES MINIMIZAR
	fscanf(archivo,"%d",&m);
	fscanf(archivo,"%d",&n);
	k=n;
	
	printf("%d %d\n",m,n);
	masNega = menosPos = -1;
	M costosAux[n+1];//La primer entrada corresponde a la solución
	double restricciones[m]; //el 3 representará Menor o igual, el 1 representará Igual y el 2 representará Mayor o igual
	double relaciones[m]; 
	double matrizAux[m][n+1]; //Al CREAR la matriz el primer renglón corresponderá a la función objetivo 
	
	for(j=0;j<n;j++){ //lee coeficientes de costos
		fscanf(archivo,"%lg",&costosAux[j].a);
		costosAux[j].b = 0;
	}
	
	restricciones[0] = 0; //corresponde a la igualdad de la función objetivo
	costosAux[n].a = 0; //corresponde a la solución
	costosAux[n].b = 0;
	
	
	if(opc== 0){ // SI SE QUIERE MAXIMIZAR SE LLEVA A CABO LA RESTA, SI SE QUIERE MINIMIZAR PRIMERO SE MULTIPLICA POR -1 Y LUEGO SE HACE LA RESTA 
		for(j=0;j<n;j++){ //POR LO QUE LOS COEFICIENTES SE QUEDAN CON EL MIMSO SIGNO
			costosAux[j].a = costosAux[j].a*-1;
		}
	}
	
	
	
	for(i=0;i<m;i++){
		for(j=0;j<n;j++)
			fscanf(archivo,"%lg",&matrizAux[i][j]);
		fscanf(archivo,"%lg",&restricciones[i]);
		fscanf(archivo,"%lg",&matrizAux[i][n]);
	}
	
	//Cuenta las variables de holgura y las artificiales necesarias
	for(i=0;i<m;i++){
		if(restricciones[i] == 3)	//Se cuentan variables de holgura
			contador++;
		else if(restricciones[i]==2) //Se cuentan variables de holgura negativas y la variable artificial
			contador1++;
		else if( restricciones[i] == 1)
			contador3++;
	}
	
	printf("c1 : %d, c2: %d c3: %d\n",contador,contador1,contador3);
	//Crea la matriz con las entradas necesarias -----------------------------------------DECLARACION--------------------------------------------
	double matriz[m][n +1+contador + 2*contador1+ contador3];
	M costos[n +1+contador + 2*contador1+contador3];
	int indices[m];
	
	////system("Pause");
	
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){//Copia la matriz original hasta la n-1 entrada
			matriz[i][j] = matrizAux[i][j];
			costos[j].a = costosAux[j].a;
			costos[j].b = costosAux[j].b;
		}
		for(;j < n +contador + 2*contador1 +contador3;j++){ //Rellena el resto de la matriz de ceros
			if(j < n +contador + 2*contador1 +contador3 ){
				matriz[i][j] = 0;
			}
		}
		matriz[i][j] = matrizAux[i][n];
	}
	for(j=n;j < n +contador + 2*contador1 +contador3;j++){//Rellena el resto de la matriz de ceros
		if(j < n +contador + 2*contador1 +contador3 ){
			costos[j].a = 0; 
			costos[j].b = 0;
		}
	}
	costos[j].a = 0; //Estos dos corresponden a la solución
	costos[j].b = 0;
	
	printf("Matriz copiada \n");
	//system("Pause");
	contador2=0;
	for(i=0;i<m;i++){
		if(restricciones[i] == 3){ //Se añaden variables de holgura
			matriz[i][n + contador2 ] = 1;
			indices[i] = n+contador2;
			contador2++; }
		else if(restricciones[i]==2){ //Se añaden variables de holgura negativas y la variable artificial
			matriz[i][n + contador2 ] = -1;
			matriz[i][n + contador2 +1] = 1; //ESTA ES LA M
			if(opc== 0)
				costos[n + contador2 +1].b = 1; //La M se pone sumando en la función objetivo DESPEJADA
			else 
				costos[n + contador2 +1].b = -1;
			//Eliminar la variable artificial de la función objetivo DESPEJADA   ---------------------CHECK POINT 2 12:47 -----------------------------------------
			indices[i] = n+contador2+1;
			contador2+=2;
		}else if( restricciones[i] == 1){
			matriz[i][n+contador2 ] = -1;
			if(opc==0)
				costos[n+contador2].b = 1;
			else 
				costos[n+contador2].b =- 1;
			//Eliminar la variable artificial de la función original
			indices[i] = n+contador2;
			contador2++;
		}
	}
	printf("\n la tabla sin despejar es: \n");
	for(i=0;i<n+contador + 2*contador1+contador3;i++){
		printf("   x%d     ",i);
	}
	printf("sol     \n");
	for(i=0;i<n+1+ contador + 2*contador1+contador3;i++)
		printf("  %lg+%lgM   ",costos[i].a,costos[i].b);
	printf("\n");
	for(i=0;i<m;i++){
		for(j=0;j<n+1+contador + 2*contador1+contador3;j++){
			printf("%9lg",matriz[i][j]);
		}
		printf("\n");
	}
	
	//Eliminar la variable artificial de la función original   ---------------------CHECK POINT 2 12:47 -----------------------------------------
	for(i=0;i<m;i++){
		if(restricciones[i] == 3)
			contador2++; 
		else if(restricciones[i]==2){ //Se añaden variables de holgura negativas y la variable artificial
			for(j=0;j<n+1 +contador + 2*contador1 +contador3;j++){
				if(opc== 0)
					costos[j].b =costos[j].b - matriz[i][j]; 
				else 
					costos[j].b = costos[j].b + matriz[i][j];
			}
			contador2+=2;	
		}else{//Eliminar la variable artificial de la función original
			for(j=0;j<n+1 +contador + 2*contador1 +contador3;j++){
				if(opc== 0)
					costos[j].b =costos[j].b - matriz[i][j]; 
				else 
					costos[j].b = costos[j].b + matriz[i][j];
			}
			contador2++;
		}
	}
	
	//Imprime la tabla óptima
	for(i=0;i<n+contador + 2*contador1+contador3;i++){
		printf("   x%d     ",i);
	}
	printf("sol     \n");
	for(i=0;i<n+1+contador + 2*contador1+contador3;i++)
		printf("  %lg+%lgM   ",costos[i].a,costos[i].b);
	printf("\n");
	for(i=0;i<m;i++){
		for(j=0;j<n+1+contador + 2*contador1+contador3;j++){
			printf("%9lg",matriz[i][j]);
		}
		printf("\n");
	}
	
	//Ahora se aplica el simplex (Todo esto irá en un DO WHILE
	n = n+ contador + 2*contador1 +contador3;
	M cero;
	cero.a = 0;
	cero.b = 0;
	do{
		//Primero se verifica el criterio de factibilidad
		contador=0;
		masNega = -1;
		for(j=0;j<n;j++){
			if(compara(costos[j],cero)){  //Si se viola el criterio de optimalidad escoje la más negativa 
				if(masNega!=-1){
					//if(matriz[1][j] < matriz[1][masNega])
					if( compara(costos[j] , costos[masNega])  )
						masNega=j;
				}
				else masNega=j; //Si es la primer negativa que encuentra
				contador++;//Cuenta los coeficientes no negativos
			}
		}
		if(contador==0){ //Usar un contador para esto
				printf("La tabla óptima es: \n");
				break;
		}
		printf("\nVerificado el criterio de optimalidad");
		/*if(contador == 0){ //Si todos son positivos entonces la solución es óptima
			//printf("La tabla óptima es:\n");
			//break;//return 0;
		}*/
		contador = 0;
		
		//Calculamos las relaciones, recordemos que la primer entrada de las costos corresponde a la solución
		for(i=0;i<m;i++){
			if( matriz[i][masNega] != 0){
				relaciones[i] = matriz[i][n]/matriz[i][masNega]; //calcula las relaciones
				printf("\n %d) Dividiendo %lg / %lg = %lg",i,matriz[i][n],matriz[i][masNega],relaciones[i]);
			}
			else relaciones[i] = -1;  //si una entrada de la matriz es cero está será ignorada con un valor negativo
		}
		printf("\n Calculadas las relaciones ");
			//Hacemos un proceso similar al mas negativo pero ahora para el menos positivo
			// Es decir, se checa el criterio de factibilidad
		menosPos = -1;
		for(i=0;i<m;i++){
			if(relaciones[i] >= 0){  //Si es menor que cero se ignora 
				if(menosPos !=-1){
					printf("\n Comparando %lg con %lg",relaciones[i], relaciones[menosPos]);
					if(relaciones[i] < relaciones[menosPos])
						menosPos=i;
				}
				else menosPos=i; //Si es la primer negativa que encuentra
				contador++; //Aumenta el contador de costos no negativas
			}
		}
		indices[menosPos] = masNega;
		if( contador == 0 ){ //Usar un contador para esto
			fprintf(salida,"%d",2);
			fprintf(salida,"La solución es no acotada");
			printf("No hay solución factible");
			break;
		}
		
		printf("\n Verificado el criterio de factiblidad \n");
		
		//El pivote es la entrada matriz[masNega][menosPos]
		//print("Entra");
		aux = matriz[menosPos][masNega];
		printf(" El pivote esta en la entrada [%d,%d] el coeficiente es %lg \n",menosPos,masNega,matriz[menosPos][masNega]);
		
		printf("\n Normalizando la entrada ");
		//system("PAUSE");
		for(j=0;j<n+1;j++)
			matriz[menosPos][j] =matriz[menosPos][j]/aux;
		for(j=0;j<n+1;j++)
			printf(" %3lg ",matriz[menosPos][j]);
		printf("\n");
		for(i=0;i<n+1;i++){
			printf("   x%d     ",i);
		}
		printf("sol     \n");
		for(i=0;i<n+1;i++)
			printf("  %lg+%lgM   ",costos[i].a,costos[i].b);
		printf("\n");
		for(i=0;i<m;i++){
			for(j=0;j<n+1;j++){
				printf("%9lg",matriz[i][j]);
			}
			printf("\n");
		}
		printf("\n Pivoteando la tabla simplex");
		//system("PAUSE");
		
		double aux2,aux3;
		aux2 = costos[masNega].a;
		aux3 = costos[masNega].b;
		printf("La entrada a eliminar es: %lg + %lgM",costos[masNega].a,costos[masNega].b);
		//system("PAUSE");
		for(j=0;j<n+1;j++){
			costos[j].a = costos[j].a - aux2*matriz[menosPos][j]; //------------------------CHECK POINT -------------------------------------
			costos[j].b = costos[j].b - aux3*matriz[menosPos][j];
			imprime(costos[j]);
		}
		
		printf("\n");
		for(i=0;i<m;i++){
			aux2 = matriz[i][masNega];
			if(i != menosPos){
				for(j=0;j<n+1;j++){
					matriz[i][j] = matriz[i][j] - matriz[menosPos][j]*aux2;
					//printf("[%d,%d] : %3lg  ",i,j,matriz[i][j]);
				}
				//for(j=0;j<n;j++)
					//printf("[%d,%d] : %3lg  ",i,j,matriz[i][j]);
				//printf("");
			} 
			printf("\n");
		}
		/*for(j=0;j<n+1;j++)
			printf(" %3lg ",matriz[menosPos][j]);
		//system("PAUSE");*/
		
		contador=0;
		for(i=0; i<n+1;i++){
			if( compara(costos[i],cero)/*matriz[1][i] < 0*/){
				contador++;
				break;
			}
		}
		if(contador == 0){
			printf("La solución es óptima");
			break;
		}
		
		
		//Imprime la tabla 
		for(i=0;i<n+1;i++){
			printf("   x%d     ",i);
		}
		printf("sol     \n");
		for(i=0;i<n+1;i++)
			printf("  %lg+%lgM   ",costos[i].a,costos[i].b);
		printf("\n");
		for(i=0;i<m;i++){
			for(j=0;j<n+1;j++){
				printf("%9lg",matriz[i][j]);
			}
			printf("\n");
		}
		/*printf("A\n");
		for(i=0;i<n;i++){
			printf("x%d   ",i);
		}
		printf("sol    rel   \n");
		for(i=0;i<m;i++){
			for(j=0;j<n;j++){
				printf("%5lg",matriz[i][j]);
			}
			printf("%lg",relaciones[i]);
			printf("\n");
		}*/
	}while(contador!=0);
	//Con este criterio se realiza el despeje del primer pivote
	
	//Imprime la tabla óptima
	printf("\n");
	for(i=0, contador2=0;i<m;i++){
		if(restricciones[i] == 3){ 
			contador2++; }
		else if(restricciones[i]==2){ 
			if(indices[i] == k+contador2+1){
				fprintf(salida,"%d",3);
				//fprintf(salida, "Hay una variable artificial en la base");
				printf("\n Hay una variable artificial en la base ");
				return 0;
			}
			contador2+=2;
		}else if( restricciones[i] == 1){
		if(	indices[i] == k+contador2){
			fprintf(salida,"%d",3);
			//fprintf(salida,"Hay una variable artificial en la base");
			printf("\n Hay una variable artificial en la base ");
			return 0;
		}
			contador2++;
		}
	}
	
	for(i=0;i<n;i++){
			printf("   x%d     ",i);
		}
		printf("sol     \n");
		for(i=0;i<n+1;i++)
			printf("   %lg+%lgM    ",costos[i].a,costos[i].b);
		printf("\n");
		for(i=0;i<m;i++){
			for(j=0;j<n+1;j++){
				printf("%11lg",matriz[i][j]);
			}
			printf("\n");
		}
	contador2=0;
	
	
	printf("\n La solución es: \n");
	fprintf(salida,"%d \n",1);
	fprintf(salida, "%f \n",costos[n].a);
	for(i=0;i<m;i++){
		fprintf(salida,"%d %lg \n",indices[i], matriz[i][n] );
		printf("x%d = %lg \n", indices[i], matriz[i][n]);
	}
	system("pause");
	return 0;
}



int compara(M a, M b){
	
	if(b.b == 0 && a.b == 0){
		if(a.a < b.a)
			return 1;
		else return 0;
	}
	else if( b.b == 0 && a.b != 0){
		if( a.b < 0)
			return 1;
		else 
			return 0;	
	}else if(b.b != 0 && a.b == 0){
		if( b.b < 0)
			return 0;
		else
			return 0;
	}
	else{
		if(a.b < b.b )
			return 1;
		else 
			return 0;
	}
	
	return 0;
}

int imprime(M a){
	if (a.a == 0){
		printf("%8lg",a.a);
	}
	else if(a.a >0)
		printf(" %3lg+%3lgM",a.a,a.b);
	else 
		printf(" %3lg%3lgM",a.a,a.b);
}

