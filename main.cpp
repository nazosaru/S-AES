#include "S_AES.h"
#include <string>
#include <vector>
using namespace std;

//16-bit加密
void test1() {
	int *mingWen = new int [16];
	int *key = new int [16];
	cout << "请输入16bit明文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> mingWen[i];
	}
	// arrPrint(mingWen,0,15);
	cout << "请输入16bit密钥:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	//调用加密函数
	encrypt(mingWen, key);
	cout << "加密后的密文是： " << endl;
	arrPrint(mingWen, 0, 15, 0);

}

//16-bit解密
void test2() {
	int *miWen = new int [16];
	int *key = new int [16];
	cout << "请输入16bit密文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> miWen[i];
	}
	// arrPrint(miWen,0,15);
	cout << "请输入16bit密钥:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	//调用解密函数
	decrypt(miWen, key);
	cout << "解密后的明文是： " << endl;
	arrPrint(miWen, 0, 15, 0);

}

void changeTObinary(string a, int p[][16]);// 将 ASCII 字符串转换为二维整数数组
int changeTOasc(int c[], int begin, int end);// 将二维整数数组转换为 ASCII 字符串
void ascen(string plaintext, int key[]);//字符串加密接口
void ascde(int encrypted[][16], int key[], int size);//字符串解密接口

//字符串加密
void test3() {
	string plaintext;
	int key[16];
	cout << "请输入明文字符串：";
	cin >> plaintext;
	cout << "请输入16bit密钥:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	// 加密
	ascen(plaintext, key);
}

//字符串解密
void test4() {
	int key[16], size;
	cout << "请输入需要解密的字符串的长度：";
	cin >> size;
	size = size / 2;//每个字符转换为8bit二进制，每两个字符为16bit进行加密
	int encrypted[size][16];//用于存储密文，每16bit为一行
	cout << "请输入二进制密文：" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 16; j++)
			cin >> encrypted[i][j];
	}
	cout << "请输入16bit密钥:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	// 解密
	ascde(encrypted, key, size);
}

//16-bit双重加密
void test5() {
	int plaintext[16], key[2][16];
	cout << "请输入16bit明文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> plaintext[i];
	}
	cout << "请输入32bit密钥:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//双重加密k1->k2
	encrypt(plaintext, key[0]);
	encrypt(plaintext, key[1]);
	cout << "加密后的密文是： " << endl;
	arrPrint(plaintext, 0, 15, 0);
}

//16-bit双重解密
void test6() {
	int ciphertext[16], key[2][16];
	cout << "请输入16bit密文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> ciphertext[i];
	}
	cout << "请输入32bit密钥:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//双重解密k2->k1
	decrypt(ciphertext, key[1]);
	decrypt(ciphertext, key[0]);
	cout << "解密后的明文是： " << endl;
	arrPrint(ciphertext, 0, 15, 0);
}

void ctb(int decimal, int binary[]);//十进制转换成二进制
int ctd(int binary[]);//二进制转换成十进制
void MitM(int plaintext[], int ciphertext[]);//中间相遇算法接口

//16-bit双重加密，中间相遇暴力破解算法
void test7() {
	int plaintext[16], ciphertext[16];
	cout << "请输入16bit明文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> plaintext[i];
	}
	cout << "请输入16bit密文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> ciphertext[i];
	}
	//调用中间相遇函数
	MitM(plaintext, ciphertext);
}

//16-bit三重加密
void test8() {
	int plaintext[16], key[2][16];
	cout << "请输入16bit明文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> plaintext[i];
	}
	cout << "请输入32bit密钥:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//三重加密k1-k2-k1
	encrypt(plaintext, key[0]);
	encrypt(plaintext, key[1]);
	encrypt(plaintext, key[0]);
	cout << "加密后的密文是： " << endl;
	arrPrint(plaintext, 0, 15, 0);
}

//16-bit三重解密
void test9() {
	int ciphertext[16], key[2][16];
	cout << "请输入16bit密文:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> ciphertext[i];
	}
	cout << "请输入32bit密钥:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//三重解密k1->k2->k1
	decrypt(ciphertext, key[0]);
	decrypt(ciphertext, key[1]);
	decrypt(ciphertext, key[0]);
	cout << "解密后的明文是： " << endl;
	arrPrint(ciphertext, 0, 15, 0);
}

//CBC加密
void test10() {
	int size, iv[16], key[16];
	cout << "请输入明文的bit长度(16的倍数):";
	cin >> size;
	size = size / 16;
	int plain[size][16];
	cout << "请输入明文：" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 16; j++)
			cin >> plain[i][j];
	}
	cout << "请输入16bit密钥：";
	for (int i = 0; i < 16; i++)
		cin >> key[i];
	cout << "请输入16bit初始向量IV：";
	for (int i = 0; i < 16; i++)
		cin >> iv[i];
	//加密过程
	for (int i = 0; i < size; i++) {
		//IV与Pi按位异或
		for (int j = 0; j < 16; j++) {
			plain[i][j] ^= iv[j];
		}
		encrypt(plain[i], key);
		//Ci成为新的IV
		for (int x = 0; x < 16; x++)
			iv[x] = plain[i][x];
		cout << "C" << i + 1 << "=";
		arrPrint(plain[i], 0, 15, 0);
	}
}

