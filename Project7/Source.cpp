//Gurobi_example.cpp : �w�q�D���x���ε{�����i�J�I�C

#include "gurobi_c++.h"
#include <iostream>
using namespace std;

int main()
{
	GRBEnv *envG; //Gurobi����
	GRBModel *modelG; //Gurobi�ƾǼҦ�
	GRBLinExpr obj; //Gurobi�ؼШ禡
	envG = new GRBEnv();
	modelG = new GRBModel(*envG);
	obj = 0.0;

	GRBVar var_A = modelG->addVar(0, 9999999, 0, GRB_CONTINUOUS);  //�ŧi�@�ӨM���ܼ� �̤p�Ȭ�0 �̤j�Ȭ�9999999 ���w�ؼШ�ƫY��0 �ܼ��������@�s����
	GRBVar var_B = modelG->addVar(0, 1, 0, GRB_BINARY); //�ŧi�@�ӨM���ܼ� �̤p�Ȭ�0 �̤j�Ȭ�1 ���w�ؼШ�ƫY��0 �ܼ��������@01�ܼ�
	modelG->update(); ////////// �b���ܼƪ��s�W�M�ק��ܧ������update()��s���Ҥ~���s�W������ʧ@
	/////////�s�W�������k����� 1. ������addConstr()�s�W 2. ����GRBLinExpr�]�p��A�HaddConstr()�s�W ///////
	modelG->addConstr(2 * var_A + 3 * var_B >= 0); //�s�W�@������� �M���ܼ�2*var_A + 3*Var_B >= 0
	GRBLinExpr second_constr = 0.0;
	second_constr += var_A;
	second_constr += 2 * var_B;
	modelG->addConstr(second_constr == 30); //�s�W�@�������  var_A + 2*Var_B == 30

	obj += var_A;
	obj += var_B; //�ؼШ�ƥثe�� Var_A + Var_B

	/*�]�wGurobi�������D�ѰѼ�*/
	//modelG->getEnv().set(GRB_IntParam_LogToConsole , 1); //gurobi�O�_��Xlog��µ��W 0������X 1����X
	modelG->getEnv().set(GRB_DoubleParam_IntFeasTol, 0.00000001); //MIP ����ܼƻ~�t�e�\�q
	modelG->getEnv().set(GRB_DoubleParam_MIPGap, 1); //�n�Dgap��
	modelG->getEnv().set(GRB_StringParam_LogFile, "log.txt"); //�Nlog��X��txt��
	modelG->setObjective(obj, GRB_MINIMIZE); //�]�m�ؼШ�� Var_A + Var_B���̤p�� �Y�n�D�̤j�ȫh�אּGRB_MAXMIZE
	modelG->write("./test.lp"); //�N�ƾǦ���X��.lp��
	modelG->optimize(); //�̨ΤƨD�Ѷ}�l

	/*���L�Ѥ��P�_�P�B�z*/
	if (modelG->get(GRB_IntAttr_Status) == 2) //2������ 3���L��
	{
		cout << "����" << endl;
		cout << "var_A = " << var_A.get(GRB_DoubleAttr_X) << endl;  //�Nvar_A���D�ѵ��G��X �Hdouble�����A
		cout << "var_B = " << var_B.get(GRB_DoubleAttr_X) << endl;  //�Nvar_B���D�ѵ��G��X �Hdouble�����A
	}

	return 0;
}