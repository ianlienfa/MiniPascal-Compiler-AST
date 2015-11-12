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
	char s[2], temp[5] = "T";
	s[0] = tempVarCount + '0';
	s[1] = '\0';
	strcpy(VarList[MAXMEMBER - tempVarCount].name, strcat(temp, s));
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