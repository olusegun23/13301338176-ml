#Python -pylab
from pylab import *
X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
C,S = np.cos(X), np.sin(X)
plot(X, C, color="blue", linewidth=1.0, linestyle="-")
plot(X, S, color="green", linewidth=1.0, linestyle="-")
xlim(-4.0,4.0)
xticks(np.linspace(-4,4,30,endpoint=True))
ylim(-1.0,1.0)
yticks(np.linspace(-1,1,5,endpoint=True))
show()
