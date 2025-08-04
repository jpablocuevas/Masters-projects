C       Programa Monte Carlo (NVT) para fluido LJ

      program mclj 
C                                      abril 2017
C                                       
C     SIMULACION MONTE CARLO DE FLUIDO LJ
C     *************************************************************
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include "mc.inc"
      pi = dacos(-1.d0)
      OPEN(UNIT=7,FILE='mc.dat',STATUS='unknown')
      CALL COMIENZO
      CALL MONTECARLO
      CALL RADIAL
      CALL FINAL
      STOP
      END
C     ****************************************************************
      SUBROUTINE COMIENZO
C     Lee variables de sistema y estado (rho,temp) de mc.in 
C     Lee posiciones de particulas en mc.old si NRUN = 1
C     Genera la configuraci¢n inicial de fcc.
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include "mc.inc"
      DIMENSION XA(6),YA(6),ZA(6)
C ACUMULADORES A CERO
      DO 3 I=1,NACC
    3 ACC(I)=0.0D00
C potencial de interaccion:
C r = distancia entre centros de dos esferas:
C u(r) = 4*epsilon*(x^(-12)-x^(-6))
C x = r/sigma
C sigma = distancia donde el potencial tiene un cero.
C -epsilon = mínimo valor de la energía potencial.

C LEE PARAMETROS DE ENTRADA
C N = numero de particulas
C RHO = densidad de particulas en unidades escaladas
C     = N*sigma^3/V, donde sigma es el diametro de esfera dura
C TEMP = temperatura escalada = kT/epsilon, donde k es la constante
C        de Boltzmann y epsilon es la energia del pozo cuadrado
C RC = radio de corte.
C NRUN = 0: se comienza con una configuracion de red
C NRUN = 1: se comienza con una configuracion previa
C NMOVE = numero de movimientos totales
C NSUB = numero de movimientos despues de los cuales se 
C        calculan subpromedios
C DISPL = desplazamiento maximo en unidades de sigma
C NAC = numero de esferas por celda unitaria
C NCX = numero de veces que se replica la celda unitaria en la
C       direccion x
C NCY = numero de veces que se replica la celda unitaria en la
C       direccion y
C NCZ = numero de veces que se replica la celda unitaria en la
C       direccion z
C NOTA: N = NAC*NCX*NCY*NCZ
C XA, YA, ZA = coordenadas de las particulas en la celda unitaria.

      OPEN (UNIT=1,FILE='mc.in',STATUS='OLD')
      READ(1,*) N
      READ(1,*) RHO
      READ(1,*) TEMP
      READ(1,*) NRUN,NMOVE,NSUB
      READ(1,*) DISPL,RC
      READ(1,*) NFDR, LFDR, XHIST
      READ(1,*) NAC,NCX,NCY,NCZ
      READ(1,*)(XA(I),YA(I),ZA(I),I=1,NAC)
      write(6,100)
      write(7,100)
      write(6,101) N,RHO,TEMP,RC
      write(7,101) N,RHO,TEMP,RC
      write(6,1011) NRUN,NMOVE,NSUB
      write(7,1011) NRUN,NMOVE,NSUB
      write(6,1012) NAC,NCX,NCY,NCZ
      write(7,1012) NAC,NCX,NCY,NCZ
      write(6,1013) DISPL
      write(7,1013) DISPL
      IF (NRUN.EQ.0.0D00) WRITE(6,102)
      IF (NRUN.EQ.0.0D00) WRITE(7,102)
      IF (NRUN.NE.0.0D00) WRITE(6,103)
      IF (NRUN.NE.0.0D00) WRITE(7,103)
C CONVERSION A UNIDADES DE PROGRAMA (Lx = 1)
C NOTA: en unidades del programa, las distancias se escalan con
C la longitud de la caja en la direccion x. Esto es, todas las 
C distancias y posiciones en el programa estan escaladas con Lx.
C El diametro de cada particula esta dada entonces por s = sigma/Lx
      XN=DFLOAT(N)
      XNCX=DFLOAT(NCX)
      XNCY=DFLOAT(NCY)
      XNCZ=DFLOAT(NCZ)
      XC=1.0D00
      YC=XNCY/XNCX
      ZC=XNCZ/XNCX
      Y2=YC/2.0D00
      Z2=ZC/2.0D00
