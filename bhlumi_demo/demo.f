**********************************************************************
*                                                                    *  
*     BBBBBBB    BBB   BBB  BBB      BBB  BBB  BBB     BBB   BBB     *
*     BBB  BBB   BBB   BBB  BBB      BBB  BBB  BBBB   BBBB   BBB     *
*     BBB  BBB   BBB   BBB  BBB      BBB  BBB  BBBBB BBBBB   BBB     *
*     BBBBBB     BBBBBBBBB  BBB      BBB  BBB  BBB BBB BBB   BBB     *
*     BBBBBBBBB  BBBBBBBBB  BBB      BBB  BBB  BBB  B  BBB   BBB     *
*     BBB  BBBB  BBB   BBB  BBB  BB  BBB  BBB  BBB     BBB   BBB     *
*     BBBBBBBBB  BBB   BBB  BBBBBBB  BBB  BBB  BBB     BBB   BBB     *
*     BBBBBBBB   BBB   BBB  BBBBBBB   BBBBBB   BBB     BBB   BBB     *
*                                                                    *
**********************************************************************
!------------------------------------------------!
! Simple (self-contained) demonstration program  !
! To execute: make demo                          !
!------------------------------------------------!

      PROGRAM MAIN
*     **********************************
      IMPLICIT DOUBLE PRECISION(A-H,O-Z) 
* histograms in labeled common   
      COMMON / cglib / b(50000)
* Initialization of histograming package --
* here we use double precision HBOOK-like histograming/plotting
* package GLIBK written by S. Jadach, (1990-96),  unpublished.
      CALL glimit(50000) 
!
! Simple (self-contained) demonstration program
      CALL Bhldem

      END

      SUBROUTINE  Bhldem
!     *******************
!----------------------------------------------------------------------
! Simplistic demo for the beginner.
! Two benchmark x-sections fof LEP2 tables are calculated
! (a) pure photonic BARE1 WW (b) VP+Z included for CALO2 WW.
! Optionaly: two properly normalized plots after generation
!----------------------------------------------------------------------
      IMPLICIT DOUBLE PRECISION(A-H,O-Z) 
      COMMON / momset / p1(4),q1(4),p2(4),q2(4),phot(100,4),nphot
      COMMON / wgtall / wtmod,wtcru1,wtcru2,wtset(300)        
      DIMENSION  xpar(100),npar(100)
!-----------------------------------
! Ploting directives and texts
! Formats for x and y axis lebels
      CHARACTER*8  fmtx,fmty
! Captions
      CHARACTER*64 capt1(3)
      DATA capt1 /
     $'This plot is in  4.x-cpc/demo/demo.tex.',
     $'The distribution d(sigma)/d(theta) [nanobars/radians].',
     $'% end-of-caption'/
      CHARACTER*64 capt2(3)
      DATA capt2 /
     $'This plot is in  4.x-cpc/demo/demo.tex.',
     $'The distribution d(sigma)/d(x), x=ln10(1-s1/s) [nanobars].',
     $'% end-of-caption'/
      CHARACTER*64 capt3(3)
      DATA capt3 /
     $'This plot is in  4.x-cpc/demo/demo.tex.',
     $'Weigt distribution, accepted events, arbitrary normalization.',
     $'% end-of-caption'/
      CHARACTER*64 capt4(3)
      DATA capt4 /
     $'This plot is in  4.x-cpc/demo/demo.tex.',
     $'Ratio (New-Old)/New for BHLUM4, $x$-axis is theta [rads].',
     $'% end-of-caption'/
!-----------------------------------
! Mark plots for plots
! In order to fool HP compilator we put backslash into DATA statement.
! Does anybody know better solution?
      CHARACTER*1 bsl
      DATA bsl / '\\' /
      CHARACTER*32 star,circle
      star    =bsl//'makebox(0,0){'//bsl//'LARGE $'//bsl//'star$}'
      circle  =bsl//'circle{30}'
!-----------------------------------
!----------------------------------------------------------------------
! General Output for everybody including Glibk
      nout =16
      OPEN(nout,FILE='./demo.output')
      REWIND(nout)
      noutb =17
      OPEN(noutb,FILE='./bhabha.dat')
      REWIND(noutb)
      noutp =18
      OPEN(noutp,FILE='./bhabha_photon.dat')
      REWIND(noutp)
      CALL goutpu(nout)
      WRITE(nout,*) '   '
      WRITE(nout,*) '=============================================='
      WRITE(nout,*) '==========*********************==============='
      WRITE(nout,*) '==========***    Bhldem     ***==============='
      WRITE(nout,*) '==========*********************==============='
      WRITE(nout,*) '=============================================='
      WRITE(nout,*) '   '
