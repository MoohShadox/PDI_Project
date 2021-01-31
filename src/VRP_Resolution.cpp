#include "VRP_Resolution.h"



VRP_Resolution::VRP_Resolution(PRP &p1, IloEnv &env1):Resolution(p1,env1){

    
    w = new IloNumVarArray(*env, prp->n,0.0,prp->Q);
    x = new IloArray<IloIntVarArray>(*env,prp->n+1);
    ostringstream varname;

    IloArray<IloIntVarArray> &xr = *x;
    for (unsigned i=0; i<prp->n+1; i++){
        IloIntVarArray XR(*env, prp->n+1,0,1);
        xr[i] = XR;
        for  (unsigned j=0; j<prp->n+1; j++)
        {
            varname.str("");
            varname<<"X_"<<i<<"_"<<j;
            xr[i][j].setName(varname.str().c_str());
        }
    }

    for(int i = 0; i < prp->n; i++) {
        varname.str("");
        varname<<"w_"<<i;
        w->operator[](i).setName(varname.str().c_str());
    }

}


void VRP_Resolution::addingFlowConstraints(){
    IloArray<IloIntVarArray> &xr = *x;
    IloRangeArray CC(*env);
    ostringstream varname;
    IloExpr exp2(*env);
    IloExpr exp1(*env);
    for (int j=1; j< prp->n; j++){
        exp1 = exp1 + xr[0][j];
        exp2 = exp2 + xr[j][0];
    }
    varname.str("");
    varname<<"C1";
    IloRange cst1(*env, -IloInfinity,exp1, prp->k, varname.str().c_str() );

    varname.str("");
    varname<<"C2";
    IloRange cst2(*env, -IloInfinity,exp1, prp->k, varname.str().c_str() );

    CC.add(cst1);
    CC.add(cst2);

    //On genère une contrainte par client 
    for (int client=1; client< prp->n+1; client++){
        IloExpr expr1(*env);
        IloExpr expr2(*env);
        for (int voisin=1; voisin< prp->n+1; voisin++){
            if(client != voisin){
                expr1 = expr1 + xr[client][voisin];
                expr2 = expr2 + xr[voisin][client];
            }
        }
        varname.str("");
        varname<<"Out(" << client << ")";
        IloRange cst3(*env, 1,expr1, 1, varname.str().c_str() );
        varname.str("");
        varname<<"In(" << client << ")";
        IloRange cst4(*env, 1,expr2, 1, varname.str().c_str() );

        CC.add(cst3);
        CC.add(cst4);

    }
    model->add(CC);
    IloCplex cplex(*model);
    cplex.exportModel("sortie2.lp");
}

void VRP_Resolution::addingCostObjectif(){
    IloArray<IloIntVarArray> &xr = *x;
    for (unsigned i=1; i<prp->n+1; i++){
        for  (unsigned j=1; j<prp->n+1; j++)
        {
            obj.setLinearCoef(xr[i][j],prp->getDistance(i,j));
        }
    }
    std::cout << "Objectif : " << obj << std::endl;

}

void VRP_Resolution::printVariables(){
     cplx->writeSolution("sol.txt");
}

void VRP_Resolution::addingMTZConstraints(int t){
    IloNumVarArray &wr = *w;
    IloArray<IloIntVarArray> &xr = *x;

    IloRangeArray CC(*env);
    ostringstream varname;

    for (unsigned i=0; i<prp->n; i++){
        for(unsigned j=0; j< prp->n; j++)
        {
            //std::cout << "i = " << i << " j = " << j << " t=" << t <<std::endl;
            if(i == j) continue;
            IloExpr exp1(*env);
            //std::cout << "Test : " << prp->d[0][i+1]<< std::endl;
            exp1 = wr[i] - wr[j] - prp->d[i+1][t] + (prp->d[i+1][t]+prp->Q)*(1-xr[i+1][j+1]);

            varname.str("");
            varname<<"MTZ(" << i << " "<< j <<") : ";
            IloRange cst(*env, 0,exp1, IloInfinity, varname.str().c_str() );
            CC.add(cst);
        }
    }
    model->add(CC);
    IloCplex cplex(*model);
    cplex.exportModel("sortie2.lp");
}

