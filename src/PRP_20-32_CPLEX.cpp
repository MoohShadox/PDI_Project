#include <ilcplex/ilocplex.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>    // std::min
#include <unordered_set>
#include"PRP.h"

#define epsilon 0.00001

using namespace std;

ILOLAZYCONSTRAINTCALLBACK5(constraint29, int, n, int, l, int, K,
   IloArray<IloArray<IloArray<IloNumVarArray>>>, x,
    IloArray<IloArray<IloNumVarArray>>, z)
{
for (int t = 0; t < l; t++) {
  for (int k = 0; k < K; k++) {
    unordered_set<int> S;
    // On initialise S avec tous les sommets qui doivent être visités
    for (int i = 1; i <= n; i++) {
      if (getValue(z[i][k][t]) > 0.5) {
        S.insert(i);
      }
    }

    if (S.size() == 0) {
      continue;
    }

    // On parcourt l'unique cycle contenant l'usine et on les supprime de S
    int u;
    for (u = 1; (u <= n) && (getValue(x[0][u][k][t]) < 0.5); u++);
    if (u == n + 1) {
      // Dans ce cas, rien ne sort de l'usine, on veut rajouter la contrainte avec S entier
      // pour sauteur la prochaine boucle, on met u = 0
      u = 0;
    }
    // Parcours du cycle de l'usine
    while (u) {
      S.erase(u);
      int v;
      for (v = 0; (v==u) || (getValue(x[u][v][k][t]) < 0.5); v++);
      u = v;
    }

    // S'il reste des éléments dans S après ce parcours, alors forcément S viole la contrainte
    if (S.size() > 0) {
      IloExpr cst(getEnv());
      for (int i : S) {
        for (int j : S) {
          if (i != j) {
            cst += x[i][j][k][t];
          }
        }
      }
      add(cst <= (int) S.size() - 1).end();
    }
  }
}
}

int main (int argc, char**argv){

  string name,nameext, nameextsol;

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
    p.add(IloNumVar(env, 0, prp.C));
    y.add(IloNumVar(env, 0, 1, ILOINT));
  }

  IloArray<IloNumVarArray> I(env); //Inventory at node i at period t
  IloArray<IloArray<IloNumVarArray>> z(env); //number of vehicule at node i at period t
  IloArray<IloArray<IloNumVarArray>> q(env); //quantity delivered at client i at period t

  for(int i = 0; i < node_number; i++){
    IloNumVarArray Ii(env);
    IloArray<IloNumVarArray> zi(env);
    IloArray<IloNumVarArray> qi(env);
    for(int t = 0; t < prp.l; t++){
      if(t==0){
        Ii.add(IloNumVar(env,prp.L0[i],prp.L0[i]));
      }
      else{
        Ii.add(IloNumVar(env,0,prp.L[i]));
      }
    }
    for(int k=0; k<prp.k;k++){
      IloNumVarArray zik(env);
      IloNumVarArray qik(env);
      for(int t = 0; t < prp.l; t++){
        zik.add(IloNumVar(env,0,1, ILOINT));
        qik.add(IloNumVar(env,0,prp.Q));
      }
      zi.add(zik);
      qi.add(qik);
    }
    I.add(Ii);
    z.add(zi);
    q.add(qi);
  }
  IloArray<IloArray<IloArray<IloNumVarArray>>> x(env); //if a vehicle travels directly from node i to node j in period t,0 otherwise;
  for(int i = 0; i < node_number; i++){
    IloArray<IloArray<IloNumVarArray>> xi(env);
    for(int j = 0; j < node_number; j++){
      IloArray<IloNumVarArray> xij(env);
      for(int k=0; k<prp.k;k++){
        IloNumVarArray xijk(env);
        for(int t = 0; t < prp.l; t++){
          if(i!=j) xijk.add(IloNumVar(env,0,1, ILOINT));
        }
        xij.add(xijk);
      }
      xi.add(xij);
    }
    x.add(xi);
  }

  vector<float> Mt;

  for(int t = 0; t < prp.l; t++){
    float sum = 0;
    for(int j = t; j < prp.l; j++){
      for(int i = 1; i < node_number; i++){
        sum=sum+prp.d[i][t];
      }
    }
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

  for(int t = 0; t < prp.l; t++){
    objective+=prp.u*p[t]+prp.f*y[t];
    for(int i = 0; i < node_number; i++){
      objective+=prp.h[i]*I[i][t];
    }
    for(int i = 0; i < node_number; i++){
      for(int j = 0; j < node_number; j++){
        IloExpr sum(env);
        for(int k=0; k<prp.k;k++){
          if(i!=j) sum+=x[i][j][k][t];
        }
        objective+=prp.getDistance(i,j)*sum;
      }
    }
  }

  //(20)

  model.add(IloMinimize(env,objective));

  //(21)
  /*
  for(int t = 1; t < prp.l; t++){
    IloExpr sum(env);
    for(int i = 1; i < node_number; i++){
      for(int k=0; k<prp.k;k++){
        sum+=q[i][k][t];
      }
    }
    model.add(I[0][t-1]+p[t]==sum+I[0][t]);
  }

  //(22)

  for(int i = 1; i < node_number; i++){
    for(int t = 1; t < prp.l; t++){
      IloExpr sum(env);
      for(int k=0; k<prp.k;k++){
        sum+=q[i][k][t];
      }
      model.add(I[i][t-1]+sum==prp.d[i][t]+I[i][t]);
    }
  }

  //(23)

  for(int t = 0; t < prp.l; t++){
    model.add(p[t]<=Mt[t]*y[t]);
  }

  //(24)

  for(int t = 0; t < prp.l; t++){
    model.add(I[0][t]<=prp.L[0]);
  }

  //(25)

  for(int i = 1; i < node_number; i++){
    for(int t = 1; t < prp.l; t++){
      IloExpr sum(env);
      for(int k=0; k<prp.k;k++){
        sum+=q[i][k][t];
      }
      model.add(I[i][t-1]+sum<=prp.L[i]);
    }
  }

  //(26)

  for(int i = 1; i < node_number; i++){
    for(int t = 0; t < prp.l; t++){
      for(int k=0; k<prp.k;k++){
        model.add(q[i][k][t]<=Mit[i][t]*z[i][k][t]);
      }
    }
  }

  //(27)

  for(int i = 1; i < node_number; i++){
    for(int t = 0; t < prp.l; t++){
      IloExpr sum(env);
      for(int k=0; k<prp.k;k++){
        sum+=z[i][k][t];
      }
      model.add(sum<=1);
    }
  }

  //(28)

  for(int i = 1; i < node_number; i++){
    for(int t = 0; t < prp.l; t++){
      for(int k=0; k<prp.k;k++){
        IloExpr sum1(env);
        for(int j = 0; j < node_number; j++){
          if(i!=j) sum1+=x[j][i][k][t];
        }
        IloExpr sum2(env);
        for(int j = 0; j < node_number; j++){

          if(i!=j) sum2+=x[i][j][k][t];
        }
        model.add(sum1+sum2==2*z[i][k][t]);
      }
    }
  }

  //(11)

  for(int k=0; k<prp.k;k++){
    for(int t = 0; t < prp.l; t++){
      IloExpr sum(env);
      for(int i = 1; i<node_number;i++){
        sum+=q[i][k][t];
      }
      model.add(sum<=prp.Q*z[0][k][t]);
    }
  }*/

  printf("model created\n");

  //(13) -(16) borne initialisation variable

  ///////////
  //// RESOLUTION
  //////////

  IloCplex cplex(model);

  //cplex.use(constraint29(env, prp.n, prp.l, prp.k, x, z));

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
