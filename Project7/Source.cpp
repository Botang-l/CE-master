//Gurobi_example.cpp : 定義主控台應用程式的進入點。

#include "gurobi_c++.h"
#include <iostream>
using namespace std;

int main()
{
	GRBEnv *envG; //Gurobi環境
	GRBModel *modelG; //Gurobi數學模式
	GRBLinExpr obj; //Gurobi目標函式
	envG = new GRBEnv();
	modelG = new GRBModel(*envG);
	obj = 0.0;

	GRBVar var_A = modelG->addVar(0, 9999999, 0, GRB_CONTINUOUS);  //宣告一個決策變數 最小值為0 最大值為9999999 內定目標函數係數0 變數類型為一連續整數
	GRBVar var_B = modelG->addVar(0, 1, 0, GRB_BINARY); //宣告一個決策變數 最小值為0 最大值為1 內定目標函數係數0 變數類型為一01變數
	modelG->update(); ////////// 在做變數的新增和修改變更後應用update()更新環境才做新增限制式的動作
	/////////新增限制式的方法有兩種 1. 直接用addConstr()新增 2. 先用GRBLinExpr設計後再以addConstr()新增 ///////
	modelG->addConstr(2 * var_A + 3 * var_B >= 0); //新增一條限制式為 決策變數2*var_A + 3*Var_B >= 0
	GRBLinExpr second_constr = 0.0;
	second_constr += var_A;
	second_constr += 2 * var_B;
	modelG->addConstr(second_constr == 30); //新增一條限制式為  var_A + 2*Var_B == 30

	obj += var_A;
	obj += var_B; //目標函數目前為 Var_A + Var_B

	/*設定Gurobi相關的求解參數*/
	//modelG->getEnv().set(GRB_IntParam_LogToConsole , 1); //gurobi是否輸出log於黑窗上 0為不輸出 1為輸出
	modelG->getEnv().set(GRB_DoubleParam_IntFeasTol, 0.00000001); //MIP 整數變數誤差容許量
	modelG->getEnv().set(GRB_DoubleParam_MIPGap, 1); //要求gap值
	modelG->getEnv().set(GRB_StringParam_LogFile, "log.txt"); //將log輸出至txt檔
	modelG->setObjective(obj, GRB_MINIMIZE); //設置目標函數 Var_A + Var_B的最小值 若要求最大值則改為GRB_MAXMIZE
	modelG->write("./test.lp"); //將數學式輸出成.lp檔
	modelG->optimize(); //最佳化求解開始

	/*有無解之判斷與處理*/
	if (modelG->get(GRB_IntAttr_Status) == 2) //2為有解 3為無解
	{
		cout << "有解" << endl;
		cout << "var_A = " << var_A.get(GRB_DoubleAttr_X) << endl;  //將var_A的求解結果輸出 以double的型態
		cout << "var_B = " << var_B.get(GRB_DoubleAttr_X) << endl;  //將var_B的求解結果輸出 以double的型態
	}

	return 0;
}