C Se calcula Lx (escrito como XL):
      XL=(XN/(RHO*YC*ZC))**(1.0D00/3.0D00)
      YL=XL*YC
      ZL=XL*ZC
      S=1.0D00/XL
      SS=S*S
      DISPL=DISPL*S
      RC2 = RC*RC*SS
C ESCRIBE LAS UNIDADES CONVERTIDAS
      WRITE(6,108) XC,YC,ZC
      WRITE(7,108) XC,YC,ZC
      WRITE(6,109) XL,YL,ZL
      WRITE(7,109) XL,YL,ZL
      WRITE(6,110) S
      WRITE(7,110) S
C GENERA LA RED INICIAL
C RX(I) = coordenada x del Centro de masas de la esfera I-esima
C RY(I) = coordenada y del Centro de masas de la esfera I-esima
C RZ(I) = coordenada z del Centro de masas de la esfera I-esima
      IF (NRUN.EQ.0.0D00) THEN
      I=1
      DO 1 IZ=1,NCZ
      DO 1 J=1,NAC
      DO 1 IX=1,NCX
      DO 1 IY=1,NCY
      RX(I)=((IX-1.0D00)+XA(J))/XNCX
      RY(I)=((IY-1.0D00)+YA(J))*YC/XNCY
      RZ(I)=((IZ-1.0D00)+ZA(J))*ZC/XNCZ
      I=I+1.0D00
    1 CONTINUE
      END IF
C LEE LA CONFIGURACION VIEJA
      IF (NRUN.EQ.0.0D00) GOTO 2
      OPEN(UNIT=3,FILE='mc.in',STATUS='OLD')
      DO 11 I=1,N
      READ(3,*) RX(I),RY(I),RZ(I)
   11 CONTINUE
    2 DO 44 I=1,20
      WRITE(6,115) RX(I),RY(I),RZ(I)
      WRITE(7,115) RX(I),RY(I),RZ(I)
   44 CONTINUE

C formatos de escritura de COMIENZO

  100 FORMAT(1X,'*********************************************',/
     +   ' ***SIMULACION MONTE CARLO NVT LJ ***',/
     +         ,' *********************************************',/,/)
  101 FORMAT(1X,' N= ',I6,' RHO= ',F10.8,' TEMP= ',F10.8,
     + ' RC=',F10.8,/,/)
 1011 FORMAT(1X,' NRUN=',I4,' NMOVE=',I10,' NSUB=',I9,/,/)
 1012 FORMAT(1X,' NAC,NCX,NCY,NCZ=',4I4,/,/)
 1013 FORMAT(1X,' DISPL=',F10.8,/,/)
  102 FORMAT(1X,'INICIO DE UNA RED',/)
  103 FORMAT(1X,'INICIO DE CONFIGURACION PREVIA',/)
  108 FORMAT(1X,'TAMANO DE CAJA EN UNIDADES DE PROGRAMA :',/
     +         ,8X,'XC=',F9.5,' YC=',F9.5,' ZC=',F9.5,/)
  109 FORMAT(1X,'TAMANO DE CAJA EN UNIDADES DE SIGMA:  ',/
     +         ,8X,'XL=',F9.5,' YL=',F9.5,' ZL=',F9.5,/)
  110 FORMAT(1X,'SIGMA EN UNIDADES DE LA CAJA:',4X,F10.8,/)
  111 FORMAT(1X,'NUMERO PREVIO DE CONFIGURACIONES',F12.0,/)
  112 FORMAT(1X,'CONFIGURACON INICIAL')
  115 FORMAT(1X,3(1X,F10.8))
      RETURN
      END

C ******************************************************************

      SUBROUTINE MONTECARLO
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include "mc.inc"
      open(unit=8,file='uav.dat')
C Inicializa secuencia al azar, el programa de generador de 
C numeros aleatorios (RAN2) usa como argumento inicial ISEED
      ISEED=-123456789
C Calculo de la energia incial del sistema
      CALL ENERGIA(UPLJ) 
C UPERP = energia por particula en unidades de epsilon
      UPERP=UPLJ/XN
      UTOT= UPLJ
      USUBAV=0.0D00
      NAVE = 0
      NCOUNT=0
      NACCPT=0
      NSUB0=NSUB
      NFDR0 = NFDR
      WRITE(6,101)UPERP
      WRITE(7,101)UPERP
