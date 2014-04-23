/*
 *  wfgrid3d.cpp
 *  
 *
 *  Created by Juan Manuel Solano Altamirano on 06/05/13.
 *  Copyright 2013. All rights reserved.
 *
 */

#ifndef _WFGRID3D_CPP_
#define _WFGRID3D_CPP_

#include "wfgrid3d.h"
#include "solcubetools.h"
#include "bondnetwork.h"

#ifndef PARALLELIZEDTK
#define PARALLELIZEDTK 0
#endif

//**********************************************************************************************
waveFunctionGrid3D::waveFunctionGrid3D()
{
   for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
         dx[i][j]=0.0e0;
      }
      npts[i]=DEFAULTPOINTSPERDIRECTION;
      xin[i]=0.0e0;
   }
   comments=string("#");
   prop1d=NULL;
   prop2plot=NONE;
   imsetup=false;
}
//**********************************************************************************************
waveFunctionGrid3D::~waveFunctionGrid3D()
{
   dealloc1DRealArray(prop1d);
}
//**********************************************************************************************
void waveFunctionGrid3D::setUpSimpleGrid(gaussWaveFunc &wf,bondNetWork &bn)
{
   if (!(bn.imstp())) {
      cout << "Error: Trying to use a non set-up bondNetWork object!\n";
      cout << "The grid could not be set up." << endl;
      return;
   }
   for (int i=0; i<3; i++) {
      xin[i]=bn.bbmin[i]-(EXTRASPACECUBEFACTOR*bn.maxBondDist);
      dx[i][i]=(bn.bbmax[i]-bn.bbmin[i]+(2.0e0*EXTRASPACECUBEFACTOR)*bn.maxBondDist)/solreal(npts[i]-1);
   }
   alloc1DRealArray("prop1d",npts[2],prop1d);
   imsetup=true;
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::setUpSmartCuboidGrid(gaussWaveFunc &wf,bondNetWork &bn,int nmx)
{
   if (!(bn.imstp())) {
      cout << "Error: Trying to use a non set-up bondNetWork object!\n";
      cout << "The grid could not be set up." << endl;
      return;
   }
   solreal xsp=(2.0e0*EXTRASPACECUBEFACTOR)*bn.maxBondDist;
   solreal maxdim=-1.0e+50,tmp;
   for (int i=0; i<3; i++) {
      tmp=bn.bbmax[i]-bn.bbmin[i]+xsp;
      if (tmp>maxdim) {
         maxdim=tmp;
      }
   }
   for (int i=0; i<3; i++) {
      tmp=bn.bbmax[i]-bn.bbmin[i]+xsp;
      npts[i]=floor((tmp/maxdim)*solreal(nmx));
      xin[i]=bn.bbmin[i]-(EXTRASPACECUBEFACTOR*bn.maxBondDist);
      dx[i][i]=(bn.bbmax[i]-bn.bbmin[i]+xsp)/solreal(npts[i]-1);
   }
   alloc1DRealArray("prop1d",npts[2],prop1d);
   imsetup=true;
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::setNPts(int nx,int ny,int nz)
{
   npts[0]=nx;
   npts[1]=ny;
   npts[2]=nz;
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::setNPts(int nn)
{
   for (int i=0; i<3; i++) {npts[i]=nn;}
   return;
}
//**********************************************************************************************
int waveFunctionGrid3D::getNPts(int ii)
{
   return npts[ii];
}
//**********************************************************************************************
void waveFunctionGrid3D::writeCubeRho(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalDensity(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::writeCubeLapRho(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalLapRho(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//*******************************************************************************************
void waveFunctionGrid3D::writeCubeELF(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalELF(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//*******************************************************************************************
void waveFunctionGrid3D::writeCubeShannonEntropy(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalShannonEntropy(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::writeCubeMagGradRho(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalMagGradRho(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//*******************************************************************************************
void waveFunctionGrid3D::writeCubeLOL(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
//#if PARALLELIZEDTK
//            prop1d[k]=wf.evalLOLNew(xx,yy,zz);
//#else
            prop1d[k]=wf.evalLOL(xx,yy,zz);
//#endif
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::writeCubeKinetEnerDensG(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalKineticEnergyG(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::writeCubeKinetEnerDensK(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalKineticEnergyK(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::writeCubeMagGradLOL(ofstream &ofil,gaussWaveFunc &wf)
{
   //solreal xx,yy,zz;
   //xx=xin[0];
   //yy=xin[1];
   //zz=xin[2];
   solreal xl[3],gl[3],hl[3][3],lol,glm;
   xl[0]=xin[0];
   for (int i=0; i<npts[0]; i++) {
      xl[1]=xin[1];
      for (int j=0; j<npts[1]; j++) {
         xl[2]=xin[2];
         for (int k=0; k<npts[2]; k++) {
            wf.evalHessLOL(xl,lol,gl,hl);
            glm=0.0e0;
            for (int m=0; m<3; m++) {glm+=gl[m]*gl[m];}
            prop1d[k]=sqrt(glm);
            //prop1d[k]=wf.evalMagGradRho(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            xl[2]+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         xl[1]+=dx[1][1];
      }
      xl[0]+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//**********************************************************************************************
void waveFunctionGrid3D::writeCubeMolElecPot(ofstream &ofil,gaussWaveFunc &wf)
{
   solreal xx,yy,zz;
   xx=xin[0];
   yy=xin[1];
   zz=xin[2];
   for (int i=0; i<npts[0]; i++) {
      yy=xin[1];
      for (int j=0; j<npts[1]; j++) {
         zz=xin[2];
         for (int k=0; k<npts[2]; k++) {
            prop1d[k]=wf.evalMolElecPot(xx,yy,zz);
            //if (prop1d[k]<1.0e-20) {prop1d[k]=0.0e0;}
            zz+=dx[2][2];
         }
         writeCubeProp(ofil,npts[2],prop1d);
         yy+=dx[1][1];
      }
      xx+=dx[0][0];
#if USEPROGRESSBAR
      printProgressBar(int(100.0e0*solreal(i)/solreal((npts[0]-1))));
#endif
   }
   return;
}
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************
void waveFunctionGrid3D::makeCube(string &onam,gaussWaveFunc &wf,ScalarFieldType ft)
{
   if (!wf.imldd) {
      cout << "Error: trying to use a non loaded wave function object!\nNothing done!\n";
      return;
   }
   switch (ft) {
      case DENS:
         comments+=string("Property: Density");
         break;
      case LAPD:
         comments+=string("Property: Laplacian of Density");
         break;
      case ELFD:
         comments+=string("Property: Electron Localization Function -ELF-");
         break;
      case SENT:
         comments+=string("Property: Shannon Entropy Density");
         break;
      case MGRD:
         comments+=string("Property: Magnitude of the Gradient of the Density");
         break;
      case LOLD:
         comments+=string("Property: Localized Orbital Locator -LOL-");
         break;
      case MGLD:
         comments+=string("Property: Magnitude of the Gradient of LOL");
         break;
      case KEDG:
         comments+=string("Property: Kinetic Energy Density G");
         break;
      case KEDK:
         comments+=string("Property: Kinetic Energy Density K");
         break;
      case MEPD:
         comments+=string("Property: Molecular Electrostatic Potential");
         break;
      default:
         cout << "Error: Field type not known!\n Cube incomplete!" << endl;
         break;
   }
   ofstream ofil;
   ofil.open(onam.c_str());
   writeCubeHeader(ofil,wf.title[0],comments,npts,xin,dx,wf.nNuc,wf.atCharge,wf.R);
#if USEPROGRESSBAR
   printProgressBar(0);
#endif
   switch (ft) {
      case DENS:
         writeCubeRho(ofil,wf);
         break;
      case LAPD:
         writeCubeLapRho(ofil,wf);
         break;
      case ELFD:
         writeCubeELF(ofil,wf);
         break;
      case SENT:
         writeCubeShannonEntropy(ofil,wf);
         break;
      case MGRD:
         writeCubeMagGradRho(ofil,wf);
         break;
      case LOLD:
         writeCubeLOL(ofil,wf);
         break;
      case MGLD:
         writeCubeMagGradLOL(ofil,wf);
         break;
      case KEDG:
         writeCubeKinetEnerDensG(ofil,wf);
         break;
      case KEDK:
         writeCubeKinetEnerDensK(ofil,wf);
         break;
      case MEPD:
         writeCubeMolElecPot(ofil,wf);
         break;
      default:
         cout << "Error: Field type not known!\n Cube incomplete!" << endl;
         break;
   }
#if USEPROGRESSBAR
   printProgressBar(100);
   cout << endl;
#endif
   ofil.close();
   return;
}
//**********************************************************************************************
//**********************************************************************************************
//**********************************************************************************************

#endif//_WFGRID3D_CPP_