!---------------------------------------------------------------------------
!|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
! User should cross-check the folowing two output cross sections
! which are calculated and printed at the very end of the output:
! Workshop95, Table14, BARE1 WW for zmin=0.5:  KeyGen=3, KeyPia=0, KeyZet=0
! Workshop95, Table18, CALO2 WW for zmin=0.5:  KeyGen=3, KeyPia=2, KeyZet=1
!|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
!---------------------------------------------------------------------------
!         Input parameters for  Bhlumi
!----------------------------------------------------------------------
! Technical parameters, nothing should depend on them:
      KeyGen =   3   ! Multiphoton Bhlumi
      KeyRem =   1   ! No remooval of photons below epsCM
      KeyRem =   0   ! Remooval of photons below epsCM, Necessary for Z!!!
! Try both options for KeyWgt, result should be the same
      KeyWgt =   2   ! weighted events, with t generation down to zero
      KeyWgt =   0   ! WT=1 events, for detector simulation
      KeyRnd =   1   ! RANMAR random numbers
      KeyOpt =1000*KeyGen +100*KeyRem +10*KeyWgt +KeyRnd 
!--------------------------------------------------
! Physics parameters:
      KeyPia =   0   ! Vacuum polarization OFF
      KeyMod =   2   ! Matrix element default version 4.x 
      KeyPia =   2   ! Vacuum polarization ON
      KeyZet =   0   ! Z contribution OFF
      KeyZet =   1   ! Z contribution ON
      KeyRad =1000*KeyZet +10*KeyMod +KeyPia
!--------------------------------------
      npar(1)=   KeyOpt
      npar(2)=   KeyRad
!-- SL: CM energy
      CmsEne =   250.D0  ! 2*Ebeam [GeV], as in Workshop95
      xpar(1)=   CmsEne
!-- SL: Range of final polar angles fir particle generator
      th1    =  .020d0      ! ThetaMin [rad]
      th2    =  .120d0       ! ThetaMax [rad]
      thmin    = th1   ! 
      thmax    = th2   ! 
      IF(KeyWgt .EQ. 2) thmin=th1  ! Because generation below th1 is on!!!
      xpar(2)=   CmsEne**2*(1-COS(thmin))/2  ! TransMin [GeV**2] 
      xpar(3)=   CmsEne**2*(1-COS(thmax))/2  ! TransMax [GeV**2]
      xpar(4)=   1D-4        ! Infrared cut on photon energy 
      CALL bhlumi(-1,xpar,npar)
!----------------------------------------------------------------------
! Book histograms
      Nbin=60
      CALL GBOOK1(1000,'Theta distribution (radians) $',Nbin,th1,th2)
      CALL GBOOK1(1100,'Energy distr: x=log10(1-s1/s)$',Nbin,-6d0,0d0)
      CALL Gmonit(  -1, 300,0D0,1D0,1D0)  ! Accepted X-section BARE1
      CALL Gmonit(  -1, 302,0D0,1D0,1D0)  ! Accepted X-section CALO2
      IF(KeyWgt .NE .0) THEN
       CALL GBOOK1(1200,'Weight distr. accepted events$',Nbin,-1d0,5d0)
       CALL GBOOK1(1300,'New-Old, BHL4-BHL2, x=theta  $',10,th1,th2)
       CALL GBOOK1(1301,'New    , BHL4     , x=theta  $',10,th1,th2)
       CALL Gmonit(  -1, 301,0D0,1D0,1D0)  ! Weight for Z contrib.
      ENDIF
!----------------------------------------------------------------------
!-- SL: Number of events
      NevTot = 10000
      DO ievent = 1,NevTot
        CALL bhlumi( 0,xpar,npar)
! Print four-momenta of first events
        IF(ievent.le.5) CALL dumps(nout)  
        IF(ievent.le.5) CALL dumps( 6)  
        CALL dumpcp(ievent,noutb,noutp,CmsEne)
        IF(MOD(ievent,50000).EQ.0) WRITE( 6,*) ' IEV= ',ievent
! Calculate angles for events with non-zero weight
        thetp=0d0
        thetq=0d0
        IF(wtcru1*wtcru2.ne.0d0) THEN
          thetp= DATAN(ABS(DSQRT(p2(1)**2+p2(2)**2)/p2(3)))
          thetq= DATAN(ABS(DSQRT(q2(1)**2+q2(2)**2)/q2(3)))
        ENDIF
