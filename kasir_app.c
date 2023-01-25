// Nama: Raynaldy Dwi Kharisma
// NIM : 2602172233

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

const char *FORMAT_DATA_READ = "%d/%d/%d, %[^,], %c, %d, %d, %s\n";

struct Sales{
	int date[4];
	char nama[50];
	char size;
	int qty;
	int total_pembayaran;
	char metode_pembayaran[20];
} array_sales[100], temp;

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

void input_order();
void load_menu();
int index_counter(char FILE_DIR[]);
void print_menu_txt();
void scan_order(int index_temp);
void rekap_order(int index_temp);
void ubah_order();
void print_receipt(char sub_input3, int index_temp);
void write_to_file(char sub_input3, int index_temp);
void menu_admin();
void sort_sales();
void search_sales();
void print_sales();

int main(){
	char input;
	printf("Aplikasi Kasir\n");
	printf("--------------\n");
	printf("1. Input Order\n");
	printf("2. Admin\n");
	printf("0. Exit\n");
	printf("--------------\n");
	printf("Input: "); scanf("%d", &input); getchar();
	switch(input){
		case 1:
			input_order();
			break;
		case 2:
			menu_admin();
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
	printf("---------------------------------------------------\n");
	printf("| No |           Nama         | Regular |  Large  |\n");
	printf("---------------------------------------------------\n");
	for(int i=0; i<index; i++){
		printf("| %-2d | %-22s |  %.0f  |  %.0f  |\n",
				i+1,
				array_menu[i].nama_menu,
				array_menu[i].harga_regular,
				array_menu[i].harga_large);
	}
	printf("---------------------------------------------------\n");
}

void scan_order(int index_temp){
	printf("Pesan (No Produk): "); scanf("%d", &array_order[index_temp].index_order); getchar();
	printf("Ukuran (R/L): "); scanf("%c", &array_order[index_temp].ukuran_order); getchar();
	printf("Qty: "); scanf("%d", &array_order[index_temp].qty_order); getchar();
}

void rekap_order(int index_temp){
	float total=0;
	printf("---------------------------------------------------------------\n");
	printf("| No |          Nama          | Ukuran | Harga | Qty | Total  |\n");
	printf("---------------------------------------------------------------\n");
	for(int i=0; i<index_temp; i++){
		if(array_order[i].ukuran_order == 'R'){
			printf("| %-2d | %-22s |    %-3c | %.0f |  %-2d | %-6.0f |\n",
				i+1,
				array_menu[array_order[i].index_order-1].nama_menu, 
				array_order[i].ukuran_order,
				array_menu[array_order[i].index_order-1].harga_regular,
				array_order[i].qty_order, 
				(array_menu[array_order[i].index_order-1].harga_regular * array_order[i].qty_order));
				total+=(array_menu[array_order[i].index_order-1].harga_regular * array_order[i].qty_order);
		}
		else if(array_order[i].ukuran_order == 'L'){
			printf("| %-2d | %-22s |    %-3c | %.0f |  %-2d | %-6.0f |\n",
				i+1,
				array_menu[array_order[i].index_order-1].nama_menu, 
				array_order[i].ukuran_order,
				array_menu[array_order[i].index_order-1].harga_large,
				array_order[i].qty_order, 
				(array_menu[array_order[i].index_order-1].harga_large * array_order[i].qty_order));
				total+=(array_menu[array_order[i].index_order-1].harga_large * array_order[i].qty_order);
		}
	}
	printf("---------------------------------------------------------------\n");
	printf("\t\t\t\t    Total Pembayaran = %.0f\n", total);
}

void ubah_order(){
	int temp;
	printf("Pilih nomor pesanan yang ingin diubah!\n");
	printf("Input: "); scanf("%d", &temp); getchar();
	print_menu_txt();
	scan_order(temp-1);
}

void print_receipt(char sub_input3, int index_temp){
	printf("---------------------------------------------------------------\n");
	printf("|\t\t\tIP Cafe 2\t\t\t      |\n");
	printf("---------------------------------------------------------------\n");
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("| %d/%02d/%02d\t\t", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	if(sub_input3 == '1'){
		printf("\t\t\t\t Cash |\n");
	}
	else if(sub_input3 == '2'){
		printf("\t\t\t\t QRIS |\n");
	}
	else if(sub_input3 == '3'){
		printf("\t\t\t     E-Wallet |\n");
	}
	printf("| %02d:%02d:%02d\t\t\t\t\t\t      |\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
	rekap_order(index_temp);
}

void write_to_file(char sub_input3, int index_temp){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	FILE *fp;
	fp = fopen("sales.txt", "a");
	for(int i=0; i<index_temp; i++){
		if(toupper(array_order[i].ukuran_order) == 'R' && sub_input3 == '1'){
			fprintf(fp, "%d/%d/%d, %s, %c, %d, %.0f, Cash\n",
				tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
				array_menu[array_order[i].index_order-1].nama_menu,
				array_order[i].ukuran_order,
				array_order[i].qty_order,
				(array_menu[array_order[i].index_order-1].harga_regular * array_order[i].qty_order));
		}
		else if(toupper(array_order[i].ukuran_order) == 'R' && sub_input3 =='2'){
			fprintf(fp, "%d/%d/%d, %s, %c, %d, %.0f, QRIS\n",
				tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
				array_menu[array_order[i].index_order-1].nama_menu,
				array_order[i].ukuran_order,
				array_order[i].qty_order,
				(array_menu[array_order[i].index_order-1].harga_regular * array_order[i].qty_order));
		}
		else if(toupper(array_order[i].ukuran_order) == 'R' && sub_input3 =='3'){
			fprintf(fp, "%d/%d/%d, %s, %c, %d, %.0f, E-Wallet\n",
				tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
				array_menu[array_order[i].index_order-1].nama_menu,
				array_order[i].ukuran_order,
				array_order[i].qty_order,
				(array_menu[array_order[i].index_order-1].harga_regular * array_order[i].qty_order));
		}
		else if(toupper(array_order[i].ukuran_order) == 'L' && sub_input3 == '1'){
			fprintf(fp, "%d/%d/%d, %s, %c, %d, %.0f, Cash\n",
				tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
				array_menu[array_order[i].index_order-1].nama_menu,
				array_order[i].ukuran_order,
				array_order[i].qty_order,
				(array_menu[array_order[i].index_order-1].harga_large * array_order[i].qty_order));
		}
		else if(toupper(array_order[i].ukuran_order) == 'L' && sub_input3 =='2'){
			fprintf(fp, "%d/%d/%d, %s, %c, %d, %.0f, QRIS\n",
				tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
				array_menu[array_order[i].index_order-1].nama_menu,
				array_order[i].ukuran_order,
				array_order[i].qty_order,
				(array_menu[array_order[i].index_order-1].harga_large * array_order[i].qty_order));
		}
		else if(toupper(array_order[i].ukuran_order) == 'L' && sub_input3 =='3'){
			fprintf(fp, "%d/%d/%d, %s, %c, %d, %.0f, E-Wallet\n",
				tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
				array_menu[array_order[i].index_order-1].nama_menu,
				array_order[i].ukuran_order,
				array_order[i].qty_order,
				(array_menu[array_order[i].index_order-1].harga_large * array_order[i].qty_order));
		}
	}
	fclose(fp);
}

void input_order(){
	char sub_input;
	char sub_input2;
	char sub_input3;
	int index_temp=0;
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
	system("pause");
	system("cls");
	main();
}

void print_array_sales(int i){
	printf("| %-2d | %d/%d/%d | %-22s |  %-3c | %-3d | %-6d | %-8s |\n", 
		i+1,
		array_sales[i].date[0],
		array_sales[i].date[1],
		array_sales[i].date[2],
		array_sales[i].nama,
		array_sales[i].size,
		array_sales[i].qty,
		array_sales[i].total_pembayaran,
		array_sales[i].metode_pembayaran);
}

void initialize_data(){
	FILE *fp;
	int file_index = index_counter("sales.txt");
	fp = fopen("sales.txt", "r");
	for(int i=0; i<file_index; i++){
		fscanf(fp, FORMAT_DATA_READ, 
			&array_sales[i].date[0],
			&array_sales[i].date[1],
			&array_sales[i].date[2],
			&array_sales[i].nama,
			&array_sales[i].size,
			&array_sales[i].qty,
			&array_sales[i].total_pembayaran,
			&array_sales[i].metode_pembayaran);
	}
	fclose(fp);
}

void view_sales(){
	int file_index = index_counter("sales.txt");
	initialize_data();
	system("cls");
	print_sales();
}

void print_sales(){
	int file_index = index_counter("sales.txt");
	printf("----------------------------------------------------------------------------\n");
	printf("| No |  Tanggal  |           Nama         | Size | Qty | Total  |  Metode  |\n");
	printf("----------------------------------------------------------------------------\n");
	for(int i=0; i<file_index; i++){
		print_array_sales(i);
	}
	printf("----------------------------------------------------------------------------\n");
}

typedef int (*compfn)(const void*, const void*);

int compare_nama(struct Sales *elem1, struct Sales *elem2){
	if(strcasecmp(elem1->nama, elem2->nama) < 0){
		return -1;
	}
	else if(strcasecmp(elem1->nama, elem2->nama) > 0){
		return 1;
	}
	return 0;	
}

int compare_qty(struct Sales *elem1, struct Sales *elem2){
	if(elem1->qty < elem2->qty){
		return -1;
	}
	else if(elem1->qty > elem2->qty){
		return 1;
	}
	return 0;
}

int compare_total(struct Sales *elem1, struct Sales *elem2){
	if(elem1->total_pembayaran < elem2->total_pembayaran){
		return -1;
	}
	else if(elem1->total_pembayaran > elem2->total_pembayaran){
		return 1;
	}
	return 0;
}

void sort_sales(){
	int file_index = index_counter("sales.txt");
	int sub_input;
	initialize_data();
	system("cls");
	printf("Sort berdasarkan:\n");
	printf("-----------------\n");
	printf("1. Nama Produk\n");
	printf("2. Qty\n");
	printf("3. Total\n");
	printf("0. Back\n");
	printf("-----------------\n");
	printf("Input: "); scanf("%d", &sub_input); getchar();
	switch(sub_input){
		case 1:
			qsort((void *) &array_sales, file_index, sizeof(struct Sales), (compfn)compare_nama);
			system("cls");
			printf("Sort Berdasarkan Nama Produk\n");
			print_sales();
			system("pause");
			sort_sales();
			break;
		case 2:
			qsort((void *) &array_sales, file_index, sizeof(struct Sales), (compfn)compare_qty);
			system("cls");
			printf("Sort Berdasarkan Qty Produk Terjual\n");
			print_sales();
			system("pause");
			sort_sales();
			break;
		case 3:
			qsort((void *) &array_sales, file_index, sizeof(struct Sales), (compfn)compare_total);
			system("cls");
			printf("Sort Berdasarkan Nama Produk\n");
			print_sales();
			system("pause");
			sort_sales();
			break;
		case 0:
			menu_admin();
			break;
		default:
			printf("Input salah!\n");
			system("pause");
			sort_sales();
			break;
	}
}

void search_sales(){
	int file_index = index_counter("sales.txt");
	int sub_input;
	char key[25];
	int found;
	int high, low;
	initialize_data();
	system("cls");
	printf("Search berdasarkan:\n");
	printf("-----------------\n");
	printf("1. Nama Produk\n");
	printf("2. Qty\n");
	printf("3. Total Pembayaran\n");
	printf("4. Metode Pembayaran\n");
	printf("0. Back\n");
	printf("-----------------\n");
	printf("Input: "); scanf("%d", &sub_input); getchar();
	switch(sub_input){
		case 1:
			system("cls");
            printf("Masukkan nama produk yang akan dicari : ");
            scanf("%s", key);
            getchar();
            found=0;
            for(int i=0; i<file_index; i++){
            	if(strstr(array_sales[i].nama, key) != NULL){
            		found = 1;
				}
			}
			if(found==0){
                printf("Data tidak ditemukan\n");
                system("pause");
                search_sales();
            }
            else{
            	printf("----------------------------------------------------------------------------\n");
				printf("| No |  Tanggal  |           Nama         | Size | Qty | Total  |  Metode  |\n");
				printf("----------------------------------------------------------------------------\n");
	            for(int i=0; i<file_index; i++){
	            	if(strstr(array_sales[i].nama, key) != NULL){
	            		print_array_sales(i);
					}
				}
				printf("----------------------------------------------------------------------------\n");
            }
            system("pause");
            search_sales();
			break;
		case 2:
			system("cls");
            printf("Masukkan qty terendah : ");
            scanf("%d", &low);
            printf("Masukkan qty tertinggi : ");
            scanf("%d", &high);
            for(int i = 0; i < file_index; i++){
                if(array_sales[i].qty >= low && array_sales[i].qty <= high){
                	found=1;
				};
            }
            if (found == 0){
                printf("Data tidak ditemukan\n");
                system("pause");
                search_sales();
            }
            else{
            	printf("----------------------------------------------------------------------------\n");
				printf("| No |  Tanggal  |           Nama         | Size | Qty | Total  |  Metode  |\n");
				printf("----------------------------------------------------------------------------\n");
                for(int i = 0; i < file_index; i++){
                    if(array_sales[i].qty >= low && array_sales[i].qty <= high){
                        print_array_sales(i);
                    }
                }
                printf("----------------------------------------------------------------------------\n");
            }
			break;
		case 3:
			system("cls");
            printf("Masukkan pembayaran terendah : ");
            scanf("%d", &low);
            printf("Masukkan pembayaran tertinggi : ");
            scanf("%d", &high);
            for(int i = 0; i < file_index; i++){
                if(array_sales[i].total_pembayaran >= low && array_sales[i].total_pembayaran <= high){
                	found=1;
				};
            }
            if (found == 0){
                printf("Data tidak ditemukan\n");
                system("pause");
                search_sales();
            }
            else{
            	printf("----------------------------------------------------------------------------\n");
				printf("| No |  Tanggal  |           Nama         | Size | Qty | Total  |  Metode  |\n");
				printf("----------------------------------------------------------------------------\n");
                for(int i = 0; i < file_index; i++){
                    if(array_sales[i].total_pembayaran >= low && array_sales[i].total_pembayaran <= high){
                        print_array_sales(i);
                    }
                }
                printf("----------------------------------------------------------------------------\n");
            }
			break;
		case 4:
			system("cls");
            printf("Masukkan metode pembayaran yang akan dicari : ");
            scanf("%s", key);
            getchar();
            found=0;
            for(int i=0; i<file_index; i++){
            	if(strstr(array_sales[i].metode_pembayaran, key) != NULL){
            		found = 1;
				}
			}
			if(found==0){
                printf("Data tidak ditemukan\n");
                system("pause");
                search_sales();
            }
            else{
            	printf("----------------------------------------------------------------------------\n");
				printf("| No |  Tanggal  |           Nama         | Size | Qty | Total  |  Metode  |\n");
				printf("----------------------------------------------------------------------------\n");
	            for(int i=0; i<file_index; i++){
	            	if(strstr(array_sales[i].metode_pembayaran, key) != NULL){
	            		print_array_sales(i);
					}
				}
				printf("----------------------------------------------------------------------------\n");
            }
            system("pause");
            search_sales();
			break;
		case 0:
			menu_admin();
			break;
		default:
			printf("Input salah!\n");
			system("pause");
			sort_sales();
			break; 
	}
}

void menu_admin(){
	char sub_input;
	system("cls");
	printf("Menu Admin\n");
	printf("--------------\n");
	printf("1. View Sales\n");
	printf("2. Sort Sales\n");
	printf("3. Search Sales\n");
	printf("0. Back\n");
	printf("--------------\n");
	printf("Input: "); scanf("%d", &sub_input); getchar();
	switch(sub_input){
		case 1:
			view_sales();
			system("pause");
			menu_admin();
			break;
		case 2:
			sort_sales();
			break;
		case 3:
			search_sales();
			break;
		case 0:
			system("cls");
			main();
			break;
		default:
			printf("Input salah!\n");
			system("pause");
			menu_admin();
			break;
	}
}
