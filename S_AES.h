#include <bits/stdc++.h>
using namespace std;


/**
 * 明文与密钥均为一维数组
 * 通过计算来对应书中的矩阵结构
*/
// 定义替换盒
const int S_BOX[4][4] = {
	{9, 4, 10, 11},
	{13, 1, 8, 5},
	{6, 2, 0, 3},
	{12, 14, 15, 7}
};

// 定义逆替换盒
const int INV_S_BOX[4][4] = {
	{10, 5, 9, 11},
	{1, 7, 8, 15},
	{6, 0, 2, 3},
	{12, 4, 13, 14}
};

// 定义轮常数
int rcon[2][8] = {
	{1, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 1, 1, 0, 0, 0, 0}
};

//GF(2^4)上的加法表
const int GF_add[16][16] = {
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	{1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14},
	{2, 3, 0, 1, 6, 7, 4, 5, 10, 11, 8, 9, 14, 15, 12, 13},
	{3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12},
	{4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11},
	{5, 4, 7, 6, 1, 0, 3, 2, 13, 12, 15, 14, 9, 8, 11, 10},
	{6, 7, 4, 5, 2, 3, 0, 1, 14, 15, 12, 13, 10, 11, 8, 9},
	{7, 6, 5, 4, 3, 2, 1, 0, 15, 14, 13, 12, 11, 10, 9, 8},
	{8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7},
	{9, 8, 11, 10, 13, 12, 15, 14, 1, 0, 3, 2, 5, 4, 7, 6},
	{10, 11, 8, 9, 14, 15, 12, 13, 2, 3, 0, 1, 6, 7, 4, 5},
	{11, 10, 9, 8, 15, 14, 13, 12, 3, 2, 1, 0, 7, 6, 5, 4},
	{12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3},
	{13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2},
	{14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1},
	{15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}
};

//GF(2^4)上的乘法表
const int GF_multi[16][16] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
	{0, 2, 4, 6, 8, 10, 12, 14, 3, 1, 7, 5, 11, 9, 15, 13},
	{0, 3, 6, 5, 12, 15, 10, 9, 11, 8, 13, 14, 7, 4, 1, 2},
	{0, 4, 8, 12, 3, 7, 11, 15, 6, 2, 14, 10, 5, 1, 13, 9},
	{0, 5, 10, 15, 7, 2, 13, 8, 14, 11, 4, 1, 9, 12, 3, 6},
	{0, 6, 12, 10, 11, 13, 7, 1, 5, 3, 9, 15, 14, 8, 2, 4},
	{0, 7, 14, 9, 15, 8, 1, 6, 13, 10, 3, 4, 2, 5, 12, 11},
	{0, 8, 3, 11, 6, 14, 5, 13, 12, 4, 15, 7, 10, 2, 9, 1},
	{0, 9, 1, 8, 2, 11, 3, 10, 4, 13, 5, 12, 6, 15, 7, 14},
	{0, 10, 7, 13, 14, 4, 9, 3, 15, 5, 8, 2, 1, 11, 6, 12},
	{0, 11, 5, 14, 10, 1, 15, 4, 7, 12, 2, 9, 13, 6, 8, 3},
	{0, 12, 11, 7, 5, 9, 14, 2, 10, 6, 1, 13, 15, 3, 4, 8},
	{0, 13, 9, 4, 1, 12, 8, 5, 2, 15, 11, 6, 3, 14, 10, 7},
	{0, 14, 15, 1, 13, 3, 2, 12, 9, 7, 6, 8, 4, 10, 11, 5},
	{0, 15, 13, 2, 9, 6, 4, 11, 1, 14, 12, 3, 8, 7, 5, 10}
};


//数组binary下标begin~end的位置的二进制转十进制
int B_to_D(int *binary, int begin, int end) {
	int decimalValue = 0;
	int size = end - begin + 1;
	for (int i = 0; i < size; ++i) {
		decimalValue += binary[end - i] * std::pow(2, i);
	}
	return decimalValue;
}

//十进制转二进制，存储在数组binary下标begin~end的位置
void D_to_B(int decimal, int *binary, int begin, int end) {
	for (int i = end; i >= begin ; --i) {
		binary[i] = decimal % 2; // 取余
		decimal /= 2;             // 除以2
	}
}

//用于输出一维数组中指定下标的内容,mode 0为二进制输出，mode 1 为十六进制输出
void arrPrint(int *a, int begin, int end,bool mode) {
	if(!mode){
        for (int i = begin; i <= end; i++)
		cout << a[i] <<" ";
	cout << endl;
    }
    else{
        for (int i = begin; i <= end; i++){
            cout<<B_to_D(a,i,i+3)<<" ";
			i+=3;
        }
        cout << endl;
    }
    
}

// 轮密钥加函数
void addRoundKey(int *mingwen, int *key) {
	for (int i = 0; i < 16; i++)
		mingwen[i] ^= key[i];
}

// 半字节替代,对数组state[begin]到state[end]进行替换
void subBytes(int *state, int begin, int end) {
	//计算需要替换的次数
	int turn = (end - begin + 1) / 4;
	for (int i = 0; i < turn; i++) {
		int row = 2 * state[4 * i] + state[4 * i + 1];
		int col = 2 * state[4 * i + 2] + state[4 * i + 3];
		D_to_B(S_BOX[row][col], state, 4 * i, 4 * i + 3);
	}
}

// 半字节替代,对数组state[begin]到state[end]进行替换
void invSubBytes(int *state, int begin, int end) {
	//计算需要替换的次数
	int turn = (end - begin + 1) / 4;
	for (int i = 0; i < turn; i++) {
		int row = 2 * state[4 * i] + state[4 * i + 1];
		int col = 2 * state[4 * i + 2] + state[4 * i + 3];
		D_to_B(INV_S_BOX[row][col], state, 4 * i, 4 * i + 3);
	}
}

// 行移位
void shiftRows(int *state) {
	for (int i = 4; i < 8; i++) {
		int temp = state[i];
		state[i] = state[i + 8];
		state[i + 8] = temp;
	}
}

// 列混淆
void mixColumns(int *state) {
	//四块的十进制表示,左上，右上，左下，右下
	int d1 = B_to_D(state, 0, 3);
	int d2 = B_to_D(state, 8, 11);
	int d3 = B_to_D(state, 4, 7);
	int d4 = B_to_D(state, 12, 15);

	//计算四个块的混淆后的值
	int d1_ = GF_add[d1][GF_multi[4][d3]];
	int d2_ = GF_add[d2][GF_multi[4][d4]];
	int d3_ = GF_add[GF_multi[4][d1]][d3];
	int d4_ = GF_add[GF_multi[4][d2]][d4];

	//将混淆后的四块放回state
	D_to_B(d1_, state, 0, 3);
	D_to_B(d2_, state, 8, 11);
	D_to_B(d3_, state, 4, 7);
	D_to_B(d4_, state, 12, 15);

}

// 逆列混淆
void invMixColumns(int *state) {
	int d1 = B_to_D(state, 0, 3);
	int d2 = B_to_D(state, 8, 11);
	int d3 = B_to_D(state, 4, 7);
	int d4 = B_to_D(state, 12, 15);

	int d1_ = GF_add[GF_multi[9][d1]][GF_multi[2][d3]];
	int d2_ = GF_add[GF_multi[9][d2]][GF_multi[2][d4]];
	int d3_ = GF_add[GF_multi[2][d1]][GF_multi[9][d3]];
	int d4_ = GF_add[GF_multi[2][d2]][GF_multi[9][d4]];

	D_to_B(d1_, state, 0, 3);
	D_to_B(d2_, state, 8, 11);
	D_to_B(d3_, state, 4, 7);
	D_to_B(d4_, state, 12, 15);
}

const int ROUNDS = 3;//密钥轮数
//将16位的初始密钥扩展到48位
int **expandKey(int *key) {
	int **roundKeys = new int *[ROUNDS];
    for (int i = 0; i < ROUNDS; i++)
    {
        roundKeys[i] = new int[16];
    }
    
	int temp[8];
	// 初始密钥,并用temp复制后8bit

	for (int i = 0; i < 16; ++i) {
		roundKeys[0][i] = key[i];
		if (i > 7)
			temp[i - 8] = key[i];
	}
	// 密钥扩展

	for (int i = 1; i < ROUNDS; ++i) {
		// RotNib
		for (int j = 0; j < 4; j++) {
			int a = temp[j];
			temp[j] = temp[j + 4];
			temp[j + 4] = a;
		}

		// SubNib
		subBytes(temp, 0, 7);

		// RCON,并且生成轮密钥的前8bit与后8bit
		for (int j = 0; j < 8; j++) {
			roundKeys[i][j] = roundKeys[i - 1][j] ^ rcon[i - 1][j] ^ temp[j];
			roundKeys[i][j + 8] = temp[j] = roundKeys[i][j] ^ roundKeys[i - 1][j + 8];
		}
	}

	return roundKeys;
}


// 加密函数
void encrypt(int *state, int *key) {
	int **roundKeys = expandKey(key);
	
	addRoundKey(state, roundKeys[0]);

	subBytes(state, 0, 15);
	shiftRows(state);
	mixColumns(state);
	addRoundKey(state, roundKeys[1]);

	subBytes(state, 0, 15);
	shiftRows(state);
	addRoundKey(state, roundKeys[2]);
}

//解密函数
void decrypt(int *state, int *key) {
	int **roundKeys = expandKey(key);
	
	addRoundKey(state, roundKeys[2]);

	shiftRows(state);
	invSubBytes(state, 0, 15);
	addRoundKey(state, roundKeys[1]);
	invMixColumns(state);

	shiftRows(state);
	invSubBytes(state, 0, 15);
	addRoundKey(state, roundKeys[0]);
}