! Define acceptance/weights
        WtAcc0=0  ! Only angular
        WtAcc1=0  ! BARE1
        WtAcc2=0  ! CALO2
        WtDiff=0
        WtZet =0
        IF(thetp.gt.th1.and.thetp.lt.th2 .AND.
     $     thetq.gt.th1.and.thetq.lt.th2        ) THEN
          s1 = 2*(p2(4)*q2(4)-p2(3)*q2(3)-p2(2)*q2(2)-p2(1)*q2(1))
          z1 = s1/CmsEne**2     ! BARE1
          v  = 1-z1
          WtAcc0 = WtMod
          IF(v .LT. 1.0d0 ) WtAcc1=WtMod ! Accepting all s
          IF(v .LT. 1.0d0 ) WtDif =wtcru1*wtcru2*(wtset(142)-wtset( 41))
          IF(v .LT. 1.0d0 ) WtZet =wtcru1*wtcru2*WtSet(12)
          vl = -1000
          IF(v.gt.1d-10) vl = log10(v)
        ENDIF
        CALL calo2(z2)
        IF(z2. GT. 0.d0 ) WtAcc2=WtMod
! Fill histograms and summ up weights for accepted events
! Accepted X-section with energy cutoff included 
        CALL Gmonit(0,300,WtAcc0,1D0,5D-4)  ! xs with theta cuts only
        CALL Gmonit(0,302,WtAcc2,1D0,5D-4)  ! CALO2
! Energy distribution
        CALL gf1(1100,vl,WtAcc0)
! Theta distribution
        CALL gf1(1000,thetp,WtAcc1/2)
        CALL gf1(1000,thetq,WtAcc1/2)
        IF(KeyWgt .NE. 0) THEN
! Z contribution O(alf1)exp
           CALL Gmonit(0,301,WtZet,1D0,5D-4)
! Weight distribution
           CALL gf1(1200,WtMod,1d0)
! Difference New-Old Bhlumi, requires at least 10**6 events
           CALL gf1(1300,thetp,WtDif)
           CALL gf1(1301,thetp,WtAcc1)
        ENDIF 
      ENDDO
      CALL bhlumi( 2,xpar,npar)
      Xcru  = xpar(20)
! Print histograms
      CALL gprint(1000)
      CALL gprint(1100)
      IF(KeyWgt .NE. 0) CALL gprint(1200)
! Print two x-sections
      WRITE(nout,*)'|||||||||||||||||||||||||||||||||||||||||||||||||||'
      CALL Gmonit(   1,300,AWT,DWT,WWMX )
      WRITE(nout,'(a,f8.6,a,f8.6,a,f15.3,a)')' Xs in ', th1, ', ', th2,
     $ ' rad : ', Xcru*AWT ,' Nanob.'
      WRITE(nout,'(a,f10.3,a)')' error      = ',Xcru*AWT*DWT,' Nanob.'
      CALL Gmonit(   1,302,AWT,DWT,WWMX )
      WRITE(nout,'(a,f10.6,a)')' Xs in FV = ',Xcru*AWT    ,' Nanob.'
     $                        ,' error    = ',Xcru*AWT*DWT,' Nanob.'
      IF(KeyWgt .NE. 0) THEN
      CALL Gmonit(   1,301,AWT,DWT,WWMX )
      WRITE(nout,'(a,f15.8,a)') ' Z  BARE1  = ',Xcru*AWT    ,' Nanob.'
     $                         ,' error     = ',Xcru*AWT*DWT,' Nanob.'
      ENDIF 
      WRITE(nout,*)'|||||||||||||||||||||||||||||||||||||||||||||||||||'
!----------------------------------------------------------------------
!----------------------------------------------------------------------

!----------------------------------------------------------------------
! Only for weighted events !
      IF(KeyWgt .NE. 0) THEN
! Weigt distribution, accepted events, arbitrary normalization.
         FacNor  = 1/(Nevtot*1d0)
         CALL Gopera(1200,'+',1200,2200,FacNor,0d0)
         fmtx='f10.2'
         fmty='f10.3'
         CALL gplset('DMOD',2d0)
         CALL gplcapt(capt3)
         CALL gplot2(  2200,' ',' ',' ',fmtx,fmty)

