import dash_cytoscape as cyto
import random

class Node:
    def __init__(self,num,x,y,h,L,L0,d):
        self.num=num
        self.x=x
        self.y=y
        self.h=h
        self.L=L
        self.L0=L0
        self.d=d
        self.color=random.choice(COLORS)

    def toDash(self):
        return ({'data':{'id':str(self.num),'label':str(self.num)}, 'position':{'x':self.x,'y':self.y}}\
                ,{'selector':"#"+str(self.num),'style':{'background-color':self.color,'content': 'data(label)'}})

class PRP:
    def __init__(self,path):
        with open(path+".prp") as f:
            prpFile = f.readlines()
            self.n=int(prpFile[1][2:])
            self.l=int(prpFile[2][2:])
            self.u=int(prpFile[3][2:])
            self.f=int(prpFile[4][2:])
            self.C=float(prpFile[5][2:])
            self.Q=int(prpFile[6][2:])
            self.k=int(prpFile[7][2:])
            self.nodes = []
            for i in range(8,8+self.n+1):
                node = prpFile[i].split()
                if(i==8):
                    node_d = None
                else:
                    node_d = prpFile[i+self.n+1].split()[1:]
                self.nodes.append(\
                Node(int(node[0])\
                    ,int(node[1])\
                    ,int(node[2])\
                    ,int(node[5])\
                    ,float(node[7])\
                    ,int(node[9])\
                    ,node_d))
            f.close()

    def getDashNodes(self):
        elements = []
        stylesheet = []
        for n in self.nodes:
            e,s = n.toDash()
            elements.append(e)
            stylesheet.append(s)

        return elements,stylesheet

    def toDash(self):
        elements = []
        stylesheet = []
        for n in self.nodes:
            e,s = n.toDash()
            elements.append(e)
            stylesheet.append(s)

        for i in range(self.n+1):
            for j in range(self.n+1):
                if(i!=j):
                    elements.append({'data': {'source': str(i), 'target': str(j)}})

        return cyto.Cytoscape(
            id='cytoscape-two-nodes',
            layout={'name': 'preset'},
            style={'width': '100%', 'height': '400px'},
            elements=elements,
            stylesheet=stylesheet
        )

COLORS=["aliceblue","antiquewhite","aqua","aquamarine","azure","beige","bisque","black","blanchedalmond","blue","blueviolet","brown","burlywood","cadetblue","chartreuse","chocolate","coral","cornflowerblue","cornsilk","crimson","cyan","darkblue","darkcyan","darkgoldenrod","darkgray","darkgreen","darkgrey","darkkhaki","darkmagenta","darkolivegreen","darkorange","darkorchid","darkred","darksalmon","darkseagreen","darkslateblue","darkslategray","darkslategrey","darkturquoise","darkviolet","deeppink","deepskyblue","dimgray","dimgrey","dodgerblue","firebrick","floralwhite","forestgreen","fuchsia","gainsboro","ghostwhite","gold","goldenrod","gray","green","greenyellow","grey","honeydew","hotpink","indianred","indigo","ivory","khaki","lavender","lavenderblush","lawngreen","lemonchiffon","lightblue","lightcoral","lightcyan","lightgoldenrodyellow","lightgray","lightgreen","lightgrey","lightpink","lightsalmon","lightseagreen","lightskyblue","lightslategray","lightslategrey","lightsteelblue","lightyellow","lime","limegreen","linen","magenta","maroon","mediumaquamarine","mediumblue","mediumorchid","mediumpurple","mediumseagreen","mediumslateblue","mediumspringgreen","mediumturquoise","mediumvioletred","midnightblue","mintcream","mistyrose","moccasin","navajowhite","navy","oldlace","olive","olivedrab","orange","orangered","orchid","palegoldenrod","palegreen","paleturquoise","palevioletred","papayawhip","peachpuff","peru","pink","plum","powderblue","purple","red","rosybrown","royalblue","saddlebrown","salmon","sandybrown","seagreen","seashell","sienna","silver","skyblue","slateblue","slategray","slategrey","snow","springgreen","steelblue","tan","teal","thistle","tomato","turquoise","violet","wheat","white","whitesmoke","yellow","yellowgreen"]
