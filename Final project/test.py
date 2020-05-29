import numpy as np
from matplotlib import pyplot as plt
from sklearn import linear_model

laserList = np.genfromtxt('laser_0.log', delimiter=',')
odo = np.genfromtxt('odoPose.log', delimiter=',')



for laser in laserList[:1]:
    # Convert to x and y coordinates
    delta_angle=laser[2]
    data = laser[5:]
    x=np.zeros_like(data)
    y=np.zeros_like(data)
    angle = laser[3]
    for i in range(len(data)):
        #print(angle)
        x[i]=laser[i]*np.cos(angle)
        y[i]=laser[i]*np.sin(angle)
        angle= angle+delta_angle
    plt.scatter(x,y)