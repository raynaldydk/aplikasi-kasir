// Nama: Raynaldy Dwi Kharisma
// NIM : 2602172233

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

struct Menu{
	char nama_menu[50];
	float harga_regular;
	float harga_large;
} array_menu[50];

struct Order{
	char date[20];
	int index_order;
	char ukuran_order;
	int qty_order;
} array_order[50];

void load_menu();
int index_counter(char FILE_DIR[]);
void print_menu_txt();
void scan_order(int index_temp);
void rekap_order(int index_temp);
void ubah_order();
void print_receipt(char sub_input3, int index_temp);
void write_to_file(char sub_input3, int index_temp);

int main(){
	char input;
	char sub_input;
	char sub_input2;
	char sub_input3;
	int index_temp=0;
	printf("Aplikasi Kasir\n");
	printf("--------------\n");
	printf("1. Input Order\n");
	printf("2. Admin\n");
	printf("0. Exit\n");
	printf("--------------\n");
	printf("Input: "); scanf("%d", &input); getchar();
	switch(input){
		case 1:
			do{
				system("cls");
				load_menu();
				print_menu_txt();
				scan_order(index_temp);
				index_temp++;
				printf("Pesan lagi? (Y/N)\n"); 
				printf("Input: "); scanf("%c", &sub_input); getchar();
			}while(toupper(sub_input)!='N');
			do{
				system("cls");
				rekap_order(index_temp);
				printf("\n");
				printf("1. Ubah pesanan\n");
				printf("2. Checkout\n");
				printf("Input: "); scanf("%c", &sub_input2); getchar();
				if(sub_input2 == '1'){
					ubah_order();
				}
			}while(sub_input2!='2');
			printf("Pilih metode pembayaran\n");
			printf("1. Cash\n");
			printf("2. QRIS\n");
			printf("3. E-Wallet\n");
			printf("Input: "); scanf("%c", &sub_input3); getchar();
			system("cls");
			print_receipt(sub_input3, index_temp);
			write_to_file(sub_input3, index_temp);
			break;
		case 0:
			exit(0);
			break;
		default:
			printf("Input salah!\n");
			system("pause");
			system("cls");
			main();
	}
	return 0;
}

void load_menu(){
	char buffer[256];
	FILE *fp;
	fp = fopen("menu.txt", "r");
	if(fp == NULL){
        printf("File menu.txt tidak ditemukan!\n");
    }
	fgets(buffer, sizeof(buffer), fp);
	int index=0;
    while(!feof(fp)){
    	fscanf(fp, "%[^,], %f, %f\n", 
				&array_menu[index].nama_menu,
				&array_menu[index].harga_regular,
				&array_menu[index].harga_large);
		index++;
	}
	fclose(fp);
}

int index_counter(char FILE_DIR[]){
	FILE *fp;
    int count = 0;
    char c;
    fp = fopen(FILE_DIR, "r");
    if (fp == NULL){
        return 0;
    }
    for (c = getc(fp); c != EOF; c = getc(fp)){
    	if (c == '\n'){
    		count = count + 1;
		}
	}
    fclose(fp);
    return count;
}

void print_menu_txt(){
	int index = index_counter("menu.txt");
	printf("----------------------------------------------------------\n");
	printf("| No |           Nama         |  Regular   |    Large    |\n");
	printf("----------------------------------------------------------\n");
	for(int i=0; i<index; i++){
		printf("| %-2d | %-22s |  %.2f  |   %.2f  |\n",
				i+1,
				array_menu[i].nama_menu,
				array_menu[i].harga_regular,
				array_menu[i].harga_large);
	}
	printf("----------------------------------------------------------\n");
}

void scan_order(int index_temp){
	printf("Pesan: "); scanf("%d", &array_order[index_temp].index_order); getchar();
	printf("Ukuran (R/L): "); scanf("%c", &array_order[index_temp].ukuran_order); getchar();
	printf("Qty: "); scanf("%d", &array_order[index_temp].qty_order); getchar();
}

