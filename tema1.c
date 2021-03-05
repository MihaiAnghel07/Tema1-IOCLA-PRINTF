#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

static int write_stdout(const char *token, int length)
{
	int rc;
	int bytes_written = 0;

	do {
		rc = write(1, token + bytes_written, length - bytes_written);
		if (rc < 0)
			return rc;

		bytes_written += rc;
	} while (bytes_written < length);

	return bytes_written;
}

//functia reverse intoarce rasturnatul unui numar intreg
static int reverse(int x)
{
	int rev = 0;

	while(x){
		rev = rev * 10 + x % 10;
		x = x / 10; 
	}

	return rev;
}

//functia strrev intoarce rasturnatul unui string
static char *strrev(char *s)
{
	char *p1, *p2;

	if(!s || ! *s)
		return s;

	for(p1 = s, p2 = s + strlen(s) - 1; p2 > p1; p1++, p2--){
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}

	return s;
}

//aceasta functie face adunare cu 1 a unui string ce reprezinta un numar binar
static void increment(char *str)
{
	char reminder = '0';

	if(str[31] == '0'){
		str[31] = '1';
		return;
	}

	if(str[31] == '1'){
		str[31] = '0';
		reminder = '1';
	}

	for(int i = 30; i >= 0; i--){
		
		if(str[i] == '1' && reminder == '1'){
			str[i] = '0';
			continue;
		}

		if(str[i] == '0' && reminder == '1'){
			str[i] = '1';
			reminder = '0';
			continue;
		}
	}
}


static char* convert_int_to_binary(int x)
{

	char *s = malloc(2000*sizeof(char));
	int cnt = 0;
	int negativ = 0;


	if(x == 0)
		return "0";


	if(x < 0){
		negativ = 1;
		x = -x;
	}


	while(x){
		s[cnt] = (x % 2) + '0';
		cnt++;
		x = x / 2;
	}


	s[cnt] = '\0';
	for(int i = cnt; i < 32; i++)
		s[i] = '0';
	

	s[32] = '\0';


	if(negativ == 1){
		for(int i = 0; i < 32; i++){
			if(s[i] == '0')
				s[i] = '1';
			else
				s[i] = '0';
		}


		increment(strrev(s));
		return s;
	}	
	
	return strrev(s);
}


static char* convert_unsignedint_to_string(unsigned int x)
{
	char *s2 = malloc(2000 * sizeof(char));
	int cnt = 0;
	
	if(x == 0)
		return "0";
	
	if(x < 0)
		x = reverse(x);

	
	while(x){
		s2[cnt] = (x % 10) + '0';
		cnt++;
		x = x / 10; 
	}

	s2[cnt] = '\0';

	return strrev(s2);
}


static char* convert_int_to_string(int x)
{
	char *s3 = malloc(2000 * sizeof(char));
	int cnt = 0, negativ = 0,min = 0;
	
	if(x == 0)
		return "0";


	if(x < 0){
		negativ = 1;
		if(x == -2147483648){
			x = x + 1;
			min = 1;
		}
		x = -x;
		
	}


	while(x){
		s3[cnt] = (x % 10) + '0';
		cnt++;
		x = x / 10; 
	}


	if(negativ == 1){
		s3[cnt] = '-';
		cnt++;
	}


	s3[cnt] = '\0';
	if(min == 1)
		s3[0] = '8';
	
	
	return strrev(s3);
}


static int convert_binary_to_int(char *s)
{
	int nr = 0, j = 1;
	for(int i = strlen(s) - 1; i >= 0; i--){
		nr += (s[i] - '0') * j;
		j *= 2; 
	}

	return nr;
}


static char* convert_binary_to_hexa(char *s)
{
	char *s4 = malloc(2000 * sizeof(char));
	char nibble[5], vect[16], c = 'a';
	int cnt = 0, i;


	for(i = 0; i < 16; i++)
		if(i < 10)
			vect[i] = i + '0';
		else
			vect[i] = c++; 


	for(i = 0; i < 31; i += 4){
		nibble[0] = s[i];
		nibble[1] = s[i + 1];
		nibble[2] = s[i + 2];
		nibble[3] = s[i + 3];
		nibble[4] = '\0';
		s4[cnt] = vect[convert_binary_to_int(nibble)];
		cnt++;
	}

	return s4;

}


static char* convert_int_to_hexa(int x)
{
	char *s5 = malloc(2000 * sizeof(char));
	int cnt = 0;
	char vect[16], c = 'a';


	if(x == 0)
		return "0";


	if(x < 0){
		char *s8 = convert_int_to_binary(x);
		return convert_binary_to_hexa(s8);
	}	


	for(int i = 0; i < 16; i++)
		if(i < 10)
			vect[i] = i + '0';
		else
			vect[i] = c++;

	
	while(x){
		s5[cnt] = vect[x % 16];
		cnt++;
		x = x / 16; 
	}
	s5[cnt] = '\0';

	
	return strrev(s5);
}

