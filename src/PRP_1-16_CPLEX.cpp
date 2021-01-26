#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>    // std::min
#include"PRP.h"

#define epsilon 0.00001

using namespace std;

int main_1_16 (int argc, char**argv){

  string name,nameext, nameextsol;
  ostringstream varname;

  //////////////
  //////  DATA
  //////////////

  if(argc!=2){
    cerr<<"usage: "<<argv[0]<<" <PRP file name>   (without .prp)"<<endl;
    return 1;
  }


  name=argv[1];
  nameextsol=name+".prp";

  ifstream fic(nameextsol.c_str());

  if (!fic){
      cerr<<"file "<<nameextsol<<" not found"<<endl;
      return 1;
  }

  PRP prp(fic);
  prp.write_screen_txt();

  fic.close();

  printf("prp created\n");

  int node_number = prp.n+1;

  std::cout << "n = " << node_number << std::endl;

  //////////////
  //////  CPLEX INITIALIZATION
  //////////////


  IloEnv   env;
  IloModel model(env);

  ////////////////////////
  //////  VAR
  ////////////////////////


  IloNumVarArray p(env); //Production quantity at period t
  IloNumVarArray y(env); //Production at period t

  for(int t = 0; t < prp.l; t++){
    varname.str("");
    varname <<"p_"<< t+1 ;
    IloNumVar v = IloNumVar(env, 0, prp.C);
    v.setName(varname.str().c_str());
    p.add(v);

    varname.str("");
    varname <<"y_"<< t+1 ;
    v = IloNumVar(env, 0, 1, ILOINT);
    v.setName(varname.str().c_str());
    y.add(v);
  }

  IloArray<IloNumVarArray> I(env); //Inventory at node i at period t
  IloArray<IloNumVarArray> z(env); //number of vehicule at node i at period t
  IloArray<IloNumVarArray> q(env); //quantity delivered at client i at period t
  IloArray<IloNumVarArray> w(env); //load of vehicule before delivery at client i at period t

  for(int i = 0; i < node_number+1; i++){
    IloNumVarArray zi(env);
    IloNumVarArray qi(env);
    IloNumVarArray wi(env);
    for(int t = 0; t < prp.l; t++){
      IloNumVar v = IloNumVar(env,0,0);
      if(i==0){
        v = IloNumVar(env,0,prp.k);
        varname.str("");
        varname <<"Z_"<< i << "_" << t+1 ;
        v.setName(varname.str().c_str());
        zi.add(v);
      }
      else{
        v = IloNumVar(env,0,1, ILOINT);
        varname.str("");
        varname <<"Z_"<< i << "_" << t+1 ;
        v.setName(varname.str().c_str());
        zi.add(v);
      }
      v = IloNumVar(env,0,prp.Q);
      varname.str("");
      varname <<"Q_"<< i << "_" << t+1 ;
      v.setName(varname.str().c_str());
      qi.add(v);


      v = IloNumVar(env,0,prp.Q);
      varname.str("");
      varname <<"W_"<< i << "_" << t+1 ;
      v.setName(varname.str().c_str());
      wi.add(v);
    }
    z.add(zi);
    q.add(qi);
    w.add(wi);
  }

    for (int i=0; i<prp.n+1; i++){
        IloNumVarArray Ii(env);
        for (int t=0; t<prp.l+1;t++){
            varname.str("");
            varname <<"I_"<< i << "_" << t ;
            IloNumVar v;
            if(t == 0){
                //Inventaire initial == L0[i]
                v = IloNumVar(env, prp.L0[i],  prp.L0[i]);
            }
            else{
                //Inventaire le reste du temps entre 0 et L[i]
                v = IloNumVar(env, 0, prp.L[i]);
            }
            v.setName(varname.str().c_str());
            Ii.add(v);
            std::cout << "added : " << v << std::endl;
        }
        I.add(Ii);
    }


  IloArray<IloArray<IloNumVarArray>> x(env); //if a vehicle travels directly from node i to node j in period t,0 otherwise;
  IloNumVar v;
  for(int i = 0; i < node_number; i++){
    IloArray<IloNumVarArray> xi(env);
    for(int j = 0; j < node_number; j++){
      IloNumVarArray xij(env);
      for(int t = 0; t < prp.l; t++){
        v = IloNumVar(env,0,1, ILOINT);
        varname.str("");
        varname <<"X_"<< i << "_" << j << "_" << t+1 ;
        v.setName(varname.str().c_str());
        xij.add(v);
      }
      xi.add(xij);
    }
    x.add(xi);
  }

  vector<float> Mt;

  for(int t = 0; t < prp.l; t++){
    float sum = 0;
    for(int j = t; j < prp.l; j++){
      for(int i = 1; i < prp.n+1; i++){
        //sum=sum+prp.d[i][t]; //C'est J pas t
        sum=sum+prp.d[i][j];
        //std::cout << "Adding : " << prp.d[i][j] << " at " << i << " " << j << std::endl;
      }
    }
    //std::cout << "finally sum = " << sum << std::endl;
    Mt.push_back(min(prp.C,sum));
  }



  vector<vector<float>> Mit;

  for(int i = 0; i < node_number; i++){
    vector<float> Mi;
    for(int t = 0; t < prp.l; t++){
      float sum = 0;
      for(int j = t; j < prp.l; j++){
        if(i!=0)  sum=sum+prp.d[i][j];
      }
      Mi.push_back(std::min({prp.L[i],prp.Q,sum}));
    }
    Mit.push_back(Mi);
  }

  ///////////
  //// constraints
  //////////


  std::cout << "constraints" << std::endl;

  IloExpr objective(env);

  for(int t = 1; t < prp.l+1; t++){
      for(int i = 0; i < node_number; i++){
        objective+=prp.h[i]*I[i][t];
      }
  }

  for(int t=0; t< prp.l ; t++){
    objective+=prp.u*p[t]+prp.f*y[t];
  }

  for(int t = 0; t < prp.l; t++){
    for(int i = 0; i < node_number; i++){
      for(int j = 0; j < node_number; j++){
        objective+=prp.getDistance(i,j)*x[i][j][t];
      }
    }
  }

  //(1)

  model.add(IloMinimize(env,objective));

  //(2)

  //constraint.setName(varname.str().c_str());
  //model->add(constraint);

  for(int t = 1; t < prp.l+1; t++){
    IloExpr sum(env);
    for(int i = 1; i < node_number; i++){
      sum+=q[i][t-1];
    }
    IloConstraint constraint = (I[0][t-1]+p[t-1]==sum+I[0][t]);
    varname.str("");
    varname <<"C2_t" << t ;
    constraint.setName(varname.str().c_str());
    model.add(constraint);
  }
  //(3)

  for(int i = 1; i < node_number; i++){
    for(int t = 1; t < prp.l+1; t++){
      IloConstraint constraint = (I[i][t-1]+q[i][t-1]==prp.d[i][t-1]+I[i][t]);
      varname.str("");
      varname <<"C3_i" << i << ";t" << t;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(4)

  for(int t = 0; t < prp.l; t++){
    IloConstraint constraint = (p[t]<=Mt[t]*y[t]);
    varname.str("");
    varname <<"C4_t"<< t+1;
    constraint.setName(varname.str().c_str());
    model.add(constraint);
  }

  //(5)

  for(int t = 0; t < prp.l+1; t++){
    IloConstraint constraint = (I[0][t]<=prp.L[0]);
    varname.str("");
    varname <<"C5_t"<< t;
    constraint.setName(varname.str().c_str());
    model.add(constraint);
  }

  //(6)

  for(int i = 1; i < node_number; i++){
    for(int t = 1; t < prp.l+1; t++){
      IloConstraint constraint = (I[i][t-1]+q[i][t-1]<=prp.L[i]);
      varname.str("");
      varname <<"C6_i" << i << ";t" << t;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(7)

  for(int i = 1; i < node_number; i++){
    for(int t = 0; t < prp.l; t++){
      IloConstraint constraint = (q[i][t]<=Mit[i][t]*z[i][t]);
      varname.str("");
      varname <<"C7_i" << i << ";t" << t+1;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(8)

  for(int i = 1; i < node_number; i++){
    for(int t = 0; t < prp.l; t++){
      IloExpr sum(env);
      for(int j = 0; j < node_number; j++){
        if(i == j)
          continue;
        sum+=x[j][i][t];
      }
      IloConstraint constraint = (sum==z[i][t]);
      varname.str("");
      varname <<"C8_i" << i << ";t" << t+1;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(9)

  for(int i = 1; i < node_number; i++){
    for(int t = 0; t < prp.l; t++){
      IloExpr sum1(env);
      IloExpr sum2(env);
      for(int j = 0; j < node_number; j++){
        if(i == j)
          continue;
        sum1+=x[j][i][t];
        sum2+=x[i][j][t];
      }
      IloConstraint constraint = (sum1+sum2==2*z[i][t]);
      varname.str("");
      varname <<"C9_i" << i << ";t" << t+1;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(10)

  for(int t = 0; t < prp.l; t++){
    IloConstraint constraint = (z[0][t]<=prp.k);
    varname.str("");
    varname <<"C10_t"<< t;
    constraint.setName(varname.str().c_str());
    model.add(constraint);
  }

  //(11)

  for(int i = 1; i < node_number; i++){
    for(int j = 1; j < node_number; j++){
      if(i!=j){
        for(int t = 0; t < prp.l; t++){
          IloConstraint constraint = (w[i][t]-w[j][t]>=q[i][t]-Mit[i][t]*(1-x[i][j][t]));
          varname.str("");
          varname <<"C11_("<< i << ";" << j << ");t" << t+1 << "";
          constraint.setName(varname.str().c_str());
          model.add(constraint);
        }
      }
    }
  }

  //(12)

  for(int i = 1; i < node_number; i++){
    for(int t = 0; t < prp.l; t++){
      IloConstraint constraint = (w[i][t]<=prp.Q*z[i][t]);
      varname.str("");
      varname <<"C12_i" << i << ";t" << t;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  printf("model created\n");

  //(13) -(16) borne initialisation variable

  ///////////
  //// RESOLUTION
  //////////

  IloCplex cplex(model);
  cplex.exportModel("sortie.lp");

  // cplex.setParam(IloCplex::Cliques,-1);
  // cplex.setParam(IloCplex::Covers,-1);
  // cplex.setParam(IloCplex::DisjCuts,-1);
  // cplex.setParam(IloCplex::FlowCovers,-1);
  // cplex.setParam(IloCplex::FlowPaths,-1);
  // cplex.setParam(IloCplex::FracCuts,-1);
  // cplex.setParam(IloCplex::GUBCovers,-1);
  // cplex.setParam(IloCplex::ImplBd,-1);
  // cplex.setParam(IloCplex::MIRCuts,-1);
  // cplex.setParam(IloCplex::ZeroHalfCuts,-1);
  // cplex.setParam(IloCplex::MCFCuts,-1);
  // cplex.setParam(IloCplex::MIPInterval,1);
  // cplex.setParam(IloCplex::HeurFreq,-1);
  // cplex.setParam(IloCplex::ClockType,1);
  // cplex.setParam(IloCplex::RINSHeur,-1);


  if ( !cplex.solve() ) {
    env.error() << "Failed to optimize LP" << endl;
    exit(1);
  }

  env.out() << "Solution status = " << cplex.getStatus() << endl;
  env.out() << "Solution value  = " << cplex.getObjValue() << endl;

  //////////////
  //////  CPLEX's ENDING
  //////////////
  cplex.writeSolution("sol.txt");

  env.end();

  return 0;
}
