
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D 

data_csv = pd.read_csv("./IO/output.csv")



df = pd.DataFrame(data_csv)


t_data = df['time']
x_data = df['x']
y_data = df['y']
z_data = df['z']


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot(x_data,y_data,z_data)

plt.plot(t_data, x_data)
plt.show()





##########FOR2DPLOT###################
#plt.plot(x_data, y_data)
#plt.plot(z_data, y_data)
#plt.plot(x_data, z_data)
#plt.plot(t_data, x_data)
#plt.axis([-30,30,-30,30])