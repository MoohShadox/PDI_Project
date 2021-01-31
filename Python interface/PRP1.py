import xml.etree.ElementTree as ET
import numpy as np
import os
from prp import *

pathPRP1 = "../src/PRP1-16/"

class PRP1Sol:

    def __init__(self,path):
        self.prp=PRP(path)
        self.sol = dict()
        self.sol["p"]=np.zeros((self.prp.l,))
        self.sol["I"]=np.zeros((self.prp.n+1,self.prp.l+1))
        self.sol["y"]=np.zeros((self.prp.l,))
        self.sol["Z"]=np.zeros((self.prp.n+1,self.prp.l))
        self.sol["X"]=np.zeros((self.prp.n+1,self.prp.n+1,self.prp.l))
        self.sol["Q"]=np.zeros((self.prp.n+1,self.prp.l))
        self.sol["W"]=np.zeros((self.prp.n+1,self.prp.l))
        os.system(str(pathPRP1)+"PRP1 "+str(path)+ ' > output.txt')
        self.parseSol()

    def parseSol(self):

        tree = ET.parse("sol.txt")
        root = tree.getroot()

        for variable in root[3]:

            parts = variable.attrib["name"].split("_")

            idx = [int(p)-1 if int(p)!=0 else int(p) for p in parts[1:]]

            self.sol[parts[0]][tuple(idx)]=float(variable.attrib["value"])

    def toDash(self,t):

        elements,stylesheet = self.prp.getDashNodes()

        for i in range(self.prp.n+1):
            for j in range(self.prp.n+1):
                if(i!=j):
                    classes = ('used' if self.sol['X'][i][j][t]>0.5 else '')
                    elements.append({'data': {'source': str(i), 'target': str(j)}, 'classes':classes})

        stylesheet.append({'selector': '.used','style': {'line-color': 'blue'}},)

        return cyto.Cytoscape(
            id='cytoscape-two-nodes',
            layout={'name': 'preset'},
            style={'width': '100%', 'height': '400px'},
            elements=elements,
            stylesheet=stylesheet
        )

        print(self.sol["X"])
        print(np.where(self.sol["X"] > 0.5 ))

PRP1Sol("../PRP_instances/1PDI_Instance").toDash(1)