! Ratio (New-Old)/New for BHLUM4, x-axis is theta [rads].
         CALL Gopera(1300,'/',1301,2300,1d0,1d0)
         CALL gminim(2300,-0.005d0)
         CALL gmaxim(2300,+0.005d0)
         CALL gidopt(2300,'ERRO')
         fmtx='f10.2'
         fmty='f10.3'
         CALL gplcapt(capt4)
         CALL gplot2(  2300,' ','*',star ,fmtx,fmty)
      ENDIF
   
!---- Close TeX file and output
      CALL gplend
      CLOSE(nout)
      END

      SUBROUTINE calo2(z2)
!     ***********************
! Energy clusters as in CALO2 of Workshop95
!     **********************************
      IMPLICIT DOUBLE PRECISION(A-H,O-Z) 
      COMMON / momset / p1(4),q1(4),p2(4),q2(4),phot(100,4),nphot
      LOGICAL lforw,lbacw
!--------------------------------------------------------
! Xs in FV
! Fiducial theta size 
      th1f=thmin
      th2f=thmax
      ntheta=16
      nphi  =32
      padthe = (th2f-th1f)/ntheta
      padphi = 2d0*pi/nphi
! ***CALO2***, (theta*phi) rectangles (semi-rings) like in SICAL,
      th1w=th1f !+padthe
      th2w=th2f !-padthe
      th1n=th1f !+2*padthe
      th2n=th2f !-4*padthe
      dlphi  = 3d0/2d0 *padphi  ! half-size of theta window
      dlthe  = 3d0/2d0 *padthe  ! half-size of phi   window
!--------------------------------------------------------
! wide/narrow sectors forward
      thetp=angfi(p2(3),dsqrt(p2(1)**2+p2(2)**2))
      phip =angfi(p2(1),p2(2))
! wide/narrow sectors backward
      thetq=angfi(-q2(3),dsqrt(q2(1)**2+q2(2)**2))
      phiq =angfi(-q2(1),-q2(2))
      lforw= thetp .GT. th1w .AND. thetp .LT. th2w
      lbacw= thetq .GT. th1w .AND. thetq .LT. th2w
!--------------------------------------------------------
! CALO2: Associate photon energy with electrons 
! wthin dlthe*dlphi semi-ring
      e1=p2(4)
      e2=q2(4)
      DO k=1,nphot
! forward
        thet1=angfi( phot(k,3),dsqrt(phot(k,1)**2+phot(k,2)**2))
        phi1 =angfi( phot(k,1),phot(k,2))
        difphi1 = min(ABS(phi1 -phip),2*pi-ABS(phi1 -phip))
        IF( abs(thet1-thetp) .LT. dlthe .AND.
     $               difphi1 .LT. dlphi)  e1=e1+phot(k,4)
! backward
        thet2=angfi(-phot(k,3),dsqrt(phot(k,1)**2+phot(k,2)**2))
        phi2 =angfi(-phot(k,1),-phot(k,2))
        difphi2 = min(ABS(phi2 -phiq),2*pi-ABS(phi2 -phiq))
        IF( abs(thet2-thetq) .LT. dlthe .AND.
     $               difphi2 .LT. dlphi)  e2=e2+phot(k,4)
      ENDDO
      Ebeam = p1(4)
      z = e1*e2/Ebeam**2
      z2= -5
      IF( lforw .AND. lbacw) z2 = z
      END



      SUBROUTINE DUMPCP(IEVT,NOUTC,NOUTP,CmsEne)     
!     **********************     
! THIS PRINTS OUT FOUR MOMENTA OF ELECTRONS 
! ON OUTPUT UNIT NOUTC
! AND FOUR MOMENTA OF PHOTONS 
! ON OUTPUT UNIT NOUTP
      IMPLICIT REAL*8(A-H,O-Z)   
      COMMON / MOMSET / P1(4),Q1(4),P2(4),Q2(4),PHOT(100,4),NPHOT  
      SAVE   / MOMSET /
      REAL*8 SUM
      SUM=P2(4)+Q2(4)         
      DO 210 I=1,NPHOT  
  210 SUM=SUM+PHOT(I,4)    
      IF(SUM < 1.1*CmsEne) THEN
           WRITE(NOUTC,3100) IEVT,(P2(K),K=1,4),(Q2(K),K=1,4),NPHOT
c                WRITE(NOUTC,3100)    
c                WRITE(NOUTC,3100)    
c                WRITE(NOUTC,'(I3)')    
                 DO 100 I=1,NPHOT  
  100      WRITE(NOUTP,3200) IEVT,(PHOT(I,K),K=1,4)
      ENDIF
 3100 FORMAT(1X,I8,8F13.6,I3)   
 3200 FORMAT(1X,I8,1X,5F13.6)   
      END   

