#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm


# In[2]:


# Reading the data from a text file

ex_data = open ("exact.txt","r");
ex_pts = ex_data.readlines();

# Creating the x and y arrays

x_ex =[];
psi_ex =[];

for line in ex_pts:
    if (line.split()[0]!='x' or line.split()[1]!='psi_0'):
        x_ex.append(float(line.split()[0]));
        psi_ex.append(float(line.split()[1]));


# In[3]:


# Reading the data from a text file

mc_data = open ("monte_carlo.txt","r");
mc_pts = mc_data.readlines();

# Creating the x and psi arrays

x_mc =[];
psi_mc =[];

for line in mc_pts:
    x_mc.append(float(line.split()[0]));
    psi_mc.append(float(line.split()[1]));


# In[14]:


fig, ax = plt.subplots(1,1,figsize=[6,6]);
ax.set_xlabel('$x$');
ax.set_ylabel('$|\psi_0 (x)|^2$');
ax.set_title("SW ground state, 40.18%, P = 70");
#ax.set_xlim([0, 1]);
#ax.set_ylim([0, 4.5]);

ax.plot (x_ex, psi_ex, color = "indigo",label = "Exact");
ax.scatter (x_mc, psi_mc, color = "goldenrod", marker = "o", label = "Monte-Carlo");
    
ax.grid();
ax.legend(loc = "upper right");
plt.show(fig); 


# In[16]:


fig.savefig ("SW_ground_state.jpg")


# In[ ]:





# In[ ]:





# In[ ]:




