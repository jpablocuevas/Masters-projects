#!/usr/bin/env python
# coding: utf-8

# # Lagrangian mechanics
# 
# This is a small library dedicated to generate some general functions wich will aid in the resolution of classical mechanical problems at a graduate level, employing the Lagrangian formalism to do so. 
# 

# In[65]:


import sympy as sp
import sympy.physics.mechanics as mech
mech.init_vprinting()
from IPython.display import display,Math,Latex


# # General parameters 

# In[66]:


# Some general parameters definitions

# Time parameter definition
def time ():

    return sp.Symbol('t', positive=True);

# Point mass of a particle with a label
def mass (i):
    
    return sp.Symbol("m_"+str(i), positive=True);

# Gravitational acceleration constant
def grav():

    return sp.Symbol('g',positive=True); # g is always positive because it represents the mangitude of the gravitational acceleration


# In[67]:


# Inner product function between two vectors. It will be useful in the computation of the potential energy. 

def inner_prod (a,b):
    
    if (len(a)!=len(b)):
        print("Vector dimensions do not match. Inner product not possible!");
        return None;
    
    s=0;
    
    for i in range (0,len(a)):
        s=s+a[i]*b[i];
        
    return s;


# # Coordinate systems
# The following functions returns two arguments, an array containing the coordinates of the system and an array emulating the position vector written in a cartesian basis. $\textbf{IMPORTANT}$: these functions only transform the $x,y,z$ components of the position vector, $not$ the cartesian basis vectors, so the returned position vector is still written in a cartesian basis.

# In[68]:


def cartesian():
    x=sp.Function("x")(time());
    y=sp.Function("y")(time());
    z=sp.Function("z")(time());
    
    coord=[x,y,z];
    p=[x,y,z];
    
    return coord,p;


# In[69]:


def polar():
    r=sp.Function("r")(time());
    theta=sp.Function("theta")(time());
    
    coord=[r,theta];
    p=[r*sp.cos(theta),r*sp.sin(theta)];
    
    return coord,p;


# In[70]:


def cylindrical(angle):
    r=sp.Function("r")(time());
    angle=sp.Function(angle)(time());
    z=sp.Function("z")(time());
    
    coord=[r,angle,z];
    p=[r*sp.cos(angle),r*sp.sin(angle),z];
    
    return coord,p;


# In[71]:


# theta is the polar angle, defined to be the figure measured from the z axis to the x-y plane.
# phi is the azimuthal angle, defined to be the figure measured from de x axis to the y axis in the x-y plane
def spherical():
    r=sp.Function("r")(time());
    theta=sp.Function("theta")(time());
    phi=sp.Function("phi")(time());
    
    coord=[r,theta,phi];
    p=[r*sp.sin(theta)*sp.cos(phi),r*sp.sin(theta)*sp.sin(phi),r*sp.cos(theta)];
    
    return coord,p;


# # Vector analysis functions & theorems
# This section provides some vector analysis operations. $\textbf{CAUTION}$: all the functions assume and underlying cartesian basis for the vectors involved. The argument $coord$ is placed so that in future implementations the vector operators can be applied in curvilinear coordinates.

# In[72]:


# Divergence function
def div(x,coord):
    D=0;
    for i in range (0,len(x),1):
        D=D+sp.diff(x[i],coord[i]);
    return D.simplify();


# In[73]:


# Levi-Civita pseudo tensor function
def Levi_Civita(i,j,k):
    if (i==j or i==k or j==k):
        return 0;
    elif (i==0 and j==1 and k==2):
        return 1;
    
    elif (i==0 and j==2 and k==1):
        return -1;
    
    elif (i==1 and j==0 and k==2):
        return -1;
    
    elif (i==1 and j==2 and k==0):
        return 1;
    
    elif (i==2 and j==1 and k==0):
        return -1;
    
    elif (i==2 and j==0 and k==1):
        return 1;
    else:
        return 0;


# In[74]:


# Cross product function
def cross (a,b):
    c=[];
    S=0;
    
    for i in range(0,len(a),1):
        for j in range(0,len(a),1):
            for k in range(0,len(a),1):
                S=S+Levi_Civita(i,j,k)*a[j]*b[k];
        if (type(S)!=int or type(S)!=float):
            S.simplify();
        c.append(S);
        S=0;
    
    return c;