void rekap_order(int index_temp){
	float total=0;
	printf("----------------------------------------------------------------------\n");
	printf("| No |          Nama          | Ukuran |  Harga   | Qty  |   Total   |\n");
	printf("----------------------------------------------------------------------\n");
	for(int i=0; i<index_temp; i++){
		if(array_order[i].ukuran_order == 'R'){
			printf("| %-2d | %-22s |    %-3c | %.2f |  %-3d | %-9.2f |\n",
				i+1,
				array_menu[array_order[i].index_order-1].nama_menu, 
				array_order[i].ukuran_order,
				array_menu[array_order[i].index_order-1].harga_regular,
				array_order[i].qty_order, 
				(array_menu[array_order[i].index_order-1].harga_regular * array_order[i].qty_order));
				total+=(array_menu[array_order[i].index_order-1].harga_regular * array_order[i].qty_order);
		}
		else if(array_order[i].ukuran_order == 'L'){
			printf("| %-2d | %-22s |    %-3c | %.2f |  %-3d | %-9.2f |\n", 
				i+1,
				array_menu[array_order[i].index_order-1].nama_menu, 
				array_order[i].ukuran_order,
				array_menu[array_order[i].index_order-1].harga_large,
				array_order[i].qty_order, 
				(array_menu[array_order[i].index_order-1].harga_large * array_order[i].qty_order));
				total+=(array_menu[array_order[i].index_order-1].harga_large * array_order[i].qty_order);
		}
	}
	printf("----------------------------------------------------------------------\n");
	printf("\t\t\t\t\tTotal Pembayaran = %.2f", total);
}

void ubah_order(){
	int temp;
	printf("Pilih nomor pesanan yang ingin diubah!\n");
	printf("Input: "); scanf("%d", &temp); getchar();
	print_menu_txt();
	scan_order(temp-1);
}

void print_receipt(char sub_input3, int index_temp){
	printf("----------------------------------------------------------------------\n");
	printf("|\t\t\t\tIP Cafe 2\t\t\t     |\n");
	printf("----------------------------------------------------------------------\n");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("| %d/%02d/%02d\t\t", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	if(sub_input3 == '1'){
		printf("\t\t\t\t        Cash |\n");
	}
	else if(sub_input3 == '2'){
		printf("\t\t\t\tQRIS\n");
	}
	else if(sub_input3 == '3'){
		printf("\t\t\t\tE-Wallet\n");
	}
	printf("| %02d:%02d:%02d\t\t\t\t\t\t\t     |\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	rekap_order(index_temp);
}

void write_to_file(char sub_input3, int index_temp){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *fp;
	fp = fopen("sales.txt", "a");
	for(int i=0; i<index_temp; i++){
		if(toupper(array_order[i].ukuran_order) == 'R'){
			if(sub_input3 == '1'){
				fprintf(fp, "%d/%d/%d, %s, %c, %d, %.2f, Cash\n",
					tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
					array_menu[array_order[i].index_order].nama_menu,
					array_order[i].ukuran_order,
					array_order[i].qty_order,
					(array_menu[array_order[i].index_order].harga_regular * array_order[i].qty_order));
			}
			else if(sub_input3 =='2'){
				fprintf(fp, "%d/%d/%d, %s, %c, %d, %.2f, QRIS\n",
					tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
					array_menu[array_order[i].index_order].nama_menu,
					array_order[i].ukuran_order,
					array_order[i].qty_order,
					(array_menu[array_order[i].index_order].harga_regular * array_order[i].qty_order));
			}
			else if(sub_input3 =='3'){
				fprintf(fp, "%d/%d/%d, %s, %c, %d, %.2f, E-Wallet\n",
					tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
					array_menu[array_order[i].index_order].nama_menu,
					array_order[i].ukuran_order,
					array_order[i].qty_order,
					(array_menu[array_order[i].index_order].harga_regular * array_order[i].qty_order));
			}
		}
		else if(toupper(array_order[i].ukuran_order) == 'L'){
			if(sub_input3 == '1'){
				fprintf(fp, "%d/%d/%d, %s, %c, %d, %.2f, Cash\n",
					tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
					array_menu[array_order[i].index_order].nama_menu,
					array_order[i].ukuran_order,
					array_order[i].qty_order,
					(array_menu[array_order[i].index_order].harga_large * array_order[i].qty_order));
			}
			else if(sub_input3 =='2'){
				fprintf(fp, "%d/%d/%d, %s, %c, %d, %.2f, QRIS\n",
					tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
					array_menu[array_order[i].index_order].nama_menu,
					array_order[i].ukuran_order,
					array_order[i].qty_order,
					(array_menu[array_order[i].index_order].harga_large * array_order[i].qty_order));
			}
			else if(sub_input3 =='3'){
				fprintf(fp, "%d/%d/%d, %s, %c, %d, %.2f, E-Wallet\n",
					tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
					array_menu[array_order[i].index_order].nama_menu,
					array_order[i].ukuran_order,
					array_order[i].qty_order,
					(array_menu[array_order[i].index_order].harga_large * array_order[i].qty_order));
			}
		}
	}
	fclose(fp);
}
