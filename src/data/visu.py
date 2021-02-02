import matplotlib.pyplot as plt


f = open("gap_PRP_1-16.dat")
L = f.readlines()
y = []
X = []
for j in L:
    ch = j.split(";")
    X.append(int(ch[0]))
    y.append(float(ch[-1].strip()))
plt.plot(X,y)
plt.xlabel("Itérations")
plt.ylabel("Gap")
plt.show()