C comienza la secuencia de Monte Carlo

C OPEN(UNIT=2,FILE='uav.dat',STATUS='NEW')

C escoge particula al azar
    1 I=INT(RAN2(ISEED)*N)+1
      NCOUNT=NCOUNT+1.0D00
      RTEST=RTEST+RAN2(ISEED)
      RXI = RX(I)
      RYI = RY(I)
      RZI = RZ(I)
C desplaza la particula
C XNEW tiene como valores minimo y maximo RXI-DISPL/2
C y RXI + DISPL/2, respectivamente.
C YNEW tiene como valores minimo y maximo RYI-DISPL/2
C y RYI + DISPL/2, respectivamente.
C ZNEW tiene como valores minimo y maximo RZI-DISPL/2
C y RZI + DISPL/2, respectivamente.
      XNEW=RXI+DISPL*(RAN2(ISEED)-0.5D00)
      YNEW=RYI+DISPL*(RAN2(ISEED)-0.5D00)
      ZNEW=RZI+DISPL*(RAN2(ISEED)-0.5D00)
C condicion periodica de frontera
      IF(XNEW.GT.1.0D00) THEN
      XNEW=XNEW-1.0D00
      ELSE IF (XNEW.LT.0.0D00) THEN
      XNEW=XNEW+1.0D00
      END IF
      IF (YNEW.GT.YC) THEN
      YNEW=YNEW-YC
      ELSE IF (YNEW.LT.0.0D00) THEN
      YNEW=YNEW+YC
      END IF
      IF (ZNEW.GT.ZC) THEN
      ZNEW=ZNEW-ZC
      ELSE IF (ZNEW.LT.0.0D00) THEN
      ZNEW=ZNEW+ZC
      END IF
      USWNEW=0.0D00
C calcula la energia de la particula I 
C en su nueva configuracion
C La energia esta escalada con epsilon
      DO 2 J=1,N
      IF (J.EQ.I) GOTO 2
      X=RX(J)-XNEW
      Y=RY(J)-YNEW
      Z=RZ(J)-ZNEW
C convencion de imagen minima
      IF (X.GT.0.5D00) THEN
      X=X-1.0D00
      ELSE IF (X.LT.-0.5D00) THEN
      X=X+1.0D00
      END IF
      IF (Y.GT.Y2) THEN
      Y=Y-YC
      ELSE IF (Y.LT.-Y2) THEN
      Y=Y+YC
      END IF
      IF (Z.GT.Z2) THEN
      Z=Z-ZC
      ELSE IF (Z.LT.-Z2) THEN
      Z=Z+ZC
      END IF
      RR=X*X+Y*Y+Z*Z
      IF (RR.LT.RC2) THEN
       IF(RR.LT.0.5D0*SS) GOTO 3
       SR2=SS/RR
       SR6=SR2*SR2*SR2
       SR12=SR6*SR6
       ULJ = 4.d0*(SR12-SR6)
      ELSE
       ULJ = 0.d0 
      ENDIF
      USWNEW=USWNEW+ULJ
    2 CONTINUE
C calcula la energia de la particula I 
C en su configuracion previa
C La energia esta escalada con epsilon
      USWOLD=0.0D00
      DO 4 J=1,N
      IF (J.EQ.I) GOTO 4
      X=RX(J)-RXI 
      Y=RY(J)-RYI
      Z=RZ(J)-RZI
C convencion de imagen minima
      IF (X.GT.0.5D00) THEN
      X=X-1.0D00
      ELSE IF (X.LT.-0.5D00) THEN
      X=X+1.0D00
      END IF
      IF (Y.GT.Y2) THEN
      Y=Y-YC
      ELSE IF (Y.LT.-Y2) THEN
      Y=Y+YC
      END IF
      IF (Z.GT.Z2) THEN
      Z=Z-ZC
      ELSE IF (Z.LT.-Z2) THEN
      Z=Z+ZC
      END IF
      RR=X*X+Y*Y+Z*Z
      IF (RR.LT.RC2) THEN
       IF (RR.LT.0.5d0*SS) GOTO 42
       SR2=SS/RR
       SR6=SR2*SR2*SR2
       SR12=SR6*SR6
       ULJ = 4.d0*(SR12-SR6)
      ELSE
       ULJ = 0.d0 
      ENDIF
      USWOLD=USWOLD+ULJ
    4 CONTINUE
      DENERG = USWNEW-USWOLD