//functia min_pointer primeste un vector cu pointeri si returneaza pointerul minim
static char* min_pointer(char *pointer[], int cnt2)
{
	char *min = pointer[0];
	
	for(int i = 1; i < cnt2; i++){
		if(pointer[i] < min){
			min = pointer[i];
		}
	}

	return min;
}


int iocla_printf(const char *format, ...)
{
	int cnt = 0, chr = 0;
    char *aux = strdup(format), *q, *p;
    char aux2[2000] = "", str[2000] = "", *pointer[6];

    
    va_list args;
    va_start(args,format);

//aceasta este cazul in care primesc doar un specificator si 2 %
    if(strlen(aux) == 3 && aux[0] == aux[1] && aux[0] == '%'){
		q = aux + 1;
        write_stdout(q,2);
        return 2; 
    }

//am luat separat si cazul in care se primeste doar un flag de tipul: %d, %u ...
    if(strlen(aux) == 2){
        if(aux[1] == 'd'){
            int a = va_arg(args,int);
            q = convert_int_to_string(a);
            write_stdout(q,strlen(q));
            return strlen(q);
        }

        if(aux[1] == 'u'){
            unsigned int a = va_arg(args,unsigned int);
            q = convert_unsignedint_to_string(a);
            write_stdout(q,strlen(q));
            return strlen(q);
        }

        if(aux[1] == 'c'){
            char a[2];
			a[0] = va_arg(args,int);
            a[1] = '\0';
			write_stdout(a,1);
            return 1;
			
        }

        if(aux[1] == 'x'){
            int a = va_arg(args,int);
            q = convert_int_to_hexa(a);
            write_stdout(q,strlen(q));
            return strlen(q);
        }

        if(aux[1] == 's'){
            char *a = va_arg(args,char*);
            write_stdout(a,strlen(a));
            return strlen(a);
        }
    
	}

//in zona urmatoare am facut implementarea pentru restul cazurilor
	while(1){
		chr = 0;
		if(strstr(aux,"%d") || strstr(aux,"%u") || strstr(aux,"%c") 
							|| strstr(aux,"%s") || strstr(aux,"%x")){
		
			int cnt2 = 0;
			//salvez pointerii catre primele aparitii ale flag-uilor si prelucrez
			//incepand de la pointer-ul minim
			if(strstr(aux,"%d")){
				pointer[cnt2] = strstr(aux,"%d");
				cnt2++;
			}
			if(strstr(aux,"%u")){
				pointer[cnt2] = strstr(aux,"%u");
				cnt2++;
			}
			if(strstr(aux,"%c")){
				pointer[cnt2] = strstr(aux,"%c");
				cnt2++;
			}
			if(strstr(aux,"%x")){
				pointer[cnt2] = strstr(aux,"%x");
				cnt2++;
			}
			if(strstr(aux,"%s")){
				pointer[cnt2] = strstr(aux,"%s");
				cnt2++;
			}
			
			p = min_pointer(pointer,cnt2);
		
		}else{break;} 
		//in functie de specificator, apelez functiile de conversie 
		//si afisez informatia								
			if(p[1] =='d'){
				int a = va_arg(args,int);
				q = convert_int_to_string(a);
				goto afisare;
			}


			if(p[1] =='u'){
				unsigned int a = va_arg(args,unsigned int);
		 		q = convert_unsignedint_to_string(a);
				goto afisare;
			}


			if(p[1] == 'c'){
            	char a[2];
				a[0] = va_arg(args,int);
            	a[1] = '\0';
				write_stdout(a,1);
				chr = 1;
            	goto afisare;
			}


			if(p[1] =='x'){
				int a = va_arg(args,int);
				q = convert_int_to_hexa(a);
				goto afisare;
			}


			if(p[1] =='s'){
				q = va_arg(args,char*);
				goto afisare;
			}
		break;
		
afisare:
//actualizez sirul din care extrag informatia
		strcpy(aux2,aux);
		aux = p + 2;
		aux2[strlen(aux2) - strlen(p)] = '\0';
//daca nu primesc o variabila de tip caracter de la input afisez.
//altfel afisez direct in zona in care prelucrez informatia primita
		if(chr == 0){
			write_stdout(aux2,strlen(aux2));
			write_stdout(q,strlen(q));
			cnt += strlen(q);
			cnt += strlen(aux2);
		}
 	}
//in cazul in care dupa ultimul specificator de tip mai am un text
//se face afisarea acestuia
 	write_stdout(aux,strlen(aux));
	cnt += strlen(aux);
    va_end(args);


	return cnt;
}
