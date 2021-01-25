#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>    // std::min
#include"PRP.h"
#include "PDI16.h"

PDI16::PDI16(PRP &p1, IloEnv &env1):Resolution(p1,env1){

    //Used for naming 
    ostringstream varname;
    //

    IloNumVarArray pr(*env); 
    IloBoolVarArray yr(*env); 
    p = pr;
    y = yr;
    //Creating p and y

    for(int t = 0; t < prp->l; t++){
        varname.str("");
        varname <<"p_"<< t+1 ;
        IloNumVar v = IloNumVar(*env, 0, prp->C);
        v.setName(varname.str().c_str());
        p.add(v);
        std::cout << "added : " << v << std::endl;

        varname.str("");
        varname <<"y_"<< t+1 ;
        IloBoolVar v2 = IloBoolVar(*env);
        v2.setName(varname.str().c_str());
        y.add(v2);
        std::cout << "added : " << v2 << std::endl;

    }

    IloArray<IloNumVarArray> Ir(*env);
    I = Ir;

    //Creating I [0..n][0..l] -> (n+1, l+1)

    for (int i=0; i<prp->n+1; i++){
        IloNumVarArray Ii(*env);
        for (int t=0; t<prp->l+1;t++){
            varname.str("");
            varname <<"I_"<< i << "_" << t ;
            IloNumVar v;
            if(t == 0){
                //Inventaire initial == L0[i]
                v = IloNumVar(*env, prp->L0[i],  prp->L0[i]);
            }
            else{
                //Inventaire le reste du temps entre 0 et L[i]
                v = IloNumVar(*env, 0, prp->L[i]);
            }
            v.setName(varname.str().c_str());
            Ii.add(v);
            std::cout << "added : " << v << std::endl;
        }
        I.add(Ii);
    }

    vector<float> Mt;

    for(int t = 0; t < prp->l; t++){
        float sum = 0;
        for(int j = t; j < prp->l; j++){
            for(int i = 1; i < prp->n + 1; i++){
                sum=sum+prp->d[i][t];
            }
        }
        Mt.push_back(min(prp->C,sum));
    }



  vector<vector<float>> Mit;

  for(int i = 0; i < prp->n + 1; i++){
    vector<float> Mi;
    for(int t = 0; t < prp->l; t++){
      float sum = 0;
      for(int j = t; j < prp->l; j++){
        if(i!=0)  sum=sum+prp->d[i][j];
      }
      Mi.push_back(std::min({prp->L[i],prp->Q,sum}));
    }
    Mit.push_back(Mi);
  }

    IloArray<IloNumVarArray> qr(*env);
    q = qr;

    for (int i=0; i<prp->n; i++){
        IloNumVarArray qi(*env);
        for (int t=0; t<prp->l;t++){
            varname.str("");
            varname <<"q_"<< i+1 << "_" << t+1 ;
            IloNumVar v = IloNumVar(*env, 0, prp->Q);
            v.setName(varname.str().c_str());
            qi.add(v);
            std::cout << "added : " << v << std::endl;
        }
        q.add(qi);
    }

    IloArray<IloNumVarArray> zr(*env);
    z = zr;

    for(int i = 0; i < prp->n+1; i++){
        IloNumVarArray zi(*env);
        for(int t = 0; t < prp->l; t++){
          IloNumVar v = IloNumVar(*env,0,0);
          if(i==0){
            v = IloNumVar(*env,0,prp->k);
            varname.str("");
            varname <<"Z_"<< i << "_" << t+1 ;
            v.setName(varname.str().c_str());
            zi.add(v);
          }
          else{
            v = IloNumVar(*env,0,1, ILOINT);
            varname.str("");
            varname <<"Z_"<< i << "_" << t+1 ;
            v.setName(varname.str().c_str());
            zi.add(v);
          }
            std::cout << "added : " << v << std::endl;
        }
        z.add(zi);
    }

    std::cout << "Model Created ! " << std::endl;
    IloCplex cplex(*model);
    cplex.exportModel("sortie.lp");
}