C Si DENERG < 0 quiere decir que USWNEW < USWOLD y se acepta
C el movimiento:
      IF (DENERG.LE.0.0D00) GOTO 5
C Si DENERG > 0 quiere decir que USWNEW > USWOLD y se acepta
C el movimiento de manera estocastica, comparando el 
C factor de Boltzmann (BOLTZ) con un numero al azar (RND)
      RND = RAN2(ISEED)
      BOLTZ= EXP(-DENERG/TEMP)
C Si BOLTZ < RND entonces se rechaza el movimiento
      IF (RND.GT.BOLTZ) GOTO 3
C Si BOLTZ > RND entonces se acepta  el movimiento
C y se actualiza la posicion de la particula I
    5 RX(I)=XNEW
      RY(I)=YNEW
      RZ(I)=ZNEW
      UTOT=UTOT+DENERG
      NACCPT=NACCPT+1.0D00
C acumula promedios
    3 ACC(1)=ACC(1)+1.0D00
      ACC(2)=ACC(2)+UTOT
      ACC(5)=ACC(5)+UTOT*UTOT
      USUBAV=USUBAV+UTOT
      XNTEST=DFLOAT(NCOUNT)
      XNACEPT=DFLOAT(NACCPT)
      IF (NCOUNT.EQ.NFDR0.AND.LFDR) CALL GOFR
      IF (NCOUNT.GE.NMOVE) GOTO 6
      IF (NCOUNT.LT.NSUB) GOTO 1
C escribe resultados
      NAVE = NAVE + 1
      xnave = dfloat(NAVE)
      UAV=USUBAV/(XN*NCOUNT)
      UAV2 = ACC(5)/XNTEST
      CVAV = (UAV2 - XN*XN*UAV*UAV)/TEMP/TEMP
      CVAV = CVAV/XN
C new 23/03/2015
      ACC(10) = ACC(10) + UAV
      ACC(12) = ACC(12) + UAV*UAV
      ACC(14) = ACC(14) + CVAV
      ACC(16) = ACC(16) + CVAV*CVAV
      xprom1 = ACC(10)/xnave
      xprom2 = ACC(12)/xnave
      xprom3 = ACC(14)/xnave
      xprom4 = ACC(16)/xnave
      std1 = xprom2 - xprom1*xprom1
      std2 = xprom4 - xprom3*xprom3
      std1 = dsqrt(std1)
      std2 = dsqrt(std2)
      WRITE(6,102) NCOUNT,XNACEPT/XNTEST,UAV,std1,CVAV,std2
      WRITE(7,102) NCOUNT,XNACEPT/XNTEST,UAV,std1,CVAV,std2
      WRITE(8,200) NCOUNT,UAV,std1,CVAV,std2
      NSUB=NSUB+NSUB0
      GOTO 1

C fin de la corrida
       NAVE = NAVE + 1
       xnave = dfloat(NAVE)
    6  UAV=USUBAV/(XN*NCOUNT)
       UAV2=ACC(5)/XNTEST
       UAVRUN=ACC(2)/(XN*ACC(1))
       CVAV = (UAV2 - XN*XN*UAV*UAV)/TEMP/TEMP
       CVAV = CVAV/XN
C new 23/03/2015
      xnave = dfloat(NAVE)
      ACC(10) = ACC(10) + UAV
      ACC(12) = ACC(12) + UAV*UAV
      ACC(14) = ACC(14) + CVAV
      ACC(16) = ACC(16) + CVAV*CVAV
      xprom1 = ACC(10)/xnave
      xprom2 = ACC(12)/xnave
      xprom3 = ACC(14)/xnave
      xprom4 = ACC(16)/xnave
      std1 = xprom2 - xprom1*xprom1
      std2 = xprom4 - xprom3*xprom3
      std1 = dsqrt(std1)
      std2 = dsqrt(std2)
      WRITE(6,102) NCOUNT,XNACEPT/XNTEST,UAV,std1,CVAV,std2
      WRITE(7,102) NCOUNT,XNACEPT/XNTEST,UAV,std1,CVAV,std2
      WRITE(8,200) NCOUNT,UAV,std1,CVAV,std2
      WRITE(6,103) ACC(1)
      WRITE(7,103) ACC(1)
      WRITE(6,104) UAVRUN
      WRITE(7,104) UAVRUN
      close(8)
      RETURN

C particulas traslapadas
   42 WRITE(6,105) I,J,RR
      WRITE(6,106) SQRT(RR),S
      WRITE(6,*) RX(I),RY(I),RZ(I)
      WRITE(6,*) RX(J),RY(J),RZ(J)
  101 FORMAT(/,1X,'inicio de corrida',/,1X,'energia inicial',G16.8,/)
  102 FORMAT(1X,'NMOV  =',I12,'  ACEPT  =',F8.4,'  UAV =',F10.4,
     & '  std1=',F10.4,'  CV =',F10.4, '  std2=',F10.4/)
  200 FORMAT(I12, F10.4, F10.4, F10.4, F10.4)
  103 FORMAT(1X,'Num total de configuraciones desde el inicio',F12.0,/)
  104 FORMAT(1X,'Energia promedio desde el inicio',F10.4,/)
  105 FORMAT(1X,'*****particulas traslapadas*****',' I=',I4,' J=',I4,
     + ' R**2=',G16.8,/)
  106 FORMAT(1X,'R=',G16.8,' S=',G16.8,/)
      RETURN
      END

C     ***************************************************************

C     Calcula la energia 

      SUBROUTINE ENERGIA(UTOT) 
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include "mc.inc"
      UTOT=0.0D0
      DO 4 I=1,N-1
      RXI = RX(I)
      RYI = RY(I)
      RZI = RZ(I)
      DO 4 J=I+1,N
      IF (J.EQ.I) GOTO 4
      X=RX(J)-RXI
      Y=RY(J)-RYI
      Z=RZ(J)-RZI
C convencion de imagen minima
      IF (X.GT.0.5D00) THEN
      X=X-1.0D00
      ELSE IF (X.LT.-0.5D00) THEN
      X=X+1.0D00
      END IF
      IF (Y.GT.Y2) THEN
      Y=Y-YC
      ELSE IF (Y.LT.-Y2) THEN
      Y=Y+YC
      END IF
      IF (Z.GT.Z2) THEN
      Z=Z-ZC
      ELSE IF (Z.LT.-Z2) THEN
      Z=Z+ZC
      END IF
      RR=X*X+Y*Y+Z*Z
      IF(RR.LT.RC2) THEN
        SR2=SS/RR
        SR6=SR2*SR2*SR2
        SR12=SR6*SR6
        ULJ = 4.0D00*(SR12-SR6)
      ELSE
        ULJ = 0.d0
      ENDIF
      UTOT = UTOT + ULJ
    4 CONTINUE
      RETURN
      END

C     *****************************************************************
      SUBROUTINE RADIAL
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include "mc.inc"
      OPEN(UNIT=9,FILE='rdf.dat',STATUS='NEW')
      NHISTG=nint(XHIST)
      XNAV=ACC(3)
      IF(XNAV.EQ.0.0D00)RETURN
      DELR=(1.d0-0.75*S)*XL/XHIST
      VOLME=XL*YL*ZL
      CONST=2.0D00/3.0D00*XN*XN*PI*XNAV/VOLME
c      NEND=INT((0.5D00*XC-S)*XHIST/(1.d0-S))
      NEND=INT((0.5D00*XC-0.75d0*S)*XHIST/(1.d0-0.75d0*S))
      DO 1 L=1,NEND
      X=FLOAT(L)
      R=(1.0D00-0.75d0*S)*(2.0D00*X-1.0D00)*XL/(XHIST*2.0D00)
      RB=(1.0D00-0.75d0*S)*(X-1.0D00)*XL/XHIST
      R=R+0.75D00
      RSQ=R*R
      RB=RB+0.75D00
      RBSQ=RB*RB
      RBCU=RBSQ*RB
      RC=RB+DELR
      RC3=RC*RC*RC
      GR=G(L)/(CONST*(RC3-RBCU))
      write(9,*)R,GR
