#include "auxiliary.h"
#include "yacc.h"

// define in yacc.c
int VarCount = 1;
int tempVarCount = 1;
int NXQ = 1;
struct QUATERLIST QuaterList[MAXMEMBER];
struct VARLIST VarList[MAXMEMBER];



/*****************************
 * Find symbol table 
 * if successfully find entry, return its index(from 0)
 * else return -1  
 *****************************/
int lookUp(char *Name)
{
	int i;
	for (i = 1; i < VarCount; ++i) {
		if (!strcmp(Name, VarList[i].name)) {
			return i;
		}
	}
	return 0;
}

/*****************************
 * Add new entry to symbol table
 *****************************/
int enter(char *Name)
{
	//printf("* %s *", Name);
	strcpy(VarList[VarCount].name, Name);
	VarList[VarCount].type = 0; // default type is INTEGER
	return VarCount++;
}

/*****************************
 * Find or Add symbol table 
 * if dont find entry, then add it to symbol table.
 *****************************/
int entry(char *Name)
{
	int i = lookUp(Name);
	if (i > 0) {
		return i;
	} else {
		return enter(Name);
	}
}

int newTemp(void)
{
	char temp[5];
	sprintf(temp, "T%d", tempVarCount);
	strcpy(VarList[MAXMEMBER - tempVarCount].name, temp);
	VarList[MAXMEMBER - tempVarCount].type = 0;
	return MAXMEMBER-(tempVarCount++);
}

/*****************************
 * ������Ԫʽ
 * ÿִ��һ�Σ���ִ��NXQ++
 *****************************/
void gen(char *op, int arg1, int arg2, int result)
{
	strcpy(QuaterList[NXQ].op, op);
	QuaterList[NXQ].arg1 = arg1;
	QuaterList[NXQ].arg2 = arg2;
	QuaterList[NXQ].result = result;
	NXQ++;
}

/*****************************
 * �����ס�ָ�롱�ֱ�Ϊp1��p2���������ϲ�Ϊһ��
 * ���������������ס�ָ�롱
 * �˴���ָ��ʵ��������Ԫʽ����ţ�Ϊ����ֵ
 *****************************/
int merge(int p1, int p2)
{
	int p;
	if (!p2) { // p2 = 0���ڶ�����Ϊ��
		return p1;
	} else {
		//find the last quadruple of chain p2
		p = p2;
		while (QuaterList[p].result) {
			p = QuaterList[p].result;
		}
		// append p1 to p2
		QuaterList[p].result = p1;
		return p2;
	}
}

/*****************************
 * ����Ԫʽ���t������pΪ�׵���
 * ������ÿ����Ԫʽ��Result���дΪt��ֵ 
 *****************************/
void backPatch(int p, int t)
{
	int q = p, q1;
	while (q) {
		q1 = QuaterList[q].result;
		QuaterList[q].result = t;
		q = q1;
	}
	return;
}

/*****************************
 * ���㲢��д���ű��NO�Ǽ���ADDR����ָ�����������е�Cֵ
 * C��CONSTPART��
 *****************************/
void FillArrMSG_C(int NO)
{
	int n = VarList[NO].ADDR->DIM;
	int C = 0, j, k, mul;
	for (j = 1; j < n; ++j) {
		mul = 1;
		for (k = j + 1; k < n + 1; ++k) {
			mul *= VarList[NO].ADDR->Vector[3*k + 1];
		}
		C += VarList[NO].ADDR->Vector[3 * j - 1] * mul;
	}
	VarList[NO].ADDR->Vector[0] = C;
}

/*****************************
* ȡ����Cֵ
*****************************/
int Access_C(int no)
{
	return VarList[no].ADDR->Vector[0];
}

/*****************************
 * �����д�����
 *****************************/
int Access_a(int no)
{
	return VarList[no].ADDR;
}

/*****************************
 * ȡ�����kά�Ľ��
 *****************************/
int Access_d(int no, int k)
{
	return VarList[no].ADDR->Vector[3 * k + 1];
}