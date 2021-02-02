import xml.etree.ElementTree as ET
import numpy as np
import os
from prp import *

pathPRP2 = "../src/PRP20-32/"

class PRP2Sol:

    def __init__(self,path):
        self.prp=PRP(path)
        self.sol = dict()
        self.sol["p"]=np.zeros((self.prp.l+1,))
        self.sol["I"]=np.zeros((self.prp.n+1,self.prp.l+1))
        self.sol["y"]=np.zeros((self.prp.l+1,))
        self.sol["Z"]=np.zeros((self.prp.n+1,self.prp.k,self.prp.l+1))
        self.sol["X"]=np.zeros((self.prp.n+1,self.prp.n+1,self.prp.k,self.prp.l+1))
        self.sol["Q"]=np.zeros((self.prp.n+1,self.prp.k,self.prp.l+1))
        self.sol["W"]=np.zeros((self.prp.n+1,self.prp.l+1))
        os.system(str(pathPRP2)+"PRP2 "+str(path)+ ' > output.txt')
        self.parseSol()

    def parseSol(self):

        tree = ET.parse("sol.txt")
        root = tree.getroot()

        for variable in root[3]:

            parts = variable.attrib["name"].split("_")

            idx = [int(p) for p in parts[1:]]

            self.sol[parts[0]][tuple(idx)]=float(variable.attrib["value"])

    def toDash(self,t):

        elements = []
        stylesheet = []
        for n in range(self.prp.n+1):
            e,s = self.prp.nodes[n].toDash(np.sum(self.sol["Z"][n],axis=0)[t])
            elements.append(e)
            stylesheet.append(s)

        for i in range(self.prp.n+1):
            #print(np.sum(self.sol['Z'][i],axis=0)[t])
            for j in range(self.prp.n+1):
                if(i!=j):
                    x = np.sum(self.sol['X'][i][j],axis=0)[t]
                    if x>0.5 :
                        label = (str(self.sol['W'][j][t]) if x>0.5 and self.sol['W'][j][t]>=1 else '')
                        curved = ('segments' if x>0.5 and np.sum(self.sol['X'][j][i],axis=0)[t]>0.5 else '')
                        elements.append({'data': {'id':str(i)+str(j),'source': str(i), 'target': str(j)}, 'classes':"used"})
                        stylesheet.append({'selector': "#"+str(i)+str(j),'style': {'curve-style':curved,'label':label}})

        #stylesheet.append({'selector': '.used','style': {'background-color':'blue','line-color': 'blue'}})

        return cyto.Cytoscape(
            id='cytoscape-two-nodes',
            layout={'name': 'preset'},
            style={'width': '100%', 'height': '400px'},
            elements=elements,
            stylesheet=stylesheet
        )
