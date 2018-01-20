import matplotlib 
import matplotlib.pyplot as plt
import  matplotlib.patches as p
plt.axis([1,100,1,100])
a =p.Circle([30,50],10)
plt.gca().add_patch(a)
plt.text(60,60,'tog')
plt.grid(1)
plt.show()

