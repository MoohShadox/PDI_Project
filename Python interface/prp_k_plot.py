import os
import matplotlib.pyplot as plt

path = "../PRP_instances/A_014_ABS1_15_1"

prpPath = "../src/PRP20-32/PRP2"

k_range=range(10,201,10)

prpFile = []

time = []
precision = []

def edit_k_instance(path,k):
    with open(path+".prp") as f:
        prpFile = f.readlines()
        f.close()

    with open(path+".prp","w") as f:
        prpFile[7]="k "+str(k)+"\n"
        f.writelines(prpFile)
        f.close()

for k in k_range:

    print(k)
    edit_k_instance(path,k)

    os.system(str(prpPath)+" "+str(path)+ ' > output.txt')

    with open("output.txt") as f:
        output = f.readlines()
        for i in range(-30,-15,1):
            last_l=output[i].split()
            if len(last_l)>0 and last_l[0]=='*':
                for e in last_l:
                    if e[-1]=="%":
                        precision.append(float(e.strip('%'))/100)
                        print(e)
        time.append(float(output[-3].split()[3]))

fig, ax1 = plt.subplots()

color = 'tab:red'
ax1.set_xlabel('k')
ax1.set_ylabel('temps(s)', color=color)
ax1.plot(k_range, time, color=color)
ax1.tick_params(axis='y', labelcolor=color)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

color = 'tab:blue'
ax2.set_ylabel("écart avec l'optimal", color=color)  # we already handled the x-label with ax1
ax2.plot(k_range, precision, color=color)
ax2.set_ylim([0,1])
ax2.tick_params(axis='y', labelcolor=color)

fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()
