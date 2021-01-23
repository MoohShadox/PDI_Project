#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>    // std::min
#include"PRP.h"

#define epsilon 0.00001

using namespace std;

int main (int argc, char**argv){

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

  fic.close();

  printf("prp created\n");
  std::cout << "n = " << prp.n << std::endl;

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
    varname <<"p_"<< t ;
    IloNumVar v = IloNumVar(env, 0, prp.C);
    v.setName(varname.str().c_str());
    p.add(v);

    varname.str("");
    varname <<"y_"<< t ;
    v = IloNumVar(env, 0, 1, ILOINT);
    v.setName(varname.str().c_str());
    y.add(v);
  }

  IloArray<IloNumVarArray> I(env); //Inventory at node i at period t
  IloArray<IloNumVarArray> z(env); //number of vehicule at node i at period t
  IloArray<IloNumVarArray> q(env); //quantity delivered at client i at period t
  IloArray<IloNumVarArray> w(env); //load of vehicule before delivery at client i at period t

  for(int i = 0; i < prp.n; i++){
    IloNumVarArray Ii(env);
    IloNumVarArray zi(env);
    IloNumVarArray qi(env);
    IloNumVarArray wi(env);
    ///REPRENDRE ICI !!!!!!
    for(int t = 0; t < prp.l; t++){
      IloNumVar v = IloNumVar(env,0,0);
      if(t==0){
        v = IloNumVar(env,0,0);
        varname.str("");
        varname <<"I_"<< i << "_" << t ;
        v.setName(varname.str().c_str());
        Ii.add(v);
      }
      else{
        v = IloNumVar(env,0,prp.L[i]);
        varname.str("");
        varname <<"I_"<< i << "_" << t ;
        v.setName(varname.str().c_str());
        Ii.add(v);
      }

      if(i==0){
        v = IloNumVar(env,0,prp.k);
        varname.str("");
        varname <<"Z_"<< i << "_" << t ;
        v.setName(varname.str().c_str());
        zi.add(v);
      }
      else{
        v = IloNumVar(env,0,1, ILOINT);
        varname.str("");
        varname <<"Z_"<< i << "_" << t ;
        v.setName(varname.str().c_str());
        zi.add(v);
      }
      v = IloNumVar(env,0,prp.Q);
      varname.str("");
      varname <<"Q_"<< i << "_" << t ;
      v.setName(varname.str().c_str());
      qi.add(v);


      v = IloNumVar(env,0,prp.Q);
      varname.str("");
      varname <<"W_"<< i << "_" << t ;
      v.setName(varname.str().c_str());
      wi.add(v);
    }
    I.add(Ii);
    z.add(zi);
    q.add(qi);
    w.add(wi);
  }
  IloArray<IloArray<IloNumVarArray>> x(env); //if a vehicle travels directly from node i to node j in period t,0 otherwise;
  IloNumVar v;
  for(int i = 0; i < prp.n; i++){
    IloArray<IloNumVarArray> xi(env);
    for(int j = 0; j < prp.n; j++){
      IloNumVarArray xij(env);
      for(int t = 0; t < prp.l; t++){
        v = IloNumVar(env,0,1, ILOINT);
        varname.str("");
        varname <<"X_"<< i << "_" << j << "_" << t ;
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
      for(int i = 1; i < prp.n; i++){
        sum=sum+prp.d[i][t];
      }
    }
    Mt.push_back(min(prp.C,sum));
  }



  vector<vector<float>> Mit;

  for(int i = 0; i < prp.n; i++){
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

  for(int t = 0; t < prp.l; t++){
    objective+=prp.u*p[t]+prp.f*y[t];
    for(int i = 0; i < prp.n; i++){
      objective+=prp.h[i]*I[i][t];
    }
    for(int i = 0; i < prp.n; i++){
      for(int j = 0; j < prp.n; j++){
        objective+=prp.getDistance(i,j)*x[i][j][t];
      }
    }
  }

  //(1)

  model.add(IloMinimize(env,objective));

  //(2)
  
  //constraint.setName(varname.str().c_str());
  //model->add(constraint);

  for(int t = 1; t < prp.l; t++){
    IloExpr sum(env);
    for(int i = 1; i < prp.n; i++){
      sum+=q[i][t];
    }
    IloConstraint constraint = (I[0][t-1]+p[t]==sum+I[0][t]);
    varname.str("");
    varname <<"C2_t" << t ;
    constraint.setName(varname.str().c_str());
    model.add(constraint);
  }
  //(3)

  for(int i = 1; i < prp.n; i++){
    for(int t = 1; t < prp.l; t++){
      IloConstraint constraint = (I[i][t-1]+q[i][t]==prp.d[i][t]+I[i][t]);
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
    varname <<"C4_t"<< t;
    constraint.setName(varname.str().c_str());
    model.add(constraint);
  }

  //(5)

  for(int t = 0; t < prp.l; t++){
    IloConstraint constraint = (I[0][t]<=prp.L[0]);
    varname.str("");
    varname <<"C5_t"<< t;
    constraint.setName(varname.str().c_str());
    model.add(constraint);
  }

  //(6)

  for(int i = 1; i < prp.n; i++){
    for(int t = 1; t < prp.l; t++){
      IloConstraint constraint = (I[i][t-1]+q[i][t]<=prp.L[i]);
      varname.str("");
      varname <<"C6_i" << i << ";t" << t;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(7)

  for(int i = 1; i < prp.n; i++){
    for(int t = 0; t < prp.l; t++){
      IloConstraint constraint = (q[i][t]<=Mit[i][t]*z[i][t]);
      varname.str("");
      varname <<"C7_i" << i << ";t" << t;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(8)

  for(int i = 1; i < prp.n; i++){
    for(int t = 0; t < prp.l; t++){
      IloExpr sum(env);
      for(int j = 0; j < prp.n; j++){
        sum+=x[j][i][t];
      }
      IloConstraint constraint = (sum==z[i][t]);
      varname.str("");
      varname <<"C8_i" << i << ";t" << t;
      constraint.setName(varname.str().c_str());
      model.add(constraint);
    }
  }

  //(9)

  for(int i = 1; i < prp.n; i++){
    for(int t = 0; t < prp.l; t++){
      IloExpr sum1(env);
      for(int j = 0; j < prp.n; j++){
        sum1+=x[j][i][t];
      }
      IloExpr sum2(env);
      for(int j = 0; j < prp.n; j++){

        sum2+=x[i][j][t];
      }
      IloConstraint constraint = (sum1+sum2==2*z[i][t]);
      varname.str("");
      varname <<"C9_i" << i << ";t" << t;
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

  for(int i = 0; i < prp.n; i++){
    for(int j = 0; j < prp.n; j++){
      if(i!=j){
        for(int t = 0; t < prp.l; t++){
          IloConstraint constraint = (w[i][t]-w[j][t]>=q[i][t]-Mit[i][t]*(1-x[i][j][t]));
          varname.str("");
          varname <<"C11_x("<< i << ";" << j << ");t" << t << "";
          constraint.setName(varname.str().c_str());

          model.add(constraint);
        }
      }
    }
  }

  //(12)

  for(int i = 1; i < prp.n; i++){
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


    cplex.exportModel("sortie.lp");


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
