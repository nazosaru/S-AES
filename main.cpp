#include "S_AES.h"
#include <string>
#include <vector>
using namespace std;

//16-bit����
void test1() {
	int *mingWen = new int [16];
	int *key = new int [16];
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> mingWen[i];
	}
	// arrPrint(mingWen,0,15);
	cout << "������16bit��Կ:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	//���ü��ܺ���
	encrypt(mingWen, key);
	cout << "���ܺ�������ǣ� " << endl;
	arrPrint(mingWen, 0, 15, 0);

}

//16-bit����
void test2() {
	int *miWen = new int [16];
	int *key = new int [16];
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> miWen[i];
	}
	// arrPrint(miWen,0,15);
	cout << "������16bit��Կ:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	//���ý��ܺ���
	decrypt(miWen, key);
	cout << "���ܺ�������ǣ� " << endl;
	arrPrint(miWen, 0, 15, 0);

}

void changeTObinary(string a, int p[][16]);// �� ASCII �ַ���ת��Ϊ��ά��������
int changeTOasc(int c[], int begin, int end);// ����ά��������ת��Ϊ ASCII �ַ���
void ascen(string plaintext, int key[]);//�ַ������ܽӿ�
void ascde(int encrypted[][16], int key[], int size);//�ַ������ܽӿ�

//�ַ�������
void test3() {
	string plaintext;
	int key[16];
	cout << "�����������ַ�����";
	cin >> plaintext;
	cout << "������16bit��Կ:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	// ����
	ascen(plaintext, key);
}

//�ַ�������
void test4() {
	int key[16], size;
	cout << "��������Ҫ���ܵ��ַ����ĳ��ȣ�";
	cin >> size;
	size = size / 2;//ÿ���ַ�ת��Ϊ8bit�����ƣ�ÿ�����ַ�Ϊ16bit���м���
	int encrypted[size][16];//���ڴ洢���ģ�ÿ16bitΪһ��
	cout << "��������������ģ�" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 16; j++)
			cin >> encrypted[i][j];
	}
	cout << "������16bit��Կ:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> key[i];
	}
	// ����
	ascde(encrypted, key, size);
}

//16-bit˫�ؼ���
void test5() {
	int plaintext[16], key[2][16];
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> plaintext[i];
	}
	cout << "������32bit��Կ:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//˫�ؼ���k1->k2
	encrypt(plaintext, key[0]);
	encrypt(plaintext, key[1]);
	cout << "���ܺ�������ǣ� " << endl;
	arrPrint(plaintext, 0, 15, 0);
}

//16-bit˫�ؽ���
void test6() {
	int ciphertext[16], key[2][16];
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> ciphertext[i];
	}
	cout << "������32bit��Կ:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//˫�ؽ���k2->k1
	decrypt(ciphertext, key[1]);
	decrypt(ciphertext, key[0]);
	cout << "���ܺ�������ǣ� " << endl;
	arrPrint(ciphertext, 0, 15, 0);
}

void ctb(int decimal, int binary[]);//ʮ����ת���ɶ�����
int ctd(int binary[]);//������ת����ʮ����
void MitM(int plaintext[], int ciphertext[]);//�м������㷨�ӿ�

//16-bit˫�ؼ��ܣ��м����������ƽ��㷨
void test7() {
	int plaintext[16], ciphertext[16];
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> plaintext[i];
	}
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> ciphertext[i];
	}
	//�����м���������
	MitM(plaintext, ciphertext);
}

//16-bit���ؼ���
void test8() {
	int plaintext[16], key[2][16];
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> plaintext[i];
	}
	cout << "������32bit��Կ:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//���ؼ���k1-k2-k1
	encrypt(plaintext, key[0]);
	encrypt(plaintext, key[1]);
	encrypt(plaintext, key[0]);
	cout << "���ܺ�������ǣ� " << endl;
	arrPrint(plaintext, 0, 15, 0);
}

//16-bit���ؽ���
void test9() {
	int ciphertext[16], key[2][16];
	cout << "������16bit����:" << endl;
	for (int i = 0; i < 16; i++) {
		cin >> ciphertext[i];
	}
	cout << "������32bit��Կ:" << endl;
	for (int i = 0; i < 2 ; i++) {
		for (int j = 0; j < 16; j++)
			cin >> key[i][j];
	}
	//���ؽ���k1->k2->k1
	decrypt(ciphertext, key[0]);
	decrypt(ciphertext, key[1]);
	decrypt(ciphertext, key[0]);
	cout << "���ܺ�������ǣ� " << endl;
	arrPrint(ciphertext, 0, 15, 0);
}

//CBC����
void test10() {
	int size, iv[16], key[16];
	cout << "���������ĵ�bit����(16�ı���):";
	cin >> size;
	size = size / 16;
	int plain[size][16];
	cout << "���������ģ�" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 16; j++)
			cin >> plain[i][j];
	}
	cout << "������16bit��Կ��";
	for (int i = 0; i < 16; i++)
		cin >> key[i];
	cout << "������16bit��ʼ����IV��";
	for (int i = 0; i < 16; i++)
		cin >> iv[i];
	//���ܹ���
	for (int i = 0; i < size; i++) {
		//IV��Pi��λ���
		for (int j = 0; j < 16; j++) {
			plain[i][j] ^= iv[j];
		}
		encrypt(plain[i], key);
		//Ci��Ϊ�µ�IV
		for (int x = 0; x < 16; x++)
			iv[x] = plain[i][x];
		cout << "C" << i + 1 << "=";
		arrPrint(plain[i], 0, 15, 0);
	}
}