1     CONTINUE
      RETURN
      END
C     *****************************************************************
      SUBROUTINE GOFR
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include "mc.inc"
      ACC(3)=ACC(3)+1.0D00
      RMAX=0.5D00*0.5D00
      DO 1 I=1,N-1
      DO 1 J=I+1,N
      X=RX(I)-RX(J)
      Y=RY(I)-RY(J)
      Z=RZ(I)-RZ(J)
C convencion de imagen minima
      IF (X.GT.0.5D00) THEN
      X=X-1.0D00
      ELSE IF (X.LT.-0.5D00) THEN
      X=X+1.0D00
      END IF
      IF (Y.GT.Y2) THEN
      Y=Y-YC
      ELSE IF (Y.LT.-Y2) THEN
      Y=Y+YC
      END IF
      IF (Z.GT.Z2) THEN
      Z=Z-ZC
      ELSE IF (Z.LT.-Z2) THEN
      Z=Z+ZC
      END IF
      RR=X*X+Y*Y+Z*Z
      IF (RR.GT.RMAX) GOTO 1
      R=DSQRT(RR)
c      L=INT((R-S)*XHIST/(1.d0-S))+1
      L=INT((R-0.75d0*S)*XHIST/(1.d0-0.75d0*S))+1
      G(L)=G(L)+1.0D00
    1 CONTINUE
      NFDR0=NFDR+NFDR0
      RETURN
      END

      SUBROUTINE FINAL
      IMPLICIT DOUBLE PRECISION(A-H,O-Z)
      include "mc.inc"
C     Escribe configuracion final en mc.new
      OPEN(UNIT=4,FILE='mc.new',STATUS='NEW')
      DO 55 I=1,N
      WRITE(4,*) RX(I),RY(I),RZ(I)
   55 CONTINUE
      
C Escribe en pantalla
      WRITE(6,101)
      WRITE(7,101)
      DO 88 I=1,N
      WRITE(6,*) RX(I),RY(I),RZ(I)
      WRITE(7,*) RX(I),RY(I),RZ(I)
   88 CONTINUE
 101  FORMAT(/,/,2X,'configuracion final',/,10X,'RX',20X,'RY',20X,
     + 'RZ',/)
      close(7)
      close(4)
      RETURN
      END

C     ****************************************************************


        FUNCTION RAN2(IDUM)
C       RAN2 OF NUMERICAL RECIPES 2ND ED.
        IMPLICIT DOUBLE PRECISION(A-H,O-Z)
        PARAMETER (IM1=2147483563,IM2=2147483399,AM=1.0D00/IM1,
     &  IMM1=IM1-1,IA1=40014,IA2=40692,IQ1=53668,IQ2=52774,IR1=12211,
     &  IR2=3791,NTAB=32,NDIV=1+IMM1/NTAB)
        PARAMETER(EPS=1.2D-14,RNMX=1.0D00-EPS)
        DIMENSION IV(NTAB)
        SAVE IV,IY,IDUM2
        DATA IDUM2/123456789/,IV/NTAB*0/,IY/0/
        IF (IDUM.LE.0)THEN
C        WRITE(6,*)'INIT.',IDUM
          IDUM=MAX(-IDUM,1)
          IDUM2=IDUM
          DO J=NTAB+8,1,-1
            K=IDUM/IQ1
            IDUM=IA1*(IDUM-K*IQ1)-K*IR1
            IF(IDUM.LT.0)IDUM=IDUM+IM1
            IF(J.LE.NTAB)IV(J)=IDUM
          ENDDO
          IY=IV(1)
        ENDIF
        K=IDUM/IQ1
        IDUM=IA1*(IDUM-K*IQ1)-K*IR1
        IF(IDUM.LT.0)IDUM=IDUM+IM1
        K=IDUM2/IQ2
        IDUM2=IA2*(IDUM2-K*IQ2)-K*IR2
        IF(IDUM2.LT.0)IDUM2=IDUM2+IM2
        J=1+IY/NDIV
        IY=IV(J)-IDUM2
        IV(J)=IDUM
        IF(IY.LT.1)IY=IY+IMM1
        RAN2=MIN(AM*IY,RNMX)
        RETURN
        END

