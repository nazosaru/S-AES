> 目录
- [编程与测试要求](#编程与测试要求)
- [开发手册](#开发手册)
  - [算法说明](#算法说明) 
  - [接口说明](#接口说明)
- [用户指南](#用户指南)
  - [概述](#概述)
  - [操作指南](#操作指南)

# 编程与测试要求  
- 第一关-基本测试
根据S-AES算法编写和调试程序，提供GUI解密支持用户交互。输入可以是16bit的数据和16bit的密钥，输出是16bit的密文。

- 第二关-交叉测试 
考虑到是"算法标准"，所有人在编写程序的时候需要使用相同算法流程和转换单元(替换盒、列混淆矩阵等)，以保证算法和程序在异构的系统或平台上都可以正常运行。
设有A和B两组位同学(选择相同的密钥K)；则A、B组同学编写的程序对明文P进行加密得到相同的密文C；或者B组同学接收到A组程序加密的密文C，使用B组程序进行解密可得到与A相同的P。

- 第三关-扩展功能  
考虑到向实用性扩展，加密算法的数据输入可以是ASII编码字符串(分组为2 Bytes)，对应地输出也可以是ACII字符串(很可能是乱码)。

- 第四关-多重加密 
3.4.1 双重加密
将S-AES算法通过双重加密进行扩展，分组长度仍然是16 bits，但密钥长度为32 bits。
3.4.2 中间相遇攻击
假设你找到了使用相同密钥的明、密文对(一个或多个)，请尝试使用中间相遇攻击的方法找到正确的密钥Key(K1+K2)。
3.4.3 三重加密
将S-AES算法通过三重加密进行扩展，下面两种模式选择一种完成：
(1)按照32 bits密钥Key(K1+K2)的模式进行三重加密解密，
(2)使用48bits(K1+K2+K3)的模式进行三重加解密。

- 第五关-工作模式  
基于S-AES算法，使用密码分组链(CBC)模式对较长的明文消息进行加密。注意初始向量(16 bits) 的生成，并需要加解密双方共享。
在CBC模式下进行加密，并尝试对密文分组进行替换或修改，然后进行解密，请对比篡改密文前后的解密结果。

# 开发手册
## 算法说明
- 初始置换（IP）

  使用初始置换表（IP）对输入的8位明文进行置换，得到置换后的8位序列。

- 密钥生成

  P10 置换：对 10 位密钥进行 P10（置换 10）操作，改变密钥的位序。

  左移操作：将置换后的密钥分成两半，对每半进行左移（循环左移 1 位）。

  P8 置换：对左移后的结果进行 P8（置换 8）操作，得到第一个子密钥 K1。

  再次左移：对两半密钥进行左移（循环左移 2 位）。

  P8 置换：对结果进行 P8 操作，得到第二个子密钥 K2。

- 拓展置换（EP-Box）

  对右半部分8位密文进行拓展置换，扩展为12位。 子密钥加轮: 将拓展置换后的12位结果与k1进行异或运算，得到12位结果。

- S盒置换

  将上一步结果分为两组，分别进行S盒置换。

- 压缩置换

  对S盒置换后的结果进行压缩置换，得到4位结果。

- 直接置换（SP-Box）

  对压缩置换后的4位结果进行直接置换。

- 最终置换（IP^-1）

  将上述结果与初始置换后的4位结果进行异或运算，得到8位加密结果。

- 解密过程

  S-DES算法加解密统一，解密与加密过程一致。

- 暴力破解过程详解

  破解过程是通过尝试所有可能的密钥，直到找到正确的密钥，从而解密密文。具体步骤如下：

  1.输入明文和密文。

  2.对于每个可能的密钥，使用SDES解密算法对密文进行解密。

  3.将解密后的明文与输入的明文进行比较，如果相同，则找到了正确的密钥。

  4.输出找到的密钥。

## 接口说明
1.void encipher(int c[], int k[])
 
描述：使用S-DES算法加密输入的8bit数字并输出密文。  
参数：  
int c[]：int[]类型，要加密的明文。  
int k[]：获取加密使用的密钥。    

2.void decrypt(int p[], int k[])

描述：使用S-DES算法解密输入的8bit数字并输出明文。  
参数：  
int p[]：int[]类型，要解密的密文。  
int k[]：获取加密使用的密钥。

3.void str()

描述：对输入的字符串进行加解密。

4.void BFA()

描述：对输入的明密文对进行暴力破解。  

- 以下为S-DES算法内部函数接口

5.void creatkey(int k[], int cipher_key[][8])

描述：10-bit密钥转换为8-bit
参数：
int k[]：转换前的密钥。
int cipher_key[][8]：接受转换后的密钥，轮密钥，每一排代表每一轮的密钥。

6.void IP(int k[], bool mode)

描述:置换函数。
参数：
int k[]：用于置换的数据。
bool mode：0表示初始置换，1表示最终置换。

7.void f(int c[], int k[])

描述：S-DES函数。
参数：
int c[]：加解密数据。
int k[]：密钥。

8.void F(int r[], int k[])

描述：轮函数。
参数：
int r[]：数据右半部分。
int k[]：密钥。

9.void swap(int c[])

描述：左右置换
参数：
int c[]：交换对象。

# 用户指南
## 概述
重庆大学大数据与软件学院信息安全导论作业：S-DES算法的实现，根据老师课堂内容，使用CPP语言完成。
实现了二进制加解密和ASCLL码字符串加解密，以及在已知明密文的前提下暴力破解出密钥等功能。

## 操作指南
编译并运行S-DES_ver1_2.cpp文件后进入初始界面。

### 初始界面
- 输入0，进行基础的8-bit数字加解密。
- 输入1，进行字符串加解密。
- 输入2，进行暴力破解。

### 基础加解密
本功能只能进行8位的二进制数的加解密，错误请重新运行程序并输入。
之后根据提示选择模式，并输入数据即可。

### 字符串加解密
根据提示选择模式，并输入数据即可。
无长度限制。

### 暴力破解
根据提示输入数据后会依次显示破解出的密钥以及破解所用时间。