//CBC����
void test11() {
	int size, iv[16], key[16], tempc[16];
	cout << "���������ĵ�bit����(16�ı���):";
	cin >> size;
	size = size / 16;
	int cipher[size][16];
	cout << "���������ģ�" << endl;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 16; j++)
			cin >> cipher[i][j];
	}
	cout << "������16bit��Կ��";
	for (int i = 0; i < 16; i++)
		cin >> key[i];
	cout << "������16bit��ʼ����IV��";
	for (int i = 0; i < 16; i++)
		cin >> iv[i];
	//���ܹ���
	for (int i = 0; i < size; i++) {
		//����һ������
		for (int x = 0; x < 16; x++)
			tempc[x] = cipher[i][x];
		decrypt(cipher[i], key);
		//Ci��IV��λ���
		for (int j = 0; j < 16; j++) {
			cipher[i][j] ^= iv[j];
		}
		//����ǰ��Ci��Ϊ�µ�IV
		for (int y = 0; y < 16; y++)
			iv[y] = tempc[y];
		cout << "P" << i + 1 << "=";
		arrPrint(cipher[i], 0, 15, 0);
	}
}

int main () {
	int control;
	cout << "16-bit��/����������1/2,�ַ�����/����������3/4��16-bit˫�ؼ�/����������5/6" << endl <<
	     "�м���������������7��16-bit���ؼ�/����������8/9��CBC��/����������10/11:"
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

// �� ASCII �ַ���ת��Ϊ��ά��������
void changeTObinary(string a, int p[][16]) {
	for (int i = 0; i < a.size(); i += 2) {//ÿ���ַ�ת��Ϊ8bit�����ƣ�ÿ�����ַ�Ϊ16bit���м���
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

// ����ά��������ת��Ϊ ASCII �ַ���
int changeTOasc(int c[], int begin, int end) {
	int sum = 0;
	int temp = 128;
	for (int i = begin; i <= end; i++) {
		sum = sum + c[i] * temp;
		temp = temp / 2;
	}
	return sum;
}

// �ַ������ܽӿ�
void ascen(string plaintext, int key[]) {
	int size = plaintext.size() / 2;//ÿ���ַ�ת��Ϊ8bit�����ƣ�ÿ�����ַ�Ϊ16bit���м���
	int plain[size][16];
	changeTObinary(plaintext, plain);//���ַ���ת��Ϊ������
	cout << "���ܺ�������ǣ� " << endl;
	for (int i = 0; i < size; i++) {
		encrypt(plain[i], key);
		arrPrint(plain[i], 0, 7, 0);
		arrPrint(plain[i], 8, 15, 0);
	}
}

// �ַ������ܽӿ�
void ascde(int encrypted[][16], int key[], int size) {
	cout << "�����ַ���Ϊ��";
	for (int i = 0; i < size; i++) {
		decrypt(encrypted[i], key);
		char a = changeTOasc(encrypted[i], 0, 7);//��ÿ8bit����ת��Ϊʮ�������ͣ�����Ϊ�ַ����
		char b = changeTOasc(encrypted[i], 8, 15);
		cout << a << b;
	}
}

//ʮ����ת���ɶ�����
void ctb(int decimal, int binary[]) {
	for (int i = 0; i < 16; i++) {
		binary[15 - i] = decimal % 2;//�ӵ�λ��ʼ
		decimal = decimal / 2;
	}
}

//������ת����ʮ����
int ctd(int binary[]) {
	int decimal = 0;
	int temp = 65535;
	for (int i = 0; i < 16; i++) {
		decimal += binary[i] * temp;
		temp = temp / 2;
	}
	return decimal;
}

//Meet-in-the-Middle�м������㷨�ӿ�
void MitM(int plaintext[], int ciphertext[]) {
	int count = 0;//���ڼ����ж��ٶ�key
	std::vector<int> mid;//���ڴ洢������ܵõ����м�����
	//�������
	for (int i = 0; i < 65536; i++) {
		int key1[16], tempp[16];
		//����һ����ʱ���ģ���ֹԭʼ���ı�����
		for (int x = 0; x < 16; x++) {
			tempp[x] = plaintext[x];
		}
		//�����key
		ctb(i, key1);
		encrypt(tempp, key1);
		//�Ѽ��ܵõ����м�����ʮ���Ƶ���ʽ��������
		mid.push_back(ctd(tempp));
	}
	//������ܣ����ȶ��м���
	for (int i = 0; i < 65536; i++) {
		int key2[16], tempc[16];
		//����һ����ʱ���ģ���ֹԭʼ���ı�����
		for (int x = 0; x < 16; x++) {
			tempc[x] = ciphertext[x];
		}
		//�����key2
		ctb(i, key2);
		decrypt(tempc, key2);
		//�ȶ��м����������key
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