# In[75]:


# Rotational function
def rot(x,coord):
    Rot=[];
    S=0;
    
    for i in range(0,len(x),1):
        for j in range(0,len(x),1):
            for k in range(0,len(x),1):
                S=S+Levi_Civita(i,j,k)*sp.diff(x[k],coord[j]);
        S.simplify();
        Rot.append(S);
        S=0;
    
    return Rot;


# In[76]:


# Gradient function
def grad(f,coord):
    g=[];
    
    for i in coord:
        g.append(sp.diff(f,i));
    
    return g;


# In[77]:


# Euler's homogeneous function theorem for the kinetick energy (the variables are the generalized velocities)
def Hom_func_thrm (f,q,k):
    s=0;
    t=time();
    
    for i in q:
        s=s+sp.diff(i,t)*sp.diff(f,sp.diff(i,t));
    
    if (k*f==s):
        print("f is homogeneous of degree",k);
        return True;
    
    else:
        print("f is not homogeneous of degree",k);
        return s;


# In[ ]:





# In[ ]:





# # Kinetic energy, potential  energy & energy function of a system
# 
# In this section kinetic and potential energies are defined, under the assumption that they are applied to the center of mass of a rigid body (or a single particle in the simplest case), denoted by $M$. The kinetic energy is then defined as the sum of its $\textit{translational}$ kinetic energy and its $\textit{rotational}$ kinetic energy.
# 
# $T := \frac{1}{2} M|\dot{\overrightarrow{x}}| +\frac{1}{2}I\omega^2$, $T=T(\overrightarrow{x},\dot{\overrightarrow{x}},\overrightarrow{\omega},t)$
# 
# whereas the potential energy is defined as 
# 
# $U:=-\overrightarrow{F}\cdot\overrightarrow{x}$, $U=U(\overrightarrow{x},t)$
# 
# where $\overrightarrow{F}$ is the force on the particle due to an external $\textit{uniform}$ field.

# In[78]:


# Kinetic energy function. The i argument is the label attached to a point mass

def T (x, I, ang, i):
    #This function assumes x is the position vector in cartesian coordinates.
    
    t=time();
    m=mass(i);
    
    v=0;
    
    for i in x:
        v=v+sp.simplify(sp.diff(i,t)**2);
        
    return sp.simplify(m/2*v+I/2*sp.diff(ang,t)**2);


# In[79]:


# Potential energy function. 
def U (x,F,Uext): 
                        
    t=time();

    return -inner_prod(F,x)+Uext;


# In[80]:


# Energy function
def Energy(L,q):
    e=0;
    t=time();
    
    for i in q:
        e=sp.simplify(e+sp.diff(i,t)*sp.diff(L,sp.diff(i,t)));

    return e-L


# # Lagrangian, generalized coordinates and equations of motion (EOM)

# In[81]:


#Lagrangian function definition.

def L (system):
    t=time();
    #m=mass();
    
    x=system[0];
    i=system[1]
    F=system[2];
    Uext=system[3];
    I=system[4];
    ang=system[5];
        
    return T(x, I, ang, i)-U(x,F,Uext);


# In[82]:


# Function that calculates the equations of motion. Lambda and f are vectors whose components are the Lagrangian multipliers
# and their respective holonomic restrictions. 

def EOM (L,q, Lambda, f):
    
    t=time();
    
    LagEq=[];
    const=0;
    
    for i in range (0,len(q),1):
        for j in range (0,len(Lambda),1):
            const=const+Lambda[j]*sp.diff(f[j],q[i]);
            
        Eq=sp.diff(sp.diff(L,sp.diff(q[i],t)),t)-sp.diff(L,q[i])-const;
        sp.simplify(Eq);
        LagEq.append(Eq);
        const=0;
    return LagEq; 


# # Generalized momenta and generalized forces

# In[83]:


# Function that computes the generalized momenta w.r.t to a generalized coordinate
def gen_p (L,q):
    P=[];
    t=time();
    
    for i in q:
        P.append(sp.diff(L,sp.diff(i,t)));
    
    return P;


# In[84]:


# Generalized force function
def gen_Q(P):
    f=[];
    t=time()
    
    for i in P:
        f.append(sp.simplify(sp.diff(i,t)));
    
    return f;


# In[ ]:





# In[ ]:




