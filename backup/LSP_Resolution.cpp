#include "LSP_Resolution.h"


void printNumVarArray(IloNumVarArray& pr){
    for (unsigned i=0; i<pr.getSize(); i++){
        std::cout << pr[i] << "\t";
    }
    std::cout << std::endl;
}

void printIntVarArray(IloIntVarArray& pr){
    for (unsigned i=0; i<pr.getSize(); i++){
        std::cout << pr[i] << "\t";
    }
    std::cout << std::endl;
}

void printForEveryClient(IloArray<IloNumVarArray> &v){
    for (unsigned i=0; i<v.getSize(); i++){
        std::cout << "---For client : " << i << "--- size = " << v[i].getSize() << "---" << std::endl;
        printNumVarArray(v[i]);
        std::cout << "---\t---" << std::endl;
    }
}



LSP_Resolution::LSP_Resolution(PRP &p1, IloEnv &env1):Resolution(p1,env1){
    p = new IloNumVarArray(*env, prp->l,0.0,prp->C);
    y = new IloIntVarArray(*env, prp->l,0,1);
    q = new IloArray<IloNumVarArray>(*env,prp->n+1);
    I = new IloArray<IloNumVarArray>(*env,prp->n+1);
    // Boucle sur les clients + 1 (Fournisseur)
    IloArray<IloNumVarArray> &qr = *q;
    IloArray<IloNumVarArray> &Ir = *I;

    std::cout << "Created y : " << y->getSize()  << std::endl;
    std::cout << "Created p : " << p->getSize()  << std::endl;
    std::cout << "Created Q : " << qr.getSize()  << std::endl;
    std::cout << "Created I : " << Ir.getSize()  << std::endl;


    for (unsigned i=0; i<prp->n; i++){
        IloNumVarArray QR(*env, prp->l,0.0,prp->C);
        qr[i] = QR;
        std::cout << "Created Q[" << i << "] : " << qr[i].getSize() << std::endl;
    }

    for (unsigned i=0; i<prp->n+1; i++){
        IloNumVarArray IR(*env,prp->l+1,0.0,prp->L[i]);
        Ir[i] = IR;
        std::cout << "Created I[" << i << "] : " << Ir[i].getSize() << std::endl;
    }

    //Les quantités initiales sont fixes ! 
    for (unsigned i=0; i<prp->n+1;i++){
        Ir[i][0].setBounds(prp->L0[i],prp->L0[i]);
    }


    ostringstream varname;
    for(int i = 0; i < prp->l; i++) {
        varname.str("");
        varname<<"y_"<<i;
        y->operator[](i).setName(varname.str().c_str());
        varname.str("");
        varname<<"p_"<<i;
        p->operator[](i).setName(varname.str().c_str());

        for (int t=0; t< prp->n; t++){
            varname.str("");
            varname<<"q_"<<t<<"_"<<i;
            qr[t][i].setName(varname.str().c_str());
        }

        for (int i=0; i < prp->l+1 ; i++){
            for (int t=0; t< prp->n+1; t++){
                varname.str("");
                varname<<"I_"<<t<<"_"<<i;
                Ir[t][i].setName(varname.str().c_str());
            }
        }
  }


}

void LSP_Resolution::generateConstraints(){
    IloNumVarArray &pr = *p;
    IloNumVarArray &I0 = I->operator[](0);
    IloArray<IloNumVarArray> &qr = *q;
    IloRangeArray CC(*env);
    ostringstream varname;
    
}

void LSP_Resolution::createObjectif(){
    IloNumVarArray &pr = *p;
    IloIntVarArray &yr = *y;
    IloArray<IloNumVarArray> &Ir = *I;
    for (unsigned t=0; t<prp->l;t++){
        obj.setLinearCoef(pr[t],prp->u);
        obj.setLinearCoef(yr[t],prp->f);
        for (unsigned i=0; i<prp->n+1; i++){
            obj.setLinearCoef(Ir[i][t],prp->h[i]);
        }
    }
    std::cout << "Objectif : " << obj << std::endl;
}

void LSP_Resolution::addDistanceToObjectif()
{
    IloRangeArray CC(*env);
    ostringstream varname;
    z = new IloArray<IloIntVarArray>(*env,prp->n);
    IloArray<IloIntVarArray> &zr = *z;
    //Ajout de variables 
    for (unsigned i=0; i<prp->n; i++){
        IloIntVarArray QR(*env, prp->l,0,1);
        //Les quantités initiales sont fixes ! 
        zr[i] = QR;
    }

    //Et on nomme la variable 
    for(int i = 0; i < prp->l; i++) {
        for (int t=0; t< prp->n; t++){
            varname.str("");
            varname<<"z_"<<t<<"_"<<i;
            zr[t][i].setName(varname.str().c_str());
        }
  }

    //Contraintes couplantes : 
    for(unsigned i=1;i<prp->n;i++){
        for(unsigned t=1;t<prp->l;t++){
            varname.str("");
            varname<<"Z[I="<<i<<";t="<<t<<"]";
            std::cout << "added : " << varname.str() << std::endl;
            IloNum var(INT16_MAX);
            IloRange cst(*env, -IloInfinity, q->operator[](i)[t]-zr[i][t]*var, 0, varname.str().c_str() );
            CC.add(cst);
        }
    }
    model->add(CC);
    //On fixe les coefficients 
    float dist;
    for(int i = 0; i < prp->n; i++) {
        //Distance entre l'entrepot et le client i
        dist = 2*prp->getCost(i);
        for (int t=0; t< prp->l; t++){
            obj.setLinearCoef(zr[i][t],dist);
        }
  }
}








//void LSP_Resolution::generateConstraints2(){
//    IloNumVarArray &pr = *p;
//    IloNumVarArray &I0 = I->operator[](0);
//    IloArray<IloNumVarArray> &qr = *q;
//    IloRangeArray CC(*env);
//    ostringstream varname;
//    /*
//    Ajout des contraintes de concervations des flots au niveau du fournisseur : 
//    for all t in [0..l] : I[0][t-1] + P[t] = sommme pour tout client i : q[i][t] + I[0][t]
//    */
//    for (unsigned t=0; t<prp->l;t++){
//        IloExpr e(*env);
//        varname.str("");
//        varname<<"Concervation[t="<<t<<"]";
//        std::cout << "added : " << varname.str() << std::endl;
//        for (unsigned i=0; i<prp->n; i++){
//            e = e + qr[i][t];
//        }
//        IloRange cst(*env, 0, I0[t-1] + pr[t] - e - I0[t], 0, varname.str().c_str() );
//        CC.add(cst);
//    }
//    /*
//    I[i,t−1] + q[i,t] = d[i,t] + I[i,t]
//    */
//    for(unsigned i=1;i<prp->n;i++){
//        for(unsigned t=1;t<prp->l;t++){
//            IloNumVarArray &In =  I->operator[](i);
//            IloNum var(prp->d[i][t]);
//            varname.str("");
//            varname<<"Demande[I:"<<i<<"t:"<<t<<"]";
//            std::cout << "added : " << varname.str() << std::endl;
//            IloRange cst(*env, 0, In[t-1] + q->operator[](i)[t]-var-In[t], 0, varname.str().c_str() );
//            CC.add(cst);
//        }
//    }
//    /*
//    p_t <= M_t y_t
//    */
//    IloNum C_var(prp->C);
//   for(unsigned t=0;t<prp->l;t++){
//        varname.str("");
//        varname<<"Lancement[t:"<<t<<"]";
//        std::cout << "added : " << varname.str() << std::endl;
//        std::cout << "added : " << y->operator[](t) << " " << p->operator[](t) << std::endl;
//        IloIfThen ift = IloIfThen(*env, (p->operator[](t) != 0), (y->operator[](t) == 1),varname.str().c_str());
//        //IloRange cst(*env, -IloInfinity, p->operator[](t)- C_var*y->operator[](t) , 0, varname.str().c_str() );
//        //CC.add(cst);
//        model->add(ift);
//    }
//
//    for(unsigned t=0;t<prp->l;t++){
//        varname.str("");
//        varname<<"CapaciteFournisseur[t:"<<t<<"]";
//        std::cout << "added : " << varname.str() << std::endl;
//        IloRange cst(*env, -IloInfinity,I->operator[](0)[t-1]- prp->L[0], 0, varname.str().c_str() );
//        CC.add(cst);
//    }
//    for(unsigned i=1;i<prp->n;i++){
//        IloNumVarArray &In =  I->operator[](i);
//        for(unsigned t=0;t<prp->l;t++){
//            IloNum var(prp->d[t][i]);
//            varname.str("");
//            varname<<"CapaciteClient[I:"<<i<<";t:"<<t<<"]";
//            std::cout << "added : " << varname.str() << std::endl;
//            IloRange cst(*env, -IloInfinity,In[t-1] + q->operator[](i)[t] - prp->L[i], 0, varname.str().c_str() );
//            CC.add(cst);
//        }
//    }
//    contraintes = &CC;
//    model->add(CC);
//}
//
//void LSP_Resolution::createObjectif(){
//    IloNumVarArray &pr = *p;
//    IloIntVarArray &yr = *y;
//    IloArray<IloNumVarArray> &Ir = *I;
//    for (unsigned t=0; t<prp->l;t++){
//        obj.setLinearCoef(pr[t],prp->u);
//        obj.setLinearCoef(yr[t],prp->f);
//        for (unsigned i=0; i<prp->n+1; i++){
//            obj.setLinearCoef(Ir[i][t],prp->h[i]);
//        }
//    }
//    std::cout << "Objectif : " << obj << std::endl;
//}
//
//void LSP_Resolution::addDistanceToObjectif()
//{
//    IloRangeArray CC(*env);
//    ostringstream varname;
//    z = new IloArray<IloIntVarArray>(*env,prp->n);
//    IloArray<IloIntVarArray> &zr = *z;
//    //Ajout de variables 
//    for (unsigned i=0; i<prp->n; i++){
//        IloIntVarArray QR(*env, prp->l,0,1);
//        //Les quantités initiales sont fixes ! 
//        zr[i] = QR;
//    }
//
//    //Et on nomme la variable 
//    for(int i = 0; i < prp->l; i++) {
//        for (int t=0; t< prp->n; t++){
//            varname.str("");
//            varname<<"z_"<<t<<"_"<<i;
//            zr[t][i].setName(varname.str().c_str());
//        }
//  }
//
//    //Contraintes couplantes : 
//    for(unsigned i=1;i<prp->n;i++){
//        for(unsigned t=1;t<prp->l;t++){
//            varname.str("");
//            varname<<"Z[I="<<i<<";t="<<t<<"]";
//            std::cout << "added : " << varname.str() << std::endl;
//            IloNum var(INT16_MAX);
//            IloRange cst(*env, -IloInfinity, q->operator[](i)[t]-zr[i][t]*var, 0, varname.str().c_str() );
//            CC.add(cst);
//        }
//    }
//    model->add(CC);
//    //On fixe les coefficients 
//    float dist;
//    for(int i = 0; i < prp->n; i++) {
//        //Distance entre l'entrepot et le client i
//        dist = 2*prp->getCost(i);
//        for (int t=0; t< prp->l; t++){
//            obj.setLinearCoef(zr[i][t],dist);
//        }
//  }
//}
//




void LSP_Resolution::printDecisionVariables(){
    std::cout << "Environnement : " << env << std::endl;
    std::cout << "P variables are in number : " << p->getSize() << std::endl;
    printNumVarArray(*p);
    std::cout << "y variables are in number : " << y->getSize() << std::endl;
    printIntVarArray(*y);
    std::cout << "Number of arrays in q : " << q->getSize() << std::endl;
    printForEveryClient(*q);
    std::cout << "Number of arrays in I : " << I->getSize() << std::endl;
    printForEveryClient(*I);
    std::cout << "PRP : " << std::endl;
    prp->write_screen_txt();
    std::cout << "======================" << std::endl;
}


void LSP_Resolution::printVariables(){
     cplx->writeSolution("sol.txt");
}