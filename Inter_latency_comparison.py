import numpy as np

size = [1024]
#8,16,32,64,128,256,512,1024
time = [0.000001526]

polVal = np.polyfit(size,time,1)
print(polVal)