//CBC解密
void test11() {
	int size, iv[16], key[16], tempc[16];
	cout << "请输入密文的bit长度(16的倍数):";
	cin >> size;
	size = size / 16;
	int cipher[size][16];
	cout << "请输入密文：" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 16; j++)
			cin >> cipher[i][j];
	}
	cout << "请输入16bit密钥：";
	for (int i = 0; i < 16; i++)
		cin >> key[i];
	cout << "请输入16bit初始向量IV：";
	for (int i = 0; i < 16; i++)
		cin >> iv[i];
	//解密过程
	for (int i = 0; i < size; i++) {
		//拷贝一份密文
		for (int x = 0; x < 16; x++)
			tempc[x] = cipher[i][x];
		decrypt(cipher[i], key);
		//Ci与IV按位异或
		for (int j = 0; j < 16; j++) {
			cipher[i][j] ^= iv[j];
		}
		//解密前的Ci成为新的IV
		for (int y = 0; y < 16; y++)
			iv[y] = tempc[y];
		cout << "P" << i + 1 << "=";
		arrPrint(cipher[i], 0, 15, 0);
	}
}

int main () {
	int control;
	cout << "16-bit加/解密请输入1/2,字符串加/解密请输入3/4，16-bit双重加/解密请输入5/6" << endl <<
	     "中间相遇攻击请输入7，16-bit三重加/解密请输入8/9，CBC加/解密请输入10/11:"
	     << endl;
	cin >> control;
	switch (control) {
		case 1:
			test1();
			break;
		case 2:
			test2();
			break;
		case 3:
			test3();
			break;
		case 4:
			test4();
			break;
		case 5:
			test5();
			break;
		case 6:
			test6();
			break;
		case 7:
			test7();
			break;
		case 8:
			test8();
			break;
		case 9:
			test9();
			break;
		case 10:
			test10();
			break;
		case 11:
			test11();
			break;
		default:
			break;
	}

	return 0;
}

// 将 ASCII 字符串转换为二维整数数组
void changeTObinary(string a, int p[][16]) {
	for (int i = 0; i < a.size(); i += 2) {//每个字符转换为8bit二进制，每两个字符为16bit进行加密
		int temp1 = a[i];
		int temp2 = a[i + 1];
		for (int j = 0; j < 8; j++) {
			if (temp1 == 0)
				p[i / 2][7 - j] = 0;
			else {
				p[i / 2][7 - j] = temp1 % 2;
				temp1 = temp1 / 2;
			}
			if (temp2 == 0)
				p[i / 2][15 - j] = 0;
			else {
				p[i / 2][15 - j] = temp2 % 2;
				temp2 = temp2 / 2;
			}
		}
	}
}

// 将二维整数数组转换为 ASCII 字符串
int changeTOasc(int c[], int begin, int end) {
	int sum = 0;
	int temp = 128;
	for (int i = begin; i <= end; i++) {
		sum = sum + c[i] * temp;
		temp = temp / 2;
	}
	return sum;
}

// 字符串加密接口
void ascen(string plaintext, int key[]) {
	int size = plaintext.size() / 2;//每个字符转换为8bit二进制，每两个字符为16bit进行加密
	int plain[size][16];
	changeTObinary(plaintext, plain);//将字符串转换为二进制
	cout << "加密后的密文是： " << endl;
	for (int i = 0; i < size; i++) {
		encrypt(plain[i], key);
		arrPrint(plain[i], 0, 7, 0);
		arrPrint(plain[i], 8, 15, 0);
	}
}

// 字符串解密接口
void ascde(int encrypted[][16], int key[], int size) {
	cout << "明文字符串为：";
	for (int i = 0; i < size; i++) {
		decrypt(encrypted[i], key);
		char a = changeTOasc(encrypted[i], 0, 7);//将每8bit明文转换为十进制整型，再作为字符输出
		char b = changeTOasc(encrypted[i], 8, 15);
		cout << a << b;
	}
}

//十进制转换成二进制
void ctb(int decimal, int binary[]) {
	for (int i = 0; i < 16; i++) {
		binary[15 - i] = decimal % 2;//从低位开始
		decimal = decimal / 2;
	}
}

//二进制转换成十进制
int ctd(int binary[]) {
	int decimal = 0;
	int temp = 65535;
	for (int i = 0; i < 16; i++) {
		decimal += binary[i] * temp;
		temp = temp / 2;
	}
	return decimal;
}

//Meet-in-the-Middle中间相遇算法接口
void MitM(int plaintext[], int ciphertext[]) {
	int count = 0;//用于计数有多少对key
	std::vector<int> mid;//用于存储正向加密得到的中间密文
	//正向加密
	for (int i = 0; i < 65536; i++) {
		int key1[16], tempp[16];
		//拷贝一个临时明文，防止原始明文被更改
		for (int x = 0; x < 16; x++) {
			tempp[x] = plaintext[x];
		}
		//计算出key
		ctb(i, key1);
		encrypt(tempp, key1);
		//把加密得到的中间结果以十进制的形式存入数组
		mid.push_back(ctd(tempp));
	}
	//逆向解密，并比对中间结果
	for (int i = 0; i < 65536; i++) {
		int key2[16], tempc[16];
		//拷贝一个临时密文，防止原始密文被更改
		for (int x = 0; x < 16; x++) {
			tempc[x] = ciphertext[x];
		}
		//计算出key2
		ctb(i, key2);
		decrypt(tempc, key2);
		//比对中间结果，并输出key
		for (int j = 0; j < 65536; j++) {
			int key1[16];
			if (ctd(tempc) == mid[j]) {
				count++;
				ctb(j, key1);
				cout << "count=" << count << " i=" << i << " j=" << j << endl;
				cout << "key1=";
				arrPrint(key1, 0, 15, 0);
				cout << "key2=";
				arrPrint(key2, 0, 15, 0);
				cout << endl;
			}
		}
	}
}